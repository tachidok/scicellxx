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
/// IN THIS FILE: The definition of a class to represent nodes

// Check whether the class has been already defined
#ifndef CCNODE_H
#define CCNODE_H

#include "../general/general.h"

#include "./cc_data.h"

namespace scicellxx
{
 
 /// @class CCNode cc_node.h
 
 // Abstract class to represent nodes
 class CCNode
 {
  
 public:
  
  /// Empty constructor
  CCNode(const unsigned dimension, const unsigned n_variables, const unsigned n_history_values=1);
  
  /// Empty destructor
  virtual ~CCNode();
  
  /// Pin al positions
  inline void pin_all_positions()
  {X.pin_all();}
  
  /// Pin all variables
  inline void pin_all_variables()
  {U.pin_all();}
  
  /// Pin i-th position
  inline void pin_position(const unsigned &i)
  {X.pin(i);}
  
  /// Unpin i-th position
  inline void unpin_position(const unsigned &i)
  {X.unpin(i);}
  
  /// Pin i-th variable
  inline void pin_u(const unsigned &i)
  {U.pin(i);}
  
  /// Unpin i-th variable
  inline void unpin_u(const unsigned &i)
  {U.unpin(i);}
  
  /// Get access to the spatial position of the node
  inline CCData &x() {return X;}
  
  /// Get the i-th spatial position of the node at time t
  inline Real get_position(const unsigned &i, const unsigned t=0)
  {return X.value(i,t);}
  
  /// Set the i-th spatial position of the node at time t
  inline void set_position(const Real i_position,
                           const unsigned &i, const unsigned t=0)
  {X.value(i,t)=i_position;}
  
  /// Get access to variables stored in the node
  inline CCData &u() {return U;}
  
  /// Get the i-th variable value at time t
  inline Real get_variable(const unsigned &i, const unsigned t=0)
  {return U.value(i,t);}
  
  /// Set the i-th variable value at time t
  inline void set_variable(const Real i_variable,
                           const unsigned &i, const unsigned t=0)
  {U.value(i,t)=i_variable;}

  /// Get the dimension of the node
  inline unsigned dimension() const {return Dimension;}

  /// Get the number of variables that the node stores
  inline unsigned n_variables() const {return N_variables;}

  /// Get the number of history values stores in the node
  inline unsigned n_history_values() const {return N_history_values;}
  
  /// Test whether the node is on any boundary (a common Node can not
  /// be on any boundary, use a CCBoundaryNode instead)
  virtual bool is_on_boundary() {return false;}
  
  /// Test whether the node is on boundary b (a common Node can not be
  /// on any boundary, use a CCBoundaryNode instead)
  virtual bool is_on_boundary(const unsigned b) {return false;}
  
  /// Output the data stored at the node (output horizontally without
  /// position by default, otherwise output vertically with position)
  virtual void output(bool output_position = false,
                      const unsigned t = 0) const;
  
  /// Output the data stored at the node to a file (output horizontally
  /// without position by default, otherwise output vertically with
  /// position)
  virtual void output(std::ofstream &outfile,
                      bool output_position = false,
                      const unsigned t = 0) const;

  /// Write-access assign an equation number to an specific variable in
  /// in the node
  inline unsigned long &equation_number(const unsigned variable_index) {return Equation_number[variable_index];}

  /// Read-access returns the equation number of an specific variable
  /// in the node
  inline unsigned long equation_number(const unsigned variable_index) const {return Equation_number[variable_index];}
  
 protected:
  
  // Copy constructor (we do not want this class to be copiable because
  // it contains dynamically allocated variables, A in this
  // case). Check
  // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  CCNode(const CCNode& node)
   : Dimension(node.dimension()), N_variables(node.n_variables()),
   N_history_values(node.n_history_values()), X(node.dimension(), node.n_history_values()),
   U(node.n_variables(), node.n_history_values())
    {
     BrokenCopy::broken_copy("CCNode");
    }
  
  // Copy constructor (we do not want this class to be copiable because
  // it contains dynamically allocated variables, A in this
  // case). Check
  // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const CCNode&) 
   {
    BrokenCopy::broken_assign("CCNode");
   }
  
  /// The spatial dimension of the node
  const unsigned Dimension;
  
  /// The number of variables stored in the node
  const unsigned N_variables;
  
  /// The number of history values of the variable stored in the node
  const unsigned N_history_values;
  
  /// Store the spatial position of the node
  CCData X;
  
  /// Store the values of the variables stored in the node
  CCData U;
  
  /// Store the associated equation number for each variable in the node
  std::vector<unsigned long> Equation_number;
  
 };
 
}

#endif // #ifndef CCNODE_H
