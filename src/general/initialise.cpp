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
// Include the initialisation rutines headers
#include "initialise.h"

namespace scicellxx
{
 /// Keep track of the total time spent in a program, since
 /// initilaisation up-to termination
 namespace Global_timings
 {
  // Initial time
  time_t Global_initial_wall_time=0;
  // Initial CPU clock time
  clock_t Global_initial_cpu_clock_time=0;
 }

 /// In charge of initialising time variables to keep track of execution time
 bool initialise_scicellxx()
 {
  scicellxx_output << std::endl;
  scicellxx_output << "---------------------------------------" << std::endl;
  scicellxx_output << "Initialising SciCell++ .." << std::endl;
  // These variables are defined in the header file
  // cmake_definitions.h, which is automatically generated from the
  // cmake_definitions.h.in file
  scicellxx_output << "This is SciCell++ version "
                  << MAJOR_VERSION
                  << "."
                  << MINOR_VERSION
                  << "."
                  << PATCH_LEVEL
                   << std::endl;
  
  print_compiler_version_information();
  
  /// Setup terminate helper
  TerminateHelper::setup();
  
  // Initialise global wall time
  Global_timings::Global_initial_wall_time=Timing::wall_time();
  // Initialise CPU clock time
  Global_timings::Global_initial_cpu_clock_time=Timing::cpu_clock_time();

#ifdef SCICELLXX_USES_MPI
  // Initialise MPI
  initialise_MPI();
#endif // #ifdef SCICELLXX_USES_MPI
  
  scicellxx_output << "[DONE]: SciCell++ initialisation" << std::endl;
  scicellxx_output << "---------------------------------------" << std::endl;
  scicellxx_output << std::endl;
  
  // Everything was alright
  return true;
  
 }
 
 /// In charge of finalising and reporting execution times
 bool finalise_scicellxx()
 {
  scicellxx_output << std::endl;
  scicellxx_output << "---------------------------------------" << std::endl;
  scicellxx_output << "Finalising SciCell++ ..." << std::endl;

#ifdef SCICELLXX_USES_MPI
  // Finalise MPI
  finalise_MPI();
#endif // #ifdef SCICELLXX_USES_MPI
  
  // Get the wall and cpu execution time of the program
  time_t final_wall_time = Timing::wall_time();
  clock_t final_cpu_clock_time = Timing::cpu_clock_time();
  double total_wall_time =
   Timing::diff_wall_time(Global_timings::Global_initial_wall_time,
                          final_wall_time);
  double total_cpu_clock_time =
   Timing::diff_cpu_clock_time(Global_timings::Global_initial_cpu_clock_time,
                               final_cpu_clock_time);
  
  scicellxx_output << "Total wall time: " << total_wall_time << std::endl;
  scicellxx_output << "Total cpu clock time: " << total_cpu_clock_time << std::endl; 
  
  scicellxx_output << "[DONE]: SciCellxx termination" << std::endl;
  scicellxx_output << "---------------------------------------" << std::endl;
  scicellxx_output << std::endl;
  // Everything was alright
  
  return true;
 }

#ifdef SCICELLXX_USES_MPI
 /// Initialise MPI
 bool initialise_MPI()
 {
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "Initialising MPI ..." << std::endl;
  
  // Initialize MPI
  MPI_Init(NULL, NULL);
  
  // Get the number of processes
  MPI_Comm_size(SciCellxxMPI::comm, &SciCellxxMPI::nprocs);
  
  // Get the rank of the process
  MPI_Comm_rank(SciCellxxMPI::comm, &SciCellxxMPI::rank);
  
  char processor_name[100];
  int processor_name_length;
  // Get the name of the processor
  MPI_Get_processor_name(processor_name, &processor_name_length);
  
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "Initialising MPI on processor [" << processor_name
                   << "], rank " << SciCellxxMPI::rank << " out of " << SciCellxxMPI::nprocs << " processors." << std::endl;
  
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "---------------------------------------" << std::endl;
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "[DONE]: MPI initialisation" << std::endl;
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "---------------------------------------" << std::endl;

  return true;
 }
 
 /// Finalise MPI
 bool finalise_MPI()
 {
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "Finalising MPI ..." << std::endl;
  
  //scicellxx_output << "Processor [" << SciCellxxMPI::rank << "] waiting on MPI_Barrier for finalisation" << std::endl;
  //MPI_Barrier(SciCellxxMPI::comm);
  //scicellxx_output << "Processor [" << SciCellxxMPI::rank << "] passed the MPI_Barrier for finalisation" << std::endl;
  
  // Finalize the MPI environment.
  MPI_Finalize();
  
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "---------------------------------------" << std::endl;
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "[DONE]: MPI termination" << std::endl;
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
                   << "---------------------------------------" << std::endl;
  
  return true;
 }
#endif // #ifdef SCICELLXX_USES_MPI
 
 /// Print compiler version information
 void print_compiler_version_information()
 {
  // Took from this post
  // https://stackoverflow.com/questions/2324658/how-to-determine-the-version-of-the-c-standard-used-by-the-compiler
  
  // Checkt the __cplusplus macro
  scicellxx_output << "C++ compiler version: ";
  if (__cplusplus == 201703L) scicellxx_output << "C++17\n";
  else if (__cplusplus == 201402L) scicellxx_output << "C++14\n";
  else if (__cplusplus == 201103L) scicellxx_output << "C++11\n";
  else if (__cplusplus == 199711L) scicellxx_output << "C++98\n";
  else scicellxx_output << "pre-standard C++\n";
  scicellxx_output << "The __cplusplus macro value is: " << __cplusplus << std::endl;
 }
 
}
