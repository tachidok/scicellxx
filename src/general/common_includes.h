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
#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H


// C headers

// ====================================================================
// Utilities libraries
// ====================================================================
#include <sys/types.h> // Folder creation
#include <sys/stat.h> // Folder creation

// ====================================================================
// Check this page for more information about the included libraries
// 
// http://en.cppreference.com/w/cpp/header
// 
// ====================================================================

// C++ headers

// ====================================================================
// Utilities library
// ====================================================================
#include <typeinfo> // 	Runtime type information utilities
#include <iomanip> // Output format
#include <utility> // Various utility components
#include <ctime> // C-style time/date utilites
// #include <chrono> // (since C++11) C++ time utilites

// ====================================================================
// Utilities library / Numeric limits
// ====================================================================
#include <climits>
#include <cfloat>
#include <limits>

// ====================================================================
// Utilities library / Error handling
// ====================================================================
#include <exception> // Exception handling utilities
#include <stdexcept> // Standard exception objects
#include <cassert> // Conditionally compiled macro that compares its
		   // argument to zero
//#include <system_error> // (since C++11) defines std::error_code, a
			// platform-dependent error code
#include <cerrno> // Macro containing the last error number

// ====================================================================
// String library
// ====================================================================
#include <cstring> // various narrow character string handling functions
#include <string>  // library to handle strings, implements std::string

// ====================================================================
// Containers library
// ====================================================================
//#include <array> // (since C++11) std::array container
#include <vector> // std::vector container
#include <list> // std::list container
#include <set> // std::set container
#include <map> // std::map container

// ====================================================================
// Algorithms library
// ====================================================================
#include <algorithm> // Algorithms that operate on containers

// ====================================================================
// Numerics library
// ====================================================================
#include <cmath>

// ====================================================================
// Random numbers library
// ====================================================================
#include <random>

// ====================================================================
// Input/output library
// ====================================================================
#include <iostream> // several standard stream objects
#include <fstream> // functions for inputting from and outputting to files
#include <sstream>

// Include ArgParse
#include "../../external_src/argparse/src/argparse.hpp"

// MPI support
#ifdef SCICELLXX_USES_MPI
#include <mpi.h>
#endif // #ifdef SCICELLXX_USES_MPI

// Own scicellxx definitions
#include "definitions.h"
#include "cmake_definitions.h"

#endif // #ifndef COMMON_INCLUDES_H
