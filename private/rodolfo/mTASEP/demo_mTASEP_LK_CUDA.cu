//LIC// ====================================================================
//LIC// This file forms part of SciCell++, an object-oriented, 
//LIC// framework for the the simulation of biological and physical
//LIC// phenomena modelled as continuous or discrete processes.
//LIC// 
//LIC// You can find a copy at https://github.com/tachidok/scicellxx
//LIC// 
//LIC//    Version 0.6.0
//LIC//
//LIC// 31/10/2022
//LIC// 
//LIC// SciCell++ Copyright (C) 2016-2022 Julio César Pérez Sansalvador
//LIC// 
//LIC// This framework is free software; you can redistribute it and/or
//LIC// modify it under the terms of the GNU GENERAL PUBLIC LICENSE
//LIC// published by the Free Software Foundation; either version 3 of
//LIC// the License, or (at your option) any later version.
//LIC// 
//LIC// This framework is distributed in the hope that it will be useful,
//LIC// but WITHOUT ANY WARRANTY; without even the implied warranty of
//LIC// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//LIC// GNU GENERAL PUBLIC LICENSE for more details.
//LIC// 
//LIC// You should have received a copy of the GNU GENERAL PUBLIC LICENSE
//LIC// along with this framework; if not, write to the Free Software
//LIC// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
//LIC// 02110-1301  USA.
//LIC// 
//LIC// The author may be contacted at jcp.sansalvador@inaoep.mx
//LIC// 
//LIC// ====================================================================
/// This demo uses the floor field based on the paper Kirchner, Ansgar
/// and Schadschneider, Andreas, Simulation of evacuation processes
/// using a bionics-inspired cellular automaton model for pedestrian
/// dynamics, Physica A, Elsevier, 2002.

// Include SciCell++ libraries
#include "../../../src/scicellxx.h"
#include "cuda_functions.h"
#include <curand_kernel.h>
#include <cuda_profiler_api.h>

// Include mTASEP algorithm
//#include "cc_mTASEP.h"

// Use the namespace of the framework
using namespace scicellxx;

// Macro for CUDA error handling
#define CUDA_CHECK(call) \
    do { \
        cudaError_t err = call; \
        if (err != cudaSuccess) { \
            fprintf(stderr, "CUDA error at %s:%d: %s\n", __FILE__, __LINE__, \
                    cudaGetErrorString(err)); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

// Used to define arguments
struct Args {
 argparse::ArgValue<unsigned> L;
 argparse::ArgValue<unsigned> N;
 argparse::ArgValue<Real> alpha_min;
 argparse::ArgValue<Real> alpha_max;
 argparse::ArgValue<Real> alpha_step;
 argparse::ArgValue<unsigned> alpha_n_points;
 argparse::ArgValue<Real> beta_min;
 argparse::ArgValue<Real> beta_max;
 argparse::ArgValue<Real> beta_step;
 argparse::ArgValue<unsigned> beta_n_points;
 argparse::ArgValue<Real> rho_min;
 argparse::ArgValue<Real> rho_max;
 argparse::ArgValue<Real> rho_step;
 argparse::ArgValue<unsigned> rho_n_points;
 argparse::ArgValue<Real> omega_in_min;
 argparse::ArgValue<Real> omega_in_max;
 argparse::ArgValue<Real> omega_in_step;
 argparse::ArgValue<unsigned> omega_in_n_points;
 argparse::ArgValue<Real> omega_out_min;
 argparse::ArgValue<Real> omega_out_max;
 argparse::ArgValue<Real> omega_out_step;
 argparse::ArgValue<unsigned> omega_out_n_points;
 argparse::ArgValue<unsigned> lateral_movement;
 argparse::ArgValue<unsigned> max_experiments;
 argparse::ArgValue<unsigned> max_simulations_per_experiment;
 argparse::ArgValue<unsigned> simulation_step_to_start_gathering_data;
 argparse::ArgValue<std::string> root_output_folder;
 argparse::ArgValue<unsigned> output_space_state_diagram;
 argparse::ArgValue<unsigned> output_microtubule_state;
 argparse::ArgValue<unsigned> numBlocks;
 argparse::ArgValue<unsigned> blockSize;
};

// Output parameters to a file
void output_parameters_to_file(std::string &filename, const int argc, const char **argv,  struct Args &args)
{
  // Output file
  std::ofstream output_parameters(filename, std::ios_base::out);
  
  // Write the command line as a comment into the parameters file
  output_parameters << "#";
  for (int i = 0; i < argc-1; i++)
   {
    output_parameters << argv[i] << " ";
   }
  output_parameters << argv[argc-1] << std::endl;

  const unsigned precision_real_values = 4;
  
  // output_parameters << "MPI_CORES:" << SciCellxxMPI::nprocs << std::endl;
  int deviceCount;
  cudaGetDeviceCount(&deviceCount);
    
  output_parameters << "Number of available GPU devices: " << deviceCount << std::endl;
  output_parameters << "L:" << args.L << std::endl;
  output_parameters << "N:" << args.N << std::endl;
  output_parameters << "alpha_min:" << setprecision(precision_real_values) << args.alpha_min << std::endl;
  output_parameters << "alpha_max:" << setprecision(precision_real_values) << args.alpha_max << std::endl;
  output_parameters << "alpha_step:" << setprecision(precision_real_values) << args.alpha_step << std::endl;
  output_parameters << "alpha_n_points:" << args.alpha_n_points << std::endl;
  output_parameters << "beta_min:" << setprecision(precision_real_values) << args.beta_min << std::endl;
  output_parameters << "beta_max:" << setprecision(precision_real_values) << args.beta_max << std::endl;
  output_parameters << "beta_step:" << setprecision(precision_real_values) << args.beta_step << std::endl;
  output_parameters << "beta_n_points:" << args.beta_n_points << std::endl;
  output_parameters << "rho_min:" << setprecision(precision_real_values) << args.rho_min << std::endl;
  output_parameters << "rho_max:" << setprecision(precision_real_values) << args.rho_max << std::endl;
  output_parameters << "rho_step:" << setprecision(precision_real_values) << args.rho_step << std::endl;
  output_parameters << "rho_n_points:" << args.rho_n_points << std::endl;
  output_parameters << "omega_in_min:" << setprecision(precision_real_values) << args.omega_in_min << std::endl;
  output_parameters << "omega_in_max:" << setprecision(precision_real_values) << args.omega_in_max << std::endl;
  output_parameters << "omega_in_step:" << setprecision(precision_real_values) << args.omega_in_step << std::endl;
  output_parameters << "omega_in_n_points:" << args.omega_in_n_points << std::endl;
  output_parameters << "omega_out_min:" << setprecision(precision_real_values) << args.omega_out_min << std::endl;
  output_parameters << "omega_out_max:" << setprecision(precision_real_values) << args.omega_out_max << std::endl;
  output_parameters << "omega_out_step:" << setprecision(precision_real_values) << args.omega_out_step << std::endl;
  output_parameters << "omega_out_n_points:" << args.omega_out_n_points << std::endl;
  output_parameters << "lateral_movement:" << args.lateral_movement << std::endl;
  output_parameters << "max_experiments:" << args.max_experiments << std::endl;
  output_parameters << "max_simulations_per_experiment:" << args.max_simulations_per_experiment << std::endl;
  output_parameters << "simulation_step_to_start_gathering_data:" << args.simulation_step_to_start_gathering_data << std::endl;
  output_parameters << "root_output_folder:" << args.root_output_folder << std::endl;
  output_parameters << "output_space_state_diagram:" << args.output_space_state_diagram << std::endl;
  output_parameters << "output_microtubule_state:" << args.output_microtubule_state << std::endl;
  output_parameters << "Number of CUDA blocks: " << args.numBlocks << std::endl;
  output_parameters << "Number of threads per block: " << args.blockSize << std::endl;

  // Close the parameters file
  output_parameters.close(); 
}

/// Output boolean matrix into a csv file
void real_matrix_to_csv_file(std::vector<std::vector<Real> > m, const unsigned nrows, const unsigned ncolumns, std::string &file_name)
{
 // Create file
 std::ofstream output_file(file_name, std::ios_base::out);

 /*
 const unsigned precision_real_values = 8;
 std::ostringstream ss;
 ss << setprecision(precision_real_values);
 */
 
 for (unsigned i = 0; i < nrows; i++)
  {
   for (unsigned j = 0; j < ncolumns-1; j++)
    {
     output_file << m[i][j] << ",";
    }
   // The last element without the ','
   output_file << m[i][ncolumns-1] << std::endl;
  }
 
 // Close the file
 output_file.close();
 
} // real_matrix_to_csv_file

/// Output boolean matrix into a csv file
void boolean_matrix_to_csv_file(bool **m, const unsigned nrows, const unsigned ncolumns, std::string &file_name)
{
 // Create file
 std::ofstream output_file(file_name, std::ios_base::out);
 
 for (unsigned i = 0; i < nrows; i++)
  {
   for (unsigned j = 0; j < ncolumns-1; j++)
    {
     output_file << m[i][j] << ",";
    }
   // The last element without the ','
   output_file << m[i][ncolumns-1] << std::endl;
  }
 
 // Close the file
 output_file.close();
 
} // boolean_matrix_to_csv_file


// Define a structure to hold all statistics
struct Statistics {
    Real mean_density;
    Real stdev_density;
    Real median_density;
    Real mean_current;
    Real stdev_current;
    Real median_current;
};

__device__ void d_compute_mean_channels_density(const bool* m, unsigned e_m, unsigned s_m, Real* density)
{
     /*
      Computes the mean channels density of the microtubule
      (cross-sectional density/along all channels/for each column)
    */
    // Move along the microtubule and compute the mean density
    for (unsigned i = 0; i < d_L; i++)
    {
        Real sum_occupation = 0.0;

        for (unsigned k = 0; k < d_N; k++)
        {
            unsigned idx = e_m + k * d_L + i;

            sum_occupation += static_cast<Real>(m[idx]);
        }
        // Mean of sum density
        density[s_m + i] = sum_occupation / static_cast<Real>(d_N);
    }
}


// CUDA function for lateral movement
__device__ void d_try_lateral_movement(bool *m, const unsigned N, const unsigned L, const unsigned k, 
                                      const unsigned i, const unsigned n_m, const unsigned e_m,
                                      curandState &state)
{

    // Double-check there is a particle at the current position
    if (!m[n_m + i])
    {
        return;
    }

    // Store the indices for the microtubule above and below
    unsigned index_microtubule_above = (k == 0) ? N - 1 : k - 1;
    unsigned index_microtubule_below = (k == N - 1) ? 0 : k + 1;

    // Use the thread's RNG state passed from d_mTASEP (without re-initializing)
    const Real r = curand_uniform(&state);

    // First choose the microtubule from above (preferred due to probability)
    if (r <= 0.5f)
    {
        // If the space at the above microtubule is free then move there!
        if (!m[e_m + index_microtubule_above * L + i])
        {
            m[n_m + i] = false;
            m[e_m + index_microtubule_above * L + i] = true;
        }
        // If the space at the below microtubule is free then move there!
        else if (!m[e_m + index_microtubule_below * L + i])
        {
            m[n_m + i] = false;
            m[e_m + index_microtubule_below * L + i] = true;
        }
    }
    // First choose the microtubule from below (preferred due to probability)
    else
    {
        if (!m[e_m + index_microtubule_below * L + i])
        {
            m[n_m + i] = false;
            m[e_m + index_microtubule_below * L + i] = true;
        }
        else if (!m[e_m + index_microtubule_above * L + i])
        {
            m[n_m + i] = false;
            m[e_m + index_microtubule_above * L + i] = true;
        }
    }
}


__device__ void d_mTASEP(bool* d_m, const unsigned e_m, const unsigned i_simulation, 
            const unsigned i_experiment,
            const unsigned d_N, const unsigned d_L,
            const Real alpha, const Real beta, const Real rho,
            const Real omega_in, const Real omega_out,
            bool lateral_movement, Real &mean_current,
            Real* mean_current_per_channel,                                      
            unsigned* step_forward_particles_list,
            unsigned* step_lateral_particles_list,
            curandState &state)
{
 /*
    Applies TASEP algorithm to a multichannel microtubule

    Return the updated microtubule as an numpy matrix

    This version implements the "(d) parallel_update" strategy as
    described in section 2.1 of "The Asymmetric Exclusion Process:
    Comparison of Update Procedures, N. Rajewsky et. al., Journal of
    Statistical Physics, Vol. 92, 1998"
 */
    // Bug 1 fix: Reset current at the beginning of each call (equivalent
    // to the vector being local in the MPI/CPU version)
    for (unsigned k = 0; k < d_N; k++) {
        mean_current_per_channel[e_m + k * d_L] = 0.0;
    }

    unsigned lateral_counts[32]; // Buffer for lateral move counts per channel

    // Perform the method for each channel
    for (unsigned k = 0; k < d_N; k++)
    {
        unsigned n_m = e_m + k * d_L;  // Start of channel per thread 
        // *******************************************
        // Apply TASEP-LK rules
        // *******************************************
        
        // -------------------------------------------
        /// Attach (omega in)
        // -------------------------------------------
        
        // Keep track of the position of the particle attached by omega_in
        // because the just attached particle cannot move
        bool omega_in_attached_a_particle = false;
        unsigned r_pos_omega_in = 0;
        
        if (omega_in > 0.0)
        {
            // Choose a random position at the microtubule
            const unsigned r_pos = static_cast<unsigned>(curand_uniform(&state) * d_L);
            
            // Generate a random number
            const Real r = curand_uniform(&state);
            
            // if r <= omega_in and d_m[k][r_pos] == 0 then add a particle to
            // the microtubule in that position
            if (r <= omega_in && d_m[n_m + r_pos] == 0)
            {
                // Attach a particle
                d_m[n_m + r_pos] = 1;
                
                // Keep track of the position of the particle attached by omega
                // in
                r_pos_omega_in = r_pos;
                omega_in_attached_a_particle = true;
            }
        }
        
        // -------------------------------------------
        /// Detach (omega out)
        // -------------------------------------------
        
        // Keep track of the position free by omega_out becasue this
        // position cannot be occupied by other particles in the current
        // simulation step
        bool omega_out_dettached_a_particle = false;
        unsigned r_pos_omega_out = 0;
        
        if (omega_out > 0.0)
        {
            // Choose a random position at the microtubule
            const unsigned r_pos = static_cast<unsigned>(curand_uniform(&state) * d_L);
            
            // Generate a random number
            const Real r = curand_uniform(&state);
            
            // if r <= omega_out and d_m[k][r_pos] == 1 then remove the
            // particle from that position of the microtubule ALSO check that
            // the particle is not there becasue it was just attached by the
            // omega_in process
            if (r <= omega_out && d_m[n_m + r_pos] == 1 && !(omega_in_attached_a_particle && r_pos == r_pos_omega_in))
            {
            // Detach the particle
            d_m[n_m + r_pos] = 0;
            
            r_pos_omega_out = r_pos;
            omega_out_dettached_a_particle = true;
            }    
        }
        
        // *******************************************
        // Update the boundaries (left and right)
        // *******************************************
        
        // -------------------------------------------
        /// LEFT END
        // -------------------------------------------
        
        // Compute a probability to add a particle at the beginning of the
        // microtubule
        const Real a = curand_uniform(&state);
        // A flag indicating whether a particle was added at the beginning
        // of the microtubule
        bool added_to_start = false;
        // Is a <= alpha and the first space is free? ALSO check whether
        // the space is free due to the dettaching process by omega_out, if
        // that is the case then we cannot add a particle to the start
        if (a <= alpha && d_m[n_m] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == 0))
            {
            // Add a particle to the start
            d_m[n_m] = 1;
            // Indicate we added a particle at the beginning of the
            // microtubule so there is no need to update its position
            added_to_start = true;
            
            }
        
        // -------------------------------------------
        /// RIGHT END
        // -------------------------------------------
        
        // Compute a probability to remove the last particle of the
        // microtubule
        const Real b = curand_uniform(&state);
        // A flag indicating whether a particle was removed from the last
        // cell of the microtubule
        bool removed_from_end = false;
        // Is b <= beta and the last space occupied, ALSO, was the particle
        // not attached by omega_in?
        if (b <= beta && d_m[n_m + d_L-1] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == d_L-1))
        {
            // Remove the particle from the microtubule
            d_m[n_m + d_L-1] = 0;
            // Indicate we removed a particle from the last position of the
            // microtubule so other particle should not step in this cell
            removed_from_end = true;
            
        }
        
        // *******************************************
        // Update particles in the microtubule
        // *******************************************
        
        // Update particles in the microtubule using a parallel update
        // strategy (delay movement, but keep track in a list of those
        // particles to update position)
        
        // -----------------------------------------------------------
        
        // Update the first and last indexes based on the added_to_start
        // and removed_from_end flags
        unsigned end_index = d_L - 2;
        if (removed_from_end)
        {
            end_index = d_L - 3;     
        }
        
        unsigned start_index = 0;
        if (added_to_start)
        {
            start_index = 1;
        }

        unsigned forward_count = 0;
        unsigned lateral_count = 0;
        
        // // Vector with particles' indexes that will move (step-forward)
        // std::vector<unsigned> step_forward_particles_list;
        // step_forward_particles_list.reserve(d_L);
            
        // // Vector with particles' indexes that will try lateral movement
        // std::vector<unsigned> step_lateral_particles_list;
        // step_lateral_particles_list.reserve(d_L);
        
        // Update the microtubule from left to right
        for (unsigned i = start_index; i <= end_index; i++)
        {
            // Check whether there is a particle at the current cell space
            // and ensure that particle was not introduced by the omega_in
            // process
            if (d_m[n_m + i] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == i))
            {
                // Check whether there is a free space at the next cell space
                // and ensure that space is not there becase the omega_out
                // process
                if (d_m[n_m + i + 1] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == i+1))
                {
                    // Compute a probability to move to the next space
                    const Real p = curand_uniform(&state);
                    if (p <= rho)
                    {
                        // Add the particle index to the step forward list
                        // step_forward_particles_list.push_back(i);
                        step_forward_particles_list[n_m + forward_count] = i;
                        forward_count++;
                    }

                    // Skip the cell at the next space since there is no particle
                    // there
                    ++i;
                
                } // if (d_m[k][i+1] == 0)
                else
                {
                    // Try lateral movement since there is a particle on the next
                    // cell OR the particle on the next cell was just removed by
                    // the omega_out process
                    // step_lateral_particles_list.push_back(i);
                    step_lateral_particles_list[n_m + lateral_count] = i;
                    lateral_count++;
                    
                } // else if (d_m[k][i+1] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == i+1))
            
            } // if (d_m[k][i] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == i))

        } // for (i <= end_index)
                
        // Perform movement of particles based on the step forward list
        // Cache the size of the list
        const unsigned step_forward_particle_list_size = forward_count;

        for (unsigned j = 0; j < step_forward_particle_list_size; j++)
        {
          // Get the index of the particle
          const unsigned i = step_forward_particles_list[n_m + j];
          d_m[n_m + i] = 0;
          d_m[n_m + i + 1] = 1;

          // Add the movement to the current per channel vector
          mean_current_per_channel[n_m]+=1;
        }
        
        // Store lateral move count for this channel
        if (d_lateral_movement && k < 32)
        {
            lateral_counts[k] = lateral_count;
        }
    
    } // for (k < d_N)

    // *******************************************
    // Apply lateral movement AFTER all forward moves
    // *******************************************
    if (d_lateral_movement)
    {
        // We use a fixed order for lateral moves to keep the animation stable,
        // but since forward moves are already done, the teleportation bias is gone.
        for (unsigned k = 0; k < d_N; k++)
        {
            unsigned n_m = e_m + k * d_L;
            const unsigned step_lateral_particle_list_size = (k < 32) ? lateral_counts[k] : 0;

            for (unsigned j = 0; j < step_lateral_particle_list_size; j++)
            {
                // Get the index of the particle
                const unsigned i = step_lateral_particles_list[n_m + j];
                d_try_lateral_movement(d_m, d_N, d_L, k, i, n_m, e_m, state);
            }
        }
    }
    
    // Compute the averaged current on all the microtubules
    mean_current = 0.0;
    for (unsigned i = 0; i < d_N; i++)
    {
        mean_current+=mean_current_per_channel[e_m + i * d_L];
    }
    
    const Real factor = 1.0/static_cast<Real>(d_L*d_N);
    
    mean_current=mean_current*factor;
}

// stride=1 for contiguous arrays, stride>1 when v points to a field in a struct
// (e.g. &stats[0].mean_density with stride=sizeof(Statistics)/sizeof(Real) = 6)
__device__ void d_statistics_mean(Real* v, unsigned v_size, Real &mean,
                                   unsigned stride = 1)
{
    Real sum = 0.0;
    for (unsigned i = 0; i < v_size; ++i)
        sum += v[i * stride];
    mean = sum / static_cast<Real>(v_size);
}

// Stride version: copy elements to local array tmp before operating,
// avoiding corruption of the struct memory when sorting in-place.
__device__ void d_statistics_mean_std_median(Real* v, unsigned v_size,
                                              Real &mean, Real &stdev, Real &median,
                                              unsigned stride = 1)
{
    // Copy elements (with stride) to a contiguous local buffer
    // max(d_n_data_to_gather, d_max_experiments) ≤ 32 in typical use
    Real tmp[64];
    for (unsigned i = 0; i < v_size; ++i)
        tmp[i] = v[i * stride];

    // Media
    Real sum = 0.0;
    for (unsigned i = 0; i < v_size; ++i) sum += tmp[i];
    mean = sum / static_cast<Real>(v_size);

    // Desviación estándar
    Real sq_sum = 0.0;
    for (unsigned i = 0; i < v_size; ++i) { Real d = tmp[i] - mean; sq_sum += d * d; }
    stdev = sqrt(sq_sum / static_cast<Real>(v_size));

    // Median (bubble sort on local copy)
    for (unsigned i = 0; i < v_size - 1; ++i)
        for (unsigned j = 0; j < v_size - i - 1; ++j)
            if (tmp[j] > tmp[j + 1]) { Real t = tmp[j]; tmp[j] = tmp[j+1]; tmp[j+1] = t; }

    median = (v_size % 2 == 0)
             ? (tmp[v_size/2 - 1] + tmp[v_size/2]) / 2.0
             : tmp[v_size / 2];
}



// ─────────────────────────────────────────────────────────────────────────────
// Animation Kernel: only runs extreme configurations and saves
// a microtubule snapshot (N×L bools) at each simulation step.
// One thread per extreme configuration (1 realization = enough to see dynamics).
// Typical size: 16 configs → ~1 MB of snapshots.
// ─────────────────────────────────────────────────────────────────────────────
__global__ void Run_animation_configurations(
    Real*     d_anim_configs,   // [n_extreme × 5]  extreme configuration parameters
    bool*     d_anim_m,         // [n_extreme × N × L]  evolving microtubule
    bool*     d_snap,           // [n_extreme × max_sims × N × L]  snapshots
    Real*     d_anim_current_ch,// [n_extreme × N × L] (current accumulator)
    unsigned* d_anim_fwd,       // [n_extreme × N × L] (step_forward_list)
    unsigned* d_anim_lat,       // [n_extreme × N × L] (step_lateral_list)
    unsigned  n_extreme)
{
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid >= (int)n_extreme) return;

    int i_extreme = tid;  // 1 thread per extreme configuration

    const Real alpha    = d_anim_configs[i_extreme * 5 + 0];
    const Real beta     = d_anim_configs[i_extreme * 5 + 1];
    const Real rho      = d_anim_configs[i_extreme * 5 + 2];
    const Real omega_in = d_anim_configs[i_extreme * 5 + 3];
    const Real omega_out= d_anim_configs[i_extreme * 5 + 4];

    // Microtubule base for this thread (reused in each step)
    unsigned e_m = (unsigned)tid * d_N * d_L;

    // Initialize microtubule to zero
    for (unsigned k = 0; k < d_N * d_L; k++)
        d_anim_m[e_m + k] = false;

    // Initialize RNG once
    curandState state;
    curand_init((unsigned long long)tid + 12345ULL, 0, 0, &state);

    for (unsigned i_sim = 0; i_sim < d_max_simulations_per_experiment; i_sim++)
    {
        Real local_current = 0.0;

        // Simulate one step on GPU using the same device function as the main kernel
        d_mTASEP(d_anim_m, e_m, i_sim, 0, d_N, d_L,
                 alpha, beta, rho, omega_in, omega_out,
                 d_lateral_movement, local_current,
                 d_anim_current_ch, d_anim_fwd, d_anim_lat,
                 state);

        // Save full microtubule snapshot at this step
        unsigned snap_base = ((unsigned)tid * d_max_simulations_per_experiment + i_sim) * d_N * d_L;
        for (unsigned k = 0; k < d_N * d_L; k++)
            d_snap[snap_base + k] = d_anim_m[e_m + k];
    }
}

// Main CUDA Kernel
__global__ void Run_all_configurations(Real* d_configurations, bool* d_m,  
                                        Real* d_density, 
                                        Statistics* d_exp_stats, 
                                        Statistics* d_conf_stats,
                                        Statistics* stats_experiment_raw,
                                        Real* mean_current_per_channel,                                      
                                        unsigned* step_forward_particles_list,
                                        unsigned* step_lateral_particles_list) {
    // Get global thread index
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    
    // Check if thread is within range of configurations
    if (tid < d_n_all_configurations) {
        // Get configuration parameters for this thread
        const Real alpha = d_configurations[tid * 5 + 0];
        const Real beta = d_configurations[tid * 5 + 1];
        const Real rho = d_configurations[tid * 5 + 2];
        const Real omega_in = d_configurations[tid * 5 + 3];
        const Real omega_out = d_configurations[tid * 5 + 4];
        
        // Perform operations with configuration parameters
        // For example, print them
        if (tid == 12345){ 
          printf("\nThread %d: alpha=%f, beta=%f, rho=%f, omega_in=%f, omega_out=%f\n", tid, alpha, beta, rho, omega_in, omega_out);
        }

        unsigned experiment_counter = 0;
        
        unsigned t_m = tid * d_max_experiments * d_N * d_L;   // Start of d_m per thread

        for (unsigned i_experiment = 0; i_experiment < d_max_experiments; i_experiment++)
        {
          unsigned e_m = t_m + i_experiment * d_N * d_L;      // Start of experiment per thread 

          for (unsigned i_cell = 0; i_cell < d_N * d_L; i_cell++)
          {
            d_m[e_m + i_cell] = 0;
          }

          if (tid == 0 && i_experiment == 0){ 
            printf("\nd_m was reset correctly %d: ", d_m[e_m + d_N * d_L - 1]);
          }
        
          unsigned simulation_counter = 0;

          // unsigned experiment = 

            // Initialize RNG state ONCE
            // Use tid and i_experiment to balance seeds
            curandState state;
            unsigned long long seed = (unsigned long long)tid + (unsigned long long)i_experiment * 10007ULL;
            curand_init(seed, 0, 0, &state);

            for (unsigned i_simulation_step = 0; i_simulation_step < d_max_simulations_per_experiment; i_simulation_step++)
            {
            // Store the current
                Real local_mean_current = 0.0;

                // Apply mTASEP
                d_mTASEP(d_m, e_m, i_simulation_step, i_experiment, d_N, d_L, 
                        alpha, beta, rho, omega_in, omega_out, 
                        d_lateral_movement, local_mean_current, mean_current_per_channel,                                      
                        step_forward_particles_list, step_lateral_particles_list,
                        state);

            // Compute density on the microtubule (direct mean from d_m, no external buffer)
            // This prevents the s_m overflow that happened when using d_density[s_m] with
            // s_m = e_m + i_simulation_step * d_L, which exceeded the thread's space.
            Real local_mean_density = 0.0;
            if (i_simulation_step >= d_simulation_step_to_start_gathering_data)
            {
              Real sum_density = 0.0;
              for (unsigned k = 0; k < d_N; k++)
                for (unsigned pos = 0; pos < d_L; pos++)
                  sum_density += static_cast<Real>(d_m[e_m + k * d_L + pos]);
              local_mean_density = sum_density / static_cast<Real>(d_N * d_L);

              unsigned data_idx = (tid * d_max_experiments + i_experiment) * d_n_data_to_gather + simulation_counter;

              stats_experiment_raw[data_idx].mean_density = local_mean_density;
              stats_experiment_raw[data_idx].mean_current = local_mean_current;

              // Increase counter
              simulation_counter++;
            }
          } // for (i_simulation_step < max_simulations_per_experiment)

          // Stride between elements of the same field in a Statistics struct array
          // sizeof(Statistics)/sizeof(Real) = 6  (mean_density, stdev_density, median_density,
          //                                        mean_current, stdev_current, median_current)
          constexpr unsigned STATS_STRIDE = sizeof(Statistics) / sizeof(Real);

          // Compute stats for density over the gathered simulation steps
          Real imean_density = 0.0;
          Real istdev_density = 0.0;
          Real imedian_density = 0.0;

          unsigned exp_start_idx = (tid * d_max_experiments + i_experiment) * d_n_data_to_gather;

          d_statistics_mean_std_median(&stats_experiment_raw[exp_start_idx].mean_density,
              d_n_data_to_gather, imean_density, istdev_density, imedian_density, STATS_STRIDE);

          unsigned tempidx = tid * d_max_experiments + experiment_counter;

          d_exp_stats[tempidx].mean_density   = imean_density;
          d_exp_stats[tempidx].stdev_density  = istdev_density;
          d_exp_stats[tempidx].median_density = imedian_density;

          // Compute stats for current over the gathered simulation steps
          Real imean_current = 0.0;
          Real istdev_current = 0.0;
          Real imedian_current = 0.0;

          d_statistics_mean_std_median(&stats_experiment_raw[exp_start_idx].mean_current,
              d_n_data_to_gather, imean_current, istdev_current, imedian_current, STATS_STRIDE);

          d_exp_stats[tempidx].mean_current   = imean_current;
          d_exp_stats[tempidx].stdev_current  = istdev_current;
          d_exp_stats[tempidx].median_current = imedian_current;

          experiment_counter++;

        } // for (i_experiment < max_experiments)

        // Average over experiments to get configuration statistics
        constexpr unsigned STATS_STRIDE2 = sizeof(Statistics) / sizeof(Real);

        Real imean_density = 0.0, istdev_density = 0.0, imedian_density = 0.0;
        Real imean_current = 0.0, istdev_current = 0.0, imedian_current = 0.0;

        unsigned tempidx_conf = tid * d_max_experiments;

        d_statistics_mean(&d_exp_stats[tempidx_conf].mean_density,   d_max_experiments, imean_density,   STATS_STRIDE2);
        d_statistics_mean(&d_exp_stats[tempidx_conf].stdev_density,  d_max_experiments, istdev_density,  STATS_STRIDE2);
        d_statistics_mean(&d_exp_stats[tempidx_conf].median_density, d_max_experiments, imedian_density, STATS_STRIDE2);
        d_statistics_mean(&d_exp_stats[tempidx_conf].mean_current,   d_max_experiments, imean_current,   STATS_STRIDE2);
        d_statistics_mean(&d_exp_stats[tempidx_conf].stdev_current,  d_max_experiments, istdev_current,  STATS_STRIDE2);
        d_statistics_mean(&d_exp_stats[tempidx_conf].median_current, d_max_experiments, imedian_current, STATS_STRIDE2);

        d_conf_stats[tid].mean_density   = imean_density;
        d_conf_stats[tid].stdev_density  = istdev_density;
        d_conf_stats[tid].median_density = imedian_density;
        d_conf_stats[tid].mean_current   = imean_current;
        d_conf_stats[tid].stdev_current  = istdev_current;
        d_conf_stats[tid].median_current = imedian_current;
    }

    // stats[tid].mean_density = alpha + beta + rho + omega_in + omega_out;
    // stats[tid].stdev_density = alpha * beta * rho * omega_in * omega_out;
    // stats[tid].median_density = (alpha + beta + rho + omega_in + omega_out) / 5;
}

/// Computes the mean channels density (the mean of the density
/// along all channels/for each column)
std::vector<Real> compute_mean_channels_density(bool **m, const unsigned N, const unsigned L)
{
 /*
   Computes the mean channels density of the microtubule
   (cross-sectional density/along all channels/for each column)
 */

 // The vector storing the mean density (initialised with zeroes)
 std::vector<Real> density(L, 0);
 
 // Move along the microtubule and compute the mean density
 for (unsigned i = 0; i < L; i++)
  {
   Real sum_occupation = 0.0;
   for (unsigned k = 0; k < N; k++)
    {
     sum_occupation+=m[k][i];
    }
   // Mean of sum density
   density[i] = sum_occupation / Real(N);
  }

 return density;
 
}

/// Tries to perform a lateral movement on the particule at position
/// (k, i) in the microtubule
void try_lateral_movement(bool **m, const unsigned N, const unsigned L, const unsigned k, const unsigned i)
{
 /*
   Performs lateral movement on a TASEP model, this check for up/down
   possibilities, does not accounts for diagonal movement
 */
 
 // Double-check there is a particle at the current position
 if (m[k][i] == 0)
  {
   return;
  }

 // Store the indices for the microtubule above and below
 unsigned index_microtubule_above = k - 1;
 unsigned index_microtubule_below = k + 1;
 
 // Correct the indexes for the microtubules above and below
 // (periodic-boundary conditions)
 if (k == 0)
  {
   index_microtubule_above = N - 1;
  }
 
 if (k == N-1)
  {
   index_microtubule_below = 0;
  }
 
 // Used to get a seed for the random number engine
 std::random_device rd;
 // Standard mersenne_twister_engine seeded with rd()
 std::mt19937 gen(rd());
 
 // Use dist to generate a random number into a Real in the range
 // [0,1)
 std::uniform_real_distribution<> dis(0.0, 1.0);
 
 // Generate a random number to choose between the above or below
 // microtubule
 const Real r = dis(gen);
 
 // First choose the microtubule from above (preferred due to probability)
 if (r <= 0.5)
  {
   // If the space at the above microtubule is free then move there!
   if (m[index_microtubule_above][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_above][i] = true;
    }
   // If the space at the below microtubule is free then move there!
   else if (m[index_microtubule_below][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_below][i] = true;
    }
  } // if (r <= 0.5)
 else // First choose the microtubule from below (preferred due to
      // probability)
  {
   if (m[index_microtubule_below][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_below][i] = true;
    }
   else if (m[index_microtubule_above][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_above][i] = true;
    }
  } // else if (r <= 0.5)
 
}

/// Perform mTASEP method
void mTASEP(bool **m, const unsigned N, const unsigned L,
            const Real alpha, const Real beta, const Real rho,
            const Real omega_in, const Real omega_out,
            bool lateral_movement, Real &mean_current)
{
 /*
    Applies TASEP algorithm to a multichannel microtubule

    Return the updated microtubule as an numpy matrix

    This version implements the "(d) parallel_update" strategy as
    described in section 2.1 of "The Asymmetric Exclusion Process:
    Comparison of Update Procedures, N. Rajewsky et. al., Journal of
    Statistical Physics, Vol. 92, 1998"
 */
 
 // Used to get a seed for the random number engine
 std::random_device rd;
 // Standard mersenne_twister_engine seeded with rd()
 std::mt19937 gen(rd());
 
 // Use dist to generate a random number into a Real in the range
 // [0,1)
 std::uniform_real_distribution<> dis(0.0, 1.0);
 
 // Used to generate a random position in the microtubule (including
 // the first and the last position)
 std::uniform_int_distribution<> dis_microtubule_size(0, L-1);
 
 // Compute the current for each channel on the microtubule
 std::vector<unsigned> mean_current_per_channel(N, 0);
 
 // Perform the method for each channel
 for (unsigned k = 0; k < N; k++)
  {
   // *******************************************
   // Apply TASEP-LK rules
   // *******************************************
   
   // -------------------------------------------
   /// Attach (omega in)
   // -------------------------------------------
   
   // Keep track of the position of the particle attached by omega_in
   // because the just attached particle cannot move
   bool omega_in_attached_a_particle = false;
   unsigned r_pos_omega_in = 0;
   
   if (omega_in > 0.0)
    {
     // Choose a random position at the microtubule
     const unsigned r_pos = dis_microtubule_size(gen);
     
     // Generate a random number
     const Real r = dis(gen);
     
     // if r <= omega_in and m[k][r_pos] == 0 then add a particle to
     // the microtubule in that position
     if (r <= omega_in && m[k][r_pos] == 0)
      {
       // Attach a particle
       m[k][r_pos] = 1;
       
       // Keep track of the position of the particle attached by omega
       // in
       r_pos_omega_in = r_pos;
       omega_in_attached_a_particle = true;
      }
    }
   
   // -------------------------------------------
   /// Detach (omega out)
   // -------------------------------------------
   
   // Keep track of the position free by omega_out becasue this
   // position cannot be occupied by other particles in the current
   // simulation step
   bool omega_out_dettached_a_particle = false;
   unsigned r_pos_omega_out = 0;
   
   if (omega_out > 0.0)
    {
     // Choose a random position at the microtubule
     const unsigned r_pos = dis_microtubule_size(gen);
     
     // Generate a random number
     const Real r = dis(gen);
     
     // if r <= omega_out and m[k][r_pos] == 1 then remove the
     // particle from that position of the microtubule ALSO check that
     // the particle is not there becasue it was just attached by the
     // omega_in process
     if (r <= omega_out && m[k][r_pos] == 1 && !(omega_in_attached_a_particle && r_pos == r_pos_omega_in))
      {
       // Detach the particle
       m[k][r_pos] = 0;
       
       r_pos_omega_out = r_pos;
       omega_out_dettached_a_particle = true;
       
      }
     
    }
   
   // *******************************************
   // Update the boundaries (left and right)
   // *******************************************
   
   // -------------------------------------------
   /// LEFT END
   // -------------------------------------------
   
   // Compute a probability to add a particle at the beginning of the
   // microtubule
   const Real a = dis(gen);
   // A flag indicating whether a particle was added at the beginning
   // of the microtubule
   bool added_to_start = false;
   // Is a <= alpha and the first space is free? ALSO check whether
   // the space is free due to the dettaching process by omega_out, if
   // that is the case then we cannot add a particle to the start
   if (a <= alpha && m[k][0] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == 0))
    {
     // Add a particle to the start
     m[k][0] = 1;
     // Indicate we added a particle at the beginning of the
     // microtubule so there is no need to update its position
     added_to_start = true;
     
    }
   
   // -------------------------------------------
   /// RIGHT END
   // -------------------------------------------
   
   // Compute a probability to remove the last particle of the
   // microtubule
   const Real b = dis(gen);
   // A flag indicating whether a particle was removed from the last
   // cell of the microtubule
   bool removed_from_end = false;
   // Is b <= beta and the last space occupied, ALSO, was the particle
   // not attached by omega_in?
   if (b <= beta && m[k][L-1] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == L-1))
    {
     // Remove the particle from the microtubule
     m[k][L-1] = 0;
     // Indicate we removed a particle from the last position of the
     // microtubule so other particle should not step in this cell
     removed_from_end = true;
     
    }
   
   // *******************************************
   // Update particles in the microtubule
   // *******************************************
   
   // Update particles in the microtubule using a parallel update
   // strategy (delay movement, but keep track in a list of those
   // particles to update position)
   
   // -----------------------------------------------------------
   
   // Update the first and last indexes based on the added_to_start
   // and removed_from_end flags
   unsigned end_index = L - 2;
   if (removed_from_end)
    {
     end_index = L - 3;     
    }
   
   unsigned start_index = 0;
   if (added_to_start)
    {
     start_index = 1;
    }
   
   // Vector with particles' indexes that will move (step-forward)
   std::vector<unsigned> step_forward_particles_list;
   step_forward_particles_list.reserve(L);
      
   // Vector with particles' indexes that will try lateral movement
   std::vector<unsigned> step_lateral_particles_list;
   step_lateral_particles_list.reserve(L);
   
   // Update the microtubule from left to right
   for (unsigned i = start_index; i <= end_index; i++)
    {
     // Check whether there is a particle at the current cell space
     // and ensure that particle was not introduced by the omega_in
     // process
     if (m[k][i] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == i))
      {
       // Check whether there is a free space at the next cell space
       // and ensure that space is not there becase the omega_out
       // process
       if (m[k][i+1] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == i+1))
        {
         // Compute a probability to move to the next space
         const Real p = dis(gen);
         if (p <= rho)
          {
           // Add the particle index to the step forward list
           step_forward_particles_list.push_back(i);
          }

         // Skip the cell at the next space since there is no particle
         // there
         ++i;
         
        } // if (m[k][i+1] == 0)
       else
        {
         // Try lateral movement since there is a particle on the next
         // cell OR the particle on the next cell was just removed by
         // the omega_out process
         step_lateral_particles_list.push_back(i);
         
        } // else if (m[k][i+1] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == i+1))
       
      } // if (m[k][i] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == i))

    } // for (i <= end_index)
        
   // Perform movement of particles based on the step forward list
   // Cache the size of the list
   const unsigned step_forward_particle_list_size = step_forward_particles_list.size();
   for (unsigned j = 0; j < step_forward_particle_list_size; j++)
    {
     // Get the index of the particle
     const unsigned i = step_forward_particles_list[j];
     m[k][i] = 0;
     m[k][i+1] = 1;
     // Add the movement to the current per channel vector
     mean_current_per_channel[k]+=1;
    }
   
   // *******************************************
   // Apply lateral movement
   // *******************************************
   if (lateral_movement)
    {
     const unsigned step_lateral_particle_list_size = step_lateral_particles_list.size();
     for (unsigned j = 0; j < step_lateral_particle_list_size; j++)
      {
       // Get the index of the particle
       const unsigned i = step_lateral_particles_list[j];
       try_lateral_movement(m, N, L, k, i);
      }
     
    }
   
  } // for (k < N)
 
 // Compute the averaged current on all the microtubules
 mean_current = 0.0;
 for (unsigned i = 0; i < N; i++)
  {
   mean_current+=mean_current_per_channel[i];
  }
 
 const Real factor = 1.0/static_cast<Real>(L*N);
 
 mean_current=mean_current*factor;
 
}

int main(int argc, const char** argv)
{
 // Initialise scicellxx
 initialise_scicellxx();

 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "mtasep-LK model for a multichannel microtubule with parallel update");
 
 // Add arguments
 
 // Optional
 parser.add_argument<unsigned>(args.L, "--L")
  .help("Size of the microtubule")
  .default_value("101");
 
 parser.add_argument<unsigned>(args.N, "--N")
  .help("Number of channels on the microtubule")
  .default_value("4");
 
 parser.add_argument<Real>(args.alpha_min, "--alpha_min")
  .help("The minimum probability to introduce a particle to the microtubule (at the first --left-- position of the microtubule)")
  .default_value("0.1");
 
 parser.add_argument<Real>(args.alpha_max, "--alpha_max")
  .help("The maximum probability to introduce a particle to the microtubule (at the first --left-- position of the microtubule)")
  .default_value("1.0");

 parser.add_argument<Real>(args.alpha_step, "--alpha_step")
  .help("The distance between alpha values")
  .default_value("0.1");
 
 parser.add_argument<unsigned>(args.alpha_n_points, "--alpha_n_points")
  .help("The number of points in the range alpha_min to alpha_max")
  .default_value("10");

 parser.add_argument<Real>(args.beta_min, "--beta_min")
  .help("The minimum probability to remove a particle from the microtubule (from the last --right-- position of the microtubule)")
  .default_value("0.1");

 parser.add_argument<Real>(args.beta_max, "--beta_max")
  .help("The maximum probability to remove a particle from the microtubule (from the last --right-- position of the microtubule)")
  .default_value("1.0");

 parser.add_argument<Real>(args.beta_step, "--beta_step")
  .help("The distance between beta values")
  .default_value("0.1");
 
 parser.add_argument<unsigned>(args.beta_n_points, "--beta_n_points")
  .help("The number of points in the range beta_min to beta_max")
  .default_value("10");
 
 parser.add_argument<Real>(args.rho_min, "--rho_min")
  .help("The minimum probability for a particle to step-forward")
  .default_value("0.8");
 
 parser.add_argument<Real>(args.rho_max, "--rho_max")
  .help("The maximum probability for a particle to step-forward")
  .default_value("1.0");

 parser.add_argument<Real>(args.rho_step, "--rho_step")
  .help("The distance between rho values")
  .default_value("0.1");
 
 parser.add_argument<unsigned>(args.rho_n_points, "--rho_n_points")
  .help("The number of points in the range rho_min to rho_max")
  .default_value("3");
 
 parser.add_argument<Real>(args.omega_in_min, "--omega_in_min")
  .help("The minimum probability for a particle to attach to a site in the microtubule")
  .default_value("0.0");
 
 parser.add_argument<Real>(args.omega_in_max, "--omega_in_max")
  .help("The maximum probability for a particle to attach to a site in the microtubule")
  .default_value("0.0");

 parser.add_argument<Real>(args.omega_in_step, "--omega_in_step")
  .help("The distance between omega_in values")
  .default_value("0.0");
 
 parser.add_argument<unsigned>(args.omega_in_n_points, "--omega_in_n_points")
  .help("The number of points in the range omega_in_min to omega_in_max")
  .default_value("1");
 
 parser.add_argument<Real>(args.omega_out_min, "--omega_out_min")
  .help("The minimum probability for a particle to detach from a site in the microtubule")
  .default_value("0.0");
 
 parser.add_argument<Real>(args.omega_out_max, "--omega_out_max")
  .help("The maximum probability for a particle to detach from a site in the microtubule")
  .default_value("0.0");

 parser.add_argument<Real>(args.omega_out_step, "--omega_out_step")
  .help("The distance between omega_out values")
  .default_value("0.0");
 
 parser.add_argument<unsigned>(args.omega_out_n_points, "--omega_out_n_points")
  .help("The number of points in the range omega_out_min to omega_out_max")
  .default_value("1");
 
 parser.add_argument<unsigned>(args.lateral_movement, "--lateral_movement")
  .help("Enables/disables lateral movement")
  .default_value("0");

 parser.add_argument<unsigned>(args.max_experiments, "--max_experiments")
  .help("Set the maximum number of experiments to perform")
  .default_value("1");
 
 parser.add_argument<unsigned>(args.max_simulations_per_experiment, "--max_simulations_per_experiment")
  .help("Set the maximum number of simulations per experiments")
  .default_value("200");
 
 parser.add_argument<unsigned>(args.simulation_step_to_start_gathering_data, "--simulation_step_to_start_gathering_data")
  .help("Set the simulation step to start gathering data (must be smaller than max_simulations_per_experiment)")
  .default_value("0");

 parser.add_argument<std::string>(args.root_output_folder, "--root_output_folder")
  .help("The root output folder")
  .default_value("RESLT");
 
 parser.add_argument<unsigned>(args.output_space_state_diagram, "--output_space_state_diagram")
  .help("Enables/disables output of the averaged space-time diagrams for all channels. Disable when performing large simulations")
  .default_value("0");
 
  parser.add_argument<unsigned>(args.output_microtubule_state, "--output_microtubule_state")
  .help("Enables/disables output of microtubule state (space-time diagrams for all channels). Disable when performing large simulations")
  .default_value("0");

 parser.add_argument<unsigned>(args.numBlocks, "--cudablocks")
  .help("Number of CUDA blocks")
  .default_value("32");
 
 parser.add_argument<unsigned>(args.blockSize, "--cudathreads")
  .help("Number of CUDA threads")
  .default_value("1024");
  
  // Parse the input arguments
  parser.parse_args(argc, argv);

  // Configure problem
  const unsigned L = args.L; // Size of the microtubule
  const unsigned N = args.N; // Number of channels of the microtubule
  const Real alpha_min = args.alpha_min;
  const Real alpha_max = args.alpha_max;
  const Real alpha_step = args.alpha_step;
  const unsigned alpha_n_points = args.alpha_n_points;
  const Real beta_min = args.beta_min;
  const Real beta_max = args.beta_max;
  const Real beta_step = args.beta_step;
  const unsigned beta_n_points = args.beta_n_points;
  const Real rho_min = args.rho_min;
  const Real rho_max = args.rho_max;
  const Real rho_step = args.rho_step;
  const unsigned rho_n_points = args.rho_n_points;
  const Real omega_in_min = args.omega_in_min;
  const Real omega_in_max = args.omega_in_max;
  const Real omega_in_step = args.omega_in_step;
  const unsigned omega_in_n_points = args.omega_in_n_points;
  const Real omega_out_min = args.omega_out_min;
  const Real omega_out_max = args.omega_out_max;
  const Real omega_out_step = args.omega_out_step;
  const unsigned omega_out_n_points = args.omega_out_n_points;
  bool lateral_movement = false;
  if (args.lateral_movement)
   {
    lateral_movement = true;
   }
  const unsigned max_experiments = args.max_experiments;
  const unsigned max_simulations_per_experiment = args.max_simulations_per_experiment;
  const unsigned simulation_step_to_start_gathering_data = args.simulation_step_to_start_gathering_data;
  std::string root_output_folder(args.root_output_folder);
  bool output_space_state_diagram = false;
  if (args.output_space_state_diagram)
   {
    output_space_state_diagram = true;
   }
  bool output_microtubule_state = false;
  if (args.output_microtubule_state)
   {
    output_microtubule_state = true;
   }

  // Validate parameters values
  if (simulation_step_to_start_gathering_data > max_simulations_per_experiment)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "There step number to start gathering data is greater than\n"
                  << "the maximum number of simulations per experiment\n"
                  << "simulation_step_to_start_gathering_data:" << simulation_step_to_start_gathering_data
                  << "max_simulations_per_experiment:" << max_simulations_per_experiment
                  << std::endl;
    throw SciCellxxLibError(error_message.str(),
                            SCICELLXX_CURRENT_FUNCTION,
                            SCICELLXX_EXCEPTION_LOCATION);
   }

  // If the output_space_state_diagram is ENABLED check whether the
  // max_simulations_per_experiment does not exceed a MAXIMUM value
  if (output_space_state_diagram && max_simulations_per_experiment > 1000)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You enabled the output_space_state_diagram but the\n"
                  << "max_simulations_per_experiment exceeds the MAXIMUM value (1,000).\n"
                  << "This may lead to memory issues due to this large memory requirement.\n"
                  << "You can DISABLED this error check if you are sure on what you are doing!\n"
                  << std::endl;
    throw SciCellxxLibError(error_message.str(),
                            SCICELLXX_CURRENT_FUNCTION,
                            SCICELLXX_EXCEPTION_LOCATION);
   }
  
  // Output formating (files names, folders names and output to files)
  const unsigned width_number = 5;
  const char fill_char = '0';
  const unsigned precision_real_values = 4;

  // This throws an error when using MPI since multiple cores try to
  // create the same output folder
// #ifndef SCICELLXX_USES_MPI
  // Create output directory
  SciCellxxFileSystem::create_directory(root_output_folder);
// #endif // #ifdef SCICELLXX_USES_MPI
  
  // The string stream for the rank (used on output filenames)
  // std::ostringstream ss_rank;
  // ss_rank << SciCellxxMPI::rank;
  
  // Output parameters to a file
  // std::string parameters_filename(root_output_folder + "/parameters_r" + ss_rank.str() + ".txt");

  const unsigned numBlocks = args.numBlocks; // Number of CUDA blocks
  const unsigned blockSize = args.blockSize; // Number of CUDA threads

  std::string parameters_filename(root_output_folder + "/parameters_r" + ".txt");
  output_parameters_to_file(parameters_filename, argc, argv, args);
  
  // ------------------------------------------------------------
  // Generate all configurations as the cartesian product of the
  // ranges of parameters
  // ------------------------------------------------------------

  // Create the "set/list" with all values for each parameter

  // Alphas
  std::vector<Real> alphas;
  SciCellxxLinearSpace::create_linear_space(alphas, alpha_min, alpha_max, alpha_step, alpha_n_points);
  // Betas
  std::vector<Real> betas;
  SciCellxxLinearSpace::create_linear_space(betas, beta_min, beta_max, beta_step, beta_n_points);
  
  // Rhos
  std::vector<Real> rhos;
  SciCellxxLinearSpace::create_linear_space(rhos, rho_min, rho_max, rho_step, rho_n_points);
  
  // Omegas_in
  std::vector<Real> omegas_in;
  SciCellxxLinearSpace::create_linear_space(omegas_in, omega_in_min, omega_in_max, omega_in_step, omega_in_n_points);
  
  // Omegas_out
  std::vector<Real> omegas_out;
  SciCellxxLinearSpace::create_linear_space(omegas_out, omega_out_min, omega_out_max, omega_out_step, omega_out_n_points);
  
  // Print the information only on the master core
  // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  //  {
    scicellxx_output << "Linear spaces:" << std::endl;
    scicellxx_output << "Alphas:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(alphas);
    scicellxx_output << "Betas:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(betas);
    scicellxx_output << "Rhos:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(rhos);
    scicellxx_output << "Omegas_in:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(omegas_in);
    scicellxx_output << "Omegas_out:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(omegas_out);
    scicellxx_output << std::endl;
  //  }
  
  // Create the list with the list of parameter values
  std::vector<std::vector<Real> > lists;
  lists.push_back(alphas);
  lists.push_back(betas);
  lists.push_back(rhos);
  lists.push_back(omegas_in);
  lists.push_back(omegas_out);

  // Print the information only on the master core
  // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  //  {
    scicellxx_output << "Computing cartesian product ..." << std::endl;
  //  }
  // Perform cartesian product
  std::vector<std::vector<Real> > configurations = SciCellxxCartesianProduct::product(lists);
  
  // Print the information only on the master core
  // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  //  {
    scicellxx_output << "Computing cartesian product [DONE]" << std::endl;
  //  }
  
  // Get the total number of configurations

  const unsigned n_all_configurations = configurations.size(); // Original
  // const unsigned n_all_configurations = 100;

  // Print the information only on the master core
  // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  //  {
    // Report the total number of configurations and the partitioning
    // for parallel computing
    scicellxx_output << "Total number of all configurations: " << n_all_configurations << std::endl;
    // scicellxx_output << "Number of cores: " << SciCellxxMPI::nprocs << std::endl;
  //  }

  // Print the information only on the master core
  // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  //  {
    // Only print the Cartesian product if PANIC mode is enabled
#ifdef SCICELLXX_PANIC_MODE
    // Report the cartesian product
    scicellxx_output << "Cartesian product:" << std::endl;
    SciCellxxCartesianProduct::print(configurations);
    scicellxx_output << std::endl;
#else
    scicellxx_output << "Cartesian product printing is DISABLED since PANIC_MODE is DISABLED" << std::endl;
#endif // #ifdef SCICELLXX_PANIC_MODE
  //  }
  
  // Validate that the number of cores is no larger than the number of
  // configurations
  // if ((unsigned)SciCellxxMPI::nprocs > n_all_configurations)
  //  {
  //       // Error message
  //   std::ostringstream error_message;
  //   error_message << "The number of cores is larger than the number of configurations.\n"
  //                 << "Reduce the number of cores such that each core process at least one configuration\n"
  //                 << std::endl;
  //   throw SciCellxxLibError(error_message.str(),
  //                           SCICELLXX_CURRENT_FUNCTION,
  //                           SCICELLXX_EXCEPTION_LOCATION);
  //  }
  
  // Compute the number of configurations per core
  // const unsigned n_configurations_per_core = n_all_configurations / SciCellxxMPI::nprocs;
  // const unsigned n_configurations_per_core = n_all_configurations;
  

  // The number of data to collect
  const unsigned n_data_to_gather = max_simulations_per_experiment - simulation_step_to_start_gathering_data;

  const unsigned tam_experiment = max_experiments * n_all_configurations;
  const unsigned tam_simulation = max_simulations_per_experiment * tam_experiment;
  const unsigned tam_steps = tam_experiment * N * L;
  // // Keep track of the means, standard deviation and median of the
  // // channel density space/state per configuration
  // std::vector<Real> mean_density(n_configurations_this_core);
  // std::vector<Real> stdev_density(n_configurations_this_core);
  // std::vector<Real> median_density(n_configurations_this_core);
  
  // // Keep track of the means, standard deviation and median of the
  // // microtubule current per configuration
  // std::vector<Real> mean_current(n_configurations_this_core);
  // std::vector<Real> stdev_current(n_configurations_this_core);
  // std::vector<Real> median_current(n_configurations_this_core);
  
  // unsigned config_counter = 0;

  /************************ Start CUDA ***************************/

  cudaMemcpyToSymbol(d_N, &N, sizeof(unsigned));
  cudaMemcpyToSymbol(d_L, &L, sizeof(unsigned));

  cudaMemcpyToSymbol(d_n_all_configurations, &n_all_configurations, sizeof(unsigned));
  cudaMemcpyToSymbol(d_max_experiments, &max_experiments, sizeof(unsigned));
  cudaMemcpyToSymbol(d_n_data_to_gather, &n_data_to_gather, sizeof(unsigned));

  cudaMemcpyToSymbol(d_max_simulations_per_experiment, &max_simulations_per_experiment, sizeof(unsigned));
  cudaMemcpyToSymbol(d_simulation_step_to_start_gathering_data, &simulation_step_to_start_gathering_data, sizeof(unsigned));
  cudaMemcpyToSymbol(d_lateral_movement, &lateral_movement, sizeof(bool));

  cudaMemcpyToSymbol(d_tam_experiment, &tam_experiment, sizeof(unsigned));
  cudaMemcpyToSymbol(d_tam_simulation, &tam_simulation, sizeof(unsigned));

  cudaProfilerStart();

  // Create a one-dimensional array to pass to the device
  std::vector<Real> flat_configurations(n_all_configurations * 5);
  for (int i = 0; i < n_all_configurations; ++i) {
      for (int j = 0; j < 5; ++j) {
          flat_configurations[i * 5 + j] = configurations[i][j];
      }
  }

  // Create and allocate memory for configurations on the device
  bool* d_m;
  unsigned* step_forward_particles_list;
  unsigned* step_lateral_particles_list;

  Real* d_density;
  Real* d_configurations;
  Real* mean_current_per_channel;

  Statistics* d_statistics;            // Statistics per experiment
  Statistics* d_statistics_config;    // Averaged statistics per configuration
  Statistics* d_statistics_experiment; // Raw data per simulation (step by step)

  Real* h_density = new Real[tam_simulation * L];
  Statistics* h_statistics = new Statistics[n_all_configurations];
  Statistics* h_statistics_experiment = new Statistics[tam_simulation * n_data_to_gather];

  CUDA_CHECK(cudaMalloc(&d_m, tam_experiment * N * L * sizeof(bool)));
  CUDA_CHECK(cudaMemset(d_m, 0, tam_experiment * N * L * sizeof(bool)));
  CUDA_CHECK(cudaMalloc(&d_density, tam_simulation * L * sizeof(Real)));
  CUDA_CHECK(cudaMemset(d_density, 0, tam_simulation * L * sizeof(Real)));

  CUDA_CHECK(cudaMalloc(&step_forward_particles_list, tam_steps * sizeof(unsigned)));
  CUDA_CHECK(cudaMalloc(&step_lateral_particles_list, tam_steps * sizeof(unsigned)));  

  CUDA_CHECK(cudaMalloc(&mean_current_per_channel, tam_steps * sizeof(Real)));
  CUDA_CHECK(cudaMalloc(&d_configurations, n_all_configurations * 5 * sizeof(Real)));

  CUDA_CHECK(cudaMalloc(&d_statistics, tam_experiment * sizeof(Statistics)));
  CUDA_CHECK(cudaMalloc(&d_statistics_config, n_all_configurations * sizeof(Statistics)));
  CUDA_CHECK(cudaMalloc(&d_statistics_experiment, tam_experiment * n_data_to_gather * sizeof(Statistics)));

  CUDA_CHECK(cudaMemcpy(d_configurations, flat_configurations.data(), n_all_configurations * 5 * sizeof(Real), cudaMemcpyHostToDevice));

  size_t memory_max = tam_steps * sizeof(Real);
  printf("Min memory required: %.2f GB\n", 100 * (float)memory_max / (1024 * 1024 * 1024));


  scicellxx_output << "\n\t ***** Simulation starts ***** \n" << std::endl;
  
  // Call kernel
  Run_all_configurations<<<numBlocks, blockSize>>>( d_configurations, 
                                                    d_m, d_density,
                                                    d_statistics,
                                                    d_statistics_config,
                                                    d_statistics_experiment, 
                                                    mean_current_per_channel,
                                                    step_forward_particles_list,
                                                    step_lateral_particles_list );

  // Wait for all threads to finish
  cudaDeviceSynchronize();

  // Check for errors
  cudaError_t error = cudaGetLastError();
  if (error != cudaSuccess) {
      printf("\n***** Error in kernel launch: %s\n", cudaGetErrorString(error));
      // You can add more debugging details here
      exit(-1);
  }

  scicellxx_output << "\n\n\t ***** Simulation finishes *****\n" << std::endl;

  cudaMemcpy(h_density, d_density, tam_simulation * L * sizeof(Real), cudaMemcpyDeviceToHost);
  cudaMemcpy(h_statistics, d_statistics_config, n_all_configurations * sizeof(Statistics), cudaMemcpyDeviceToHost);
  cudaMemcpy(h_statistics_experiment, d_statistics_experiment, tam_simulation * n_data_to_gather * sizeof(Statistics), cudaMemcpyDeviceToHost);

  // *** [MISSING FEATURE 2] Microtubule state output ***
  // Copy d_m from device to host only if output is required
  bool* h_m = nullptr;
  if (output_microtubule_state)
  {
    const size_t d_m_size = tam_experiment * N * L;
    h_m = new bool[d_m_size];
    cudaMemcpy(h_m, d_m, d_m_size * sizeof(bool), cudaMemcpyDeviceToHost);
  }

  // Deallocate memory on the device
  cudaFree(step_forward_particles_list);
  cudaFree(step_lateral_particles_list);
  cudaFree(mean_current_per_channel);
  cudaFree(d_statistics_experiment);
  cudaFree(d_configurations);
  cudaFree(d_statistics);
  cudaFree(d_statistics_config);
  cudaFree(d_density);
  cudaFree(d_m);

  // *** Space-state diagram output ***
  if (output_space_state_diagram)
  {
    std::string ssd_folder(root_output_folder + "/space_state_diagrams");
    SciCellxxFileSystem::create_directory(ssd_folder);
    scicellxx_output << "Writing space-state diagrams..." << std::endl;
    for (unsigned i_config = 0; i_config < n_all_configurations; i_config++)
    {
      const Real alpha    = flat_configurations[i_config * 5 + 0];
      const Real beta     = flat_configurations[i_config * 5 + 1];
      const Real rho      = flat_configurations[i_config * 5 + 2];
      const Real omega_in = flat_configurations[i_config * 5 + 3];
      const Real omega_out= flat_configurations[i_config * 5 + 4];

      std::ostringstream ss_a;   ss_a   << setprecision(precision_real_values) << alpha;
      std::ostringstream ss_b;   ss_b   << setprecision(precision_real_values) << beta;
      std::ostringstream ss_r;   ss_r   << setprecision(precision_real_values) << rho;
      std::ostringstream ss_oi;  ss_oi  << setprecision(precision_real_values) << omega_in;
      std::ostringstream ss_oo;  ss_oo  << setprecision(precision_real_values) << omega_out;

      for (unsigned i_experiment = 0; i_experiment < max_experiments; i_experiment++)
      {
        std::ostringstream ss_exp;
        ss_exp << std::setw(width_number) << std::setfill(fill_char) << i_experiment;

        const std::string ssd_filename(
          ssd_folder +
          "/space_state_exp" + ss_exp.str() +
          "_a" + ss_a.str() +
          "_b" + ss_b.str() +
          "_r" + ss_r.str() +
          "_oi" + ss_oi.str() +
          "_oo" + ss_oo.str() + ".csv");

        std::ofstream ssd_file(ssd_filename, std::ios_base::out);

        for (unsigned pos = 0; pos < L - 1; pos++)
          ssd_file << pos << ",";
        ssd_file << L - 1 << "\n";

        const unsigned e_density_base = (i_config * max_experiments + i_experiment) * max_simulations_per_experiment;

        for (unsigned i_sim = 0; i_sim < max_simulations_per_experiment; i_sim++)
        {
          const unsigned row_start = (e_density_base + i_sim) * L;
          for (unsigned pos = 0; pos < L - 1; pos++)
            ssd_file << h_density[row_start + pos] << ",";
          ssd_file << h_density[row_start + L - 1] << "\n";
        }
        ssd_file.close();
      }
    }
    scicellxx_output << "Writing space-state diagrams [DONE]" << std::endl;
  }

  // *** Final microtubule state output ***
  if (output_microtubule_state && h_m != nullptr)
  {
    std::string mts_folder(root_output_folder + "/microtubule_states");
    SciCellxxFileSystem::create_directory(mts_folder);
    scicellxx_output << "Writing microtubule states..." << std::endl;
    for (unsigned i_config = 0; i_config < n_all_configurations; i_config++)
    {
      const Real alpha    = flat_configurations[i_config * 5 + 0];
      const Real beta     = flat_configurations[i_config * 5 + 1];
      const Real rho      = flat_configurations[i_config * 5 + 2];
      const Real omega_in = flat_configurations[i_config * 5 + 3];
      const Real omega_out= flat_configurations[i_config * 5 + 4];

      std::ostringstream ss_a;   ss_a   << setprecision(precision_real_values) << alpha;
      std::ostringstream ss_b;   ss_b   << setprecision(precision_real_values) << beta;
      std::ostringstream ss_r;   ss_r   << setprecision(precision_real_values) << rho;
      std::ostringstream ss_oi;  ss_oi  << setprecision(precision_real_values) << omega_in;
      std::ostringstream ss_oo;  ss_oo  << setprecision(precision_real_values) << omega_out;

      for (unsigned i_experiment = 0; i_experiment < max_experiments; i_experiment++)
      {
        std::ostringstream ss_exp;
        ss_exp << std::setw(width_number) << std::setfill(fill_char) << i_experiment;

        const std::string mts_filename(
          mts_folder +
          "/microtubule_state_exp" + ss_exp.str() +
          "_a" + ss_a.str() +
          "_b" + ss_b.str() +
          "_r" + ss_r.str() +
          "_oi" + ss_oi.str() +
          "_oo" + ss_oo.str() + ".csv");

        std::ofstream mts_file(mts_filename, std::ios_base::out);

        const unsigned e_m_base = (i_config * max_experiments + i_experiment) * N * L;

        for (unsigned k = 0; k < N; k++)
        {
          for (unsigned pos = 0; pos < L - 1; pos++)
            mts_file << static_cast<int>(h_m[e_m_base + k * L + pos]) << ",";
          mts_file << static_cast<int>(h_m[e_m_base + k * L + L - 1]) << "\n";
        }
        mts_file.close();
      }
    }
    scicellxx_output << "Writing microtubule states [DONE]" << std::endl;
    delete[] h_m;
  }

  /************************ CUDA end ***************************/
  cudaProfilerStop();
  
  // // ****************************************************************************************
  // // Generate a single output file with the results from all processors
  // // ****************************************************************************************  
  // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  //  {
  //   scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Flushing gathered results into disk ..." << std::endl;
    
    // Open the file
    std::string output_final_results_filename(root_output_folder + "/output" + ".csv");
    std::ofstream output_final_results_file(output_final_results_filename, std::ios_base::out);
    // The header
    output_final_results_file << "id,alpha,beta,rho,omega_in,omega_out,density,std_density,median_density,current,std_current,median_current" << std::endl;
    
    // For each configuration
    for (unsigned i_config = 0; i_config < n_all_configurations; i_config++)
     {
      const unsigned start_index = i_config;
      
      // Get values for each configuration
      // const unsigned configuration_index = static_cast<unsigned>(data_received_on_master[start_index + 0]);
      const Real alpha = flat_configurations[start_index * 5 + 0];
      const Real beta = flat_configurations[start_index * 5 +  1];
      const Real rho = flat_configurations[start_index * 5 + 2];
      const Real omega_in = flat_configurations[start_index * 5 + 3];
      const Real omega_out = flat_configurations[start_index * 5 + 4];
      
      const Real imean_density = h_statistics[i_config].mean_density;
      const Real istdev_density = h_statistics[i_config].stdev_density;
      const Real imedian_density = h_statistics[i_config].median_density;
      
      const Real imean_current = h_statistics[i_config].mean_current;
      const Real istdev_current = h_statistics[i_config].stdev_current;
      const Real imedian_current = h_statistics[i_config].median_current;
      
      // Transform to string to output to file
      std::ostringstream ss_alpha;
      ss_alpha << setprecision(precision_real_values) << alpha;
      std::ostringstream ss_beta;
      ss_beta << setprecision(precision_real_values) << beta;
      std::ostringstream ss_rho;
      ss_rho << setprecision(precision_real_values) << rho;
      std::ostringstream ss_omega_in;
      ss_omega_in << setprecision(precision_real_values) << omega_in;
      std::ostringstream ss_omega_out;
      ss_omega_out << setprecision(precision_real_values) << omega_out;
      
      std::ostringstream ss_imean_density;
      ss_imean_density << setprecision(precision_real_values) << imean_density;
      std::ostringstream ss_istdev_density;
      ss_istdev_density << setprecision(precision_real_values) << istdev_density;
      std::ostringstream ss_imedian_density;
      ss_imedian_density << setprecision(precision_real_values) << imedian_density;
      
      std::ostringstream ss_imean_current;
      ss_imean_current << setprecision(precision_real_values) << imean_current;
      std::ostringstream ss_istdev_current;
      ss_istdev_current << setprecision(precision_real_values) << istdev_current;
      std::ostringstream ss_imedian_current;
      ss_imedian_current << setprecision(precision_real_values) << imedian_current;
      
      output_final_results_file << i_config << "," << ss_alpha.str() << "," << ss_beta.str() << "," << ss_rho.str() << "," << ss_omega_in.str() << "," << ss_omega_out.str() << "," << ss_imean_density.str() << "," << ss_istdev_density.str() << "," << ss_imedian_density.str() << "," << ss_imean_current.str() << "," << ss_istdev_current.str() << "," << ss_imedian_current.str() << std::endl;
      
      //scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "id:" << configuration_index << "\talpha:" << ss_alpha.str() << "\tbeta:" << ss_beta.str() << "\trho:" << ss_rho.str() << "\tomega_in:" << ss_omega_in.str() << "\tomega_out:" << ss_omega_out.str() << "\tdensity:" << ss_imean_density.str() << "\tdensity(std):" << ss_istdev_density.str() << "\tdensity(median):" << ss_imedian_density.str() << "\tcurrent:" << ss_imean_current.str() << "\tcurrent(std):" << ss_istdev_current.str() << "\tcurrent(median):" << ss_imedian_current.str() << std::endl;
      
     } // for (i_config < n_all_configurations)
    
    // Close the file
    output_final_results_file.close();
    
    // scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Flushing gathered results into disk [DONE]" << std::endl;
    scicellxx_output<< "Flushing gathered results into disk [DONE]" << std::endl;
    
  //  } // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  
  // *** Animation output for EXTREME configurations ***
  // An "extreme configuration" is one where ALL its parameters are at
  // their minimum or maximum value of the sweep. With 5 parameters there are up to 2^5 = 32 configs.
  // For these configs, the simulation is re-executed on the GPU and snapshots
  // of the microtubule are saved step-by-step, compatible with csv_to_jpeg_for_microtubule_animations.py.
  if (output_microtubule_state)
  {
    // ── 1. Identify extreme configurations (trivial to do on CPU) ──────────
    Real alpha_lo = alphas.front(),    alpha_hi = alphas.back();
    Real beta_lo  = betas.front(),     beta_hi  = betas.back();
    Real rho_lo   = rhos.front(),      rho_hi   = rhos.back();
    Real oin_lo   = omegas_in.front(), oin_hi   = omegas_in.back();
    Real oout_lo  = omegas_out.front(),oout_hi  = omegas_out.back();

    const Real tol = 1e-9;
    auto is_extreme = [&](Real v, Real lo, Real hi) {
      return std::abs(v - lo) < tol || std::abs(v - hi) < tol;
    };

    std::vector<unsigned> extreme_ids;
    std::vector<Real> extreme_flat;   // extreme config params (n_extreme × 5)
    for (unsigned i = 0; i < n_all_configurations; i++)
    {
      Real a  = flat_configurations[i*5+0];
      Real b  = flat_configurations[i*5+1];
      Real r  = flat_configurations[i*5+2];
      Real oi = flat_configurations[i*5+3];
      Real oo = flat_configurations[i*5+4];
      if (is_extreme(a,alpha_lo,alpha_hi) && is_extreme(b,beta_lo,beta_hi) &&
          is_extreme(r,rho_lo,rho_hi)     && is_extreme(oi,oin_lo,oin_hi)  &&
          is_extreme(oo,oout_lo,oout_hi))
      {
        extreme_ids.push_back(i);
        extreme_flat.push_back(a);
        extreme_flat.push_back(b);
        extreme_flat.push_back(r);
        extreme_flat.push_back(oi);
        extreme_flat.push_back(oo);
      }
    }

    const unsigned n_extreme      = extreme_ids.size();
    // 1 experiment per config (animations show dynamics, not statistics)
    const unsigned n_anim_threads  = n_extreme;

    scicellxx_output << "Extreme configurations found: " << n_extreme << std::endl;
    scicellxx_output << "Running animation kernel on GPU..." << std::endl;

    // ── 2. Calculate buffer sizes ────────────────────────────────────────
    // d_anim_m      : [n_anim_threads × N × L]  evolving microtubule
    // d_snap        : [n_anim_threads × max_sims × N × L]  snapshots per step
    // d_anim_current: [n_anim_threads × N × L]
    // d_anim_fwd    : [n_anim_threads × N × L]
    // d_anim_lat    : [n_anim_threads × N × L]
    const size_t anim_NL   = (size_t)n_anim_threads * N * L;
    const size_t snap_size = (size_t)n_anim_threads * max_simulations_per_experiment * N * L;

    // ── 3. Allocate buffers in GPU ──────────────────────────────────────────────
    Real*     d_anim_configs;
    bool*     d_anim_m;
    bool*     d_snap;
    Real*     d_anim_current_ch;
    unsigned* d_anim_fwd;
    unsigned* d_anim_lat;

    cudaMalloc(&d_anim_configs,    n_extreme * 5 * sizeof(Real));
    cudaMalloc(&d_anim_m,          anim_NL * sizeof(bool));
    cudaMalloc(&d_snap,            snap_size * sizeof(bool));
    cudaMalloc(&d_anim_current_ch, anim_NL * sizeof(Real));
    cudaMalloc(&d_anim_fwd,        anim_NL * sizeof(unsigned));
    cudaMalloc(&d_anim_lat,        anim_NL * sizeof(unsigned));

    cudaMemset(d_anim_m,  0, anim_NL * sizeof(bool));
    cudaMemset(d_snap,    0, snap_size * sizeof(bool));

    cudaMemcpy(d_anim_configs, extreme_flat.data(),
               n_extreme * 5 * sizeof(Real), cudaMemcpyHostToDevice);

    // ── 4. Launch animation kernel ─────────────────────────────────────────
    cudaGetLastError(); // discard any previous pending error in the queue

    int anim_block = 128;
    int anim_grid  = ((int)n_anim_threads + anim_block - 1) / anim_block;
    Run_animation_configurations<<<anim_grid, anim_block>>>(
        d_anim_configs, d_anim_m, d_snap,
        d_anim_current_ch, d_anim_fwd, d_anim_lat,
        n_extreme);

    // Check for launch error
    cudaError_t launch_err = cudaGetLastError();
    if (launch_err != cudaSuccess) {
        printf("Error LAUNCHING animation kernel: %s\n", cudaGetErrorString(launch_err));
    }

    // Check for execution error (syncs GPU)
    cudaError_t sync_err = cudaDeviceSynchronize();
    if (sync_err != cudaSuccess) {
        printf("Error in animation kernel EXECUTION: %s\n", cudaGetErrorString(sync_err));
    } else {
        scicellxx_output << "Animation kernel [DONE]" << std::endl;
    }

    // ── 5. Copy snapshots to host ────────────────────────────────────────────
    bool* h_snap = new bool[snap_size];
    cudaMemcpy(h_snap, d_snap, snap_size * sizeof(bool), cudaMemcpyDeviceToHost);

    cudaFree(d_anim_configs);
    cudaFree(d_anim_m);
    cudaFree(d_snap);
    cudaFree(d_anim_current_ch);
    cudaFree(d_anim_fwd);
    cudaFree(d_anim_lat);

    // ── 6. Write CSVs from host ───────────────────────────────────────────
    std::string animations_root(root_output_folder + "/animations");
    SciCellxxFileSystem::create_directory(animations_root);

    scicellxx_output << "Writing microtubule animations for extreme configurations..." << std::endl;

    for (unsigned ei = 0; ei < n_extreme; ei++)
    {
      const Real alpha    = extreme_flat[ei*5+0];
      const Real beta     = extreme_flat[ei*5+1];
      const Real rho      = extreme_flat[ei*5+2];
      const Real omega_in = extreme_flat[ei*5+3];
      const Real omega_out= extreme_flat[ei*5+4];

      std::ostringstream ss_a;   ss_a   << setprecision(precision_real_values) << alpha;
      std::ostringstream ss_b;   ss_b   << setprecision(precision_real_values) << beta;
      std::ostringstream ss_r;   ss_r   << setprecision(precision_real_values) << rho;
      std::ostringstream ss_oi;  ss_oi  << setprecision(precision_real_values) << omega_in;
      std::ostringstream ss_oo;  ss_oo  << setprecision(precision_real_values) << omega_out;

      // One folder per extreme configuration (no experiment index)
      std::string anim_folder(
        animations_root +
        "/anim_a" + ss_a.str() + "_b" + ss_b.str() +
        "_r" + ss_r.str() + "_oi" + ss_oi.str() +
        "_oo" + ss_oo.str());
      SciCellxxFileSystem::create_directory(anim_folder);

      // tid in kernel = ei (one thread per extreme configuration)
      for (unsigned i_sim = 0; i_sim < max_simulations_per_experiment; i_sim++)
      {
        std::ostringstream ss_step;
        ss_step << std::setw(width_number) << std::setfill(fill_char) << i_sim;
        std::string csv_filename(anim_folder + "/microtubule_" + ss_step.str() + ".csv");

        std::ofstream csv_file(csv_filename, std::ios_base::out);

        // snapshot base: [ei * max_sims + i_sim] × N × L
        unsigned snap_base = (ei * max_simulations_per_experiment + i_sim) * N * L;

        for (unsigned k = 0; k < N; k++)
        {
          for (unsigned pos = 0; pos < L - 1; pos++)
            csv_file << static_cast<int>(h_snap[snap_base + k * L + pos]) << ",";
          csv_file << static_cast<int>(h_snap[snap_base + k * L + L - 1]) << "\n";
        }
        csv_file.close();
      }
    }

    delete[] h_snap;
    scicellxx_output << "Writing microtubule animations [DONE]" << std::endl;
  }

  // Finalise chapcom
  finalise_scicellxx();

  delete[] h_statistics;
  delete[] h_statistics_experiment;
  delete[] h_density;
  
  return 0;
  
}

