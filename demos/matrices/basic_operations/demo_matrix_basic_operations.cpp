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

// ===================================================================
// Rotate
// ===================================================================
void rotate(Real *input_vector,
            Real *rotated_vector,
            const Real roll, const Real pitch, const Real yaw,
            bool inverse_rotation = false)
{
 // The number of elements in the input vector is three
 const unsigned DIM = 3;
 
 // Create the rotation matrix
 CCMatrix R(DIM, DIM);
 //R.allocate_memory();
 
 const Real sin_theta_x = sin(roll);
 const Real sin_theta_y = sin(pitch);
 const Real sin_theta_z = sin(yaw);
 const Real cos_theta_x = cos(roll);
 const Real cos_theta_y = cos(pitch);
 const Real cos_theta_z = cos(yaw);
 
 R(0,0) = cos_theta_y*cos_theta_z;
 R(0,1) = cos_theta_y*sin_theta_z;
 R(0,2) = -sin_theta_y;
 
 R(1,0) = sin_theta_x*sin_theta_y*cos_theta_z - cos_theta_x*sin_theta_z;
 R(1,1) = sin_theta_x*sin_theta_y*sin_theta_z+cos_theta_x*cos_theta_z;
 R(1,2) = sin_theta_x*cos_theta_y;
 
 R(2,0) = cos_theta_x*sin_theta_y*cos_theta_z + sin_theta_x*sin_theta_z;
 R(2,1) = cos_theta_x*sin_theta_y*sin_theta_z-sin_theta_x*cos_theta_z;
 R(2,2) = cos_theta_x*cos_theta_y;

 if (inverse_rotation)
  {
   // Tranpose the matrix to perform inverse rotation
   R.transpose();
  }
 
 // ----------------------------------
 // Perform the actual transformation
 
 // Create a vector to representation and copy there the input data
 CCVector b(input_vector, DIM);
 // A vector for the output (rotated) data
 CCVector r(DIM);
 // Apply rotation
 multiply_matrix_times_vector(R, b, r);
 // Copy back result in output structure
 rotated_vector[0]=r(0);
 rotated_vector[1]=r(1);
 rotated_vector[2]=r(2);
 
}

int main(int argc, char *argv[])
{
 // Initialise scicellxx
 initialise_scicellxx();
 
 // Output for testing/validation
 std::ofstream output_test("output_test.dat", std::ios_base::out);
 
 {
  // Show matrix manipulation for addition, multiplication and
  // transpose
  
  // Size of the matrix (number of rows and columns)
  const unsigned long n_rows = 10;
  const unsigned long n_columns = 10;
 
  // ----------------------------------------------------------
  // Identity matrix
  // ----------------------------------------------------------
  // Instantiate Real type matrices.
  CCMatrix I(n_rows, n_columns);
  
  // Allocate memory
  //I.allocate_memory();
  
  // Feed some data to the matrix (the identity)
  for (unsigned i = 0; i < n_rows; i++)
   {
    I(i,i) = 1;
   }
  
  std::cout << std::endl << "The identity matrix (I)" << std::endl << std::endl;
  output_test << std::endl << "The identity matrix (I)" << std::endl << std::endl;
  I.output();
  I.output(output_test);
 
  // ----------------------------------------------------------
  // Matrix from vector
  // ----------------------------------------------------------
  // Create an array to initialise another matrix with it
  Real *matrix_pt = new Real[n_rows*n_columns];
 
  // Add some data to the array
  for (unsigned i = 0; i < n_rows; i++)
   {
    for (unsigned j = 0; j < n_columns; j++)
     {
      if (i==j)
       {
        matrix_pt[i*n_columns+j] = i;
       }
      else 
       {
        // Get the distance to the diagonal
        matrix_pt[i*n_columns+j] = i;
       }
     }
   }
 
  // ---------------------------------------
  // Create the matrix from the vector data
  // ---------------------------------------
  CCMatrix B(matrix_pt, n_rows, n_columns);
  CCMatrix B_t;
  B.transpose(B_t);
 
  std::cout << std::endl << "Matrix created from vector" << std::endl << std::endl;
  output_test << std::endl << "Matrix created from vector" << std::endl << std::endl;
  B.output();
  B.output(output_test);

  std::cout << std::endl << "Matrix transposed" << std::endl << std::endl;
  output_test << std::endl << "Matrix transposed" << std::endl << std::endl;
  B_t.print();
  B_t.print(output_test);
 
  // ----------------------------------------------------------
  // Create a matrix where to store results
  // ----------------------------------------------------------
  // Solution matrix
  // Instantiate Real type matrices.
  CCMatrix C(n_rows, n_columns);

  // Allocate memory
  //C.allocate_memory();
 
  std::cout << std::endl
            << "------------------------------------------------------------------------------"
            << std::endl << "Matrix operations\n"
            << "------------------------------------------------------------------------------"
            << std::endl;
  
  output_test << std::endl
              << "------------------------------------------------------------------------------"
              << std::endl << "Matrix operations\n"
              << "------------------------------------------------------------------------------"
              << std::endl;
  
  // --------------------------------------
  // Sum of matrices C = I + B
  // --------------------------------------
  for (unsigned i = 0; i < n_rows; i++)
   {
    for (unsigned j = 0; j < n_columns; j++)
     {
      C(i,j) = I(i,j) + B(i,j);
     }
   }
 
  std::cout << std::endl << "The sum of the matrices is:"
            << std::endl << std::endl;
  output_test << std::endl << "The sum of the matrices is:"
              << std::endl << std::endl;
  C.output();
  C.output(output_test);
 
  // --------------------------------------
  // Matrix multiplication C = I * B
  // -------------------------------------- 
  // Zeroe the result matrix
  for (unsigned i = 0; i < n_rows; i++)
   {
    for (unsigned j = 0; j < n_columns; j++)
     {
      C(i,j) = 0;
     }
   }
 
  for (unsigned i = 0; i < n_rows; i++)
   {
    for (unsigned j = 0; j < n_columns; j++)
     {
      for (unsigned k = 0; k < n_columns; k++)
       {
        C(i,j)+= I(i,k) * B(k,j);
       }
     }
   }
 
  std::cout << std::endl << "The multiplication of the matrices is:"
            << std::endl << std::endl;
  output_test << std::endl << "The multiplication of the matrices is:"
              << std::endl << std::endl;
  C.output();
  C.output(output_test); 
 
  // ##############################################################################
  // Now do the same operations but using operator overloading
  // ##############################################################################
  std::cout << std::endl << ""
            << "##############################################################################\n"
            << "Now do the same operations but using operator overloading\n"
            << "##############################################################################"
            << std::endl;
  output_test << std::endl << ""
              << "##############################################################################\n"
              << "Now do the same operations but using operator overloading\n"
              << "##############################################################################"
              << std::endl;
  // --------------------------------------
  // Sum of matrices C = I + B
  // --------------------------------------
  C = I + B;
 
  std::cout << std::endl << "The sum of the matrices is:"
            << std::endl << std::endl;
  output_test << std::endl << "The sum of the matrices is:"
              << std::endl << std::endl;
  C.output();
  C.output(output_test);
 
  // --------------------------------------
  // Matrix multiplication C = I * B
  // --------------------------------------
  C = I * B;
 
  std::cout << std::endl << "The multiplication of the matrices is:"
            << std::endl << std::endl;
  output_test << std::endl << "The multiplication of the matrices is:"
              << std::endl << std::endl;
  C.output();
  C.output(output_test);
 
  // -----------------------------------------------------------------------------
  // Create a non square matrix
  // -----------------------------------------------------------------------------
  const unsigned long n_rows_A = 5;
  const unsigned long n_columns_A = 10;
  Real *matrix_A_pt = new Real[n_rows_A*n_columns_A];
 
  // Add some data to the array
  for (unsigned i = 0; i < n_rows_A; i++)
   {
    for (unsigned j = 0; j < n_columns_A; j++)
     {
      if (i==j)
       {
        matrix_A_pt[i*n_columns_A+j] = i;
       }
      else 
       {
        // Get the distance to the diagonal
        matrix_A_pt[i*n_columns_A+j] = i;
       }
     }
   }
  // Create the non square matrix
  CCMatrix A(matrix_A_pt, n_rows_A, n_columns_A);
  std::cout << std::endl << "Non square matrix"
            << std::endl << std::endl;
  output_test << std::endl << "Non square matrix"
              << std::endl << std::endl;
  A.output();
  A.output(output_test);
 
  // -----------------------------------------------------------------------------
  // Create a vector (using Matrix structure) to multiply with the non
  // square matrix
  // -----------------------------------------------------------------------------
  const unsigned long n_rows_x = 10;
  const unsigned long n_columns_x = 1;
  Real *matrix_x_pt = new Real[n_rows_x*n_columns_x];
 
  // Add some data to the array
  for (unsigned i = 0; i < n_rows_x; i++)
   {
    for (unsigned j = 0; j < n_columns_x; j++)
     {
      if (i==j)
       {
        matrix_x_pt[i*n_columns_x+j] = i;
       }
      else 
       {
        // Get the distance to the diagonal
        matrix_x_pt[i*n_columns_x+j] = i;
       }
     }
   }
 
  // Create the vector (matrix)
  CCMatrix x(matrix_x_pt, n_rows_x, n_columns_x);
  std::cout << std::endl << "Vector"
            << std::endl << std::endl;
  output_test << std::endl << "Vector"
              << std::endl << std::endl;
  x.output();
  x.output(output_test);
 
  // --------------------------------------
  // Matrix multiplication A * x = b
  // --------------------------------------
  CCMatrix b(n_rows_A, n_columns_x); // Note that we do not
  // need to force the
  // allocation of memory to
  // store the entries of
  // the matrix b, the times
  // operation (*) takes
  // care of the allocation
  // if it is required
  b = A * x;
 
  std::cout << std::endl << "The multiplication of the matrices is:"
            << std::endl << std::endl;
  output_test << std::endl << "The multiplication of the matrices is:"
              << std::endl << std::endl;
  b.output();
  b.output(output_test);
 
  // --------------------------------------------------
  // Apply transpose
  // --------------------------------------------------
  CCMatrix b_t(b.n_columns(), b.n_rows());
  b.transpose(b_t);
  std::cout << std::endl << "The transposed matrix:"
            << std::endl << std::endl;
  output_test << std::endl << "The transposed matrix:"
              << std::endl << std::endl;
  b_t.output();
  // Output for test
  b_t.output(output_test);
  
  // Free memory
  delete [] matrix_pt;
  matrix_pt = 0;
  delete [] matrix_A_pt;
  matrix_A_pt = 0;
  delete [] matrix_x_pt;
  matrix_x_pt = 0;
  
 }
 
 {
  // ##############################################################################
  // Matrix permutations
  // ##############################################################################
  std::cout << std::endl << ""
            << "##############################################################################\n"
            << "Matrix permutations (using own matrix methods)\n"
            << "##############################################################################"
            << std::endl;
  output_test << std::endl << ""
              << "##############################################################################\n"
              << "Matrix permutations (using own matrix methods)\n"
              << "##############################################################################"
              << std::endl;

  {
   const unsigned DIM = 3;
   
   // Matrix to permute
   CCMatrix A(DIM, DIM);
   //A.allocate_memory();
   
   A(0,0) = 1;   A(0,1) = 2;   A(0,2) = 3;
   A(1,0) = 4;   A(1,1) = 5;   A(1,2) = 6;
   A(2,0) = 7;   A(2,1) = 8;   A(2,2) = 9;

   std::cout << std::endl << ""
             << "---------------------------------------------------\n"
             << "Rows permutation (two rows only)\n"
             << "---------------------------------------------------\n"
             << std::endl;
   output_test << std::endl << ""
               << "---------------------------------------------------\n"
               << "Rows permutation (two rows only)\n"
               << "---------------------------------------------------\n"
               << std::endl;
  
   // Permute rows
   A.permute_rows(1, 2);
   
   A.output();
   A.output(output_test);
   
   std::cout << std::endl << ""
             << "---------------------------------------------------\n"
             << "Columns permutation (two columns only)\n"
             << "---------------------------------------------------\n"
             << std::endl;
   output_test << std::endl << ""
               << "---------------------------------------------------\n"
               << "Columns permutation (two columns only)\n"
               << "---------------------------------------------------\n"
               << std::endl;
   
   // Permute columns
   A.permute_columns(1, 2);
   
   A.output();
   A.output(output_test);
   
  }
  
  const unsigned DIM = 4;
   
  // Matrix to permute
  CCMatrix A(DIM, DIM);
  //A.allocate_memory();
   
  A(0,0) = 1;   A(0,1) = 2;   A(0,2) = 3;   A(0,3) = 4;
  A(1,0) = 5;   A(1,1) = 6;   A(1,2) = 7;   A(1,3) = 8;
  A(2,0) = 9;   A(2,1) = 10;   A(2,2) = 11;   A(2,3) = 12;
  A(3,0) = 13;   A(3,1) = 14;   A(3,2) = 15;   A(3,3) = 16;
  
  std::cout << std::endl << ""
            << "---------------------------------------------------\n"
            << "Rows permutation (multiple rows)\n"
            << "---------------------------------------------------\n"
            << std::endl;
  output_test << std::endl << ""
              << "---------------------------------------------------\n"
              << "Rows permutation (multiple rows)\n"
              << "---------------------------------------------------\n"
              << std::endl;
  
  // Create the list of rows to permute
  std::vector<std::pair<unsigned long, unsigned long> > permute_rows_list(2);
  permute_rows_list[0] = std::make_pair<unsigned long, unsigned long>(0, 3);
  permute_rows_list[1] = std::make_pair<unsigned long, unsigned long>(1, 2);
  
  // Permute rows
  A.permute_rows(permute_rows_list);
  
  A.output();
  A.output(output_test);
   
  std::cout << std::endl << ""
            << "---------------------------------------------------\n"
            << "Columns permutation (multiple columns)\n"
            << "---------------------------------------------------\n"
            << std::endl;
  output_test << std::endl << ""
              << "---------------------------------------------------\n"
              << "Columns permutation (multiple columns)\n"
              << "---------------------------------------------------\n"
               << std::endl;
  
  // Create the list of columns to permute
  std::vector<std::pair<unsigned long, unsigned long> > permute_columns_list(2);
  permute_columns_list[0] = std::make_pair<unsigned long, unsigned long>(0, 3);
  permute_columns_list[1] = std::make_pair<unsigned long, unsigned long>(1, 2);
  
  // Permute rows
  A.permute_columns(permute_columns_list); 
  
  A.output();
  A.output(output_test);
  
 }
 
 {
  // ##############################################################################
  // Matrix vector operations
  // ##############################################################################
  std::cout << std::endl << ""
            << "##############################################################################\n"
            << "Matrix vector operations\n"
            << "##############################################################################"
            << std::endl;
  output_test << std::endl << ""
              << "##############################################################################\n"
              << "Matrix vector operations\n"
              << "##############################################################################"
              << std::endl;
 
  // --------------------------------------------
  // Rotations to test matrix vector operations
  // --------------------------------------------
  const unsigned DIM = 3;
  // The gravity vector
  Real gravity[] ={0.0, 0.0, -9.81};
  // The rotated gravity vector
  Real rotated_gravity[DIM];
  // Roll, pitch and yaw Euluer angles
  const Real roll = M_PI/4.0;
  const Real pitch = M_PI/3.0;
  const Real yaw = -M_PI/8.0;
  rotate(gravity, rotated_gravity, roll, pitch, yaw);
 
  // Create vectors to output the data
  CCVector g(gravity, DIM);
  std::cout << std::endl << ""
            << "---------------------------------------------------\n"
            << "Gravity vector (0 0 -9.81) m/s^2\n"
            << "---------------------------------------------------\n"
            << std::endl;
  output_test << std::endl << ""
              << "---------------------------------------------------\n"
              << "Gravity vector (0 0 -9.81) m/s^2\n"
              << "---------------------------------------------------\n"
              << std::endl;
  g.output();
  g.output(output_test);

  // Rotated gravity vector
  CCVector rg(rotated_gravity, DIM); 
  std::cout << std::endl << ""
            << "---------------------------------------------------\n"
            << "Rotated gravity vector m/s^2\n"
            << "---------------------------------------------------\n"
            << std::endl;
  output_test << std::endl << ""
              << "---------------------------------------------------\n"
              << "Rotated gravity vector m/s^2\n"
              << "---------------------------------------------------\n"
              << std::endl;
  rg.output();
  rg.output(output_test);
  
  // Rotate the vector again to validate matrix vector operations and
  // rotations
  Real Real_rotated_gravity[DIM];
  rotate(rotated_gravity, Real_rotated_gravity, roll, pitch, yaw, true);
  // Create a vector to output the data
  CCVector drg(Real_rotated_gravity, DIM);
  std::cout << std::endl << ""
            << "---------------------------------------------------\n"
            << "Rotated-back gravity vector (original vector) m/s^2\n"
            << "---------------------------------------------------\n"
            << std::endl;
  output_test << std::endl << ""
              << "---------------------------------------------------\n"
              << "Rotated-back gravity vector (original vector) m/s^2\n"
              << "---------------------------------------------------\n"
              << std::endl;
  drg.output();
  drg.output(output_test);
  
  // Get the difference between the ORIGINAL gravity vector and the
  // REAL rotated gravity
  CCVector diff = g - drg;
  // Get the norm
  const Real norm = diff.norm_2();
  std::cout << std::endl << ""
            << "---------------------------------------------------\n"
            << "Norm 2 of original vector and rotated-back vector\n"
            << "---------------------------------------------------\n"
            << std::endl;
  output_test << std::endl << ""
              << "---------------------------------------------------\n"
              << "Norm 2 of original vector and rotated-back vector\n"
              << "---------------------------------------------------\n"
              << std::endl;
  std::cout << norm << std::endl;
  output_test << norm << std::endl;
  
 }

 {
  // ##############################################################################
  // Vector-matrix operations
  // ##############################################################################
  std::cout << std::endl << ""
            << "##############################################################################\n"
            << "Vector-matrix operations\n"
            << "##############################################################################"
            << std::endl;
  output_test << std::endl << ""
              << "##############################################################################\n"
              << "Vector-matrix operations\n"
              << "##############################################################################"
              << std::endl;
  
  // ------------------------------------------------------------------
  // Vector-matrix multiplication. Use a row vector to extract some
  // data from the matrix to the right.
  // ------------------------------------------------------------------
  const unsigned DIM = 3;

  // Create a matrix
  CCMatrix A(DIM, DIM);
  //A.allocate_memory();
  
  A(0,0) = 0.5;   A(0,1) = 0.5;   A(0,2) = 0.5;
  A(1,0) = 1.0;   A(1,1) = 1.0;   A(1,2) = 1.0;
  A(2,0) = 2.0;   A(2,1) = 2.0;   A(2,2) = 2.0;
  
  // Create a vector indicating the data to extract from the matrix,
  // the vector is created as a row vector
  bool is_column_vector = false;
  CCVector v(DIM, is_column_vector);
  //v.allocate_memory();
  
  // The following vector states to take the first row of matrix A and
  // add it to half the second row and a quarter of the third row
  v(0) = 1.0;  v(1) = 0.5;  v(2) = 0.25;

  // A vector (as a matrix) where to store the resulting vector-matrix
  // multiplication
  CCMatrix S(1, DIM);
  
  multiply_vector_times_matrix(v, A, S);

  std::cout << std::endl << ""
            << "---------------------------------------------------\n"
            << "Extract data of a matrix by matrix-multiplication\n"
            << "Get the first row + half the second row + a quarter of the third row\n"
            << "---------------------------------------------------\n"
            << std::endl;
  output_test << std::endl << ""
              << "---------------------------------------------------\n"
              << "Extract data of a matrix by matrix-multiplication\n"
              << "Get the first row + half the second row + a quarter of the third row\n"
              << "---------------------------------------------------\n"
              << std::endl;
  S.output();
  S.output(output_test);
  
 }
 
 {
  // ##############################################################################
  // Matrix-matrix operations
  // ##############################################################################
  std::cout << std::endl << ""
            << "##############################################################################\n"
            << "Matrix matrix operations (row permutations)\n"
            << "##############################################################################"
            << std::endl;
  output_test << std::endl << ""
              << "##############################################################################\n"
              << "Matrix matrix operations (row permutations)\n"
              << "##############################################################################"
              << std::endl;
  
  // ------------------------------------------------------------------
  // Use permutation matrices to check the correct implementation of
  // matrix-matrix multiplication
  // ------------------------------------------------------------------
  const unsigned DIM = 3;

  // Create two matrices

  // The permutation matrix (row permutations - multiply P to the left
  // of the matrix to permute)
  CCMatrix P(DIM, DIM);
  //P.allocate_memory();
  
  // Each row of this matrix tell us how much of the rows of the
  // matrix to the right should we take to create the new
  // matrix. Example, the first row tell us to take all the elements
  // of the first row of the matrix to the right and copy them to the
  // first row of the resulting matrix. Elements from the other rows
  // are not taken. The second row tell us to take all the elements of
  // the third row of the matrix to the right and copy them in the
  // second row of the resulting matrix. The third row tell us to take
  // only the elements of the second row of the matrix to the right to
  // create the third row of the resulting matrix. We can specify "how
  // much" of each row to take to create the rows of the matrix to the
  // right.
  P(0,0) = 1;   P(0,1) = 0;   P(0,2) = 0;
  P(1,0) = 0;   P(1,1) = 0;   P(1,2) = 1;
  P(2,0) = 0;   P(2,1) = 1;   P(2,2) = 0;

  // Matrix to permute
  CCMatrix A(DIM, DIM);
  //A.allocate_memory();

  A(0,0) = 1;   A(0,1) = 2;   A(0,2) = 3;
  A(1,0) = 4;   A(1,1) = 5;   A(1,2) = 6;
  A(2,0) = 7;   A(2,1) = 8;   A(2,2) = 9;
  
  // Permuted matrix
  CCMatrix S(DIM, DIM);
  
  multiply_matrices(P, A, S);

  S.output();
  S.output(output_test);
  
 }
 
 {

  // ##############################################################################
  // Matrix-matrix operations
  // ##############################################################################
  std::cout << std::endl << ""
            << "##############################################################################\n"
            << "Matrix matrix operations (column permutations)\n"
            << "##############################################################################"
            << std::endl;
  output_test << std::endl << ""
              << "##############################################################################\n"
              << "Matrix matrix operations (column permutations)\n"
              << "##############################################################################"
              << std::endl;
 
  // ------------------------------------------------------------------
  // Use permutation matrices to check the correct implementation of
  // matrix-matrix multiplication
  // ------------------------------------------------------------------
  const unsigned DIM = 3;

  // Create two matrices

  // The permutation matrix (column permutations - multiply P to the
  // right of the matrix to permute)
  CCMatrix P(DIM, DIM);
  //P.allocate_memory();
  
  // Each column of this matrix tell us how much of the columns of the
  // matrix to the left should we take to create the new
  // matrix. Example, the first column tell us to take all the
  // elements of the first column of the matrix to the left and copy
  // them to the first column of the resulting matrix. Elements from
  // the other columns are not taken. The second column tell us to
  // take all the elements of the third column of the matrix to the
  // left and copy them in the second column of the resulting
  // matrix. The third column tell us to take only the elements of the
  // second column of the matrix to the left to create the third
  // column of the resulting matrix. We can specify "how much" of each
  // column to take to create the columns of the matrix to the left.
  P(0,0) = 1;   P(0,1) = 0;   P(0,2) = 0;
  P(1,0) = 0;   P(1,1) = 0;   P(1,2) = 1;
  P(2,0) = 0;   P(2,1) = 1;   P(2,2) = 0;
  
  // Matrix to permute
  CCMatrix A(DIM, DIM);
  //A.allocate_memory();
  
  A(0,0) = 1;   A(0,1) = 4;   A(0,2) = 7;
  A(1,0) = 2;   A(1,1) = 5;   A(1,2) = 8;
  A(2,0) = 3;   A(2,1) = 6;   A(2,2) = 9;
  
  // Permuted matrix
  CCMatrix S(DIM, DIM);
  
  multiply_matrices(A, P, S);
  
  S.output();
  S.output(output_test);
  
 }
 
 // Close the output for test
 output_test.close();
 
 // Finalise scicellxx
 finalise_scicellxx();
 
 return 0;
 
}
