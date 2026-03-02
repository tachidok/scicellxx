# SciCell++ Dependencies (mTASEP)

To compile and run this project correctly, ensure you have the following dependencies installed based on the version you wish to use.

## 1. Base Dependencies (Common)
- **CMake (v3.9 or higher):** Main build system.
- **C++ Compiler (C++14 support):** GCC 7+ or equivalent Clang recommended.
- **SciCell++ Library:** Included in this repository (src/).
- **Argparse:** Command-line argument handling (included in external_src/).

## 2. MPI Version Dependencies
- **MPI (Message Passing Interface):** OpenMPI or MPICH.
- **Configuration:** Use the `./configs/advanced/mpi` configuration file.

## 3. CUDA Version Dependencies
- **NVIDIA CUDA Toolkit:** v11.0 or higher recommended.
- **NVCC Compiler:** Included in the CUDA Toolkit.
- **Compatible NVIDIA Drivers:** Ensure your GPU supports the installed CUDA version.
- **Hardware:** NVIDIA GPU with compatible architecture (Compute Capability 3.5+ suggested).
- **Configuration:** Use the `./configs/advanced/CUDA` configuration file.

## 4. Optional Dependencies
- **Armadillo (v9.0+):** For advanced linear algebra operations.
- **VTK:** For complex data visualization and export.
- **ImageMagick:** Required if you wish to use the GIF generation scripts automatically.
