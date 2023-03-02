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
// This demo driver is based on the Program 6.1
// RBFInterpolation2Dlinear.m from the book "Meshfree Approximation
// Methods with MATLAB, Gregory E. Fasshauer", World Scientific
// Publishing, 2007

// Include SciCell++ libraries
#include "../../../../../src/scicellxx.h"

using namespace scicellxx;

// Franke's function (2D version)
template<class VEC_TYPE>
const Real test_function(VEC_TYPE &x)
{
 return (x(0) + x(1)) / 2.0;
}

template<class MAT_TYPE, class VEC_TYPE>
void compute_distance_matrix(MAT_TYPE &data_sites, MAT_TYPE &centers,
                             MAT_TYPE &distance_matrix)
{
 // Get the number of "vector points" on "data_sites"
 // Get the number of "vector points" on "centers"
 const unsigned n_vector_points_data_sites = data_sites.n_columns();
 const unsigned n_vector_points_centers = centers.n_columns();
 
 // The dimension of input vector points must be the same, otherwise
 // there is an error
 const unsigned dimension = data_sites.n_rows();
 const unsigned tmp_dimension = centers.n_rows();
 
 if (dimension != tmp_dimension)
  {
   // Error message
   std::ostringstream error_message;
   error_message << "The dimensions of the data sites vector and the\n"
                 << "centers vector are different\n"
                 << "dim(data_site):" << dimension
                 << "\ndim(centers):" << tmp_dimension
                 << std::endl;
   throw SciCellxxLibError(error_message.str(),
                          SCICELLXX_CURRENT_FUNCTION,
                          SCICELLXX_EXCEPTION_LOCATION);
  }
 
 // Loop over all the data points in the first matrix
 for (unsigned m = 0; m < n_vector_points_data_sites; m++)
  {
   // Loop over all the data points in the second matrix
   for (unsigned n = 0; n < n_vector_points_centers; n++)
    {
     VEC_TYPE distance(dimension);
     //distance.allocate_memory();
     // Loop over the elements of both vectors
     for (unsigned k = 0; k < dimension; k++)
      {
       distance(k) = data_sites(k, m) - centers(k, n);
      }
     distance_matrix(m,n)=distance.norm_2();
    }
  }
 
}

template<class MAT_TYPE>
void psi(MAT_TYPE &distance_matrix,
         MAT_TYPE &interpolation_matrix, const Real epsilon)
{
 const unsigned n_rows_src = distance_matrix.n_rows();
 const unsigned n_columns_src = distance_matrix.n_columns();
 
 const unsigned n_rows_dst = interpolation_matrix.n_rows();
 const unsigned n_columns_dst = interpolation_matrix.n_columns();
 
 if (n_rows_src != n_rows_dst || n_columns_src != n_columns_dst)
  {
   // Error message
   std::ostringstream error_message;
   error_message << "The dimensions of the distance matrix and the\n"
                 << "interpolation matrix are different\n"
                 << "distance_matrix: " << n_rows_src << " x " << n_columns_src << "\n"
                 << "interpolation_matrix: " << n_rows_dst << " x " << n_columns_dst << "\n"
                 << std::endl;
   throw SciCellxxLibError(error_message.str(),
                          SCICELLXX_CURRENT_FUNCTION,
                          SCICELLXX_EXCEPTION_LOCATION);
  }
 
 for (unsigned i = 0; i < n_rows_dst; i++)
  {
   for (unsigned j = 0; j < n_columns_dst; j++) 
    {
     const Real r = distance_matrix(i, j);
     interpolation_matrix(i,j) = std::exp(-((r*epsilon)*(r*epsilon)));
    }
   
  }
 
}

struct Args {
 argparse::ArgValue<Real> epsilon;
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

 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "Distance Matrix");
 
 // Add arguments  
 parser.add_argument<Real>(args.epsilon, "--epsilon")
  .help("Epsilon for phi")
  .default_value("6.0");
 
 // Parse the input arguments
 parser.parse_args(argc, argv);
 
 // --------------------------------------------------------------
 // Domain specification
 // --------------------------------------------------------------
 // TODO: Create a DOMAIN (mesh?) type class
 
 // Dimension of the problem
 const unsigned dim = 2;
 
 // The value for epsilon
 Real epsilon = args.epsilon;
 
 // Interpolant degree
 const unsigned degree = 3;
 
 // Specify the one-dimensional lenght of the domain
 const unsigned L = 1.0;

 // The interpolation polynomial order (as indicated in program 6.1)
 const unsigned M = 3;
 
 // --------------------------------------------------------------
 // Create and give position to nodes
 // --------------------------------------------------------------
 // Nodes per dimension
 const unsigned n_nodes_per_dim = pow(2, degree+1);
 // The number of nodes
 const unsigned n_nodes = pow(n_nodes_per_dim, dim);
 // A vector of nodes
 std::vector<CCNode<Real> *> nodes_pt(n_nodes);
 
 // Number of variables stored in the node
 const unsigned n_variables = 1;
 // Number of history values
 const unsigned n_history_values = 1;
 
 // Distance between a pair of nodes
 //const Real h = L / (Real)(n_nodes_per_dim - 1);
 std::vector<Real> x(dim, 0.0);
 // Create the nodes
 for (unsigned long i = 0; i < n_nodes; i++)
  {
   nodes_pt[i] = new CCNode<Real>(dim, n_variables, n_history_values);
  }
 
 // --------------------------------------------------------------
 // Output supporting nodes
 // --------------------------------------------------------------
 std::ofstream nodes_file("RESLT/nodes.dat");
 
 // Assign positions
 for (unsigned long i = 0; i < n_nodes; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     const Real r = rand();
     const Real position = static_cast<Real>(r / RAND_MAX) * L;
     // Generate position and assign it
     //const Real position = x[k];
     nodes_pt[i]->set_position(position, k);
     //x[k]+=h;
     nodes_file << position;
     if (k + 1 < dim)
      {
       nodes_file << " ";
      }
    }
   nodes_file << std::endl;
  }
 
 // Close support nodes file
 nodes_file.close();
 
 // --------------------------------------------------------------
 // Set initial conditions
 // --------------------------------------------------------------
 /*
 for (unsigned i = 0; i < n_nodes; i++)
 {
 for (unsigned j = 0; j < n_variables; j++)
 {
 const Real u = 0.0;
 nodes_pt[i]->set_variable(u, j);
 }
 }
 */
 
 // --------------------------------------------------------------
 // Set boundary conditions
 // --------------------------------------------------------------
 /*
 // Move the first and the last node to the boundary of the domain
 nodes_pt[0]->set_position(0.0, 0);
 nodes_pt[0]->set_variable(0.0, 0);
 nodes_pt[n_nodes-1]->set_position(1.0, 0);
 nodes_pt[n_nodes-1]->set_variable(1.0, 0);
 */
 
 // --------------------------------------------------------------
 // Set the problem and solve it
 // --------------------------------------------------------------
 
 // TODO: The distance matrix may be formed while we loop over the
 // nodes to extract their position
 
 // --------------------------------------------------------------
 // Loop over the nodes and extract their position and store them in a
 // matrix
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> nodes_position(dim, n_nodes);
#else
 CCMatrix<Real> nodes_position(dim, n_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // Each column stores the vector position of a node
 //nodes_position.allocate_memory();
 for (unsigned i = 0; i < n_nodes; i++)
  {
   for (unsigned j = 0; j < dim; j++)
    {
     nodes_position(j, i) = nodes_pt[i]->get_position(j);
    }
  }
  
 // -------------------------------------------------------------- 
 // Create the distance matrix
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> distance_matrix(n_nodes, n_nodes);
#else
 CCMatrix<Real> distance_matrix(n_nodes, n_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // --------------------------------------------------------------
 // Generate the distance matrix using the nodes position centers
 // shifted by the same nodes position
 // --------------------------------------------------------------
 //distance_matrix.allocate_memory();
#ifdef SCICELLXX_USES_ARMADILLO
 compute_distance_matrix<CCMatrixArmadillo<Real>, CCVectorArmadillo<Real> >(nodes_position, nodes_position, distance_matrix);
#else
 compute_distance_matrix<CCMatrix<Real>, CCVector<Real> >(nodes_position, nodes_position, distance_matrix);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 
 // -------------------------------------------------------------- 
 // Create the interpolation matrix (the one that applies the RBF)
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> interpolation_matrix(n_nodes, n_nodes);
#else
 CCMatrix<Real> interpolation_matrix(n_nodes, n_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // --------------------------------------------------------------
 // Generate the interpolation matrix using the RBF PSI
 // --------------------------------------------------------------
 //interpolation_matrix.allocate_memory();
#ifdef SCICELLXX_USES_ARMADILLO
 psi<CCMatrixArmadillo<Real> >(distance_matrix, interpolation_matrix, epsilon);
#else
 psi<CCMatrix<Real> >(distance_matrix, interpolation_matrix, epsilon);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 
 // --------------------------------------------------------------
 // Set right-hand side
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> rhs(n_nodes);
#else 
 CCVector<Real> rhs(n_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //rhs.allocate_memory();
 for (unsigned i = 0; i < n_nodes; i++)
  {
#ifdef SCICELLXX_USES_ARMADILLO
   CCVectorArmadillo<Real> tmp_v(dim);
#else
   CCVector<Real> tmp_v(dim);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
   //tmp_v.allocate_memory();
   for (unsigned j = 0; j < dim; j++)
    {
     tmp_v(j) = nodes_pt[i]->get_position(j);
    }
   // --------------------------------------------------------------
   // Evaluate the KNOWN function at the centers positions
   // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
   rhs(i) = test_function<CCVectorArmadillo<Real> >(tmp_v);
#else
   rhs(i) = test_function<CCVector<Real> >(tmp_v);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
  }
 
 // ================================================================
 // Create the additional entries for the matrices considering the
 // number of entries set by the interpolation polynomial (M = 3)
 // ================================================================
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> additional_rhs(M);
#else
 CCVector<Real> additional_rhs(M);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //additional_rhs.allocate_memory();
 // Set the constraints given in page 55 and 56, where the sum is
 // equal to zero, thus
 for (unsigned i = 0; i < M; i++)
  {
   additional_rhs(i) = 0;
  }
 
 // Create the block version of the rhs concatenating the original rhs
 // and the additional rhs
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> block_rhs(n_nodes + M);
#else 
 CCVector<Real> block_rhs(n_nodes + M);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // Concatenate
 concatenate_vectors_vertically(rhs, additional_rhs, block_rhs);
 
 // -------------------------------------------------------------
 // Create the matrix P / Additional matrix with polynomial
 // information to ensure matrix inversion
 // -------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> P(n_nodes, M);
#else
 CCMatrix<Real> P(n_nodes, M);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //P.allocate_memory();
 for (unsigned i = 0; i < n_nodes; i++)
  {
   P(i, 0) = 1;
   P(i, 1) = nodes_pt[i]->get_position(0);
   P(i, 2) = nodes_pt[i]->get_position(1);
  }
 
 // The transposed P
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> P_t;
#else
 CCMatrix<Real> P_t;
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // Get the transpose of P
 P.transpose(P_t);
 
 // Create the zero matrix
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> Zeroes(M, M);
#else
 CCMatrix<Real> Zeroes(M, M);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // Fill it with zeroes
 //Zeroes.allocate_memory();
 Zeroes.fill_with_zeroes();

 // ---------------------------------------------------------
 // Create the block matrix
 // ---------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> block_upper_interpolation_matrix;
#else
 CCMatrix<Real> block_upper_interpolation_matrix;
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 
 // Concatenate upper parts
 concatenate_matrices_horizontally(interpolation_matrix, P, block_upper_interpolation_matrix);
 
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> block_lower_interpolation_matrix;
#else
 CCMatrix<Real> block_lower_interpolation_matrix;
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 concatenate_matrices_horizontally(P_t, Zeroes, block_lower_interpolation_matrix);
 
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> block_interpolation_matrix;
#else
 CCMatrix<Real> block_interpolation_matrix;
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 
 // Concatenate upper parts
 concatenate_matrices_vertically(block_upper_interpolation_matrix, block_lower_interpolation_matrix, block_interpolation_matrix);
 
 // The solution vector (with the respective number of rows) stores
 // the coefficients for the interpolant polynomials
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> sol(n_nodes + M);
#else
 CCVector<Real> sol(n_nodes + M);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 
 // --------------------------------------------------------------
 // Solve
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
  // Create an Armadillo linear solver
  CCSolverArmadillo<Real> linear_solver;
#else
 // Create a linear solver
 CCLUSolverNumericalRecipes<Real> linear_solver;
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 
 //std::cerr << "Distance matrix" << std::endl;
 //distance_matrix.print();
 //interpolation_matrix.print();
 
 // --------------------------------------------------------------
 // Solve the system of equations
 // -------------------------------------------------------------- 
 //linear_solver.solve(block_interpolation_matrix, rhs, sol);
 linear_solver.solve(block_interpolation_matrix, block_rhs, sol);
 
 //std::cerr << "Solution vector" << std::endl;
 //sol.print();
 
 //std::cerr << "Nodes positions and values" << std::endl;
 /*
 // Show results
 for (unsigned i = 0; i < n_nodes; i++)
  {
   nodes_pt[i]->print(true);
  }
 */
 
 // --------------------------------------------------------------
 // --------------------------------------------------------------
 // EVALUATION STAGE
 // --------------------------------------------------------------
 // --------------------------------------------------------------
 std::cerr << "\nEVALUATION\n" << std::endl;
 
 // --------------------------------------------------------------
 // Evaluate (compute error RMSE)
 // --------------------------------------------------------------
 const unsigned n_evaluation_points_per_dimension = pow(dim+1, degree+1);
 const unsigned n_evaluation_nodes = pow(n_evaluation_points_per_dimension, dim);
 // Distance between a pair of nodes
 const Real h_test = L / (Real)(n_evaluation_points_per_dimension - 1);
 
 // A vector of evaluation nodes
 std::vector<CCNode<Real> *> evaluation_nodes_pt(n_evaluation_nodes);
 // Create evaluation nodes
 for (unsigned long i = 0; i < n_evaluation_nodes; i++)
  {
   evaluation_nodes_pt[i] = new CCNode<Real>(dim, n_variables, n_history_values);
  } 
 
 // Assign positions
 unsigned long kk = 0;
 for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
  {
   const Real position_i = i*h_test;
   for (unsigned j = 0; j < n_evaluation_points_per_dimension; j++)
    {
     const Real position_j = j*h_test;
     evaluation_nodes_pt[kk]->set_position(position_i, 0);
     evaluation_nodes_pt[kk]->set_position(position_j, 1);
     kk++;
    }
  }
 
 // Compute approximated solution at new positions
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> evaluation_nodes_position(dim, n_evaluation_nodes);
#else
 CCMatrix<Real> evaluation_nodes_position(dim, n_evaluation_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //evaluation_nodes_position.allocate_memory();
 // --------------------------------------------------------------
 // Assign positions
 // --------------------------------------------------------------
 for (unsigned long i = 0; i < n_evaluation_nodes; i++)
  {
   evaluation_nodes_position(0, i) = evaluation_nodes_pt[i]->get_position(0);
   evaluation_nodes_position(1, i) = evaluation_nodes_pt[i]->get_position(1);
  }
 
 // Compute distance matrix with new positions
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> evaluation_distance_matrix(n_evaluation_nodes, n_nodes);
#else
 CCMatrix<Real> evaluation_distance_matrix(n_evaluation_nodes, n_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // --------------------------------------------------------------
 // Generate the distance matrix using the evaluation nodes position
 // and the original nodes positions
 // --------------------------------------------------------------
 //evaluation_distance_matrix.allocate_memory();
#ifdef SCICELLXX_USES_ARMADILLO
 compute_distance_matrix<CCMatrixArmadillo<Real>, CCVectorArmadillo<Real> >(evaluation_nodes_position, nodes_position, evaluation_distance_matrix);
#else
 compute_distance_matrix<CCMatrix<Real>, CCVector<Real> >(evaluation_nodes_position, nodes_position, evaluation_distance_matrix);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //approx_distance_matrix.print();
 
 // --------------------------------------------------------------
 // Create the interpolation matrix for evaluation nodes
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> interpolation_evaluation_matrix(n_evaluation_nodes, n_nodes);
#else
 CCMatrix<Real> interpolation_evaluation_matrix(n_evaluation_nodes, n_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // --------------------------------------------------------------
 // Generate the interpolation matrix using the RBF psi
 // --------------------------------------------------------------
 //interpolation_evaluation_matrix.allocate_memory();
#ifdef SCICELLXX_USES_ARMADILLO
 psi<CCMatrixArmadillo<Real> >(evaluation_distance_matrix, interpolation_evaluation_matrix, epsilon);
#else
 psi<CCMatrix<Real> >(evaluation_distance_matrix, interpolation_evaluation_matrix, epsilon);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 
 // -------------------------------
 // Create the matrix Pe
 // -------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> Pe(n_evaluation_nodes, M);
#else
 CCMatrix<Real> Pe(n_evaluation_nodes, M);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //Pe.allocate_memory();
 for (unsigned i = 0; i < n_evaluation_nodes; i++)
  {
   Pe(i, 0) = 1;
   Pe(i, 1) = evaluation_nodes_pt[i]->get_position(0);
   Pe(i, 2) = evaluation_nodes_pt[i]->get_position(1);
  }
 
 // ---------------------------------------------------------
 // Create the block matrix
 // --------------------------------------------------------- 
#ifdef SCICELLXX_USES_ARMADILLO
 CCMatrixArmadillo<Real> block_interpolation_evaluation_matrix;
#else
 CCMatrix<Real> block_interpolation_evaluation_matrix;
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 concatenate_matrices_horizontally(interpolation_evaluation_matrix, Pe, block_interpolation_evaluation_matrix); 
 
 // Approximated solution
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> approx_sol(n_evaluation_nodes);
#else
 CCVector<Real> approx_sol(n_evaluation_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 // Approximate solution at given points
 multiply_matrix_times_vector(block_interpolation_evaluation_matrix, sol, approx_sol);
 
 // --------------------------------------------------------------
 // Assign solution to nodes
 // --------------------------------------------------------------
 for (unsigned long i = 0; i < n_evaluation_nodes; i++)
  {
   evaluation_nodes_pt[i]->set_variable(approx_sol(i), 0);
  }
 
 // Move the positions of the nodes and the results into two CCData<T>
 // vectors
 std::vector<CCData<Real> > positions;
 std::vector<CCData<Real> > values;
 for (unsigned long i = 0; i < n_evaluation_nodes; i++)
  {
   positions.push_back(evaluation_nodes_pt[i]->x());
   values.push_back(evaluation_nodes_pt[i]->u());
  }
 
 // Output to file
 std::ostringstream output_filename;
 output_filename
  << "./RESLT/soln" << "_" << std::setfill('0') << "0";
 
 CCSciCellxx2VTK::get_instance().output_position_and_attribute_datas(positions, values, output_filename);
 
 /*
 // --------------------------------------------------------------
 // Output data for plotting
 // --------------------------------------------------------------
 std::ofstream output_file("RESLT/output.dat");
 // Output the number of data/points
 output_file << n_evaluation_points << std::endl;
 output_file << n_evaluation_points_per_dimension << std::endl;
 for (unsigned i = 0; i < n_evaluation_points; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     output_file << approx_solution_position(k, i) << " ";
    }
   output_file << approx_sol(i) << std::endl;
  }
 
 // Close output file
 output_file.close();
 */
 // --------------------------------------------------------------
 // Get real solution at given points and get the error 
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> real_sol(n_evaluation_nodes);
#else 
 CCVector<Real> real_sol(n_evaluation_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //real_sol.allocate_memory();
 for (unsigned i = 0; i < n_evaluation_nodes; i++)
  {
#ifdef SCICELLXX_USES_ARMADILLO
   CCVectorArmadillo<Real> tmp_v(dim);
#else
   CCVector<Real> tmp_v(dim);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
   //tmp_v.allocate_memory();
   for (unsigned j = 0; j < dim; j++)
    {
     tmp_v(j) = evaluation_nodes_position(j, i);
    }
   // ------------------------
   // Evaluation at approx_solution_position
   real_sol(i) = test_function(tmp_v);
  }
 /*
   {
   // Move the positions of the nodes and the results into two CCData<T>
   // vectors
   std::vector<CCData<Real> > solution_values;
   for (unsigned long i = 0; i < n_evaluation_nodes; i++)
   {
   CCData<Real> u(1);
   u(0) = real_sol(i);
   solution_values.push_back(u);
   DEB(real_sol(i));
   }
   // Output to file
   std::ostringstream output_sol_filename;
   output_sol_filename
   << "./RESLT/soln_real" << "_" << std::setfill('0') << "0";
  
   CCSciCellxx2VTK::get_instance().output_position_and_attribute_datas(positions, solution_values, output_sol_filename);
   }
 */
 // --------------------------------------------------------------
 // Compute error
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> error(n_evaluation_nodes);
#else
 CCVector<Real> error(n_evaluation_nodes);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //error.allocate_memory();
 std::cerr << "ERRORS" << std::endl;
 for (unsigned i = 0; i < n_evaluation_nodes; i++)
  {
   //error(i) = real_sol(i) - 0.0;
   error(i) = real_sol(i) - approx_sol(i);
   //std::cerr << i << ": " << std::fabs(error(i)) << std::endl;
   //std::cerr << i << ": " << real_sol(i) << ":" << approx_sol(i) << std::endl;
  }
 
 // Move the error into a vector
 std::vector<CCData<Real> > error_to_plot;
 for (unsigned long i = 0; i < n_evaluation_nodes; i++)
  {
   CCData<Real> data(1);
   data(0) = error(i);
   error_to_plot.push_back(data);
  }
 
 // Output to file
 std::ostringstream error_filename;
 error_filename
  << "./RESLT/errorn" << "_" << std::setfill('0') << "0";

 CCSciCellxx2VTK::get_instance().output_position_and_attribute_datas(positions, error_to_plot, error_filename);
 
 const Real rms_error = error.norm_2() / sqrt(n_evaluation_nodes);
 
 // --------------------------------------------------------------
 // Output error
 // --------------------------------------------------------------
 std::ofstream error_file("RESLT/error.dat");
 for (unsigned i = 0; i < n_evaluation_nodes; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     error_file << evaluation_nodes_position(k, i) << " ";
    }
   error_file << error(i) << std::endl;
  }
 
 // Close error file
 error_file.close();
 
 // --------------------------------------------------------------
 // Summary
 // --------------------------------------------------------------
 std::cerr << std::endl;
 std::cerr << "Polynomial degree: " << degree << std::endl;
 std::cerr << "N. nodes per dimension: " << n_nodes_per_dim << std::endl;
 std::cerr << "N. total nodes: " << n_nodes << std::endl;
 
 std::cerr << "RBF[Epsilon]: " << epsilon << std::endl;
 
 std::cerr << "N. nodes per dimension (evaluation): " << n_evaluation_points_per_dimension << std::endl;
 std::cerr << "N. total nodes (evaluation): " << n_evaluation_nodes << std::endl;
 
 std::cerr << "RMS-error: " << rms_error << std::endl;
 
 // ==============================================================
 // ==============================================================
 
 // --------------------------------------------------------------
 // Delete nodes storage
 // --------------------------------------------------------------
 for (unsigned i = 0; i < n_nodes; i++)
  {
   delete nodes_pt[i];
  }
 
 // Finalise scicellxx
 finalise_scicellxx();
 
 return 0;
 
}

