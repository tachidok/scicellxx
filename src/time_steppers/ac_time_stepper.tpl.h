#ifndef ACTIMESTEPPER_TPL_H
#define ACTIMESTEPPER_TPL_H

#include "../general/general.h"

#include "../data_structures/data_structures.h"

namespace scicellxx
{ 
 /// @class ACTimeStepper ac_time_stepper.tpl.h

 /// This class implements the interfaces for time
 /// steppers/integration methods
 template<class EQUATIONS_TYPE>
 class ACTimeStepper
 {
 
 public:
 
  /// Empty constructor
  ACTimeStepper();
  
  /// Empty destructor
  virtual ~ACTimeStepper();
  
  /// Performs a time step applying a time integration method to the
  /// given equations from the current time "t" to the time
  /// "t+h". Previous to the call of the method, the values of u at
  /// time "t" should be stored at index k (default k = 0). After the
  /// call, the values at time "t+h" will be stored at index k,
  /// therefore the values at time "t" will be at index k+1
  virtual void time_step(EQUATIONS_TYPE &equations,
                         const Real h,
                         const Real t,
                         CCData &u,
                         unsigned k = 0) = 0;
  
  /// Resets the time stepper to its initial state.
  
  /// For the BDF 2 method we require to re-enable the computation of
  /// the initial guess for the value (t+2h) only the first time that
  /// the methods is called.
  
  /// For ADAPTIVE time steppers we need to indicate no previous "time
  /// step (h)" has been computed. Thus the given time step should be
  /// considered as the initial time step
  virtual void reset() { }
  
  /// Get the associated number of history values (each method is in
  /// charge of setting this value based on the number of history
  /// values it requires)
  inline unsigned n_history_values() const {return N_history_values;}
  
 protected:
 
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  ACTimeStepper(const ACTimeStepper &copy)
   {
    BrokenCopy::broken_copy("ACTimeStepper");
   }

  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const ACTimeStepper &copy)
   {
    BrokenCopy::broken_assign("ACTimeStepper");
   }
  
  /// The number of history values
  unsigned N_history_values;
  
 };

}
 
#endif // #ifndef ACTIMESTEPPER_TPL_H