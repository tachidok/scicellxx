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
#include "../../src/scicellxx.h"

// Use the namespace of the framework
using namespace scicellxx;

struct Args {
 argparse::ArgValue<bool> boolean_argument;
 argparse::ArgValue<std::string> string_argument;
 argparse::ArgValue<size_t> verbosity;
 argparse::ArgValue<Real> real_argument;
 argparse::ArgValue<std::vector<Real> > vector_real_arguments_one_or_more;
 argparse::ArgValue<std::vector<Real> > vector_real_arguments_none_or_more;
};

int main(int argc, const char** argv)
{
 // Initialise scicellxx
 initialise_scicellxx();
 
 // Output for testing/validation
 std::ofstream output_test("output_test.dat", std::ios_base::out);
 
 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "Description of application");
 
 // Add arguments
 
 // Positional
 parser.add_argument(args.string_argument, "string_argument")
  .help("File to process");
 
 // Optional
 parser.add_argument(args.boolean_argument, "--boolean_argument")
  .help("Boolean argument")
  .default_value("false")
  .action(argparse::Action::STORE_TRUE); 
 
 // Note the use of choices values
 parser.add_argument(args.verbosity, "--verbosity", "-v")
  .help("Sets the verbosity")
  .default_value("1")
  .choices({"0", "1", "2"});
 
 parser.add_argument<Real>(args.real_argument, "--real_argument")
  .help("Sets Real argument")
  .default_value("1.0");
 
 // One or more real arguments with default values
 parser.add_argument(args.vector_real_arguments_one_or_more, "--vector_real_arguments_one_or_more")
  .help("One or more Real values")
  .nargs('+')
  .default_value({"1.0", "0.2"});
 
 // None or more real arguments
 parser.add_argument(args.vector_real_arguments_none_or_more, "--vector_real_arguments_none_or_more")
  .help("Zero or more float values")
  .nargs('*')
  .default_value({});
 
 // Parse the input arguments
 parser.parse_args(argc, argv);
 
 //Show the arguments
 std::cout << "args.string_argument: " << args.string_argument << "\n";
 std::cout << "args.boolean_argument: " << args.boolean_argument << "\n";
 std::cout << "args.verbosity: " << args.verbosity << "\n";
 std::cout << "args.real_argument: " << args.real_argument << "\n";
 std::cout << "args.vector_real_arguments_one_or_more: " << argparse::join(args.vector_real_arguments_one_or_more.value(), ", ") << "\n";
 std::cout << "args.vector_real_arguments_none_or_more: " << argparse::join(args.vector_real_arguments_none_or_more.value(), ", ") << "\n";
 std::cout << "\n";

 output_test << "args.string_argument: " << args.string_argument << "\n";
 output_test << "args.boolean_argument: " << args.boolean_argument << "\n";
 output_test << "args.verbosity: " << args.verbosity << "\n";
 output_test << "args.real_argument: " << args.real_argument << "\n";
 output_test << "args.vector_real_arguments_one_or_more: " << argparse::join(args.vector_real_arguments_one_or_more.value(), ", ") << "\n";
 output_test << "args.vector_real_arguments_none_or_more: " << argparse::join(args.vector_real_arguments_none_or_more.value(), ", ") << "\n";
 
 // Close the output for test
 output_test.close();
  
 // Finalise scicellxx
 finalise_scicellxx();
 
 return 0;
 
}

