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
/// IN THIS FILE: The abstract class
/// ACNewtonsMethodForImplicitTimeStepper that defines the interfaces
/// of Newton's method for Implicit Time Step Methods

// Check whether the class has been already defined
#ifndef ACNEWTONSMETHODFORIMPLICITTIMESTEPPER_TPL_H
#define ACNEWTONSMETHODFORIMPLICITTIMESTEPPER_TPL_H

#include "../general/general.h"

#include "../data_structures/data_structures.h"
#include "../newtons_method/newtons_method.h"

#include "ac_jacobian_and_residual_for_implicit_time_stepper.h"

namespace scicellxx
{
 
 /// An abstract class that implements Newton's method for implicit
 /// time stepping methods
 template<class EQUATIONS_TYPE>
 class ACNewtonsMethodForImplicitTimeStepper : public virtual CCNewtonsMethod
 {
  
 public:
  
  /// Empty constructor
  ACNewtonsMethodForImplicitTimeStepper();
  
  /// Empty destructor
  ~ACNewtonsMethodForImplicitTimeStepper();
  
  /// Set data for Jacobian and residual computation. The odes, the
  /// time step 'h', the current time 't', the values of 'u' and the
  /// index where the values of 'u' at time 't+h' will be stored
  void set_data_for_jacobian_and_residual(EQUATIONS_TYPE *odes_pt, const Real h, const Real t,
                                          CCData *u_pt, const unsigned k);
  
  /// Set the strategy to compute the ODE's Jacobian
  void set_strategy_for_odes_jacobian(ACJacobianAndResidualForImplicitTimeStepper<EQUATIONS_TYPE> *jacobian_strategy_for_odes_pt);
   
 protected:
  
  /// Performs actions before initial convergence check
  virtual void actions_before_initial_convergence_check() = 0;
  
  /// Performs actions before Newton's method step
  void actions_before_newton_step();
  
  /// Performs actions after Newton's method step
  void actions_after_newton_step();
  
  /// Get access to the odes pointer
  inline EQUATIONS_TYPE* odes_pt() {return ODEs_pt;}
  
  /// Gets access to the time step
  inline Real time_step() const {return Time_step;}
  
  /// Gets access to the current time
  inline Real current_time() const {return Current_time;}
  
  /// Gets access to the u values
  inline CCData *u_pt() {return U_pt;}
   
  /// Get access to the history index
  inline unsigned history_index() {return History_index;}
  
  /// Checks whether the data to compute the Jacobian and the residual
  /// has been set
  inline bool data_for_jacobian_and_residual_has_been_set()
  {return Data_for_jacobian_and_residual_has_been_set;}
  
 private:
  
  /// Copy constructor (we do not want this class to be copiable because
  /// it contains dynamically allocated variables, A in this
  /// case). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  ACNewtonsMethodForImplicitTimeStepper(const ACNewtonsMethodForImplicitTimeStepper &copy)
   : CCNewtonsMethod()
   {
    BrokenCopy::broken_copy("ACNewtonsMethodForImplicitTimeStepper");
   }
   
  /// Copy constructor (we do not want this class to be copiable because
  /// it contains dynamically allocated variables, A in this
  /// case). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const ACNewtonsMethodForImplicitTimeStepper &copy)
   {
    BrokenCopy::broken_assign("ACNewtonsMethodForImplicitTimeStepper");
   }
  
  /// The ODEs
  EQUATIONS_TYPE* ODEs_pt;
   
  /// The current time step of the Time Stepper
  Real Time_step;
   
  /// The current time
  Real Current_time; 
   
  /// The U values of the function at the current time
  CCData *U_pt;
   
  /// The index of the values of u at time "Current_time + Time_step"
  unsigned History_index;
   
  /// A flag to indicate whether the data for the Jacobian and
  /// residual computation has been set
  bool Data_for_jacobian_and_residual_has_been_set;
   
 };
 
}

#endif // #ifndef ACNEWTONSMETHODFORIMPLICITTIMESTEPPER_TPL_H

