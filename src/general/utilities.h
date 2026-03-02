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
#ifndef UTILITIES_H
#define UTILITIES_H

#include "common_includes.h"

namespace scicellxx
{

 // ======================================================================
 /// The error messages are based on oomph-lib's implementation to deal
 /// with errors
 //=======================================================================

 namespace BrokenCopy
 {
  /// Issue error message and terminate execution
  extern void broken_assign(const std::string& class_name);
 
  /// Issue error message and terminate execution
  extern void broken_copy(const std::string& class_name);
 }

 //=======================================================================
 /// Helper namespace for set_terminate function -- used to spawn
 /// messages from uncaught errors (their destructor may not be called)
 ///======================================================================
 namespace TerminateHelper
 {
  /// Setup terminate helper
  extern void setup();

  /// \short Suppress error messages (e.g. because error has been caught)
  extern void suppress_exception_error_messages();

  /// Function to spawn messages from uncaught errors
  extern void spawn_errors_from_uncaught_errors();

  /// Stream to output error messages
  extern std::ostream* Error_message_stream_pt;

  /// String stream that records the error message
  extern std::stringstream* Exception_stringstream_pt;

 }
 
 //=======================================================================
 /// Helper namespace for file system operations -- mainly used to
 /// create the RESLT folder
 ///======================================================================
 namespace SciCellxxFileSystem
 {
  // Check whether a given directory exists
  bool directory_exists(std::string &directory_name);

  // Create a directory
  bool create_directory(std::string &directory_name);
 }

#ifdef SCICELLXX_USES_MPI
 //=======================================================================
 /// Helper namespace for MPI variables
 //=======================================================================
 namespace SciCellxxMPI
 {
  // Store the MPI communicator
  extern MPI_Comm comm;
  
  // Store the number of processors on MPI_WORLD
  extern int nprocs;
  // Store the rank of this process
  extern int rank;
  
  // Store the master core number
  extern int master_core;
 }
#endif // #ifdef SCICELLXX_USES_MPI
 
 //=====================================================================
 /// Run-time exception handling  (error and warning).
 ///
 /// The (protected) constructor combines its string arguments into a
 /// standard format for uniform exception reports which are written to
 /// the specified output stream.
 ///
 //=====================================================================

 /// The class can only be instantiated by the derived classes
 /// SciCellxxLibError and SciCellxxLibWarning.
 class SciCellxxLibException : public std::runtime_error
 {
 
 public:
 
  /// Suppress error message in destructor (useful if error is caught
  /// successfully!)
  void disable_error_message();
 
 protected:
 
  /// Constructor takes the error description, function name and a
  /// location string provided by the SCICELLXX_EXCEPTION_LOCATION macro
  /// and combines them into a standard header. The exception type will
  /// be the string "WARNING" or "ERROR" and the message is written to
  /// the exception_stream, with a specified output_width. Optionally
  /// provide a traceback of the function calls.
  SciCellxxLibException(const std::string &error_description,
                       const std::string &function_name,
                       const char *location,
                       const std::string &exception_type,
                       std::ostream &exception_stream,
                       const unsigned &output_width);
 
  /// The destructor cannot throw an exception (C++ STL standard)
  ~SciCellxxLibException() throw(); 
 
  /// Exception stream to which we write message in destructor        
  std::ostream* Exception_stream_pt;
 
  /// String stream that records the error message
  std::stringstream* Exception_string_stream_pt;
 
  /// Boolean to suppress issuing of the error message in destructor
  /// (useful if error is caught successfully!)
  bool Suppress_error_message;
 
 };

 /// ====================================================================
 /// Throw this object when an run-time / error is encountered. The
 /// error stream and stream width can be specified. The default is
 /// cerr with a width of 70 characters.
 /// ====================================================================
 class SciCellxxLibError : public SciCellxxLibException
 {
  /// Output stream that is used to write the errors
  static std::ostream *Stream_pt;
 
  /// Width in characters of the output report
  static unsigned Output_width;

 public:

  /// Constructor requires the error description and the function in
  /// which the error occured and the location provided by the
  /// SCICELLXX_EXCEPTION_LOCATION macro
 SciCellxxLibError(const std::string &error_description,
		  const std::string &function_name,
		  const char *location) :
  SciCellxxLibException(error_description,function_name,location,"ERROR",
                       *Stream_pt, Output_width) 
   { }
 
  /// Static member function used to specify the error stream, which
  /// must be passed as a pointer because streams cannot be copied.
  static inline void set_stream_pt(std::ostream* const &stream_pt)
  {Stream_pt = stream_pt;}
 
  /// Static member function used to specify the width (in characters)
  /// of the error stream
  static inline void set_output_width(const unsigned &output_width)
  {Output_width = output_width;}
 
 };

 //====================================================================
 /// An SciCellxxLibWarning object which should be created as a temporary
 /// object to issue a warning. The warning stream and stream width can
 /// be specified. The default is cerr with a width of 70 characters.
 //====================================================================
 class SciCellxxLibWarning : public SciCellxxLibException
 {
  /// Output stream that is used to write the errors
  static std::ostream *Stream_pt;

  /// Width of output
  static unsigned Output_width;

 public:

  /// Constructor requires the warning description and the function
  /// in which the warning occurred.
 SciCellxxLibWarning(const std::string &warning_description,
		    const std::string &function_name,
		    const char* location) :
  SciCellxxLibException(warning_description,function_name, location,
                       "WARNING",*Stream_pt,Output_width) { }
 
  /// Static member function used to specify the error stream, which
  /// must be passed as a pointer because streams cannot be copied.
  static inline void set_stream_pt(std::ostream* const &stream_pt)
  {Stream_pt = stream_pt;}
 
  /// \short Static member function used to specify the width (in characters)
  /// of the error stream
  static inline void set_output_width(const unsigned &output_width)
  {Output_width = output_width;}
 
 };

 ////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////
 
 //========================================================================
 // Wrapper to a stream and an output modifier used to control the
 // output from scicellxx. Its instantiation can be used like std::cout.
 // =======================================================================
 class SciCellxxOutput
 {
  
 private:
  
  ///Pointer to the output stream -- defaults to std::cout
  std::ostream *Stream_pt;
  
 public:
  
  ///\short Set default values for the output stream (cout)
  ///and modifier (no modification)
  SciCellxxOutput();
  
#if 0
  ///\short Overload the << operator, writing output to the stream addressed by
  ///Stream_pt and calling the function defined by the object addressed by
  ///Output_modifier_pt
  template<class _Tp>
   std::ostream &operator<<(_Tp argument);
#endif // #if 0
  
#if 1
  template<class _Tp>
   std::ostream &operator<<(_Tp argument)
   {
    *Stream_pt << argument;
    return (*Stream_pt);
   }
#endif // #if 1
  
  ///Access function for the stream pointer
  std::ostream* &stream_pt() {return Stream_pt;}
 
  ///Overload insertor to handle stream modifiers
  std::ostream &operator<<(std::ostream& (*f)(std::ostream &))
   {
    return f(*Stream_pt);
   }
 
 };
 
 //========================================================================
 // Single (global) instantiation of the SciCellxxOutput object -- this
 // is used throughout the library as a "replacement" for std::cout
 //========================================================================
 extern SciCellxxOutput scicellxx_output;
 
 //==================================================================
 // Utility method to time a program
 //==================================================================
 namespace Timing
 {
  // ================================================================
  // Use this method when you want to know the "wall time" a section
  // of code, method or program takes for its execution. Call it at
  // the beggining of the section and then at the end of the section
  // you want to time
  // ================================================================
  inline time_t wall_time() {return time(0);}
  
  // ================================================================
  // Use this method to get the difference (IN SECONDS) between two
  // "wall_time()" method calls
  // ================================================================
  inline double diff_wall_time(time_t &initial_time, time_t &final_time)
  {return difftime(final_time, initial_time);}
  
  // ================================================================
  // Use this method when you want to know the "cpu time" a section of
  // code, method or program takes for its execution. Call it at the
  // beggining of the section and then at the end of the section you
  // want to time
  // ================================================================
  inline clock_t cpu_clock_time() {return clock();}
  
  // ================================================================
  // Use this method to get the difference (IN SECONDS) between two
  // "cpu_clock_time()" method calls
  // ================================================================
  inline double diff_cpu_clock_time(clock_t &initial_cpu_time,
                                    clock_t &final_cpu_time)
  {return
    static_cast<double>(final_cpu_time-initial_cpu_time)/CLOCKS_PER_SEC;}
  
 }

  //=======================================================================
 /// Helper namespace to generate the catesian product of multiple
 // /vectors
 ///======================================================================
 namespace SciCellxxCartesianProduct
 {
  // Print the cartesian product of the vectors
  template<class DATA_TYPE>
  void print(const std::vector<std::vector<DATA_TYPE> >& v)
  {
   scicellxx_output << "{ ";
   for (const auto& p : v) {
    scicellxx_output << "(";
    for (const auto& e : p) {
     scicellxx_output << e << " ";
    }
    scicellxx_output << ") ," << std::endl;
   }
   scicellxx_output << "}" << std::endl;
  }
  
  // Compute the cartesian product of a set of vectors
  template<class DATA_TYPE>
  std::vector<std::vector<DATA_TYPE> > product(const std::vector<std::vector<DATA_TYPE> >& lists)
  {
   std::vector<std::vector<DATA_TYPE> > result;
   if (std::find_if(std::begin(lists), std::end(lists), 
                    [](auto e) -> bool { return e.size() == 0; }) != std::end(lists))
    {
     return result;
    }
   
   for (auto& e : lists[0])
    {
    result.push_back({ e });
    }
   
   for (size_t i = 1; i < lists.size(); ++i)
    {
     std::vector<std::vector<DATA_TYPE>> temp;
     for (auto& e : result)
      {
      for (auto f : lists[i])
       {
        auto e_tmp = e;
        e_tmp.push_back(f);
        temp.push_back(e_tmp);
       }
      }
     result = temp;
    }
   return result;
  }
  
  /*  
  int main() {
   std::vector<std::vector<int>> prods[] = {
    { { 1, 2 }, { 3, 4 } },
    { { 3, 4 }, { 1, 2} },
    { { 1, 2 }, { } },
    { { }, { 1, 2 } },
    { { 1776, 1789 }, { 7, 12 }, { 4, 14, 23 }, { 0, 1 } },
    { { 1, 2, 3 }, { 30 }, { 500, 100 } },
    { { 1, 2, 3 }, { }, { 500, 100 } }
   };
   for (const auto& p : prods) {
    print(product(p));
   }
   std::cin.ignore();
   std::cin.get();
   return 0;
  }
  */
  
 }

 //=======================================================================
 /// Helper namespace to a linearspace in the range [min_value,
 /// max_value] with the specified number of points
 ///======================================================================
 namespace SciCellxxLinearSpace
 {
  // Create a linear space with Real values
  template<class DATA_TYPE>
  void create_linear_space(std::vector<DATA_TYPE> &linear_space,
                           const DATA_TYPE min_value, const DATA_TYPE max_value,
                           const DATA_TYPE step, const unsigned n_points)
  {
   DATA_TYPE ival = min_value;
   for (unsigned i = 0; i < n_points; i++)
    {
     linear_space.push_back(ival);
     ival+=step;
    }
   
  }
  
  // Print the linear space (unsigned)
  template<class DATA_TYPE>
  void print_linear_space(std::vector<DATA_TYPE> &linear_space)
  {
   // Get the number of elements
   const unsigned n_ele = linear_space.size();
   // Print them
   scicellxx_output << "[";
   for (unsigned i = 0; i < n_ele-1; i++)
    {
     scicellxx_output << linear_space[i] << ",";
    }
   scicellxx_output << linear_space[n_ele-1] << "]" << std::endl;
   
  }
  
 }
 
 //=======================================================================
 /// Helper namespace to compute statistics from certain data structures
 ///======================================================================
 namespace SciCellxxStatistics
 {
  // Computes statistics for a std::vector
  template<class DATA_TYPE>
  void statistics_mean_std_median(std::vector<DATA_TYPE> &v, Real &mean, Real &stdev, DATA_TYPE &median)
  {
   // Vector size
   const unsigned v_size = v.size();
   // Mean
   const Real sum = std::accumulate(v.begin(), v.end(), 0.0);
   mean = sum / Real(v_size);

   // Standard deviation
   std::vector<DATA_TYPE> diff(v_size);
   //std::transform(v.begin(), v.end(), diff.begin(),
   //               std::bind2nd(std::minus<double>(), mean));
   std::transform(v.begin(), v.end(), diff.begin(), [mean](DATA_TYPE x) { return x - mean; });
   const Real sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
   stdev = std::sqrt(sq_sum / Real(v_size));

   // Median
   median = v[std::floor(v_size/2)];
  }
  
  // Computes mean for a std::vector
  template<class DATA_TYPE>
  void statistics_mean(std::vector<DATA_TYPE> &v, Real &mean)
  {
   // Vector size
   const unsigned v_size = v.size();
   // Mean
   const Real sum = std::accumulate(v.begin(), v.end(), 0.0);
   mean = sum / Real(v_size);
   
  }
  
 }
 
} 

#endif // #ifndef UTILITIES_H
