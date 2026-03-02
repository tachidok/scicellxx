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
#include "ac_problem.h"

namespace scicellxx
{

 // ===================================================================
 /// Constructor, in charge of initialising any stuff required for the
 /// framework
 // ===================================================================
 ACProblem::ACProblem(const unsigned dim)
  : Output_file_index(0),
    Dim(dim),
    Allow_free_memory_for_U(false),
    N_equations(0)
 { 
  
 }
 
 // ===================================================================
 /// Destructor
 // ===================================================================
 ACProblem::~ACProblem()
 {
  if (Allow_free_memory_for_U)
   {
    // Free memory
    delete U_pt;
    // Set pointer to null
    U_pt = 0;
    // Disabled free memory for U
    Allow_free_memory_for_U = false;
   }
  
 }
 
 // ===================================================================
 /// Document nodes positions
 // ===================================================================
 void ACProblem::document_nodes_positions(std::string &filename)
 {
  // Output supporting nodes
  std::ofstream nodes_file(filename.c_str());
  
  // Get the number of nodes
  const unsigned long nnodes = n_nodes();
  
  // Loop over the nodes and output their position to a file
  for (unsigned long i = 0; i < nnodes; i++)
   {
    // Cache the node
    CCNode *inode_pt = node_pt(i);
    // Output the node index
    nodes_file << i;
    // Loop over the dimensions
    for (unsigned k = 0; k < Dim; k++)
     {
      // Get the node position
      Real position = inode_pt->get_position(k);
      
      // Output nodes positions to file
      nodes_file << "," << position;
     }
    nodes_file << std::endl;
   }
  
  // Close support nodes file
  nodes_file.close();
 }
 
 // ===================================================================
 /// Set/get the i-th node
 // ===================================================================
 CCNode* ACProblem::node_pt(const unsigned long i)
 {
#ifdef SCICELLXX_RANGE_CHECK
  const unsigned long nnodes = n_nodes();
  
  // Check whether the requested node index is in the range [0, n_nodes - 1]
  if (i >= nnodes)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The node you are trying to access is out of range\n"
                  << "Number of nodes: " << nnodes << std::endl
                  << "Requested node: " << i << std::endl;
    throw SciCellxxLibError(error_message.str(),
                           SCICELLXX_CURRENT_FUNCTION,
                           SCICELLXX_EXCEPTION_LOCATION);    
   }
#endif // #ifdef SCICELLXX_RANGE_CHECK
  
  return Nodes_pt[i];
  
 }

 // ===================================================================
 /// Initialise the u vector (solution)
 // ===================================================================
 void ACProblem::initialise_u(const unsigned n_equations, const unsigned n_history_values)
 {
  U_pt = new CCData(n_equations, n_history_values);
  Allow_free_memory_for_U = true;  
 }

 // ===================================================================
 /// Assign equations number
 // ===================================================================
 const unsigned long ACProblem::assign_equations_number()
 {
  // Get the number of nodes
  const unsigned long nnodes = n_nodes();
  // Get the number of variables of the first node to approximate the
  // final size of the
  // "Global_equation_number_to_node_and_local_variable" vector
  const unsigned n_variables_first_node = node_pt(0)->n_variables();
  // Reserve memory space
  Global_equation_number_to_node_and_local_variable.
   reserve(nnodes * n_variables_first_node);

  // Loop over the nodes and assign an equation number
  unsigned long inode = 0;
  unsigned long eq_idx = 0;
  
  // Loop over all nodes and assign a global equation number
  while (inode < nnodes)
   {
    // Get the i-th node
    CCNode *inode_pt = node_pt(inode);
    
    // Get the number of variables in the node
    const unsigned nvariables = inode_pt->n_variables();
    
    // Assign an equation number for each variable
    for (unsigned i = 0; i < nvariables; i++)
     {
      // Assign the equation number
      inode_pt->equation_number(i) = eq_idx;
      
      // Keep track of the node pointer and the local variable number
      Global_equation_number_to_node_and_local_variable[eq_idx++] =
       std::make_pair(inode_pt, i);
      
     }
    
    // Increase the index for nodes
    inode++;
    
   }
  
  // Assign the number of equations
  N_equations = eq_idx;
  
  // Return the number of equations
  return eq_idx;
  
 }
 
}
