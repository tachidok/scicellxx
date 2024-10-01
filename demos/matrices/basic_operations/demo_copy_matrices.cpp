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

// -------------------------------------------------------------------
// 1 - Show assignment of data to matrices from arrays and vectors
// 2 - Show COPY contructor matrix
// 3 - Show operations to compute the transpose of matrices
// -------------------------------------------------------------------
int main(int argc, char *argv[])
{
 // Initialise scicellxx
 initialise_scicellxx();
 
 // Output for testing/validation
 std::ofstream output_test("output_test.dat", std::ios_base::out);
 
 // =============================================================
 // Create matrices from an array and a CCVector
 // =============================================================
 // ------------------------------------------------------------- 
 // Create matrix from array
 // ------------------------------------------------------------- 
 const unsigned n_rows = 5; 
 const unsigned n_columns = 3; 
 // Create the array where the values are stored
 Real *matrix_pt = new Real[n_rows*n_columns];
 
 // Add some data to the array (represents the distance to the
 // diagonal)
 for (unsigned i = 0; i < n_rows; i++)
  {
   for (unsigned j = 0; j < n_columns; j++)
    {
     if (i==j)
      {
       matrix_pt[i*n_columns+j] = 0;
      }
     else 
      {
       // Get the distance to the diagonal
       matrix_pt[i*n_columns+j] = static_cast<int>(j)-static_cast<int>(i);
      }
    }
  }
 
 // Output array
 std::cout << std::endl << "Array matrix_pt[i]" << std::endl << std::endl;
 output_test << std::endl << "Array matrix_pt[i]" << std::endl << std::endl;
 for (unsigned i = 0; i < (n_rows * n_columns) - 1; i++)
  {
   std::cout << matrix_pt[i] << " ";
   output_test << matrix_pt[i] << " ";
  }
 std::cout << matrix_pt[(n_rows * n_columns) - 1] << std::endl;
 output_test << matrix_pt[(n_rows * n_columns) - 1] << std::endl;
 
 // Create the matrix from the array data
 CCMatrix A(matrix_pt, n_rows, n_columns);
 std::cout << std::endl << "CCMatrix (A) created from array" << std::endl << std::endl;
 output_test << std::endl << "CCMatrix (A) created from array" << std::endl << std::endl;
 A.output();
 A.output(output_test);
 
 // -------------------------------------------------------------
 // Create a matrix form a CCVector
 // -------------------------------------------------------------  
 // The vector
 CCVector b(n_rows);

 // Allocate memory for the vector
 //b.allocate_memory();
 
 // Fill the vector with data
 b(0) = -16.0;
 b(1) = -8.0;
 b(2) = 0.0;
 b(3) = 8.0;
 b(4) = 16.0;
 std::cout << std::endl << "Column CCVector (b)" << std::endl << std::endl;
 output_test << std::endl << "Column CCVector (b)" << std::endl << std::endl;
 b.output();
 b.output(output_test);
 
 // Create a matrix form a vector
 CCMatrix B(b);
 std::cout << std::endl << "CCMatrix (B) created from a CCVector (b)" << std::endl << std::endl;
 B.output();
 B.output(output_test);
 
 // Create a row vector
 bool is_column_vector = false;
 CCVector r(n_columns, is_column_vector);
 // Allocate memory for the vector
 //r.allocate_memory();
 
 // Fill the vector with data
 r(0) = 1.0;
 r(1) = 2.0;
 r(2) = 3.0;
 std::cout << std::endl << "Row CCVector (r)" << std::endl << std::endl;
 output_test << std::endl << "Row CCVector (r)" << std::endl << std::endl;
 bool output_indexes = true;
 r.print(output_indexes);
 r.print(output_test, output_indexes);
 
 // Create matrix from row CCVector
 CCMatrix R(r);
 std::cout << std::endl << "CCMatrix (R) created from row CCVector (r)" << std::endl << std::endl;
 output_test << std::endl << "CCMatrix (R) created from row CCVector (r)" << std::endl << std::endl;
 R.output();
 R.output(output_test);
 
 // =============================================================
 // Copy constructor 
 // =============================================================
 std::cout << std::endl << "Copy CCMatrix constructor (from CCMatrix A create CCMatrix C)" << std::endl << std::endl;
 output_test << std::endl << "Copy CCMatrix constructor (from CCMatrix A create CCMatrix C)" << std::endl << std::endl;
 CCMatrix C = A;
 C.output();
 C.output(output_test);
 
 std::cout << std::endl << "Copy CCMatrix constructor (from CCMatrix B create CCMatrix D)" << std::endl << std::endl;
 output_test << std::endl << "Copy CCMatrix constructor (from CCMatrix B create CCMatrix D)" << std::endl << std::endl;
 CCMatrix D = B;
 D.output();
 D.output(output_test);
 
 // =============================================================
 // Transpose operations 
 // =============================================================
 std::cout << std::endl << "========================================" << std::endl;
 std::cout << "Transpose operations" << std::endl;
 std::cout << std::endl << "========================================" << std::endl;
 output_test << std::endl << "========================================" << std::endl;
 output_test << "Transpose operations" << std::endl;
 output_test << std::endl << "========================================" << std::endl;
 // Transpose the matrix and print it
 A.transpose();
 std::cout << std::endl<< "Transposed CCMatrix (A)" << std::endl << std::endl;
 output_test << std::endl<< "Transposed CCMatrix (A)" << std::endl << std::endl;
 A.output();
 A.output(output_test); 
 // Transpose the vector
 CCVector b_t;
 b.transpose(b_t);
 std::cout << std::endl<< "Transposed CCVector (b)" << std::endl << std::endl;
 output_test << std::endl<< "Transposed CCVector (b)" << std::endl << std::endl;
 b_t.output();
 b_t.output(output_test);
 
 // Transpose the matrix but store the result in a new CCMatrix
 CCMatrix B_t;
 B.transpose(B_t);
 std::cout << std::endl << "Transposed CCMatrix (B)" << std::endl << std::endl;
 output_test << std::endl << "Transposed CCMatrix (B)" << std::endl << std::endl;
 B_t.output();
 B_t.output(output_test);
 
 // Transpose the row vector to generate a column vector
 r.transpose();
 std::cout << std::endl << "Transposed row CCVector (r)" << std::endl << std::endl;
 output_test << std::endl << "Transposed row CCVector (r)" << std::endl << std::endl;
 r.output(output_indexes);
 r.output(output_test, output_indexes);
 
 // Create matrix from column CCVector
 CCMatrix R_t(r);
 std::cout << std::endl << "CCMatrix (R_t) created from transposed row CCVector (r)" << std::endl << std::endl;
 output_test << std::endl << "CCMatrix (R_t) created from transposed row CCVector (r)" << std::endl << std::endl;
 R_t.output();
 R_t.output(output_test);
 
 // Transpose and store in new matrix
 CCMatrix C_t; 
 C.transpose(C_t);
 std::cout << std::endl << "Transposed CCMatrix (C)" << std::endl << std::endl;
 output_test << std::endl << "Transposed CCMatrix (C)" << std::endl << std::endl;
 C_t.output();
 C_t.output(output_test);
 
 // Transpose and store in the same CCMatrix
 D.transpose();
 std::cout << std::endl << "Transposed CCMatrix (D)" << std::endl << std::endl;
 output_test << std::endl << "Transposed CCMatrix (D)" << std::endl << std::endl;
 D.output();
 D.output(output_test);
 
 // Close the output for test
 output_test.close();
 
 // Free memory
 delete [] matrix_pt;
 matrix_pt = 0;
 
 // Finalise scicellxx
 finalise_scicellxx();
 
 return 0;
 
}
