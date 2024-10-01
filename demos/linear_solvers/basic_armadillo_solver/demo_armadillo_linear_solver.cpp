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

using namespace scicellxx;

int main(int argc, char *argv[])
{
 // Initialise scicellxx
 initialise_scicellxx();
 
 // Output for testing/validation
 std::ofstream output_test("output_test.dat", std::ios_base::out);
 
 // Create a square matrix
 const unsigned n_rows = 3;
 const unsigned n_cols = n_rows;
 
 // The matrix A
 CCMatrixArmadillo A(n_rows, n_cols);
 // Allocate memory
 //A.allocate_memory();
 
 // Fill in some data
 A(0,0) = 7.0;  A(0,1) = 5.0;  A(0,2) = -3.0;
 A(1,0) = 3.0;  A(1,1) = -5.0;  A(1,2) = 2.0;
 A(2,0) = 5.0;  A(2,1) = 3.0;  A(2,2) = -7.0;
 
 // ----------------------------------------------------------------
 // Solve using vectors for a unique right-hand side
 // ----------------------------------------------------------------
 {
  // The right hand side vector (by default it is created as a column
  // vector)
  CCVectorArmadillo b(n_rows);
  // Allocate memory
  //b.allocate_memory();
  
  // Solve the following system of equations Ax = b
  
  // ... with the following right hand side
  b(0) = 16.0;
  b(1) = -8.0;
  b(2) = 0.0;
  
  // Print the matrices
  std::cout << std::endl;
  std::cout << "Matrix A" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Matrix A" << std::endl;
  output_test << std::endl;
  A.print();
  A.print(output_test);
  std::cout << std::endl;
  std::cout << "Vector b" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Vector b" << std::endl;
  output_test << std::endl;
  b.print();
  b.print(output_test);
  std::cout << std::endl;
  output_test << std::endl;

  // Create a factory for the linear solver
  CCFactoryLinearSolver factory_linear_solver;   
  
  // Create an Armadillo linear solver
  ACLinearSolver *armadillo_linear_solver_pt =
   factory_linear_solver.create_linear_solver("armadillo");
  
  // The solution vector (with the corresponding number of rows, that
  // in this case refers to the number of cols as well)
  CCVectorArmadillo sol(n_cols);
  
  // Solve the system of equations
  armadillo_linear_solver_pt->solve(&A, &b, &sol);
  
  // Print the solution
  std::cout << std::endl;
  std::cout << "Vector sol -----" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Vector sol -----" << std::endl;
  output_test << std::endl;
  sol.print();
  sol.print(output_test);
  std::cout << std::endl;
  output_test << std::endl;
  
   // Apply the solution and check the result
  CCMatrixArmadillo C = A*sol;
  std::cout << std::endl;
  std::cout << "Matrix C" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Matrix C" << std::endl;
  output_test << std::endl;
  C.print();
  C.print(output_test);
 }
 
 // ----------------------------------------------------------------
 // Solve using matrices to store multiple right-hand sides
 // ----------------------------------------------------------------
 {
  // The right hand side vectors
  CCMatrixArmadillo B(n_rows, n_rows);
  // Allocate memory
  //B.allocate_memory();
  
  // Solve the following system of equations AX = B
  
  // ... with the following right hand sides
  B(0,0) = 16.0;
  B(1,0) = -8.0;
  B(2,0) = 0.0;
  
  B(0,1) = 0.0;
  B(1,1) = -16.0;
  B(2,1) = -8.0;
  
  B(0,2) = 0.0;
  B(1,2) = 0.0;
  B(2,2) = 0.0;
  
  // Print the matrices
  std::cout << std::endl;
  std::cout << "Matrix A" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Matrix A" << std::endl;
  output_test << std::endl;
  A.print();
  A.print(output_test);
  std::cout << std::endl;
  std::cout << "Matrix B" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Matrix B" << std::endl;
  output_test << std::endl;
  B.print();
  B.print(output_test);
  std::cout << std::endl;
  output_test << std::endl;

  // Create a factory for the linear solver
  CCFactoryLinearSolver factory_linear_solver;   
  
  // Create an Armadillo linear solver
  ACLinearSolver *armadillo_linear_solver_pt =
   factory_linear_solver.create_linear_solver("armadillo");
  
  // The solution vector
  CCMatrixArmadillo SOL(A.n_rows(), B.n_columns());
  
  // Solve the system of equations
  armadillo_linear_solver_pt->solve(&A, &B, &SOL);
  
  // Print the solution
  std::cout << std::endl;
  std::cout << "Matrix SOL -----" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Matrix SOL -----" << std::endl;
  output_test << std::endl;
  SOL.print();
  SOL.print(output_test);
  std::cout << std::endl;
  output_test << std::endl;
  
  // Apply the solution and check the result
  CCMatrixArmadillo C = A*SOL;
  std::cout << std::endl;
  std::cout << "Matrix C" << std::endl;
  std::cout << std::endl;
  output_test << std::endl;
  output_test << "Matrix C" << std::endl;
  output_test << std::endl;
  C.print();
  C.print(output_test);
 }
 
 // Close the output for test
 output_test.close();
 
 // Finalise scicellxx
 finalise_scicellxx(); 
 
 return 0;
 
}
