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
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

namespace scicellxx
{

#ifdef TYPEDEF_REAL_IS_DOUBLE
 typedef double Real;
#ifdef SCICELLXX_USES_MPI
#define MPI_SC_REAL MPI_DOUBLE
#endif // #ifdef SCICELLXX_USES_MPI
#else
 typedef float Real;
#ifdef SCICELLXX_USES_MPI
#define MPI_SC_REAL MPI_FLOAT
#endif // #ifdef SCICELLXX_USES_MPI
#endif // #ifdef TYPEDEF_REAL_IS_DOUBLE

#ifdef SCICELLXX_USES_MPI
#define MPI_RANK_NPROCS_PRINT(RANK,NPROCS) "["<<RANK<<"/"<<NPROCS<<"]: "
#endif // #ifdef SCICELLXX_USES_MPI
 
#ifdef SCICELLXX_PANIC_MODE
 // =======================================================================
 /// Macros for debugging
 // =======================================================================
#define DEB(x) std::cerr<<"DEB ["<<#x<<"] = "<<x<<std::endl<<std::flush;
#define DEB2(x,y) std::cerr<<"DEB ["<<x<<" "<<y<<"]"<<std::endl<<std::flush;
#define DEB3(x,y,z) std::cerr<<"DEB ["<<x<<" "<<y<<" "<<z<<"]"<<std::endl<<std::flush;
#define DEB4(t,x,y,z) std::cerr<<"DEB ["<<t<<" "<<x<<" "<<y<<" "<<z<<"]"<<std::endl<<std::flush;
#define DEB_TO_FILE(F,x) F<<"["<<#x<<"] = "<<x<<std::endl<<std::flush;
#define DEB_TO_FILE1(F,x) F<<x<<std::endl<<std::flush;
#define DEB_TO_FILE2(F,x,y) F<<x<<" "<<y<<std::endl<<std::flush;
#define DEB_TO_FILE3(F,x,y,z) F<<x<<" "<<y<<" "<<z<<std::endl<<std::flush;
#define DEB_TO_FILE4(F,t,x,y,z) F<<t<<" "<<x<<" "<<y<<" "<<z<<std::endl<<std::flush;
#endif // #ifdef SCICELLXX_PANIC_MODE
 
 // =======================================================================
 /// Macros to transform from degrees to radians and vice versa
 // =======================================================================
#define TO_RADIANS (M_PI/180.0)
#define TO_DEGREES (180.0/M_PI)
 
 // =======================================================================
 /// The macros to deal with strings for error messages are taken from
 /// oomph-lib
 // =======================================================================

 ///Pre-processor magic for error reporting
 ///Macro that converts argument to string
#define SCICELLXX_MAKE_STRING(x) #x

 ///Macro wrapper to SCICELLXX_MAKE_STRING, required because calling
 ///SCICELLXX_MAKE_STRING(__LINE__) directly returns __LINE__
 ///i.e. the conversion of __LINE__ into a number must be performed before
 ///its conversion into a string
#define SCICELLXX_TO_STRING(x) SCICELLXX_MAKE_STRING(x)

 ///Combine the FILE and LINE built-in macros into a string that can
 ///be used in error messages.
#define SCICELLXX_EXCEPTION_LOCATION __FILE__ ":" SCICELLXX_TO_STRING(__LINE__)

 /// Get the current function name. All the mess is due to different
 /// compilers naming the macro we need differently.
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
# define SCICELLXX_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define SCICELLXX_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__FUNCSIG__)
# define SCICELLXX_CURRENT_FUNCTION __FUNCSIG__

#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define SCICELLXX_CURRENT_FUNCTION __FUNCTION__

#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define SCICELLXX_CURRENT_FUNCTION __FUNC__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define SCICELLXX_CURRENT_FUNCTION __func__

#else
# define SCICELLXX_CURRENT_FUNCTION "[Unknown function -- unrecognised compiler]"

#endif

} // namespace scicellxx
 
#endif // #ifdef DEFINITIONS_H

