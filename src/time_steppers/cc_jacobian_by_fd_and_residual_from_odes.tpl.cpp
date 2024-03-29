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
#include "cc_jacobian_by_fd_and_residual_from_odes.tpl.h"

namespace scicellxx
{
 // ===================================================================
 /// Empty constructor
 // ===================================================================
 template<class EQUATIONS_TYPE>
 CCJacobianByFDAndResidualFromODEs<EQUATIONS_TYPE>::CCJacobianByFDAndResidualFromODEs()
  : ACJacobianAndResidualForImplicitTimeStepper<EQUATIONS_TYPE>()
 {
  
 }
 
 // ===================================================================
 /// Empty destructor
 // ===================================================================
 template<class EQUATIONS_TYPE>
 CCJacobianByFDAndResidualFromODEs<EQUATIONS_TYPE>::~CCJacobianByFDAndResidualFromODEs()
 {
 
 }
 
 // ===================================================================
 /// In charge of computing the Jacobian using Finite Differences
 /// (virtual function implementation)
 // ===================================================================
 template<class EQUATIONS_TYPE>
 void CCJacobianByFDAndResidualFromODEs<EQUATIONS_TYPE>::compute_jacobian()
 {
  // Get a pointer to the ODEs
  EQUATIONS_TYPE *odes_pt = this->odes_pt();
  
  // Get a pointer to the u values
  CCData *u_pt = this->u_pt();
  
  // Check whether the data for the computation of the jacobian has
  // been set
  if (!this->data_for_jacobian_and_residual_has_been_set() || odes_pt == NULL || u_pt == NULL)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the data required for\n"
                  << "the computation of the Jacobian\n"
                  << "You need to call the method\n"
                  << "set_data_for_jacobian_and_residual()\n"
                  << std::endl;
    throw SciCellxxLibError(error_message.str(),
                           SCICELLXX_CURRENT_FUNCTION,
                           SCICELLXX_EXCEPTION_LOCATION);
   }
  
  // Get the number of ODEs
  const unsigned n_dof = odes_pt->n_equations();
  
  // Allocate memory for the Jacobian (delete previous data)
  this->Jacobian_pt->allocate_memory(n_dof, n_dof);
  
  // Get the current time
  const Real t = this->current_time();
  
  // Get the time step
  const Real h = this->time_step();
  
  // Get the index of history values of the u vector at time 't+h'
  // that should be used to compute the values of the Jacobian
  const unsigned k = this->history_index();
  
  // Store the evaluation of the odes
  CCData dudt(n_dof);
  
  // Evaluate the ODEs using the history values of u at time t+h'
  // indicated in the index k
  odes_pt->evaluate_time_derivatives(t+h, (*u_pt), dudt, k);
  
  // Compute the approximated Jacobian
  for (unsigned i = 0; i < n_dof; i++)
   {
    // Create a copy of the U values and add an slight perturbation in
    // the i-th DOF and histroy value k, then evaluate all the
    // equations (this will helps us to approximate the column i of
    // the Jacobian)
    CCData u_plus((*u_pt));
    const Real delta_u = 1.0e-8;
    // ... the perturbation
    u_plus(i,k)+=delta_u;
    
    // Evaluate the ODEs with the slighted perturbed data
    CCData dudt_plus(n_dof);
    // Evaluate the ODEs using the history values indicated in the
    // index K
    odes_pt->evaluate_time_derivatives(t+h, u_plus, dudt_plus, k);
    // Compute the values for the Jacobian matrix, add entries for the
    // current i-column only (all functions with an slight
    // perturbation in the i-th dof)
    for (unsigned j = 0; j < n_dof; j++)
     {
      // The indices are reversed because we are computing the
      // approximation to the i-th column of the Jacobian (all
      // equations -index j- with a perturbation in the i-th dof)
      (*this->Jacobian_pt)(j, i) = (dudt_plus(j) - dudt(j)) / delta_u;
     }
   }
  
 }
 
 // ===================================================================
 /// In charge of computing the residual
 // ===================================================================
 template<class EQUATIONS_TYPE>
 void CCJacobianByFDAndResidualFromODEs<EQUATIONS_TYPE>::compute_residual()
 {
  
 } 
 
}
