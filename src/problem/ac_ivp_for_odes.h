#ifndef ACIVPFORODES_H
#define ACIVPFORODES_H

#include "../general/common_includes.h"
#include "../general/utilities.h"

#include "../data_structures/ac_odes.h"
#include "../data_structures/cc_data.h"
#include "ac_problem.h"

namespace scicellxx
{

 /// @class ACIVPForODEs ac_ivp_for_odes.h

 /// This class implements the interfaces for an initial value problem
 /// for ODEs. It specifies a template for solving a problem thus one
 /// needs to create a class that inherents from this one to solve a
 /// particular initial value problem for ODEs
 class ACIVPForODEs : public virtual ACProblem
 {
  
 public:
  
  /// Constructor
  ACIVPForODEs(ACODEs *odes_pt, ACTimeStepper *time_stepper_pt);
  
  /// Destructor
  virtual ~ACIVPForODEs();
  
  /// Get access to the U vector
  CCData *u_pt() const {return U_pt;}
  
  /// Read-only access to the vector U values
  inline const Real u(const unsigned i, const unsigned t = 0) const {return U_pt->value(i,t);}
  
  /// Write access to the vector U values
  inline Real &u(const unsigned i, const unsigned t = 0) {return U_pt->value(i,t);}
  
  // -------------------------------------------------------------------------
  // THESE METHODS MUST BE IMPLEMENTED IN THE CONCRETE PROBLEM CLASS [BEGIN]
  // -------------------------------------------------------------------------
  /// Set initial conditions
  virtual void set_initial_conditions() = 0;
  
  /// Document solution
  virtual void document_solution()
  {
   // Error message
   std::ostringstream error_message;
   error_message << "Virtual function in ACIVPForODEs class, you should implement\n"
                 << "it to document your solution" << std::endl;
   throw SciCellxxLibError(error_message.str(),
                          SCICELLXX_CURRENT_FUNCTION,
                          SCICELLXX_EXCEPTION_LOCATION);
  }
  // -------------------------------------------------------------------------
  // THESE METHODS MUST BE IMPLEMENTED IN THE CONCRETE PROBLEM CLASS [END]
  // -------------------------------------------------------------------------
  
  /// A helper function to complete the problem setup (empty)
  void complete_problem_setup() { }
  
  /// We perform an unsteady solve by default, if you require a
  /// different solving strategy then override this method
  void solve();
  
 protected:
  
  /// Copy constructor (we do not want this class to be
   /// copiable). Check
   /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 ACIVPForODEs(const ACIVPForODEs &copy)
  : ACProblem()
   {
    BrokenCopy::broken_copy("ACIVPForODEs");
   }
  
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const ACIVPForODEs &copy)
   {
    BrokenCopy::broken_assign("ACIVPForODEs");
   } 
  
  /// Problem steady solve (empty)
  void steady_solve() { }
  
  /// Problem unsteady solve
  void unsteady_solve();
  
  /// The set of actions to be performed before a time stepping
  virtual void actions_before_time_stepping() { }
  
  /// The set of actions to be performed after a time stepping
  virtual void actions_after_time_stepping() { } 
  
  /// The set of actions to be performed before newton solve (empty)
  virtual void actions_before_newton_solve() { }
  
  /// The set of actions to be performed after newton solve (empty)
  virtual void actions_after_newton_solve() { }
  
  /// The ODEs
  ACODEs *ODEs_pt;
  
  /// The storage for the approximated solution of the time integration
  /// of the ODEs
  CCData *U_pt;
  
 };
 
}

#endif // #ifndef ACIVPFORODES_H

