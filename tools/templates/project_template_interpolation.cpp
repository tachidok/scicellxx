#include <iostream>
#include <cmath>

// Include general/common includes, utilities and initialisation
#include "../../src/general/common_includes.h"
#include "../../src/general/utilities.h"
#include "../../src/general/initialise.h"

// The interpolator
#include "../../src/problem/cc_newton_interpolator.h"

using namespace scicellxx;

inline Real f(const Real x)
{return sin(x);}

// This is an interpolation example using different polynomial degress
// (first order, second order and third order)
int main(int argc, char *argv[])
{
 // Initialise scicell++
 initialise_scicellxx();
 
 // Output for testing/validation
 std::ofstream output_test("output_test.dat", std::ios_base::out);
 
 // Output for testing/validation
 std::ofstream deb("deb.dat", std::ios_base::out);
 
 // -------------------------------------------------------------------
 // Original function
 
 // The vector with some values of the function
 const unsigned n_data = 10;
 std::vector<Real> x(n_data);
 x[0]=-M_PI;
 // Create a vector with the values of an "unknown" function (here we
 // known the function thus we can use it to compute the error)
 std::vector<Real> fx(n_data);
 fx[0]=f(x[0]);
 const Real step = 2.0*M_PI/(n_data-1);
 // Get the values of the "unknown function" at corresponding x-values
 for (unsigned i = 1; i < n_data; i++)
  {
   x[i]=x[i-1]+step;
   fx[i]=f(x[i]);
  }
 
 // -------------------------------------------------------------------
 // Interpolated values
 
 // The number of points at which we want to perform interpolation
 // (double the number of original points)
 const unsigned n_interpolated_data = n_data*2;
 const Real interpolated_step = 2.0*M_PI/(n_interpolated_data-1);
 // The values at which we want to perform interpolation
 std::vector<Real> x_to_interpolate(n_interpolated_data);
 x_to_interpolate[0] = -M_PI;
 // Fill the x-values at which perform interpolation
 for (unsigned i = 1; i < n_interpolated_data; i++)
  {
   x_to_interpolate[i] = x_to_interpolate[i-1]+interpolated_step;
  }
 
 // The interpolator object
 CCNewtonInterpolator interpolator;
 
 Real max_linear_error = 0.0;
 Real max_quadratic_error = 0.0;
 Real max_cubic_error = 0.0;
 
 // ------------------------
 // Linear interpolation
 {
  // Storage for interpolations
  std::vector<Real> fx_linear(n_interpolated_data);
  const unsigned interpolation_order = 1;
  // Do interpolation
  interpolator.interpolate_1D(x, fx, x_to_interpolate, fx_linear, interpolation_order);
  
  // Get errors
  std::vector<Real> error(n_interpolated_data);
  std::cout << std::endl;
  std::cout << "Error linear interpolation: " << std::endl;
  output_test << "Error linear interpolation: " << std::endl;
  for (unsigned i = 0; i < n_interpolated_data; i++)
   {
    DEB_TO_FILE3(deb, x_to_interpolate[i], fx_linear[i], f(x_to_interpolate[i]));
    error[i]=std::fabs(fx_linear[i]-f(x_to_interpolate[i]));
    if (error[i]>max_linear_error)
     {
      max_linear_error=error[i];
     }
    std::cout << "error["<<i<<"]:"<<error[i]<<std::endl;
    output_test << "error["<<i<<"]:"<<error[i]<<std::endl;
   }
 }
 
 // ------------------------
 // Quadratic interpolation
 {
  // Storage for interpolations
  std::vector<Real> fx_quadratic(n_interpolated_data);
  const unsigned interpolation_order = 2;
  interpolator.interpolate_1D(x, fx, x_to_interpolate, fx_quadratic, interpolation_order);
  // Get errors
  std::vector<Real> error(n_interpolated_data);
  std::cout << "Error quadratic interpolation: " << std::endl;
  output_test << "Error quadratic interpolation: " << std::endl;
  for (unsigned i = 0; i < n_interpolated_data; i++)
   {
    error[i]=std::fabs(fx_quadratic[i]-f(x_to_interpolate[i])); 
    if (error[i]>max_quadratic_error)
     {
      max_quadratic_error=error[i];
     }
    std::cout << "error["<<i<<"]:"<<error[i]<<std::endl;
    output_test << "error["<<i<<"]:"<<error[i]<<std::endl;
   }
 }
 
 // ------------------------
 // Cubic interpolation
 {
  // Storage for interpolations
  std::vector<Real> fx_cubic(n_interpolated_data);
  const unsigned interpolation_order = 3;
  interpolator.interpolate_1D(x, fx, x_to_interpolate, fx_cubic, interpolation_order);
  // Get errors
  std::vector<Real> error(n_interpolated_data);
  std::cout << "Error cubic interpolation: " << std::endl;
  output_test << "Error cubic interpolation: " << std::endl;
  for (unsigned i = 0; i < n_interpolated_data; i++)
   {
    error[i]=std::fabs(fx_cubic[i]-f(x_to_interpolate[i]));
    if (error[i]>max_cubic_error)
     {
      max_cubic_error=error[i];
     }
    std::cout << "error["<<i<<"]:"<<error[i]<<std::endl;
    output_test << "error["<<i<<"]:"<<error[i]<<std::endl;
   }
 }
 
 std::cout << "Max. linear interpolation error: " << max_linear_error << std::endl;
 std::cout << "Max. quadratic interpolation error: " << max_quadratic_error << std::endl;
 std::cout << "Max. cubic interpolation error: " << max_cubic_error << std::endl;

 output_test << "Max. linear interpolation error: " << max_linear_error << std::endl;
 output_test << "Max. quadratic interpolation error: " << max_quadratic_error << std::endl;
 output_test << "Max. cubic interpolation error: " << max_cubic_error << std::endl;
 
 // Close the output for test
 output_test.close();
 
 deb.close();
 
 // Finalise scicell++
 finalise_scicellxx();
 
 return 0;
 
}
