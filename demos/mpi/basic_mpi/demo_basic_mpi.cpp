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

/// This demo implements basic MPI features such as message passing,
/// reduction and all to all send data

// Include SciCell++ libraries
#include "../../../src/scicellxx.h"

// Use the namespace of the framework
using namespace scicellxx;

int main(int argc, const char** argv)
{
 // Initialise scicellxx
 initialise_scicellxx();
 
 // Output for testing/validation
 std::ofstream output_test("output_test.dat", std::ios_base::out);
 
 // Get the number of processors
 const int nprocs = SciCellxxMPI::nprocs;
 
 // Get the rank of the current processor
 const int my_rank = SciCellxxMPI::rank;

 // Cache the communicator
 MPI_Comm comm = SciCellxxMPI::comm;
 
 // -------------------
 // One-to-one (blocking) communications
 // Review this post on one-to-one MPI communication
 // https://mpitutorial.com/tutorials/mpi-send-and-receive/
 // -------------------
 // Communicate processor zero with processor one
 
 // Note that these are blocking communications, it means, the sending
 // and receiving cores block their execution until all data has been
 // sent and received
 
 // Processor zero and one perform an specific task
 int sending_core = 0;
 int receiving_core = 1;
 if (my_rank == sending_core)
  {
   int sent_data = my_rank;
   scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Processor: " << my_rank << " sending data ...";
   MPI_Send(&sent_data, 1, MPI_INT, receiving_core, 0, comm);
  }
 else if (my_rank == receiving_core)
  {
   int received_data = 0;
   MPI_Status status; // Ignore status
   MPI_Recv(&received_data, 1, MPI_INT, sending_core, 0, comm, &status);
   scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Processor: " << my_rank << " received data ...";
  }
 
 // -------------------
 // Colllective with operations communication
 // Review this post on collective communication with operations
 // https://mpitutorial.com/tutorials/mpi-reduce-and-allreduce/
 // -------------------
 
 // -------------------
 // Reduce (sum)
 // -------------------
 const int master_core = SciCellxxMPI::master_core;
 
 int mpi_reduce_sum_on_master_core = 0;
 MPI_Reduce(&my_rank, &mpi_reduce_sum_on_master_core, 1, MPI_INT, MPI_SUM,
            master_core, comm);
 
 // On master core, output the value of the sum
 if (my_rank == master_core)
  {
   scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << mpi_reduce_sum_on_master_core;
  }
 
 // -------------------
 // Reduce (max)
 // -------------------
 int mpi_reduce_max_on_master_core = 0;
 MPI_Reduce(&my_rank, &mpi_reduce_max_on_master_core, 1, MPI_INT, MPI_MAX,
            master_core, comm);
 
 // On master core, output the max value
 if (my_rank == master_core)
  {
   scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << mpi_reduce_max_on_master_core;
  }
 
 // -------------------
 // Reduce (min)
 // -------------------
 int mpi_reduce_min_on_master_core = 0;
 MPI_Reduce(&my_rank, &mpi_reduce_min_on_master_core, 1, MPI_INT, MPI_MIN,
            master_core, comm);

 // On master core, output the min value
 if (my_rank == master_core)
  {
   scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << mpi_reduce_min_on_master_core;
  }

 // -------------------
 // Synchronize (wait for all processors to reach this point, and the
 // continue)
 // Review this post synchronization and broadcast (collective communication)
 // https://mpitutorial.com/tutorials/mpi-broadcast-and-collective-communication/
 // -------------------
 scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Core: " << my_rank
                  << " reaching synchronization point" << std::endl;
 MPI_Barrier(comm);
 scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Core: " << my_rank
                  << " passing synchronization point" << std::endl;

 // -------------------
 // Report back the results to all processors (broadcast)
 // -------------------
 MPI_Bcast(&mpi_reduce_sum_on_master_core, 1, MPI_INT, master_core, comm);
 MPI_Bcast(&mpi_reduce_max_on_master_core, 1, MPI_INT, master_core, comm);
 MPI_Bcast(&mpi_reduce_min_on_master_core, 1, MPI_INT, master_core, comm);
 
 // Output results
 scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << mpi_reduce_sum_on_master_core;
 scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << mpi_reduce_max_on_master_core;
 scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << mpi_reduce_min_on_master_core;
  
 // -------------------
 // Allgather (collective selective communication)
 // Review this post on collective selective communication
 // https://mpitutorial.com/tutorials/mpi-scatter-gather-and-allgather/
 // -------------------
  
 // Generate a vector with the size of the number of processors
 int *processors_ranks = new int[nprocs];
 
 // All processors send its rank to each other (AllGather)
 MPI_Allgather(&my_rank, 1, MPI_INT,
               processors_ranks, 1, MPI_INT,
               comm);
 
 for (int i = 0; i < nprocs; i++)
  {
   scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << processors_ranks[i];
  }
 std::cout << std::endl; 
 

 /*

  
 // Output formating (files names, folders names and output to files)
 const unsigned width_number = 5;
 const char fill_char = '0';
 const unsigned precision_real_values = 4;


 
 // The string stream for the rank (used on output filenames)
 std::ostringstream ss_rank;
 ss_rank << SciCellxxMPI::rank;


 
 std::ostringstream ss;
 ss << std::setw(width_number) << std::setfill(fill_char) << std::to_string(i_simulation_step);
 
 
 std::ostringstream ss_alpha;
 ss_alpha << setprecision(precision_real_values) << alpha;
 
 scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "alpha:" << ss_alpha.str() << std::endl;
 
 // Close the output for test
 output_test.close();
 
 */

 // Finalise scicellxx
 finalise_scicellxx();
 
 return 0;
 
}

