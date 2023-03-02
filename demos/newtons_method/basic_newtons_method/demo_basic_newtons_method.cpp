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

/// This demo solves the nonlinear problem
/// x^3 = 27
/// using Newton's method

// A concrete class to compute the Jacobian matrix and the residual
// vector for the problem F(x) = x^3 - 27
class CCJacobianAndResidualBasic : virtual public ACJacobianAndResidual
{
 
public:
 
 // Constructor (empty)
 CCJacobianAndResidualBasic()
  : ACJacobianAndResidual()
 { }
 
 // Destructor (empty)
 ~CCJacobianAndResidualBasic() { }
 
 // In charge of computing the Jacobian
 // (virtual function implementation)
 void compute_jacobian()
 {
  Real x = x_value();
  this->Jacobian_pt->allocate_memory(1,1);
  (*this->Jacobian_pt)(0,0) = 3.0*(x*x); // The derivative of F(x)=x^3-27 w.r.t. x
 }
 
 // In charge of computing the residual
 // (virtual function implementation)
 void compute_residual()
 {
  Real x = x_value();
  this->Residual_pt->allocate_memory(1);
  (*this->Residual_pt)(0) = -((x*x*x) - 27.0); // -F(x)
 }
 
 inline void set_x_pt(ACVector *x_pt) {X_pt = x_pt;}
 
private:
 
 // Copy constructor (we do not want this class to be copiable because
 // it contains dynamically allocated variables, A in this
 // case). Check
 // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 CCJacobianAndResidualBasic(const CCJacobianAndResidualBasic &copy)
 {
  BrokenCopy::broken_copy("CCJacobianAndResidualBasic");
 }
 
 // Copy constructor (we do not want this class to be copiable because
 // it contains dynamically allocated variables, A in this
 // case). Check
 // http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 void operator=(const CCJacobianAndResidualBasic &copy)
 {
  BrokenCopy::broken_assign("CCJacobianAndResidualBasic");
 }
 
 // Access function for X
 inline Real x_value(){return X_pt->value(0);}
 
 // A pointer to the vector where the values are stored
 ACVector *X_pt;
 
};

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
 
 // Output for testing/validation
 std::ofstream output_test("output_test.dat", std::ios_base::out);
   
 // Create an instance of Newton's method
 CCNewtonsMethod newtons_method;
 
 // Create the Jacobian and residual strategy for the problem to solve
 CCJacobianAndResidualBasic jacobian_and_residual;
 
 // Set Jacobian strategy for Newton's method
 newtons_method.set_jacobian_and_residual_strategy(&jacobian_and_residual);
 
 // Number of dofs
 const unsigned n_dof = 1;
 // Initial guess
 const Real initial_guess = 5.0;
 
 // Create a vector with the initial guess
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo x(n_dof);
#else 
 CCVector x(n_dof);
#endif
 
 //x.allocate_memory();
 x(0) = initial_guess;
 
 // Set initial dofs in Jacobian and residual strategy
 jacobian_and_residual.set_x_pt(&x);
 
 // Change maximum allowed residual
 newtons_method.set_maximum_allowed_residual(100.0);
 
 // Solver using Newton's method
 newtons_method.solve(&x);
 
 // Print result
 x.print();
 x.print(output_test);
 
 // Close the output for test
 output_test.close();
 
 std::cout << "[FINISHING UP] ... " << std::endl;
 
 // Finalise scicellxx
 finalise_scicellxx();
 
 return 0;
 
}
