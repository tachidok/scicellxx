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
#include "cc_euler_method.tpl.h"

namespace scicellxx
{

 // ===================================================================
 // Constructor
 // ===================================================================
 template<class EQUATIONS_TYPE>
 CCEulerMethod<EQUATIONS_TYPE>::CCEulerMethod()
  : ACTimeStepper<EQUATIONS_TYPE>()
 {
  
  // Sets the number of history values
  this->N_history_values = 2;
  
 }
 
 // ===================================================================
 // Empty destructor
 // ===================================================================
 template<class EQUATIONS_TYPE>
 CCEulerMethod<EQUATIONS_TYPE>::~CCEulerMethod()
 {
 
 }
 
 // ===================================================================
 // Applies Eulers method to the given odes from the current time "t"
 // to the time "t+h". The values of u at time t+h will be stored at
 // index k (default k = 0).
 // ===================================================================
 template<class EQUATIONS_TYPE>
 void CCEulerMethod<EQUATIONS_TYPE>::time_step(EQUATIONS_TYPE &odes,
                                               const Real h,
                                               const Real t,
                                               CCData &u,
                                               const unsigned k)
 {
#ifdef SCICELLXX_PANIC_MODE
  // Check if the ode has the correct number of history values to
  // apply Euler's method
  const unsigned n_history_values = u.n_history_values();
  if (n_history_values < this->N_history_values)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The number of history values is less than\n"
                  << "the required by Euler's method\n"
                  << "Required number of history values: "
                  << this->N_history_values << "\n"
                  << "Number of history values: "
                  << n_history_values << std::endl;
    throw SciCellxxLibError(error_message.str(),
                           SCICELLXX_CURRENT_FUNCTION,
                           SCICELLXX_EXCEPTION_LOCATION);
   }
#endif // #ifdef SCICELLXX_PANIC_MODE
  
  // Get the number of odes
  const unsigned n_equations = odes.n_equations();
  
  // Temporary vector to store the evaluation of the odes.
  CCData dudt(n_equations);
  
  // Evaluate the ODE at time "t" using the values of "u" at index k
  odes.evaluate_time_derivatives(t, u, dudt, k);
  
  // Shift values to the right to provide storage for the new values
  u.shift_history_values();
  
  // Perform one step of Euler's method
  for (unsigned i = 0; i < n_equations; i++)
   {
    u(i,k) = u(i,k+1) + (h * dudt(i));
   }
  
 }

}
