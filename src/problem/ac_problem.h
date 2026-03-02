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
#ifndef ACPROBLEM_H
#define ACPROBLEM_H

#include "../general/general.h"
#include "../time_steppers/time_steppers.h"
#include "../data_structures/data_structures.h"

namespace scicellxx
{

 /// @class ACProblem ac_problem.h

 /// This class implements the interfaces for the problem class. It
 /// specifies a template for solving a problem thus one needs to
 /// create a class that inherents from this one to solve a particular
 /// problem
 class ACProblem
 {
  
 public:
  
  /// Constructor
  ACProblem(const unsigned dim = 1);
  
  /// Destructor
  virtual ~ACProblem();

  /// Complete the problem setup (things you could not do in the
  /// construtor as initial conditions/boundary conditions/solver/etc)
  virtual void complete_problem_setup() = 0;
  
  /// Every derived class must implement its own solve method (calling
  /// the corresponding steady_solve() and unsteady_solve() methods)
  virtual void solve() = 0;

  /// Document solution
  virtual void document_solution() = 0;
  
  /// Write access to the current time step
  inline unsigned &output_file_index() {return Output_file_index;}
  
  /// Read-only access to the current time step
  inline unsigned output_file_index() const {return Output_file_index;}
  
  /// Document nodes positions
  void document_nodes_positions(std::string &filename);
  
  /// The dimension of the problem
  inline unsigned dim() const {return Dim;}
  
  /// Get the number of nodes
  inline unsigned long n_nodes() const {return Nodes_pt.size();}
  
  /// Set/get the i-th node
  CCNode* node_pt(const unsigned long i);

  /// Get access to the U vector
  CCData *u_pt() const {return U_pt;}
  
  /// Read-only access to the vector U values
  inline const Real u(const unsigned i, const unsigned t = 0) const {return U_pt->value(i,t);}
  
  /// Write access to the vector U values
  inline Real &u(const unsigned i, const unsigned t = 0) {return U_pt->value(i,t);}
  
  /// Initialise the u vector (solution)
  void initialise_u(const unsigned n_equations, const unsigned n_history_values = 1);
  
  /// Assign equations number
  const unsigned long assign_equations_number();
  
  /// Return the number of equations
  inline const unsigned long n_equations() const {return N_equations;}
  
  // Get node and local variable number from equation number
  inline std::pair<CCNode*, unsigned> &get_node_and_local_variable_from_global_equation(const unsigned long equation_number)
  {return Global_equation_number_to_node_and_local_variable[equation_number];}
  
 protected:
  
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 ACProblem(const ACProblem &copy)
  : Output_file_index(0),
   Dim(0)
    {
     BrokenCopy::broken_copy("ACProblem");
    }
  
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const ACProblem &copy)
   {
    BrokenCopy::broken_assign("ACProblem");
   }
  
  /// Problem steady solve
  virtual void steady_solve() = 0;
  
  /// Problem unsteady solve
  virtual void unsteady_solve() = 0;
  
  /// The set of actions to be performed before newton solve
  virtual void actions_before_newton_solve() = 0;
  
  /// The set of actions to be performed after newton solve
  virtual void actions_after_newton_solve() = 0;
  
  /// A counter to store the current output file index
  unsigned Output_file_index;
    
  /// Dimension
  const unsigned Dim;
  
  // Total number of nodes
  unsigned N_nodes;
  
  // The nodes
  std::vector<CCNode *> Nodes_pt;
  
  /// The storage for the computed solution
  CCData *U_pt;
  
  /// Flag to allow release of memory by the class
  bool Allow_free_memory_for_U;
  
  /// Store the number of equations of the problem
  unsigned long N_equations;
  
  /// Keep track of the node pointer and the local variable number
  /// associated to a global equation number
  std::vector<std::pair<CCNode*, unsigned> > Global_equation_number_to_node_and_local_variable;
  
 };
 
}

#endif // #ifndef ACPROBLEM_H
