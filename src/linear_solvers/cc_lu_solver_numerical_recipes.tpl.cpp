// IN THIS FILE: Implementation of a concrete class to solve systems
// of equations by LU decomposition. This class calls the methods
// ludcmp() and lubksb() from numerical recipes to perform the LU
// decomposition and back-substitution, respectively.

#include "cc_lu_solver_numerical_recipes.tpl.h"

namespace chapchom
{

 // ===================================================================
 // Empty constructor
 // ===================================================================
 template<class T>
 CCLUSolverNumericalRecipes<T>::CCLUSolverNumericalRecipes()
  : ACLinearSolver<T>(),
    Resolve_enabled(false) { }

 // ===================================================================
 // Constructor where we specify the matrix A of size m X n
 // ===================================================================
 template<class T>
 CCLUSolverNumericalRecipes<T>::CCLUSolverNumericalRecipes(const CCMatrix<T> &A)
  : ACLinearSolver<T>(A),
    Resolve_enabled(false) { }

 // ===================================================================
 // Empty destructor
 // ===================================================================
 template<class T>
 CCLUSolverNumericalRecipes<T>::~CCLUSolverNumericalRecipes() { }

 // ===================================================================
 // Solve a system of equations with input A. We specify the right-hand
 // side b and the x vector where the result is returned. We assume
 // that the input/output vectors have the correct dimensions (size n).
 // ===================================================================
 template<class T>
 void CCLUSolverNumericalRecipes<T>::solve(const CCMatrix<T> &A,
                                           const CCMatrix<T> &b,
                                           CCMatrix<T> &x)
 {
  // Set the matrix and its size
  set_matrix_A(A);
 
  // Solve
  solve(b, x);
 
 }

 // ===================================================================
 // Solve a system of equations with the already stored matrix A. We
 // specify the right-hand side b and the x vector where the result is
 // returned. We assume that the input/output vectors have the correct
 // dimensions (size n).
 // ===================================================================
 template<class T>
 void CCLUSolverNumericalRecipes<T>::solve(const CCMatrix<T> &b,
                                           CCMatrix<T> &x)
 {
  // We can only call solve if the matrix A has been set
  if (this->Matrix_A_has_been_set)
   {     
    // Factorise
    factorise();
   
    // ... and do back substitution
    back_substitution(b, x);
   
   }
  else
   {
    // Error message
   std:ostringstream error_message;
    error_message << "You have not specific any matrix for the system of\n"
                  << "equations. Set one matrix first by calling the/"
                  << "set_matrix() method or use the solve() method where\n"
                  << "you can specify the matrix associated to the system\n"
                  << "of equations." << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
 
 }

 // ===================================================================
 // Re-solve a system of equations with the already stored matrix
 // A. Reusing the LU decomposition. We specify the right-hand side b
 // and the x vector where the result is returned. We assume that the
 // input/output vectors have the correct dimensions (size n).
 // ===================================================================
 template<class T>
 void CCLUSolverNumericalRecipes<T>::resolve(const CCMatrix<T> &b,
                                             CCMatrix<T> &x)
 {
  // We can only do back-substitution if a matrix has been
  // factorised
  if (Resolve_enabled)
   {
    // Do the back substitution
    back_substitution(b, x);
   }
  else
   {
    // Error message
   std:ostringstream error_message;
    error_message << "Resolve is not enabled.\n" << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
 
 }

 // ===================================================================
 // Performs LU factorisation of the input matrix, the factorisation is
 // internally stored such that it can be re-used when calling resolve
 // ===================================================================
 template<class T>
 void CCLUSolverNumericalRecipes<T>::factorise(const CCMatrix<T> &A)
 {
  // Set the matrix and its size
  set_matrix_A(A);
 
  // Factorise
  factorise();
 
 }

 // ===================================================================
 // Performs LU factorisation of already stored matrix A, the
 // factorisation is internally stored such that it can be re-used when
 // calling resolve
 // ===================================================================
 template<class T>
 void CCLUSolverNumericalRecipes<T>::factorise()
 {
  // Prepare the matrix to call ludcmp() from Numerical Recipes
 
  // Check that we are working with an square matrix, otherwise this
  // will not work
  const unsigned long nrows = this->A.nrows();
  const unsigned long ncolumns = this->A.ncolumns();
  if (nrows!=ncolumns)
   {
    // Error message
   std:ostringstream error_message;
    error_message << "The matrix is not square." << std::endl
                  << "The matrix is of size: " << nrows << " x "
                  << ncolumns << std::endl;
    throw ChapchomLibError(error_message.str(),
                           CHAPCHOM_CURRENT_FUNCTION,
                           CHAPCHOM_EXCEPTION_LOCATION);
   }
 
  // The matrix used as input and output, after calling ludcmp it has
  // the LU factorisation
  Mat_DP lu_a(nrows, ncolumns);
 
  // Output vector of size n x 1 that records the row permutations
  // performed by partial pivoting.
  Vec_INT lu_indx(nrows);
 
  // An output indicating whether the number of rows interchanges was
  // even or odd (+1 or -1, respectively)
  DP dummy;
 
  // Copy the matrix A to the representation required by ludcmp()
  for (unsigned i = 0; i < nrows; i++)
   {
    for (unsigned j = 0; j < ncolumns; j++)
     {
      lu_a[i][j] = this->A(i,j);
     }
   }
 
  // Do the factorisation
  NR::ludcmp(lu_a, lu_indx, dummy);
 
  // Set the flag to indicate that resolve is enabled since we have
  // computed the LU decomposition
  Resolve_enabled = true; 
 
 }

 // ===================================================================
 // Performs the back substitution with the LU decomposed matrix
 // ===================================================================
 template<class T>
 void CCLUSolverNumericalRecipes<T>::back_substitution(const CCMatrix<T> &b,
                                                       CCMatrix<T> &x_output)
 {
  // Prepare the data to call lubksb()
 
  // The size of the right-hand side
  const unsigned n_rows = b.nrows();
 
  // Number of right hand sizes (same as the number of output
  // x-vectors)
  const unsigned n_rhs = b.ncolumns();
 
  // The solution vector size n x 1 (Numerical Recipes definition)
  Vec_DP x(n_rows);
 
  // Copy the right-hand side into the solution vectors
  for (unsigned j = 0; j < n_rhs; j++)
   {
    for (unsigned i = 0; i < n_rows; i++)
     {
      x[i] = b(i,j);
     }
   
    // Back-substitution
    NR::lubksb(lu_a, *lu_indx, x);
   
    // Copy the solution into the output vector
    for (unsigned i = 0; i < n_rows; i++)
     {
      x_output(i, j) =  x[i];
     }
   
   }
 
 }

}