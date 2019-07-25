#include "cc_adaptive_runge_kutta_45F_method.h"

namespace chapchom
{

 // ===================================================================
 // Constructor
 // ===================================================================
 CCAdaptiveRK45FMethod::CCAdaptiveRK45FMethod()
  : ACTimeStepper(),
    Maximum_iterations(DEFAULT_RK45F_MAXIMUM_ITERATIONS),
    Maximum_step_size(DEFAULT_RK45F_MAXIMUM_STEP_SIZE),
    Minimum_step_size(DEFAULT_RK45F_MINIMUM_STEP_SIZE),
    Maximum_tolerance(DEFAULT_RK45F_MAXIMUM_TOLERANCE),
    Minimum_tolerance(DEFAULT_RK45F_MINIMUM_TOLERANCE)
 {
  
  // Sets the number of history values
  N_history_values = 2;
  
 }
 
// ===================================================================
// Empty destructor
// ===================================================================
 CCAdaptiveRK45FMethod::~CCAdaptiveRK45FMethod()
 {
  
 }
 
 // ===================================================================
 // Applies Runge-Kutta 4(5) Fehlberg method to the given odes from
 // "t" to the time "t+h". The values of u at time t+h will be stored
 // at index k (default k = 0).
 // ===================================================================
 void CCAdaptiveRK45FMethod::time_step(ACODEs &odes, const Real h,
                                       const Real t,
                                       CCData<Real> &u,
                                       const unsigned k)
 {
  // Check if the ode has the correct number of history values to
  // apply Runge-Kutta 4(5) Fehlberg method
  const unsigned n_history_values = u.n_history_values();
  if (n_history_values < N_history_values)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The number of history values is less than\n"
                  << "the required by Adaptive RK45F method" << std::endl
                  << "Required number of history values: "
                  << N_history_values << std::endl
                  << "Number of history values: "
                  << n_history_values << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Get the number of odes
  const unsigned n_odes = odes.n_odes();
  
  // Temporary vector to store the evaluation of the odes.
  CCData<Real> dudt(n_odes);
  
  // Evaluate the ODE at time "t" using the current values of "u"
  odes.evaluate_derivatives(t, u, dudt, k);
  
  // Temporary vector to store the K_i evaluations proper of
  // Runge-Kutta methods
  CCData<Real> K1(n_odes);
  CCData<Real> K2(n_odes);
  CCData<Real> K3(n_odes);
  CCData<Real> K4(n_odes);
  CCData<Real> K5(n_odes);
  CCData<Real> K6(n_odes);
  
  // Create a copy of the u vector
  CCData<Real> u_copy(u);

  // Counter for iterations
  unsigned n_iterations = 0;
  // Sum up the error
  Real total_error = 0.0;
  
  // --------------------------------------------------------------------
  // Runge-Kutta 4(5) Fehlberg method
  // --------------------------------------------------------------------
  // K1 = y(t, u)
  // K2 = y(t + \frac{1}{4}h, u + h(\frac{1}{4}K1))
  // K3 = y(t + \frac{3}{8}h, u + h(\frac{3}{32}K1 + \frac{9}{32}K2))
  // K4 = y(t + \frac{12}{13}h, u + h(\frac{1932}{2197}K1 - \frac{7200}{2197}K2 + \frac{7296}{2197}K3))
  // K5 = y(t + h, u + h(\frac{439}{216}K1 - 8K2 + \frac{3680}{513}K3 - \frac{845}{4104}K4))
  // K6 = y(t + \frac{1}{2}h, u + h(-\frac{8}{27}K1 + 2K2 - \frac{3544}{2565}K3 + \frac{1859}{4104}K4 - \frac{11}{40}K5))
  
  // \hat{u}(t+h) = u(t) + \frac{1}{8}h (\frac{25}{216} K1 + \frac{1408}{2565} K3 + \frac{2197}{4104} K4 - \frac{1}{5} K5)
  // \tilde{u}(t+h) = u(t) + \frac{1}{8}h (\frac{16}{135} K1 + \frac{6656}{12825} K3 + \frac{28561}{56430} K4 - \frac{9}{50} K5 + \frac{2}{55} K6)

  // error = \hat{u}(t+h) - \tilde{u}(t+h) = h(\frac{1}{360} K1 - \frac{128}{4275} K3 - \frac{2197}{75240} K4 + \frac{1}{50} K5 + \frac{2}{55} K6)
  // -------------------------------------------------------------------- 
  // Butcher tableau
  
  // --------------------------------------------------------------------
  // 0             |
  // \frac{1}{4}   | \frac{1}{4}
  // \frac{3}{8}   | \frac{3}{32}        \frac{9}{32}
  // \frac{12}{13} | \frac{1932}{2197}   -\frac{7200}{2197}  \frac{7296}{2197}
  // 1             | \frac{439}{216}     -8                  \frac{3680}{513}     -\frac{845}{4104}
  // \frac{1}{2}   | -\frac{8}{27}       2                  -\frac{3544}{2565}     \frac{1859}{4104}   -\frac{11}{40}
  // --------------------------------------------------------------
   //               | \frac{25}{216}      0                   \frac{1408}{2565}     \frac{2197}{4104}   -\frac{1}{5}     0
   //               | \frac{16}{135}      0                   \frac{6656}{12825}    \frac{28561}{56430} -\frac{9}{50}   \frac{2}{55}

   // Perform at least one computation
   do
   {
    // If new step size has been computed then take it as the initial
    // step size
    Real hh = h;
    if (Next_step_size_computed)
     {
      hh = Next_step_size;
     }
    
    // --------------------------------------------------------------------
    // Evaluate the ODE at time "t" using the current values of "u"
    // K1
    odes.evaluate_derivatives(t, u, K1, k);
    // --------------------------------------------------------------------
    // Evaluate the ODE at time "t+(hh/4)" and with "u+(hh/4)K1"
    for (unsigned i = 0; i < n_odes; i++)
       {
        u_copy(i,k) = u(i,k)+(hh*(0.25*K1(i)));
       }
      // K2
    odes.evaluate_derivatives(t+(0.25*hh), u_copy, K2, k);
  
    // --------------------------------------------------------------------
    // Evaluate the ODE at time "t+(3hh/8)" and with "u+(3hh/32)K1 + 9hh/32K2"
    for (unsigned i = 0; i < n_odes; i++)
     {
      u_copy(i,k) = u(i,k)+hh*((3.0/32.0)*K1(i)+(9.0/32.0)*K2(i));
     }
    // K3
    odes.evaluate_derivatives(t+((3.0/8.0)*hh), u_copy, K3, k);
    
    // --------------------------------------------------------------------
    // Evaluate the ODE at time "t+(12hh/13)" and with "u+(1932hh/2197)K1 - (7200hh/2197)K2 + (7296hh/2197)K3"
    for (unsigned i = 0; i < n_odes; i++)
     {
      u_copy(i,k) = u(i,k)+hh*((1932.0/2197.0)*K1(i)-(7200.0/2197.0)*K2(i)+(7296.0/2197.0)*K3(i));
     }
    // K4
    odes.evaluate_derivatives(t+((12.0/13.0)*hh), u_copy, K4);
    
    // --------------------------------------------------------------------
    // Evaluate the ODE at time "t+hh" and with "u+(439hh/216)K1 - 8hhK2 + (3680hh/513)K3 - (845hh/4104)K4"
    for (unsigned i = 0; i < n_odes; i++)
     {
      u_copy(i,k) = u(i,k)+hh*((439.0/216.0)*K1(i)-8*K2(i)+(3680.0/513.0)*K3(i)-(845.0/4104.0)*K4(i));
     }
    // K5
    odes.evaluate_derivatives(t+hh, u_copy, K5);
    // --------------------------------------------------------------------
    // Evaluate the ODE at time "t+(1hh/2)" and with "u+(-8hh/27)K1 + 2hhK2 - (3544hh/2565)K3 + (1859hh/4104)K4 - (11hh/40)K5"
    for (unsigned i = 0; i < n_odes; i++)
     {
      u_copy(i,k) = u(i,k)+hh*((-8.0/27.0)*K1(i)+2*K2(i)-(3544.0/2565.0)*K3(i)+(1859.0/4104.0)*K4(i)-(11.0/40.0)*K5(i));
     }
    // K6
    odes.evaluate_derivatives(t+(0.5*hh), u_copy, K6);

#if 0
    // Create temporary storages for u, these temporary approximations
    // will be used at the error and time step computation stages.
    CCData<Real> u_hat(n_odes);
    CCData<Real> u_tilde(n_odes);
    // Compute the hat and tilde u's
    for (unsigned i = 0; i < n_odes; i++)
     {
      u_hat(i) = u(i,k) + ((25.0/216.0)*K1(i) + (1408.0/2565.0)*K3(i) + (2197.0/4104.0)*K4(i) - (1.0/5.0)*K5(i));
      u_tilde(i) = u(i,k) + ((16.0/135.0)*K1(i) + (6656.0/12825.0)*K3(i) + (28561.0/56430.0)*K4(i) - (9.0/50.0)*K5(i) + (2.0/55.0)*K6(i));
     }
#endif // #if 0
    
    // A storage for the error
    CCData<Real> error(n_odes);
    // Reset the error
    total_error = 0.0;
    for (unsigned i = 0; i < n_odes; i++)
     {
      error(i) = hh*((1.0/360.0)*K1(i) - (128.0/4275.0)*K3(i) - (2197.0/75240.0)*K4(i) + (1.0/50.0)*K5(i) + (2.0/55.0)*K6(i));
      total_error+=error(i); 
     }
    
    // Check whether the total was too much (the step size was too big)
    if (total_error > Maximum_tolerance)
     {
      // Step was too big
      Next_step_size = hh *0.5;
      //Real new_h = safety_coefficient * h * (total_error/Maximum_tolerance, goodPower);
     }
    else
     {
      // Step was good
      Next_step_size = hh *2.0;
     }
    
    // A new step size has been computed
    Next_step_size_computed = true;
    
    // Increase the number of iterations
    n_iterations++;
    
    if (n_iterations >= Maximum_iterations)
     {
      chapchom_output << "Runge-Kutta 4(5) Fehlberg MAXIMUM NUMBER OF ITERATIONS reached ["<<Maximum_iterations<<"]\n"
                      << "If you consider you require more iterations you can\n"
                      << "set your own by calling the method\n\n"
                      << "set_maximum_interations()\n"
                      << std::endl;
     }
    
   }while(!(Minimum_tolerance <= total_error && total_error <= Maximum_tolerance) && n_iterations < Maximum_iterations);
   
   // Shift values to the right to provide storage for the new values
   u.shift_history_values();
  
   // Once the derivatives have been obtained compute the new "u" as
   // the weighted sum of the K's
   for (unsigned i = 0; i < n_odes; i++)
           {
            u(i,k) = u(i,k+1) + ((16.0/135.0)*K1(i) + (6656.0/12825.0)*K3(i) + (28561.0/56430.0)*K4(i) - (9.0/50.0)*K5(i) + (2.0/55.0)*K6(i));
           }
  
 }

}
