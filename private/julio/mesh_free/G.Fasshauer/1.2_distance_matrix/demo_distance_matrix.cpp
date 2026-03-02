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
// This demo driver is based on the Program 1.2 DistanceMatrixFit.m
// from the book "Meshfree Approximation Methods with MATLAB, Gregory
// E. Fasshauer", World Scientific Publishing, 2007

// Include SciCell++ libraries
#include "../../../../../src/scicellxx.h"

using namespace scicellxx;

// This fucntion has it maximum value at the center, depending on the
// dimension s. At the boundaries it is zero.
template <class VECTOR_TYPE>
const Real test_function(VECTOR_TYPE *x_pt, const unsigned s)
{
 Real prod=1.0;
 for (unsigned i = 0; i < s; i++)
  {
   Real x_i = x_pt->get_value(i);
   prod*=x_i*(1.0-x_i);
  }
 return pow(4, s)*prod;
}

// The problem class
template <class MATRIX_TYPE, class VECTOR_TYPE>
class CCDistanceMatrixProblem : public virtual ACProblem
{
public:

 /// Constructor
 CCDistanceMatrixProblem(const unsigned dim, const unsigned degree, const unsigned n_evaluation_points_per_dimension)
  : ACProblem(dim),
    Degree(degree),
    L(1), // One-dimensional lenght
    N_nodes_per_dim(std::pow(2, degree+1)),
    N_evaluation_points_per_dimension(n_evaluation_points_per_dimension)
    
 {
  const unsigned long nnodes = std::pow(N_nodes_per_dim, dim);
  
  // Allocate memory for Nodes_pt vector
  Nodes_pt.resize(nnodes);  
 }
 
  /// Destructor
 ~CCDistanceMatrixProblem()
 {
  // Get the number of nodes
  const unsigned long nnodes = this->n_nodes();
  
  // --------------------------------------------------------------
  // Delete nodes storage
  // --------------------------------------------------------------
  for (unsigned long i = 0; i < nnodes; i++)
   {
    delete node_pt(i);
   }
 }
 
 // Complete the setup of the problem
 void complete_problem_setup()
 {
  // Create nodes and assign position
  bool random_positions = true;
  create_nodes(random_positions);
  
  // Set initial conditions
  set_initial_conditions();
  
  const unsigned long n_eq = this->assign_equations_number();
  
  // Initialise u
  this->initialise_u(n_eq);
  
 }
 
 // Set initial conditions
 void set_initial_conditions()
 {
  // Get the number of nodes
  const unsigned long nnodes = this->n_nodes();
  
  // Get the number of variables per node
  const unsigned n_variables = node_pt(0)->n_variables();
  for (unsigned long i = 0; i < nnodes; i++)
   {
    // All variables to zero
    const Real u = 0.0;
    for (unsigned j = 0; j < n_variables; j++)
     {
      node_pt(i)->set_variable(u, j);
     }
   }
  
 }
 
 // Solve the problem
 void solve()
 {  
  // Create a matrix representation of the nodes with their space-position
  
  // Get the number of nodes
  const unsigned long nnodes = this->n_nodes();
  
  // Get the dimension of the nodes in the problem
  const unsigned dimension = this->dim();
  
  // --------------------------------------------------------------
  // Loop over the nodes and extract their position and store them in
  // a matrix
  // --------------------------------------------------------------
  MATRIX_TYPE *nodes_matrix_position_pt =
   Factory_matrices_and_vectors.create_matrix(dimension, nnodes);
  // Each column stores the vector position of a node
  for (unsigned long i = 0; i < nnodes; i++)
   {
    // Cache i-th node position
    CCData position = node_pt(i)->x();
    
    for (unsigned j = 0; j < dimension; j++)
     {
      // Get the j-th dimensional position
      const Real pos = position(j);
      // Store its position on a vector
      nodes_matrix_position_pt->set_value(j, i, pos);
     }
    
   }
 
  // -------------------------------------------------------------- 
  // Create the distance matrix
  // --------------------------------------------------------------
  MATRIX_TYPE *distance_matrix_pt =
   Factory_matrices_and_vectors.create_matrix(nnodes, nnodes);
  
  // --------------------------------------------------------------
  // Generate the distance matrix using the nodes position centers
  // shifted by the same nodes position
  // --------------------------------------------------------------
  compute_distance_matrix(nodes_matrix_position_pt,
                          nodes_matrix_position_pt,
                          distance_matrix_pt);
  
  // --------------------------------------------------------------
  // Set right-hand side
  // --------------------------------------------------------------
  VECTOR_TYPE *rhs_pt = Factory_matrices_and_vectors.create_vector(nnodes);
  VECTOR_TYPE *tmp_v_pt = Factory_matrices_and_vectors.create_vector(dimension);
  for (unsigned long i = 0; i < nnodes; i++)
   {
    // Cache i-th node position
    CCData position = node_pt(i)->x();
    for (unsigned j = 0; j < dimension; j++)
     {
      const Real pos = position(j);
      tmp_v_pt->set_value(j, pos);
     }
    // --------------------------------------------------------------
    // Evaluate the KNOWN function at the centers positions
    // --------------------------------------------------------------
    const Real test_function_value = test_function<VECTOR_TYPE>(tmp_v_pt, dimension);
    rhs_pt->set_value(i, test_function_value);
   }
  
  // The solution vector (with the respective number of rows) stores
  // the coefficients for the interpolant polynomials
  VECTOR_TYPE *sol_pt = Factory_matrices_and_vectors.create_vector(nnodes);
  
  // --------------------------------------------------------------
  // Solve
  // -------------------------------------------------------------- 
  // Create the linear solver
  ACLinearSolver *linear_solver_pt = Factory_linear_solver.create_linear_solver();
  
  //std::cerr << "Distance matrix" << std::endl;
  //distance_matrix.print();
  
  // --------------------------------------------------------------
  // Solve the system of equations
  // --------------------------------------------------------------
  linear_solver_pt->solve(distance_matrix_pt, rhs_pt, sol_pt);
  //std::cerr << "Solution vector" << std::endl;
  //sol.print();
  
 }
 
 /// Documento the solution of the problem
 void document_solution()
 {
  // --------------------------------------------------------------
  // --------------------------------------------------------------
  // EVALUATION STAGE
  // --------------------------------------------------------------
  // --------------------------------------------------------------
  std::cerr << "\nEVALUATION\n" << std::endl;
  
  // Get the dimension of the problem
  const unsigned dimension = this->dim();
  // Get the number of nodes in the problem
  const unsigned long nnodes = this->n_nodes();
  
  // Generate matrix with positions of nodes in the problem
  MATRIX_TYPE *nodes_matrix_position_pt =
   Factory_matrices_and_vectors.create_matrix(dimension, nnodes);
  
  // Each column stores the vector position of a node
  for (unsigned long i = 0; i < nnodes; i++)
   {
    // Cache i-th node position
    CCData position = node_pt(i)->x();
    
    for (unsigned j = 0; j < dimension; j++)
     {
      // Get the j-th dimensional position
      const Real pos = position(j);
      // Store its position on a vector
      nodes_matrix_position_pt->set_value(j, i, pos);
     }
    
   }
  
  // --------------------------------------------------------------
  // Evaluate (compute error RMSE)
  // --------------------------------------------------------------
  const unsigned n_data_in_evaluation_points = pow(N_evaluation_points_per_dimension, dimension);
  // Distance between a pair of nodes
  const Real h_test = L / (Real)(N_evaluation_points_per_dimension - 1);
 
  // Compute approximated solution at new positions
  MATRIX_TYPE *approx_solution_position_pt = Factory_matrices_and_vectors->create_matrix(dimension, N_evaluation_points_per_dimension);
  // --------------------------------------------------------------
  // Assign positions
  // --------------------------------------------------------------
  std::vector<Real> x_eval(dimension, 0.0);
  for (unsigned i = 0; i < N_evaluation_points_per_dimension; i++)
   {
    for (unsigned k = 0; k < dimension; k++)
     {
      const Real r = rand();
      const Real position = static_cast<Real>(r / RAND_MAX) * L;
      // Generate position and assign it
      //const Real position = x_eval[k];
      approx_solution_position_pt->set_value(k, i, position);
      //x_eval[k]+=h_test;
     }
   }
  
  // Compute distance matrix with new positions
  MATRIX_TYPE *approx_distance_matrix_pt = Factory_matrices_and_vectors->create_matrix(N_evaluation_points_per_dimension, n_nodes);
  // --------------------------------------------------------------
  // Generate the distance matrix using the nodes position centers
  // shifted by the new positions
  // --------------------------------------------------------------
  compute_distance_matrix(approx_solution_position_pt, nodes_matrix_position_pt, approx_distance_matrix_pt);
  
  // Approximated solution
  VECTOR_TYPE *approx_sol_pt = Factory_matrices_and_vectors->create_vector(N_evaluation_points_per_dimension);

  // HERE HERE HERE
  // Approximate solution at given points
  multiply_matrix_times_vector(approx_distance_matrix_pt, sol_pt, approx_sol_pt);
  //(*approx_sol_pt) = (*approx_distance_matrix_pt) * (*sol_pt);
  //approx_distance_matrix_pt->multiply by vector
 
  // --------------------------------------------------------------
  // Output data for plotting
  // --------------------------------------------------------------
  std::ostringstream filename;
  filename << "RESLT/soln" << std::setfill('0') << std::setw(2) << this->output_file_index()++ << ".dat";
  std::ofstream output_file(filename);
  for (unsigned i = 0; i < N_evaluation_points_per_dimension; i++)
   {
    for (unsigned k = 0; k < dimension; k++)
     {
      output_file << approx_solution_position(k, i) << " ";
     }
    output_file << approx_sol(i) << std::endl;
   }
  
  // Close output file
  output_file.close();  
 }
 
 /// Document the error of the solution
 void document_error()
 {
  // --------------------------------------------------------------
  // Get real solution at given points and get the error 
  // --------------------------------------------------------------
  VECTOR_TYPE real_sol(N_evaluation_points_per_dimension);
  
  // Get the dimension of the problem
  const unsigned dimension = this->dim();
  
  // Compute the solution at each position of the approximate solution
  for (unsigned i = 0; i < N_evaluation_points_per_dimension; i++)
   {
    VECTOR_TYPE tmp_v(dimension);
    //tmp_v.allocate_memory();
    for (unsigned j = 0; j < dimension; j++)
     {
      tmp_v(j) = approx_solution_position(j, i);
     }
    // ------------------------
    // Evaluation at approx_solution_position
    real_sol(i) = test_function<VECTOR_TYPE>(tmp_v, dim);
   }
  
  // --------------------------------------------------------------
  // Compute error
  // --------------------------------------------------------------
  VECTOR_TYPE error(N_evaluation_points_per_dimension);
  std::cerr << "ERRORS" << std::endl;
  for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
   {
    error(i) = real_sol(i) - approx_sol(i);
    std::cerr << i << ": " << std::fabs(error(i)) << std::endl;
    std::cerr << i << ": " << real_sol(i) << ":" << approx_sol(i) << std::endl;
   }
  
  const Real rms_error = error.norm_2() / sqrt(n_data_in_evaluation_points);
 
  // --------------------------------------------------------------
  // Output error
  // --------------------------------------------------------------
  std::ofstream error_file(std::string("RESLT/error.dat"));
  for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
   {
    for (unsigned k = 0; k < dim; k++)
     {
      error_file << approx_solution_position(k, i) << " ";
     }
    error_file << error(i) << std::endl;
   }
 
  // Close error file
  error_file.close();
 }
 
private:

 /// Copy constructor (we do not want this class to be
 /// copiable). Check
 /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 CCDistanceMatrixProblem(const CCDistanceMatrixProblem &copy)
  : ACProblem()
 {
  BrokenCopy::broken_copy("CCDistanceMatrixProblem");
 }
 
 /// Assignment operator (we do not want this class to be
 /// copiable. Check
 /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
 void operator=(const CCDistanceMatrixProblem &copy)
 {
  BrokenCopy::broken_assign("CCDistanceMatrixProblem");
 }
 
 // Create nodes, assign dimension, number of variables per node, and
 // number of history values per node
 void create_nodes(bool random_position = true)
 {
  // Number of variables stored in each node
  const unsigned n_variables = 1;
  // Number of history values per variable
  const unsigned n_history_values = 1;
  
  // Compute the `h` distance just in case random_position was not
  // selected
  // Distance between a pair of consecutive nodes
  const Real h = L / (Real)(N_nodes_per_dim - 1);
  std::vector<Real> x(Dim, 0.0);
  
  // Create the nodes, assign dimension, number of variables and
  // number of history values per variable
  for (unsigned long i = 0; i < N_nodes; i++)
   {
    Nodes_pt[i] = new CCNode(Dim, n_variables, n_history_values);
    for (unsigned k = 0; k < Dim; k++)
     {
      if (random_position)
       {
        const Real r = rand();
        const Real position = static_cast<Real>(r / RAND_MAX) * L;
        Nodes_pt[i]->set_position(position, k); 
       }
      else
       {
        const Real position = x[k];
        Nodes_pt[i]->set_position(position, k); 
        x[k]+=h;
       }
      
     }
    
   }
  
 }
 
 // Compute the distance between nodes in a distance matrix
 void compute_distance_matrix(MATRIX_TYPE *data_sites_pt, MATRIX_TYPE *centers_pt,
                              MATRIX_TYPE *distance_matrix_pt)
 {
  // Get the number of "vector points" on "data_sites_pt"
  // Get the number of "vector points" on "centers_pt"
  const unsigned n_vector_points_data_sites = data_sites_pt->n_columns();
  const unsigned n_vector_points_centers = centers_pt->n_columns();
  
  // The dimension of input vector points must be the same, otherwise
  // there is an error
  const unsigned dimension = data_sites_pt->n_rows();
  const unsigned tmp_dimension = centers_pt->n_rows();
  
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
  
  VECTOR_TYPE *distance_pt = Factory_matrices_and_vectors->create_vector(dimension);
  
  // Loop over all the data points in the first matrix
  for (unsigned m = 0; m < n_vector_points_data_sites; m++)
   {
    // Loop over all the data points in the second matrix
    for (unsigned n = 0; n < n_vector_points_centers; n++)
     {
      // Loop over the elements of both vectors
      for (unsigned k = 0; k < dimension; k++)
       {
        // Distance among points in matrices
        const Real dis = data_sites_pt->get_value(k, m) - centers_pt->get_value(k, n);
        distance_pt->set_value(k, dis);
       }
      const Real norm2 = distance_pt->norm_2();
      distance_matrix_pt->set_value(m,n, norm2);
     }
   }
  
  delete distance_pt;
  
 }
 
 // Degree of the interpolant polynomial
 const unsigned Degree;
 
 // One-dimensional lenght of the domain
 const unsigned L;

 // Number of node per dimension
 const unsigned N_nodes_per_dim;
 
 // Number of evaluation points per dimension
 const unsigned N_evaluation_points_per_dimension;
 
 // A factory to create matrices and vectors
 CCFactoryMatrices<MATRIX_TYPE, VECTOR_TYPE> Factory_matrices_and_vectors;
 
 // A factory to create the linear solver
 CCFactoryLinearSolver Factory_linear_solver;
 
};

struct Args {
 argparse::ArgValue<unsigned> dimension;
 argparse::ArgValue<unsigned> degree;
 argparse::ArgValue<unsigned> n_evaluation_points_per_dimension;
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

 // A factory to create matrices and vectors
 CCFactoryMatrices factory_matrices_and_vectors;
 
 // A factory to create the linear solver
 CCFactoryLinearSolver factory_linear_solver;
 
 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "Distance Matrix");
 
 // Add arguments 
 parser.add_argument<unsigned>(args.dimension, "--dim")
  .help("Dimension")
  .default_value("2")
  .choices({"1", "2", "3"});
 
 parser.add_argument<unsigned>(args.degree, "--degree")
  .help("Degree")
  .default_value("3")
  .choices({"1", "2", "3"});
 
 parser.add_argument<unsigned>(args.n_evaluation_points_per_dimension, "--n_evaluation_points_per_dimension")
  .help("Number of evaluation points per dimension")
  .default_value("1000");
 
 // Parse the input arguments
 parser.parse_args(argc, argv);
 
 // Create and initialise the problem
 CCDistanceMatrixProblem<CCMatrix, CCVector> problem(args.dimension, args.degree);

 // Complete problem setup/create nodes and set initial conditions
 problem.complete_problem_setup();

 // Document nodes positions
 problem.document_nodes_positions(std::string("RESLT/nodes.csv"));

 // Solve the problem
 problem.solve();
 
 // Document the solution
 problem.document_solution();

 // Document the error
 problem.documet_error();
 
 // --------------------------------------------------------------
 // Summary
 // --------------------------------------------------------------
 std::cerr << std::endl;
 std::cerr << "Polynomial degree: " << args.degree << std::endl;

#if 0
 
 std::cerr << "N. nodes per dimension: " << n_nodes_per_dim << std::endl;
 std::cerr << "N. total nodes: " << n_nodes << std::endl; 
 std::cerr << "RMS-error: " << rms_error << std::endl;
 

 // **************************************************************************
 
 // --------------------------------------------------------------
 // Domain specification
 // --------------------------------------------------------------
 // TODO: Create a DOMAIN (mesh?) type class
 
 // Dimension of the problem
 unsigned dim = args.dimension; // (if you change the dimension then
                                // also change
                                // 'n_evaluation_points_per_dimension')
 
 // Interpolant degree
 unsigned degree = args.degree;
 
 // Specify the one-dimensional lenght of the domain
 const unsigned L = 1.0;
 
 // --------------------------------------------------------------
 // Create and give position to nodes
 // --------------------------------------------------------------
 // Nodes per dimension
 const unsigned n_nodes_per_dim = pow(2, degree+1);
 // The number of nodes
 const unsigned n_nodes = pow(n_nodes_per_dim, dim);
 // A vector of nodes
 std::vector<CCNode *> nodes_pt(n_nodes);
 
 // Number of variables stored in the node
 const unsigned n_variables = 1;
 // Number of history values
 const unsigned n_history_values = 1;
 
 // Distance between a pair of nodes
 const Real h = L / (Real)(n_nodes_per_dim - 1);
 std::vector<Real> x(dim, 0.0);
 // Create the nodes
 for (unsigned i = 0; i < n_nodes; i++)
  {
   nodes_pt[i] = new CCNode(dim, n_variables, n_history_values);
  }

 // --------------------------------------------------------------
 // Output supporting nodes
 // --------------------------------------------------------------
 std::ofstream nodes_file("RESLT/nodes.dat");
 
 // Assign positions
 for (unsigned i = 0; i < n_nodes; i++)
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
 for (unsigned i = 0; i < n_nodes; i++)
  {
   for (unsigned j = 0; j < n_variables; j++)
    {
     const Real u = 0.0;
     nodes_pt[i]->set_variable(u, j);
    }
  }
 
 // --------------------------------------------------------------
 // Set boundary conditions
 // --------------------------------------------------------------
 
 // Move the first and the last node to the boundary of the domain
 //nodes_pt[0]->set_position(0.0, 0);
 //nodes_pt[0]->set_variable(0.0, 0);
 //nodes_pt[n_nodes-1]->set_position(1.0, 0);
 //nodes_pt[n_nodes-1]->set_variable(1.0, 0);
 
 // --------------------------------------------------------------
 // Set the problem and solve it
 // --------------------------------------------------------------
 
 // TODO: The distance matrix may be formed while we loop over the
 // nodes to extract their position
 
 // --------------------------------------------------------------
 // Loop over the nodes and extract their position and store them in a
 // matrix
 // --------------------------------------------------------------
 ACMatrix *nodes_matrix_position_pt = factory_matrices_and_vectors.create_matrix(dim, n_nodes);
 // Each column stores the vector position of a node
  for (unsigned i = 0; i < n_nodes; i++)
  {
   for (unsigned j = 0; j < dim; j++)
    {
     Real pos = nodes_pt[i]->get_position(j);
     nodes_matrix_position_pt->set_value(j, i, pos);
    }
  }
 
 // -------------------------------------------------------------- 
 // Create the distance matrix
 // --------------------------------------------------------------
  ACMatrix *distance_matrix_pt = factory_matrices_and_vectors.create_matrix(n_nodes, n_nodes);
 // --------------------------------------------------------------
 // Generate the distance matrix using the nodes position centers
 // shifted by the same nodes position
 // --------------------------------------------------------------
  
  compute_distance_matrix(nodes_matrix_position_pt, nodes_matrix_position_pt, distance_matrix_pt);
  
  // --------------------------------------------------------------
  // Set right-hand side
  // --------------------------------------------------------------
  ACVector *rhs_pt = factory_matrices_and_vectors.create_vector(n_nodes);
  ACVector *tmp_v_pt = factory_matrices_and_vectors.create_vector(dim);
 for (unsigned i = 0; i < n_nodes; i++)
  {
   for (unsigned j = 0; j < dim; j++)
    {
     Real pos = nodes_pt[i]->get_position(j);
     tmp_v_pt->set_value(j, pos);
    }
   // --------------------------------------------------------------
   // Evaluate the KNOWN function at the centers positions
   // --------------------------------------------------------------
   Real test_function_value = test_function<VECTOR_TYPE>(tmp_v_pt, dim);
   rhs_pt->set_value(i, test_function_value);
  }
 
 // The solution vector (with the respective number of rows) stores
 // the coefficients for the interpolant polynomials
 ACVector *sol_pt = factory_matrices_and_vectors.create_vector(n_nodes);
 
 // --------------------------------------------------------------
 // Solve
 // -------------------------------------------------------------- 
 // Create the linear solver
 ACLinearSolver *linear_solver_pt = factory_linear_solver.create_linear_solver();
 
 std::cerr << "Distance matrix" << std::endl;
 //distance_matrix.print();
 
 // --------------------------------------------------------------
 // Solve the system of equations
 // --------------------------------------------------------------
 linear_solver_pt->solve(distance_matrix_pt, rhs_pt, sol_pt);
 std::cerr << "Solution vector" << std::endl;
 //sol.print();

 std::cerr << "Nodes positions and values" << std::endl;
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
 unsigned n_evaluation_points_per_dimension = args.n_evaluation_points_per_dimension;
 const unsigned n_data_in_evaluation_points = pow(n_evaluation_points_per_dimension, dim);
 // Distance between a pair of nodes
 const Real h_test = L / (Real)(n_evaluation_points_per_dimension - 1);
 
 // Compute approximated solution at new positions
 ACMatrix *approx_solution_position_pt = factory_matrices_and_vectors->create_matrix(dim, n_evaluation_points_per_dimension);
 //approx_solution_position.allocate_memory();
 // --------------------------------------------------------------
 // Assign positions
 // --------------------------------------------------------------
 std::vector<Real> x_eval(dim, 0.0);
 for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     const Real r = rand();
     const Real position = static_cast<Real>(r / RAND_MAX) * L;
     // Generate position and assign it
     //const Real position = x_eval[k];
     approx_solution_position_pt->set_value(k, i, position);
     //x_eval[k]+=h_test;
    }
  }
 
 // Compute distance matrix with new positions
 ACMatrix *approx_distance_matrix_pt = factory_matrices_and_vectors->create_matrix(n_evaluation_points_per_dimension, n_nodes);
 // --------------------------------------------------------------
 // Generate the distance matrix using the nodes position centers
 // shifted by the new positions
 // --------------------------------------------------------------
 compute_distance_matrix(approx_solution_position_pt, nodes_matrix_position_pt, approx_distance_matrix_pt);
 //approx_distance_matrix.print();

 // Approximated solution
 ACVector *approx_sol_pt = factory_matrices_and_vectors->create_vector(n_evaluation_points_per_dimension);

 // HERE HERE HERE
 // Approximate solution at given points
 multiply_matrix_times_vector(approx_distance_matrix_pt, sol_pt, approx_sol_pt);
 (*approx_sol_pt) = (*approx_distance_matrix_pt) * (*sol_pt);
 approx_distance_matrix_pt->multiply by vector
 
 // --------------------------------------------------------------
 // Output data for plotting
 // --------------------------------------------------------------
 std::ofstream output_file("RESLT/output.dat");
 for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     output_file << approx_solution_position(k, i) << " ";
    }
   output_file << approx_sol(i) << std::endl;
  }
 
 // Close output file
 output_file.close();
 
 // --------------------------------------------------------------
 // Get real solution at given points and get the error 
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> real_sol(n_evaluation_points_per_dimension);
#else 
 CCVector<Real> real_sol(n_evaluation_points_per_dimension);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //real_sol.allocate_memory();
 for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
  {
#ifdef SCICELLXX_USES_ARMADILLO
   CCVectorArmadillo<Real> tmp_v(dim);
#else
   CCVector<Real> tmp_v(dim);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
   //tmp_v.allocate_memory();
   for (unsigned j = 0; j < dim; j++)
    {
     tmp_v(j) = approx_solution_position(j, i);
    }
   // ------------------------
   // Evaluation at approx_solution_position
   real_sol(i) = test_function<VECTOR_TYPE>(tmp_v, dim);
  }
 
 // --------------------------------------------------------------
 // Compute error
 // --------------------------------------------------------------
#ifdef SCICELLXX_USES_ARMADILLO
 CCVectorArmadillo<Real> error(n_evaluation_points_per_dimension);
#else
 CCVector<Real> error(n_evaluation_points_per_dimension);
#endif // #ifdef SCICELLXX_USES_ARMADILLO
 //error.allocate_memory();
 std::cerr << "ERRORS" << std::endl;
 for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
  {
   error(i) = real_sol(i) - approx_sol(i);
   //std::cerr << i << ": " << std::fabs(error(i)) << std::endl;
   //std::cerr << i << ": " << real_sol(i) << ":" << approx_sol(i) << std::endl;
  }
 
 const Real rms_error = error.norm_2() / sqrt(n_data_in_evaluation_points);
 
 // --------------------------------------------------------------
 // Output error
 // --------------------------------------------------------------
 std::ofstream error_file("RESLT/error.dat");
 for (unsigned i = 0; i < n_evaluation_points_per_dimension; i++)
  {
   for (unsigned k = 0; k < dim; k++)
    {
     error_file << approx_solution_position(k, i) << " ";
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
 std::cerr << "RMS-error: " << rms_error << std::endl;
 
 // ==============================================================
 // ==============================================================

 // Free memory
 delete linear_solver_pt;
 
 delete sol_pt;
 delete tmp_v_pt;
 delete rhs_pt;
 delete distance_matrix_pt;
 delete nodes_matrix_position_pt;
 
 // --------------------------------------------------------------
 // Delete nodes storage
 // --------------------------------------------------------------
 for (unsigned i = 0; i < n_nodes; i++)
  {
   delete nodes_pt[i];
  }

#endif // #if 0
 
 // Finalise chapcom
 finalise_scicellxx();
 
 return 0;
 
}

