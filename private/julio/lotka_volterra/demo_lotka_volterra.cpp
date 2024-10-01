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
// Include SciCell++ libraries
#include "../../../src/scicellxx.h"

// Odes for Lotka-Volkaterra problem
#include "cc_lotka_volterra_odes.h"

using namespace scicellxx;

/// This class implements inherits from the ACIVPForODEs class, we
/// implement specific functions to solve the Lotka-Volterra equations
class CCLotkaVolterraProblem : public virtual ACIVPForODEs
{
  
public:
 
 /// Constructor
 CCLotkaVolterraProblem(ACODEs *odes_pt,
                        ACTimeStepper *time_stepper_pt,
                        std::ostringstream &output_filename)
  : ACIVPForODEs(odes_pt, time_stepper_pt)
 {
  Output_file.open((output_filename.str()).c_str());
 }
 
 /// Destructor
 ~CCLotkaVolterraProblem()
 {
  Output_file.close();
 }
 
 // Set initial conditions
 void set_initial_conditions()
 {
  // Initial conditions
  u(0) = 2.0; // Initial number of prey
  u(1) = 1.0; // Initial number of predators
  //u(0) = 0.9; // Initial number of prey
  //u(1) = 0.9; // Initial number of predators 
 }
 
 // Set boundary conditions
 void set_boundary_conditions() { }
 
 // Document the solution
 void document_solution()
 {
  const Real t = this->time();
  // Initial problem configuration
  Output_file << t << "\t" << u(0) << "\t" << u(1) << std::endl;
 }
 
protected:

 // The output file
 std::ofstream Output_file;
 
}; // class CCLotkaVolterraProblem

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
 CCFactoryTimeStepper factory_time_stepper;
 
 // Euler method test
 {
  std::cout << "Euler test" << std::endl;
  // -----------------------------------------------------------------
  // Instantiation of the ODEs
  // -----------------------------------------------------------------
  CCLotkaVolterraODEs odes(1.2, 0.6, 0.8, 0.3);
  //CCLotkaVolterraODEs odes(2.0/3.0, 4.0/3.0, 1.0, 1.0);
  
  // ----------------------------------------------------------------
  // Time stepper
  // ----------------------------------------------------------------
  // Create an instance of the integration method
  ACTimeStepper *time_stepper_pt =
   factory_time_stepper.create_time_stepper("Euler");
  
  // ----------------------------------------------------------------
  // Prepare the output file name
  // ----------------------------------------------------------------
  std::ostringstream output_filename;
  output_filename << "RESLT/euler.dat";
  output_filename.precision(8);
  
  // Create an instance of the problem
  CCLotkaVolterraProblem lotka_volterra_problem(&odes,
                                                time_stepper_pt,
                                                output_filename);
  
  // Prepare time integration data
  const Real initial_time = 0.0;
  const Real final_time = 40.0;
  const Real time_step = 0.0625;
  
  // ----------------------------------------------------------------
  // Configure problem
  // ----------------------------------------------------------------
  // Initial time
  lotka_volterra_problem.time() = initial_time;
  
  // Initial time step
  lotka_volterra_problem.time_step() = time_step;
  
  // Set initial conditions
  lotka_volterra_problem.set_initial_conditions();
  
  // Document intiial configuration
  lotka_volterra_problem.document_solution();
  
  // Flag to indicate whether to continue processing
  bool LOOP = true;
 
  // Main LOOP (loop until reaching final time)
  while(LOOP)
   {
    // Performs an unsteady solve
    lotka_volterra_problem.solve();
    
    // Update time of the problem
    lotka_volterra_problem.time()+=lotka_volterra_problem.time_step();
    
    // Check whether we have reached the final time
    if (lotka_volterra_problem.time() >= final_time)
     {
      LOOP = false;
     }
    
    lotka_volterra_problem.document_solution();
    
   } // while(LOOP)
  
  // Free memory
  delete time_stepper_pt;
  time_stepper_pt = 0;
  
 }
 
 {
  std::cout << "Runge-Kutta 4 test" << std::endl;
  // -----------------------------------------------------------------
  // Instantiation of the ODEs
  // -----------------------------------------------------------------
  CCLotkaVolterraODEs odes(1.2, 0.6, 0.8, 0.3);
  //CCLotkaVolterraODEs odes(2.0/3.0, 4.0/3.0, 1.0, 1.0);
  
  // ----------------------------------------------------------------
  // Time stepper
  // ----------------------------------------------------------------
  ACTimeStepper *time_stepper_pt =
   factory_time_stepper.create_time_stepper("RK4");
  
  // ----------------------------------------------------------------
  // Prepare the output file name
  // ----------------------------------------------------------------
  std::ostringstream output_filename;
  output_filename << "RESLT/rk4.dat";
  output_filename.precision(8);
  
  // Create an instance of the problem
  CCLotkaVolterraProblem lotka_volterra_problem(&odes,
                                                time_stepper_pt,
                                                output_filename);
  
  // Prepare time integration data
  const Real initial_time = 0.0;
  const Real final_time = 40.0;
  const Real time_step = 0.0625;
  
  // ----------------------------------------------------------------
  // Configure problem
  // ----------------------------------------------------------------
  // Initial time
  lotka_volterra_problem.time() = initial_time;
  
  // Initial time step
  lotka_volterra_problem.time_step() = time_step;
  
  // Set initial conditions
  lotka_volterra_problem.set_initial_conditions();

  // Document intiial configuration
  lotka_volterra_problem.document_solution();
  
  // Flag to indicate whether to continue processing
  bool LOOP = true;
  
  // Main LOOP (loop until reaching final time)
  while(LOOP)
   {
    // Performs an unsteady solve
    lotka_volterra_problem.solve();
    
    // Update time of the problem
    lotka_volterra_problem.time()+=lotka_volterra_problem.time_step();
    
    // Check whether we have reached the final time
    if (lotka_volterra_problem.time() >= final_time)
     {
      LOOP = false;
     }
    
    lotka_volterra_problem.document_solution();
    
   } // while(LOOP)
  
  // Free memory
  delete time_stepper_pt;
  time_stepper_pt = 0;
  
 }

 {
  std::cout << "Adams-Moulton 2 - Predictor-Corrector test" << std::endl;
  // -----------------------------------------------------------------
  // Instantiation of the ODEs
  // -----------------------------------------------------------------
  CCLotkaVolterraODEs odes(1.2, 0.6, 0.8, 0.3);
  //CCLotkaVolterraODEs odes(2.0/3.0, 4.0/3.0, 1.0, 1.0);
  
  // ----------------------------------------------------------------
  // Time stepper
  // ----------------------------------------------------------------
  ACTimeStepper *time_stepper_pt =
   factory_time_stepper.create_time_stepper("AM2PC");
  
  // ----------------------------------------------------------------
  // Prepare the output file name
  // ----------------------------------------------------------------
  std::ostringstream output_filename;
  output_filename << "RESLT/am2pc.dat";
  output_filename.precision(8);
  
  // Create an instance of the problem
  CCLotkaVolterraProblem lotka_volterra_problem(&odes,
                                                time_stepper_pt,
                                                output_filename);
  
  // Prepare time integration data
  const Real initial_time = 0.0;
  const Real final_time = 40.0;
  const Real time_step = 0.1;
  
  // ----------------------------------------------------------------
  // Configure problem
  // ----------------------------------------------------------------
  // Initial time
  lotka_volterra_problem.time() = initial_time;
  
  // Initial time step
  lotka_volterra_problem.time_step() = time_step;
  
  // Set initial conditions
  lotka_volterra_problem.set_initial_conditions();

  // Document intiial configuration
  lotka_volterra_problem.document_solution();
  
  // Flag to indicate whether to continue processing
  bool LOOP = true;
  
  // Main LOOP (loop until reaching final time)
  while(LOOP)
   {
    // Performs an unsteady solve
    lotka_volterra_problem.solve();
    
    // Update time of the problem
    lotka_volterra_problem.time()+=lotka_volterra_problem.time_step();
    
    // Check whether we have reached the final time
    if (lotka_volterra_problem.time() >= final_time)
     {
      LOOP = false;
     }
    
    lotka_volterra_problem.document_solution();
    
   } // while(LOOP)
  
  // Free memory
  delete time_stepper_pt;
  time_stepper_pt = 0;
  
 }
 
 {
  std::cout << "BDF 1 test" << std::endl;
  // -----------------------------------------------------------------
  // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCLotkaVolterraODEs odes(1.2, 0.6, 0.8, 0.3);
   //CCLotkaVolterraODEs odes(2.0/3.0, 4.0/3.0, 1.0, 1.0);
  
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper *time_stepper_pt =
    factory_time_stepper.create_time_stepper("BDF1");
  
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/bdf1.dat";
   output_filename.precision(8);
  
   // Create an instance of the problem
   CCLotkaVolterraProblem lotka_volterra_problem(&odes,
                                                 time_stepper_pt,
                                                 output_filename);
  
   // Prepare time integration data
   const Real initial_time = 0.0;
   const Real final_time = 40.0;
   const Real time_step = 0.0625;
  
   // ----------------------------------------------------------------
   // Configure problem
   // ----------------------------------------------------------------
   // Initial time
   lotka_volterra_problem.time() = initial_time;
  
   // Initial time step
   lotka_volterra_problem.time_step() = time_step;
  
   // Set initial conditions
   lotka_volterra_problem.set_initial_conditions();
   
   // Document intiial configuration
   lotka_volterra_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Performs an unsteady solve
     lotka_volterra_problem.solve();
    
     // Update time of the problem
     lotka_volterra_problem.time()+=lotka_volterra_problem.time_step();
    
     // Check whether we have reached the final time
     if (lotka_volterra_problem.time() >= final_time)
      {
       LOOP = false;
      }
    
     lotka_volterra_problem.document_solution();
    
    } // while(LOOP)
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;

  }
 
  {
   std::cout << "Adams-Moulton 2 test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCLotkaVolterraODEs odes(1.2, 0.6, 0.8, 0.3);
   //CCLotkaVolterraODEs odes(2.0/3.0, 4.0/3.0, 1.0, 1.0);
  
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper *time_stepper_pt =
    factory_time_stepper.create_time_stepper("AM2");
  
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/am2.dat";
   output_filename.precision(8);
  
   // Create an instance of the problem
   CCLotkaVolterraProblem lotka_volterra_problem(&odes,
                                                 time_stepper_pt,
                                                 output_filename);
  
   // Prepare time integration data
   const Real initial_time = 0.0;
   const Real final_time = 40.0;
   const Real time_step = 0.1;
  
   // ----------------------------------------------------------------
   // Configure problem
   // ----------------------------------------------------------------
   // Initial time
   lotka_volterra_problem.time() = initial_time;
  
   // Initial time step
   lotka_volterra_problem.time_step() = time_step;
  
   // Set initial conditions
   lotka_volterra_problem.set_initial_conditions();
   
   // Document intiial configuration
   lotka_volterra_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Performs an unsteady solve
     lotka_volterra_problem.solve();
    
     // Update time of the problem
     lotka_volterra_problem.time()+=lotka_volterra_problem.time_step();
    
     // Check whether we have reached the final time
     if (lotka_volterra_problem.time() >= final_time)
      {
       LOOP = false;
      }
    
     lotka_volterra_problem.document_solution();
    
    } // while(LOOP)
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;

  }
  
  {
   std::cout << "BDF 2 test" << std::endl;
   // -----------------------------------------------------------------
   // Instantiation of the ODEs
   // -----------------------------------------------------------------
   CCLotkaVolterraODEs odes(1.2, 0.6, 0.8, 0.3);
   //CCLotkaVolterraODEs odes(2.0/3.0, 4.0/3.0, 1.0, 1.0);
   
   // ----------------------------------------------------------------
   // Time stepper
   // ----------------------------------------------------------------
   ACTimeStepper *time_stepper_pt =
    factory_time_stepper.create_time_stepper("BDF2");
   
   // ----------------------------------------------------------------
   // Prepare the output file name
   // ----------------------------------------------------------------
   std::ostringstream output_filename;
   output_filename << "RESLT/bdf2.dat";
   output_filename.precision(8);
   
   // Create an instance of the problem
   CCLotkaVolterraProblem lotka_volterra_problem(&odes,
                                                 time_stepper_pt,
                                                 output_filename);
   
   // Prepare time integration data
   const Real initial_time = 0.0;
   const Real final_time = 40.0;
   const Real time_step = 0.1;
   
   // ----------------------------------------------------------------
   // Configure problem
   // ----------------------------------------------------------------
   // Initial time
   lotka_volterra_problem.time() = initial_time;
  
   // Initial time step
   lotka_volterra_problem.time_step() = time_step;
  
   // Set initial conditions
   lotka_volterra_problem.set_initial_conditions();
   
   // Document intiial configuration
   lotka_volterra_problem.document_solution();
   
   // Flag to indicate whether to continue processing
   bool LOOP = true;
  
   // Main LOOP (loop until reaching final time)
   while(LOOP)
    {
     // Performs an unsteady solve
     lotka_volterra_problem.solve();
    
     // Update time of the problem
     lotka_volterra_problem.time()+=lotka_volterra_problem.time_step();
    
     // Check whether we have reached the final time
     if (lotka_volterra_problem.time() >= final_time)
      {
       LOOP = false;
      }
    
     lotka_volterra_problem.document_solution();
    
    } // while(LOOP)
  
   // Free memory
   delete time_stepper_pt;
   time_stepper_pt = 0;

  }
  
  // Finalise scicellxx
  finalise_scicellxx();
  
  return 0;
 
}
