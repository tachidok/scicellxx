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
/// IN THIS FILE: Implementation of a concrete class to represent
/// nodes. This is the simplest implementation

#include "cc_node.h"

namespace scicellxx
{

 /// ===================================================================
 /// Empty constructor
 /// ===================================================================
 CCNode::CCNode(const unsigned dimension, const unsigned n_variables,
                const unsigned n_history_values)
  : Dimension(dimension), N_variables(n_variables),
    N_history_values(n_history_values), X(Dimension, N_history_values),
    U(N_variables, N_history_values)
 {
  // Resize the equaiton number vector to keep track of the equation
  // number in the problem
  Equation_number.resize(N_variables);
 }
 
 /// ===================================================================
 /// Empty destructor
 /// ===================================================================
 CCNode::~CCNode()
 { }
 
 /// ===================================================================
 /// Output the data stored at the node (output horizontally without
 /// position by default, otherwise output vertically with position)
 /// ===================================================================
 void CCNode::output(bool output_position,
                     const unsigned t) const
 {
  // Check whether we should output positions
  if (output_position)
   {
    for (unsigned i = 0; i < this->Dimension; i++)
     {
      std::cout << X.value(i,t) << "\t";
     } // for (i < this->Dimension)
   } // if (output_position)
  
  for (unsigned i = 0; i < this->N_variables; i++)
   {
    std::cout << U.value(i,t) << "\t";
   } // for (i < this->N_variables)
  std::cout << std::endl; 
 }
 
 /// ===================================================================
 /// Output the data stored at the node to a file (output horizontally
 /// without position by default, otherwise output vertically with
 /// position)
 /// ===================================================================
 void CCNode::output(std::ofstream &outfile,
                     bool output_position,
                     const unsigned t) const
 {
  // Check whether we should output positions
  if (output_position)
   {
    for (unsigned i = 0; i < this->Dimension; i++)
     {
      outfile << X.value(i,t) << "\t";
     } // for (i < this->Dimension)
   } // if (output_position)
  
  for (unsigned i = 0; i < this->N_variables; i++)
   {
    outfile << U.value(i,t) << "\t";
   } // for (i < this->N_variables)
  outfile << std::endl;
 }
 
}
