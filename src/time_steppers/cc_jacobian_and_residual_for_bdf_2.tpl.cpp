#include "cc_jacobian_and_residual_for_bdf_2.tpl.h"

namespace chapchom
{
 // ===================================================================
 // Constructor
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::CCJacobianAndResidualForBDF2()
  : ACJacobianAndResidual<MAT_TYPE, VEC_TYPE>(),
  ODEs_pt(NULL),
  ODEs_has_been_set(false), 
  U_pt(NULL),
  U_has_been_set(false),
  U_new_pt(NULL),
  U_new_has_been_set(false),
  Current_time_has_been_set(false),
  Time_step_has_been_set(false)
 {
  
 }
 
 // ===================================================================
 // Destructor
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::~CCJacobianAndResidualForBDF2()
 {
  
 }
 
 // ===================================================================
 // In charge of computing the Jacobian
 // (virtual function implementation)
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 void CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::compute_jacobian()
 {
  // Check whether the ODEs have been set
  if (!ODEs_has_been_set || ODEs_pt == NULL)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the ODEs used to compute\n"
                  << "the Jacobian matrix.\n"
                  << "You need to call the method set_ODEs()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the U values have been set
  if (!U_has_been_set || U_pt == NULL)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the U function values\n"
                  << "used to compute the Jacobian matrix.\n"
                  << "You need to call the method set_U()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the U_new values have been set
  if (!U_new_has_been_set || U_new_pt == NULL)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the U_new function values\n"
                  << "used to compute the Jacobian matrix.\n"
                  << "You need to call the method set_U_new()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the constant time has been set
  if (!Current_time_has_been_set)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the current time\n"
                  << "used to compute the Jacobian matrix.\n"
                  << "You need to call the method set_current_time()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the time step has been set
  if (!Time_step_has_been_set)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the time step to compute\n"
                  << "the residual vector and the Jacobian matrix.\n"
                  << "You need to call the method set_time_step()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Get the number of ODEs
  const unsigned n_dof = ODEs_pt->n_odes();
  
  // Allocate memory for the Jacobian (delete previous data)
  this->Jacobian.allocate_memory(n_dof, n_dof);
  
  // Set the data required to compute the Jacobian of F(Y)
  Jacobian_FY_strategy.set_ODEs(ODEs_pt);
  Jacobian_FY_strategy.set_U(U_new_pt);
  Jacobian_FY_strategy.set_current_time(Current_time+Time_step+Time_step);
  
  // Compute Jacobian
  Jacobian_FY_strategy.compute_jacobian();
  
  // Store the Jacobian for FY, used in the computation of the BDF2
  // Jacobian $J = I - \frac{2}{3}h J_{FY}$
  MAT_TYPE Jacobian_FY = Jacobian_FY_strategy.jacobian();
  
  // Two thirds of the time step
  const Real two_thirds_time_step = (2.0/3.0) * Time_step;
  
  // Compute the approximated Jacobian (I - \frac{2}{3}h * Jacobian_FY(i, j))
  for (unsigned i = 0; i < n_dof; i++)
   {
    for (unsigned j = 0; j < n_dof; j++)
     {
      if (i == j)
       {
        this->Jacobian(i, j) = 1.0 - (two_thirds_time_step * Jacobian_FY(i, j));
       }
      else
       {
        this->Jacobian(i, j) = 0.0 - (two_thirds_time_step * Jacobian_FY(i, j));
       }
     }
   }
  
 }
 
 // ===================================================================
 // In charge of computing the residual
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 void CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::compute_residual()
 {
  // Check whether the ODEs have been set
  if (!ODEs_has_been_set || ODEs_pt == NULL)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the ODEs used to compute\n"
                  << "the Jacobian matrix.\n"
                  << "You need to call the method set_ODEs()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the U values have been set
  if (!U_has_been_set || U_pt == NULL)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the U function values\n"
                  << "used to compute the Jacobian matrix.\n"
                  << "You need to call the method set_U()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the U_new values have been set
  if (!U_new_has_been_set || U_new_pt == NULL)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the U_new function values\n"
                  << "used to compute the Jacobian matrix.\n"
                  << "You need to call the method set_U_new()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the constant time has been set
  if (!Current_time_has_been_set)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the current time\n"
                  << "used to compute the Jacobian matrix.\n"
                  << "You need to call the method set_current_time()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Check whether the time step has been set
  if (!Time_step_has_been_set)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established the time step to compute\n"
                  << "the residual vector and the Jacobian matrix.\n"
                  << "You need to call the method set_time_step()\n"
                  << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
  
  // Get the number of ODEs
  const unsigned n_dof = ODEs_pt->n_odes();
  
  // Temporary vector to store the evaluation of the odes with
  // U_new_pt (current Newton's iteration)
  CCData<Real> dudt_new(n_dof);
  
  // Time step weights
  const Real two_thirds_time_step = (2.0/3.0) * Time_step;
  // Previous Y value weights
  const Real one_third = 1.0/3.0;
  const Real four_thirds = 4.0/3.0;
  
  // Evaluate the ODE at time "t+2" using the "u" values of the current
  // Newton's iteration
  ODEs_pt->evaluate(Current_time+Time_step+Time_step, (*U_new_pt), dudt_new);
  
  // Allocate memory for the Residual (delete previous data)
 this->Residual.allocate_memory(n_dof);
  
 for (unsigned i = 0; i < n_dof; i++)
   {
    this->Residual(i) =
     -(U_new_pt->value(i) - (four_thirds * U_pt->value(i,1)) + (one_third * U_pt->value(i,0)) - (two_thirds_time_step * dudt_new(i)));
   }
  
 }
 
 // ===================================================================
 // Set the ODEs
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 void CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::set_ODEs(ACODEs *odes_pt)
 {
  // Set the odes
  ODEs_pt = odes_pt;
  
  // Indicate that the ODEs have been set
  ODEs_has_been_set = true;
  
 }
 
 // ===================================================================
 // Set the U vector/matrix with the values of the function at the
 // current time
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 void CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::set_U(CCData<Real> *u_pt)
 {
  // Set the storage of the data
  U_pt = u_pt;
  
  // Indicate that the U vector has been set
  U_has_been_set = true;
  
 }

 // ===================================================================
 // Set the U vector/matrix with the values of the function at the
 // current Newton's iteration
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 void CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::set_U_new(CCData<Real> *u_new_pt)
 {
  // Set the storage of the data
  U_new_pt = u_new_pt;
  
  // Indicate that the U_new vector has been set
  U_new_has_been_set = true;
  
 }
 
 // ===================================================================
 // Sets the current time
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 void CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::set_current_time(const Real t)
 {
  // Set the constant time
  Current_time = t;
  
  // Indicate that the current time has been set
  Current_time_has_been_set = true; 
 }
  
 // ===================================================================
 // Sets the time step
 // ===================================================================
 template<class MAT_TYPE, class VEC_TYPE>
 void CCJacobianAndResidualForBDF2<MAT_TYPE, VEC_TYPE>::set_time_step(const Real h)
 {
  // Set the time step 
  Time_step = h;

  // Indicate that the time step has been set
  Time_step_has_been_set = true;
  
 }
 
}

