#ifndef CCBACKWARDEULERMETHOD_TPL_H
#define CCBACKWARDEULERMETHOD_TPL_H

#include "ac_time_stepper.h"
#include "../matrices/cc_matrix.h"
#include "../linear_solvers/ac_linear_solver.h"
#include "../linear_solvers/cc_lu_solver_numerical_recipes.h"

#ifdef CHAPCHOM_USES_ARMADILLO
// Include Armadillo type matrices since the templates may include
// Armadillo type matrices
#include "../matrices/cc_matrix_armadillo.h"
// Linear solver using ARMADILLO matrices
#include "../linear_solvers/cc_solver_armadillo.h"
#endif // #ifdef CHAPCHOM_USES_ARMADILLO

// Newton's method
#include "cc_newtons_method_for_backward_euler.h"

// The Jacobian strategy used for Newton's method for Backward Euler
#include "cc_jacobian_and_residual_for_backward_euler.h"

// Time stepper to compute the initial guess for Newton's method
#include "cc_euler_method.h"

namespace chapchom
{
 
 /// @class CCBackwardEulerMethod cc_backward_euler_method.h This
 /// class implements Backward Euler's method to integrate ODE's
 template<class MAT_TYPE, class VEC_TYPE>
  class CCBackwardEulerMethod : public virtual ACTimeStepper
 {
  
 public:
  
  /// Constructor
  CCBackwardEulerMethod();
  
  /// Empty destructor
  virtual ~CCBackwardEulerMethod();
  
  /// Applies Backward Eulers method to the given odes from the
  /// current time "t" to the time "t+h".
  void time_step(ACODEs &odes, const double h, const double t,
                 CCData<double> &u);
  
 protected:
  
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 CCBackwardEulerMethod(const CCBackwardEulerMethod<MAT_TYPE, VEC_TYPE> &copy)
  : ACTimeStepper()
   {
    BrokenCopy::broken_copy("CCBackwardEulerMethod");
   }
 
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(const CCBackwardEulerMethod<MAT_TYPE, VEC_TYPE> &copy)
   {
    BrokenCopy::broken_assign("CCBackwardEulerMethod");
   }
  
  // Newton's method for backward Euler
  CCNewtonsMethodForBackwardEuler<MAT_TYPE, VEC_TYPE> Newtons_method;
  
  // A linear solver for Newton's method
  ACLinearSolver<MAT_TYPE, VEC_TYPE> *Linear_solver_pt;
  
  // A pointer for the strategy to compute the Jacobian and the
  // residual during time stepping call for Newton's method
  CCJacobianAndResidualForBackwardEuler<MAT_TYPE, VEC_TYPE> *Jacobian_and_residual_for_backward_euler_pt;
  
  // The time stepper used to compute the initial guess
  ACTimeStepper *Time_stepper_initial_guess_pt;
  
 };
 
}

#endif // #ifndef CCBACKWARDEULERMETHOD_TPL_H