# Porting Guide: From C++/MPI to CUDA (mTASEP Simulation)

This document details the technical transformation of the mTASEP simulation code from its original CPU-based architecture with distributed parallelism (MPI) to a massively parallel architecture on GPU (CUDA).

## 1. Paradigm Shift in Parallelization

### MPI (C++/CPU)
- **Granularity:** Process-level parallelism (Rank).
- **Distribution:** Each CPU core processes a subset of configurations from the Cartesian product.
- **Main Loop:**
  ```cpp
  for (unsigned i = SciCellxxMPI::rank; i < n_all_configurations; i += SciCellxxMPI::nprocs) {
      // Simulation on CPU...
  }
  ```

### CUDA (GPU)
- **Granularity:** Thread-level parallelism.
- **Distribution:** A single GPU thread handles one complete configuration (1 Simulation = 1 Thread).
- **Main Kernel:**
  ```cuda
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  if (tid < d_n_all_configurations) {
      // Simulation on GPU...
  }
  ```

## 2. Memory Management and Precision

| Component | MPI Version | CUDA Version |
| :--- | :--- | :--- |
| **Microtubule** | `bool** m` (Array of pointers in RAM). | `bool* d_m` (Linear array in VRAM). |
| **Statistics** | `std::vector<Real>` in RAM. | `Statistics* d_statistics` in VRAM. |
| **Indexing** | `unsigned int` (32-bit). | `size_t` (64-bit) to prevent overflows. |
| **Parameters** | Passed via standard variables. | **Constant Memory** (`__constant__`) for speed. |

### Critical Optimization: Using `size_t`
In GPU computing, when performing large parameter sweeps (Cartesian Product), the total size of arrays (e.g., `d_density`) easily exceeds the 4GB limit of a 32-bit `unsigned int`. The use of `size_t` for all memory calculations is mandatory to prevent silent overflows and crashes during `cudaMalloc`.

### Hardware Limits (VRAM vs RAM)
Unlike CPU/MPI where "Swap" memory can handle overflows, a GPU will return an `out of memory` error and abort if the VRAM limits are exceeded. Monitoring with tools like `nvtop` is essential.

## 3. Random Number Generation (RNG)

- **MPI:** Uses the standard C++ library (`std::mt19937`). Requires one seed per process.
- **CUDA:** Uses the `curand` library.
  - **Performance Bottleneck:** Initialization (`curand_init`) is extremely expensive. It must be performed **once per thread** at the start of the kernel, and the state must be passed by reference to subsequent functions.
  ```cuda
  curandState state;
  curand_init(seed, 0, 0, &state); // Outside simulation loops
  Real r = curand_uniform(&state);  // Inside simulation hot loops
  ```

## 4. Algorithm Porting & Quality Fixes

### Two-Phase Update
- In MPI, lateral moves were often attempted during the same pass as longitudinal moves.
- In CUDA, to ensure data consistency and prevent race conditions between threads/channels, a **Two-Phase Update** was implemented:
  1. **Longitudinal Phase:** Calculate all forward movements.
  2. **Lateral Phase:** Perform channel jumps based on the updated longitudinal state.

### Lateral Movement Bias Fix (Balanced Jump)
The original implementation had an "upward drift" because it checked the "above" channel before the "below" channel. The CUDA version fixed this by introducing a **Balanced Jump** logic:
- A 50/50 probability decides which direction to check first.
- This ensures physical unbiasedness across the multi-channel structure.

### Fast Reductions and Stride
- Support functions were ported as `__device__` functions.
- To handle Arrays of Structures (AoS) efficiently, a `stride` parameter was added to statistical functions (`d_statistics_mean`). This allows threads to process data contiguously without corrupting the surrounding structure memory.

## 5. Build System (CMake)

The [CMakeLists.txt](file:///home/rodolfo/Descargas/mTASEP_CUDA/private/rodolfo/mTASEP/CMakeLists.txt) was updated to handle the CUDA language and suppress non-critical warnings from the core framework:

```cmake
# Enable CUDA Support
set_target_properties(rodolfo_demo_mTASEP_LK PROPERTIES CUDA_ARCHITECTURES 86)

# Warning suppression for clean builds
target_compile_options(target_name PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-Xcudafe --diag_suppress=815>)
```

## 6. Execution Comparison

| Feature | MPI Version | CUDA Version |
| :--- | :--- | :--- |
| **Launch Command** | `mpiexec -n 8 ./binary` | `./binary --cudablocks 64 --cudathreads 256` |
| **Scalability** | Limited by CPU core count. | Massive (thousands of simultaneous threads). |
| **Performance** | Days/Weeks for large sweeps. | Minutes/Hours for the same task. |

## 7. Hardware Tuning: Blocks and Threads

To maximize performance, the execution parameters `--cudablocks` and `--cudathreads` (blockSize) must be tuned according to the total number of configurations and the specific GPU architecture.

### Optimal Thread Count per Block (`blockSize`)
*   **Rule of Thumb:** Use multiples of **32** (the size of a "warp").
*   **Recommendation:** **256** or **512** threads per block are generally optimal. 
*   High thread counts (e.g., 1024) may limit the number of registers available per thread, while low counts (e.g., 64) may not hide memory latency effectively.

### Calculating the Number of Blocks (`numBlocks`)
The number of blocks should be sufficient to process all configurations in the Cartesian product:
*   **Formula:** `numBlocks = ceil(Total_Configurations / blockSize)`
*   In C++, this is implemented as: `(total_configs + blockSize - 1) / blockSize`.

### Empirical Case: RTX 3070 Mobile
For an **RTX 3070 Mobile** processing **14,641 configurations**, the optimal configuration was found to be:
*   `--cudathreads 256`
*   `--cudablocks 64`

#### Why 64 blocks instead of the theoretical 58?
While `ceil(14641 / 256) = 58` is the minimum number of blocks required to cover all tasks, increasing this to **64** (a power of 2) often results in better performance because:
1.  **SM Distribution:** The RTX 3070 Mobile (GA104) typically has 40 Streaming Multiprocessors (SMs). Using 64 blocks provides a more balanced load (approx 1.6 blocks per SM), allowing the GPU scheduler to better hide latencies.
2.  **Scheduling Alignment:** CUDA hardware and schedulers are highly optimized for powers of 2 (32, 64, 128).
3.  **Warp Occupancy:** Having a few extra threads (the last block only processes the remaining 49 configurations) ensures that all active SMs remain saturated until the very end of the sweep.


*Note: The code includes a guard `if (tid < d_n_all_configurations)` to safely handle the inactive threads in the last block.*

---
> [!IMPORTANT]
> Porting to CUDA allowed for a speedup of several orders of magnitude, but required much stricter memory management and hardware-specific synchronization techniques.
