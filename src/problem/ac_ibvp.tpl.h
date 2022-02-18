#ifndef ACIBVP_TPL_H
#define ACIBVP_TPL_H

#include "../general/general.h"

#include "../data_structures/data_structures.h"

#include "ac_problem.h"

namespace scicellxx
{

 /// @class ACIBVP ac_ibvp.tpl.h

 /// This class implements the interfaces for an initial value problem
 /// It specifies a template for solving a problem thus one needs to
 /// create a class that inherents from this one to solve a particular
 /// initial value problem
 template<class EQUATIONS_TYPE>
 class ACIBVP : public virtual ACProblem
 {
  
 public:
  
  /// Constructor
  ACIBVP();
  
  /// Constructor
  ACIBVP(EQUATIONS_TYPE *equations_pt, ACTimeStepper<EQUATIONS_TYPE> *time_stepper_pt);
  
  /// Destructor
  virtual ~ACIBVP();
  
  /* /// Complete the problem setup (initial conditions/boundary */
  /// conditions/set solver/etc)
  virtual void complete_problem_setup() = 0;
  
  /// Set initial conditions (should be called as part of
  /// complete_problem_setup)
  virtual void set_initial_conditions() = 0;
  
  /// We perform an unsteady solve by default, if you require a
  /// different solving strategy then override this method
  void solve();
  
  /// Document solution
  virtual void document_solution() = 0;
  
  /// Add a time stepper
  void add_time_stepper(ACTimeStepper<EQUATIONS_TYPE> *time_stepper_pt,
                        const Real initial_time = 0,
                        const Real time_step = 0);
  
  /// Read-only access to the time i-th stepper pointer
  ACTimeStepper<EQUATIONS_TYPE> *time_stepper_pt(const unsigned i = 0) const;
  
  /// Get the number of time steppers
  inline const unsigned n_time_steppers() {return Time_stepper_pt.size();}
  
  /// Write access to the current time
  Real &time(const unsigned i = 0);
  
  /// Read-only access to the current time
  Real time(const unsigned i = 0) const;
  
  /// Write access to the current time step
  Real &time_step(const unsigned i = 0);
  
  /// Read-only access to the current time step
  Real time_step(const unsigned i = 0) const;
  
 protected:
  
  /// Copy constructor (we do not want this class to be
   /// copiable). Check
   /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 ACIBVP(const ACIBVP &copy)
  : ACProblem()
   {
    BrokenCopy::broken_copy("ACIBVP");
   }
  
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const ACIBVP &copy)
   {
    BrokenCopy::broken_assign("ACIBVP");
   } 
    
  /// Problem steady solve (empty)
  void steady_solve() { }
  
  /// Problem unsteady solve
  void unsteady_solve();
  
  /// The set of actions to be performed before a time stepping (do nothing)
  virtual void actions_before_time_stepping() { }
  
  /// The set of actions to be performed after a time stepping (do nothing)
  virtual void actions_after_time_stepping() { } 
  
  /// The set of actions to be performed before newton solve (do nothing)
  virtual void actions_before_newton_solve() { }
  
  /// The set of actions to be performed after newton solve (do nothing)
  virtual void actions_after_newton_solve() { }

  /// A time steppers vector, possibly to solve a problem with
  /// different time scales
  std::vector<ACTimeStepper<EQUATIONS_TYPE> *> Time_stepper_pt;
  
  /// The current time vector (each time stepper has an associated
  /// time)
  std::vector<Real> Time;
  
  /// The time step vector (each time stepper has an associated time step)
  std::vector<Real> Time_step;
  
  /// The equations
  EQUATIONS_TYPE *Equations_pt;
  
 };
 
}

#endif // #ifndef ACIBVP_TPL_H
