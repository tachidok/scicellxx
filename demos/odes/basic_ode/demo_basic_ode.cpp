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
#include <iostream>
#include <cmath>
#include <fstream>

// Include SciCell++ libraries
#include "../../../src/scicellxx.h"

using namespace scicellxx;
// =================================================================
// =================================================================
// =================================================================
/// This class implements the system of ODEs to be solved
///
/// \frac{du}{dt} = -u^{2}, with initial values u(0) = 1
// =================================================================
// =================================================================
// =================================================================
class CCBasicODEs : public virtual ACODEs
{
 
public:
 
 /// Constructor
 CCBasicODEs()
  :ACODEs(1) // The number of equations
 {
  
 }
 
 /// Empty destructor
 virtual ~CCBasicODEs()
 {
  
 }
 
 /// Evaluates the system of odes at time 't', using the history
 /// values of u at index k
 void evaluate_time_derivatives(const Real t, CCData &u, CCData &dudt, const unsigned k = 0)
 {
  // \frac{du}{dt} = -u^{2}
  dudt(0) = -(u(0,k)*u(0,k));
 }
 
protected:
 
 /// Copy constructor (we do not want this class to be
 /// copiable). Check
 /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 CCBasicODEs(const CCBasicODEs &copy)
  : ACODEs(copy)
 {
  BrokenCopy::broken_copy("CCBasicODEs");
 }
 
 /// Assignment operator (we do not want this class to be
 /// copiable. Check
 /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 void operator=(const CCBasicODEs &copy)
 {
  BrokenCopy::broken_assign("CCBasicODEs");
 }
 
};

// =================================================================
// =================================================================
// =================================================================
// This class implements the strategy for the computation of the
// Jacobian of the system of ODEs to be solved (it is optional to
// implement it for your specific problem, but if you know the
// Jacobian the specification of this class may reduce the
// computational cost)
//
// \frac{dF}{du} = -2u
// =================================================================
// =================================================================
// =================================================================
template<class EQUATIONS_TYPE>
class CCJacobianStrategyForMyODEs : public virtual ACJacobianAndResidualForImplicitTimeStepper<EQUATIONS_TYPE>
{
 
public:
 
 // Empty constructor
 CCJacobianStrategyForMyODEs()
  : ACJacobianAndResidualForImplicitTimeStepper<EQUATIONS_TYPE>()
 {
  
 }
 
 // Empty destructor
 ~CCJacobianStrategyForMyODEs()
 {
  
 }
 
 // In charge of computing the Jacobian
 void compute_jacobian()
 {
  // Get a pointer to the ODEs
  ACODEs *odes_pt = this->odes_pt();
  
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
  
  // Double check that the number of odes is correct
  if (n_dof != 1)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The number of odes is not correct (ehhh!!!!)\n"
                  << std::endl;
    throw SciCellxxLibError(error_message.str(),
                           SCICELLXX_CURRENT_FUNCTION,
                           SCICELLXX_EXCEPTION_LOCATION);
   }
  
  // Allocate memory for the Jacobian (delete previous data)
  this->Jacobian_pt->allocate_memory(n_dof, n_dof);
  
  // Get the index of history values of the u vector at time 't+h'
  // that should be used to compute the values of the Jacobian
  const unsigned k = this->history_index();
  
  // Fill in the Jacobian
  this->Jacobian_pt->value(0, 0) = -(2*u_pt->value(0,k));
  
 }
 
 // No residual computation
 void compute_residual()
 {
  
 }
 
private:
 
 // Copy constructor (we do not want this class to be copiable because
 // it contains dynamically allocated variables, A in this
 // case). Check
 // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 CCJacobianStrategyForMyODEs(const CCJacobianStrategyForMyODEs &copy)
  : ACJacobianAndResidualForImplicitTimeStepper<EQUATIONS_TYPE>()
 {
  BrokenCopy::broken_copy("CCJacobianStrategyForMyODEs");
 }
 
 // Copy constructor (we do not want this class to be copiable because
 // it contains dynamically allocated variables, A in this
 // case). Check
 // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 void operator=(const CCJacobianStrategyForMyODEs &copy)
 {
  BrokenCopy::broken_assign("CCJacobianStrategyForMyODEs");
 }
 
};

// =================================================================
// =================================================================
// =================================================================
// This class inherits from the ACIBVP class and solves the system of
// ODEs from above
// =================================================================
// =================================================================
// =================================================================
template<class EQUATIONS_TYPE>
class CCBasicODEsProblem : public virtual ACIBVP<EQUATIONS_TYPE>
{
 
 public:
 
  /// Constructor
  CCBasicODEsProblem(EQUATIONS_TYPE *odes_pt,
                     ACTimeStepper<EQUATIONS_TYPE> *time_stepper_pt,
                     std::ostringstream &output_filename,
                     std::ostringstream &output_filename_error)
   : ACIBVP<EQUATIONS_TYPE>(odes_pt, time_stepper_pt)
  {
   Output_file.open((output_filename.str()).c_str());
   Output_error_file.open((output_filename_error.str()).c_str());
  }
 
  /// Destructor
  ~CCBasicODEsProblem()
  {
   Output_file.close();
   Output_error_file.close();
  }
  
  /// Complete the problem setup
  void complete_problem_setup()
  {
   // Prepare time integration data
   const Real initial_time = 0.0;
   const Real final_time = 2.0;
   const Real time_step = 0.1;
   // ----------------------------------------------------------------
   // Complete problem configuration
   // ----------------------------------------------------------------
   
   // Initial time
   this->time() = initial_time;
   
   // Initial time step
   this->time_step() = time_step;
   
   Final_time = final_time;
   
   // Set initial conditions
   set_initial_conditions();
  }
  
  // Set initial conditions
  void set_initial_conditions()
  {
   // Initial conditions
   this->u(0) = 1.0;
  }
  
  // Document the solution
  void document_solution()
  {
   // Get the inital time
   const Real t = this->time();
   // Initial problem configuration
   Output_file << t << "\t" << this->u(0) << std::endl;
   output_error();
  }
  
  // Output error
  void output_error()
  {
   // Compute the error 
   const Real t = this->time();
   const Real u_analytical = 1.0/(1.0+t);
   const Real error = std::fabs(this->u(0)-u_analytical);
   Output_error_file << t << "\t" << error << std::endl;
  }
  
  // Return the final time
  inline Real final_time() const {return Final_time;}
  
 protected:
 
  // The output file
  std::ofstream Output_file;
  // The error output file
  std::ofstream Output_error_file;
  
  // Final integration time
  Real Final_time;
  
 }; // class CCBasicODEsProblem<CCBasicODEs>

 // ==================================================================
 // ==================================================================
 // ==================================================================
 // Main function
 // ==================================================================
 // ==================================================================
 // ==================================================================
 int main(int argc, char *argv[])
 {
  // Initialise scicellxx
  initialise_scicellxx();
  
  // Create the factory for the time steppers (integration methods)
  // specifically for the ODEs on this demo
  CCFactoryTimeStepper<CCBasicODEs> factory_time_stepper;
  
  // Euler method test
  {
   std::cout << "Euler test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCBasicODEs odes;
   
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   // Create an instance of the integration method for the specific
   // ODEs of this demo
   ACTimeStepper<CCBasicODEs> *time_stepper_pt =
    factory_time_stepper.create_time_stepper("Euler");
   
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/euler.dat";
   output_filename.precision(8);
   
   // ----------------------------------------------------------------
   // Prepare the output error file name
   // ----------------------------------------------------------------
   std::ostringstream output_error_filename;
   output_error_filename << "RESLT/euler_error.dat";
   output_error_filename.precision(8);
   
   // Create an instance of the problem for the specific ODEs on this demo
   CCBasicODEsProblem<CCBasicODEs> basic_ode_problem(&odes,
                                                     time_stepper_pt,
                                                     output_filename,
                                                     output_error_filename);
   
   // Complete problem setup (time integration data and initial
   // conditions)
   basic_ode_problem.complete_problem_setup();
   
   // Document initial configuration
   basic_ode_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
   
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Solve (unsteady solve)
     basic_ode_problem.solve();
     
     // Update time of the problem
     basic_ode_problem.time()+=basic_ode_problem.time_step();
    
     // Check whether we have reached the final time
     if (basic_ode_problem.time() >= basic_ode_problem.final_time())
      {
       LOOP = false;
      }
    
     basic_ode_problem.document_solution();
    
    } // while(LOOP)
  
   std::cout << "[FINISHING UP] ... " << std::endl;
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;
  
  }
  
  {
   std::cout << "Runge-Kutta 4 test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCBasicODEs odes;
  
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper<CCBasicODEs> *time_stepper_pt =
    factory_time_stepper.create_time_stepper("RK4");
  
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/rk4.dat";
   output_filename.precision(8);

   // ----------------------------------------------------------------
   // Prepare the output error file name
   // ----------------------------------------------------------------
   std::ostringstream output_error_filename;
   output_error_filename << "RESLT/rk4_error.dat";
   output_error_filename.precision(8);
  
   // Create an instance of the problem
   CCBasicODEsProblem<CCBasicODEs> basic_ode_problem(&odes,
                                                     time_stepper_pt,
                                                     output_filename,
                                                     output_error_filename);

   // Complete problem setup (time integration data and initial
   // conditions)
   basic_ode_problem.complete_problem_setup();
   
   // Document initial configuration
   basic_ode_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Solve (unsteady solve)
     basic_ode_problem.solve();
    
     // Update time of the problem
     basic_ode_problem.time()+=basic_ode_problem.time_step();
    
     // Check whether we have reached the final time
     if (basic_ode_problem.time() >= basic_ode_problem.final_time())
      {
       LOOP = false;
      }
    
     basic_ode_problem.document_solution();
    
    } // while(LOOP)
  
   std::cout << "[FINISHING UP] ... " << std::endl;
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;
  
  }
  
  {
   std::cout << "Backward-Euler - Predictor-Corrector test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCBasicODEs odes;
   
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper<CCBasicODEs> *time_stepper_pt =
    factory_time_stepper.create_time_stepper("BEPC");
   
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/bepc.dat";
   output_filename.precision(8);
   
   // ----------------------------------------------------------------
   // Prepare the output error file name
   // ----------------------------------------------------------------
   std::ostringstream output_error_filename;
   output_error_filename << "RESLT/bepc_error.dat";
   output_error_filename.precision(8);
   
   // Create an instance of the problem
   CCBasicODEsProblem<CCBasicODEs> basic_ode_problem(&odes,
                                        time_stepper_pt,
                                        output_filename,
                                        output_error_filename);

   // Complete problem setup (time integration data and initial
   // conditions)
   basic_ode_problem.complete_problem_setup();
   
   // Document initial configuration
   basic_ode_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Solve (unsteady solve)
     basic_ode_problem.solve();
    
     // Update time of the problem
     basic_ode_problem.time()+=basic_ode_problem.time_step();
    
     // Check whether we have reached the final time
     if (basic_ode_problem.time() >= basic_ode_problem.final_time())
      {
       LOOP = false;
      }
    
     basic_ode_problem.document_solution();
    
    } // while(LOOP)
  
   std::cout << "[FINISHING UP] ... " << std::endl;
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;
  
  }
  
  {
   std::cout << "Adams-Moulton 2 or Trapezoidal Rule - Predictor-Corrector test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCBasicODEs odes;
  
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper<CCBasicODEs> *time_stepper_pt =
    factory_time_stepper.create_time_stepper("AM2PC");
  
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/am2pc.dat";
   output_filename.precision(8);

   // ----------------------------------------------------------------
   // Prepare the output error file name
   // ----------------------------------------------------------------
   std::ostringstream output_error_filename;
   output_error_filename << "RESLT/am2pc_error.dat";
   output_error_filename.precision(8);
  
   // Create an instance of the problem
   CCBasicODEsProblem<CCBasicODEs> basic_ode_problem(&odes,
                                        time_stepper_pt,
                                        output_filename,
                                        output_error_filename);

   // Complete problem setup (time integration data and initial
   // conditions)
   basic_ode_problem.complete_problem_setup();
   
   // Document initial configuration
   basic_ode_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Solve (unsteady solve)
     basic_ode_problem.solve();
     
     // Update time of the problem
     basic_ode_problem.time()+=basic_ode_problem.time_step();
    
     // Check whether we have reached the final time
     if (basic_ode_problem.time() >= basic_ode_problem.final_time())
      {
       LOOP = false;
      }
    
     basic_ode_problem.document_solution();
    
    } // while(LOOP)
  
   std::cout << "[FINISHING UP] ... " << std::endl;
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;
  
  }
  
  {
   std::cout << "Backward Differentiation Formula 1 - Fully Implicit test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCBasicODEs odes;
   
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper<CCBasicODEs> *time_stepper_pt =
    factory_time_stepper.create_time_stepper("BDF1");
   
   // Create an instance of the strategy to compute the Jacobian for
   // the specific ODEs on this demo
   CCJacobianStrategyForMyODEs<CCBasicODEs> my_jacobian_strategy;
   
   // Get a pointer to the specific time stepper
   CCBackwardEulerMethod<CCBasicODEs> *backward_euler_time_stepper_pt =
    dynamic_cast<CCBackwardEulerMethod<CCBasicODEs> *>(time_stepper_pt);
   
   // Set the Jacobian strategy for the time stepper
   backward_euler_time_stepper_pt->set_strategy_for_odes_jacobian(&my_jacobian_strategy);
   
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/bdf1.dat";
   output_filename.precision(8);

   // ----------------------------------------------------------------
   // Prepare the output error file name
   // ----------------------------------------------------------------
   std::ostringstream output_error_filename;
   output_error_filename << "RESLT/bdf1_error.dat";
   output_error_filename.precision(8);
  
   // Create an instance of the problem
   CCBasicODEsProblem<CCBasicODEs> basic_ode_problem(&odes,
                                        time_stepper_pt,
                                        output_filename,
                                        output_error_filename);

   // Complete problem setup (time integration data and initial
   // conditions)
   basic_ode_problem.complete_problem_setup();

   // Document initial configuration
   basic_ode_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Solve (unsteady solve)
     basic_ode_problem.solve();
    
     // Update time of the problem
     basic_ode_problem.time()+=basic_ode_problem.time_step();
    
     // Check whether we have reached the final time
     if (basic_ode_problem.time() >= basic_ode_problem.final_time())
      {
       LOOP = false;
      }
    
     basic_ode_problem.document_solution();
    
    } // while(LOOP)
  
   std::cout << "[FINISHING UP] ... " << std::endl;
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;
  
  }
  
  {
   std::cout << "Adams-Moulton 2 or Trapezoidal Rule - Fully Implicit test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCBasicODEs odes;
  
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper<CCBasicODEs> *time_stepper_pt =
    factory_time_stepper.create_time_stepper("AM2");
   
   // Create an instance of the strategy to compute the Jacobian 
   CCJacobianStrategyForMyODEs<CCBasicODEs> my_jacobian_strategy;
   
   // Get a pointer to the specific time stepper   
   CCAdamsMoulton2Method<CCBasicODEs> *amd2_time_stepper_pt =
    dynamic_cast<CCAdamsMoulton2Method<CCBasicODEs> *>(time_stepper_pt);
   
   // Set the Jacobian strategy for the time stepper
   amd2_time_stepper_pt->set_strategy_for_odes_jacobian(&my_jacobian_strategy);
   
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/am2.dat";
   output_filename.precision(8);

   // ----------------------------------------------------------------
   // Prepare the output error file name
   // ----------------------------------------------------------------
   std::ostringstream output_error_filename;
   output_error_filename << "RESLT/am2_error.dat";
   output_error_filename.precision(8);
  
   // Create an instance of the problem
   CCBasicODEsProblem<CCBasicODEs> basic_ode_problem(&odes,
                                        time_stepper_pt,
                                        output_filename,
                                        output_error_filename);
   
   // Complete problem setup (time integration data and initial
   // conditions)
   basic_ode_problem.complete_problem_setup();
   
   // Document initial configuration
   basic_ode_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Solve (unsteady solve)
     basic_ode_problem.solve();
    
     // Update time of the problem
     basic_ode_problem.time()+=basic_ode_problem.time_step();
    
     // Check whether we have reached the final time
     if (basic_ode_problem.time() >= basic_ode_problem.final_time())
      {
       LOOP = false;
      }
    
     basic_ode_problem.document_solution();
    
    } // while(LOOP)
  
   std::cout << "[FINISHING UP] ... " << std::endl;
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;
  
  }
  
  {
   std::cout << "Backward Differentiation Formula 2 - Fully Implicit test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCBasicODEs odes;
  
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper<CCBasicODEs> *time_stepper_pt =
    factory_time_stepper.create_time_stepper("BDF2");
   
   // Create an instance of the strategy to compute the Jacobian 
   CCJacobianStrategyForMyODEs<CCBasicODEs> my_jacobian_strategy;
   
   // Get a pointer to the specific time stepper
   CCBDF2Method<CCBasicODEs> *bdf2_time_stepper_pt = dynamic_cast<CCBDF2Method<CCBasicODEs> *>(time_stepper_pt);
   
   // Set the Jacobian strategy for the time stepper
   bdf2_time_stepper_pt->set_strategy_for_odes_jacobian(&my_jacobian_strategy);
   
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/bdf2.dat";
   output_filename.precision(8);

   // ----------------------------------------------------------------
   // Prepare the output error file name
   // ----------------------------------------------------------------
   std::ostringstream output_error_filename;
   output_error_filename << "RESLT/bdf2_error.dat";
   output_error_filename.precision(8);
  
   // Create an instance of the problem
   CCBasicODEsProblem<CCBasicODEs> basic_ode_problem(&odes,
                                        time_stepper_pt,
                                        output_filename,
                                        output_error_filename);

   // Complete problem setup (time integration data and initial
   // conditions)
   basic_ode_problem.complete_problem_setup();
   
   // Document initial configuration
   basic_ode_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Solve (unsteady solve)
     basic_ode_problem.solve();
    
     // Update time of the problem
     basic_ode_problem.time()+=basic_ode_problem.time_step();
    
     // Check whether we have reached the final time
     if (basic_ode_problem.time() >= basic_ode_problem.final_time())
      {
       LOOP = false;
      }
    
     basic_ode_problem.document_solution();
    
    } // while(LOOP)
  
   std::cout << "[FINISHING UP] ... " << std::endl;
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;
  
  }
  
  // Finalise scicellxx
  finalise_scicellxx();
  
  return 0;
 
}
