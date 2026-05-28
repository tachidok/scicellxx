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
/// reduction, broadcast, gather, and allgather operations, showing
/// how to use them within SciCell++.

// Include SciCell++ libraries
#include "../../../src/scicellxx.h"

// Use the namespace of the framework
using namespace scicellxx;

int main(int argc, const char** argv)
{
  // Initialise scicellxx
  initialise_scicellxx();
  
  // Get the number of processors
  const int nprocs = SciCellxxMPI::nprocs;
  
  // Get the rank of the current processor
  const int my_rank = SciCellxxMPI::rank;

  // Cache the communicator
  MPI_Comm comm = SciCellxxMPI::comm;
  
  // The master core rank
  const int master_core = SciCellxxMPI::master_core;

  // We require at least 2 processors for the one-to-one communication demo
  if (nprocs < 2)
  {
    if (my_rank == master_core)
    {
      std::cerr << "Error: This demo requires at least 2 processors." << std::endl;
    }
    finalise_scicellxx();
    return 1;
  }

  // ------------------------------------------------------------------
  // 1. One-to-one (blocking) communication
  // ------------------------------------------------------------------
  int one_to_one_val = 0;
  if (my_rank == 0)
  {
    int sent_data = 42;
    scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Rank 0 sending " << sent_data << " to Rank 1." << std::endl;
    MPI_Send(&sent_data, 1, MPI_INT, 1, 0, comm);
    
    // Wait for response from Rank 1
    MPI_Status status;
    MPI_Recv(&one_to_one_val, 1, MPI_INT, 1, 0, comm, &status);
    scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Rank 0 received response " << one_to_one_val << " from Rank 1." << std::endl;
  }
  else if (my_rank == 1)
  {
    int received_data = 0;
    MPI_Status status;
    MPI_Recv(&received_data, 1, MPI_INT, 0, 0, comm, &status);
    scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Rank 1 received " << received_data << " from Rank 0." << std::endl;
    
    // Compute received_data * 2 and send it back to Rank 0
    int response_data = received_data * 2;
    scicellxx_output << MPI_RANK_NPROCS_PRINT(my_rank, nprocs) << "Rank 1 sending response " << response_data << " back to Rank 0." << std::endl;
    MPI_Send(&response_data, 1, MPI_INT, 0, 0, comm);
  }

  // Synchronize before moving to the next section
  MPI_Barrier(comm);

  // ------------------------------------------------------------------
  // 2. Collective Reduce operations
  // ------------------------------------------------------------------
  int reduce_sum_int = 0;
  int reduce_max_int = 0;
  int reduce_min_int = 0;
  
  MPI_Reduce(&my_rank, &reduce_sum_int, 1, MPI_INT, MPI_SUM, master_core, comm);
  MPI_Reduce(&my_rank, &reduce_max_int, 1, MPI_INT, MPI_MAX, master_core, comm);
  MPI_Reduce(&my_rank, &reduce_min_int, 1, MPI_INT, MPI_MIN, master_core, comm);

  // Also perform a reduction with Real values to test MPI_SC_REAL
  Real my_rank_real = static_cast<Real>(my_rank) * 1.5;
  Real reduce_sum_real = 0.0;
  MPI_Reduce(&my_rank_real, &reduce_sum_real, 1, MPI_SC_REAL, MPI_SUM, master_core, comm);

  // Synchronize
  MPI_Barrier(comm);

  // ------------------------------------------------------------------
  // 3. Collective Broadcast operation
  // ------------------------------------------------------------------
  int broadcast_val = 0;
  if (my_rank == master_core)
  {
    broadcast_val = 100;
  }
  
  // Broadcast the value from master core to all processes
  MPI_Bcast(&broadcast_val, 1, MPI_INT, master_core, comm);
  
  // ------------------------------------------------------------------
  // 4. Collective Gather operation
  // ------------------------------------------------------------------
  // Each processor computes gather_input = broadcast_val + my_rank
  int gather_input = broadcast_val + my_rank;
  std::vector<int> gathered_values;
  if (my_rank == master_core)
  {
    gathered_values.resize(nprocs);
  }
  
  MPI_Gather(&gather_input, 1, MPI_INT, gathered_values.data(), 1, MPI_INT, master_core, comm);

  // ------------------------------------------------------------------
  // 5. Collective Allgather operation
  // ------------------------------------------------------------------
  // Each processor contributes allgather_input = my_rank * 10
  int allgather_input = my_rank * 10;
  std::vector<int> allgathered_values(nprocs);
  
  MPI_Allgather(&allgather_input, 1, MPI_INT, allgathered_values.data(), 1, MPI_INT, comm);

  // ------------------------------------------------------------------
  // 6. Write results to output_test.dat (only from Master Core)
  // ------------------------------------------------------------------
  if (my_rank == master_core)
  {
    std::ofstream output_test("output_test.dat", std::ios_base::out);
    if (output_test.is_open())
    {
      output_test << "MPI Basic Demo Results" << std::endl;
      output_test << "----------------------" << std::endl;
      output_test << "Number of processors: " << nprocs << std::endl;
      output_test << "Master core: " << master_core << std::endl;
      output_test << std::endl;

      output_test << "One-to-one communication:" << std::endl;
      output_test << "  Rank 0 sent: 42" << std::endl;
      output_test << "  Rank 1 received and processed: 84" << std::endl;
      output_test << "  Rank 0 received back: " << one_to_one_val << std::endl;
      output_test << std::endl;

      output_test << "Reduce operations (Integer Ranks):" << std::endl;
      output_test << "  Sum of ranks: " << reduce_sum_int << std::endl;
      output_test << "  Max of ranks: " << reduce_max_int << std::endl;
      output_test << "  Min of ranks: " << reduce_min_int << std::endl;
      output_test << std::endl;

      output_test << "Reduce operation (Real Ranks * 1.5):" << std::endl;
      output_test << "  Sum of real ranks: " << reduce_sum_real << std::endl;
      output_test << std::endl;

      output_test << "Broadcast and Gather operation:" << std::endl;
      output_test << "  Broadcasted value: " << broadcast_val << std::endl;
      output_test << "  Gathered values from all processors:" << std::endl;
      for (int i = 0; i < nprocs; ++i)
      {
        output_test << "    Rank " << i << ": " << gathered_values[i] << std::endl;
      }
      output_test << std::endl;

      output_test << "Allgather operation:" << std::endl;
      output_test << "  Allgathered values across all processors:" << std::endl;
      for (int i = 0; i < nprocs; ++i)
      {
        output_test << "    Rank " << i << ": " << allgathered_values[i] << std::endl;
      }
      
      output_test.close();
    }
    else
    {
      std::cerr << "Error: Could not open output_test.dat for writing." << std::endl;
    }
  }

  // Finalise scicellxx
  finalise_scicellxx();
  
  return 0;
}
