// cuda_functions.h

#ifndef CUDA_FUNCTIONS_H
#define CUDA_FUNCTIONS_H

#ifdef TYPEDEF_REAL_IS_DOUBLE
 typedef double Real;
#else
 typedef float Real;
#endif // #ifdef TYPEDEF_REAL_IS_DOUBLE

#include "../../../src/scicellxx.h"


__device__ __constant__ unsigned d_N;
__device__ __constant__ unsigned d_L;
__device__ __constant__ unsigned d_n_all_configurations;
__device__ __constant__ unsigned d_max_experiments;
__device__ __constant__ unsigned d_n_data_to_gather;
__device__ __constant__ unsigned d_max_simulations_per_experiment;
__device__ __constant__ unsigned d_simulation_step_to_start_gathering_data;
__device__ __constant__ unsigned d_tam_experiment;
__device__ __constant__ unsigned d_tam_simulation;
__device__ __constant__ bool d_lateral_movement;


#endif // CUDA_FUNCTIONS_H
