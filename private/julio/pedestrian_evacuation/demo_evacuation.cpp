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
/// This demo uses the floor field based on the paper Kirchner, Ansgar
/// and Schadschneider, Andreas, Simulation of evacuation processes
/// using a bionics-inspired cellular automaton model for pedestrian
/// dynamics, Physica A, Elsevier, 2002.

#include "../../../src/general/common_includes.h"
#include "../../../src/general/utilities.h"
#include "../../../src/general/initialise.h"


#include "cc_floor_field.h"

// Use the namespace of the framework
using namespace CA;

// Used to define arguments
struct Args {
 argparse::ArgValue<bool> test; // Test (used when only one single configuration is used)
 argparse::ArgValue<Real> rho_min; // Minimum density
 argparse::ArgValue<Real> rho_max; // Maximum density
 argparse::ArgValue<Real> rho_h; // Density step from rho_min to rho_max
 argparse::ArgValue<unsigned> ls; // Lattice size
 argparse::ArgValue<unsigned> nc; // Number of configurations to test
};

//#define OUTPUT_ONE_SINGLE_RUN

int main(int argc, const char** argv)
{
 // Initialise ca
 initialise_ca();
 
 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "Demo evacuation from a rectangular lattice");
 
 // Add arguments
 
 // Optional
 parser.add_argument(args.test, "--test")
  .help("Only one configuration with the minimum density")
  .default_value("false")
  .action(argparse::Action::STORE_TRUE); 
 
 // Optional
 parser.add_argument<Real>(args.rho_min, "--rho_min")
  .help("Minimum density of people (number of people on the lattice)")
  .default_value("0.0");

 // Optional
 parser.add_argument<Real>(args.rho_max, "--rho_max")
  .help("Maximum density of people (number of people on the lattice)")
  .default_value("1.0");

 // Optional
 parser.add_argument<Real>(args.rho_h, "--rho_h")
  .help("Density step until reaching maximum density")
  .default_value("0.1");

 // Optional
 parser.add_argument<unsigned>(args.ls, "--ls")
  .help("Lattice size, consider only squared lattices")
  .default_value("10");

 // Optional
 parser.add_argument<unsigned>(args.nc, "--nc")
  .help("Number of configurations to test")
  .default_value("100");
 
 // Parse the input arguments
 parser.parse_args(argc, argv);
 
 bool test = args.test; // Test
 const Real rho_min = args.rho_min; // Minimum density
 const Real rho_max = args.rho_max; // Maximum density
 const Real rho_h = args.rho_h; // Density step from rho_min to rho_max
 const unsigned lattice_size = args.ls; // Lattice size
 const unsigned n_configurations = args.nc; // Number of configurations to test
 
 // Output for testing/validation
 std::ostringstream output_filename;
 output_filename << "RESLT/output.dat";
 // Output for testing/validation
 std::ofstream output_file((output_filename.str()).c_str(), std::ios_base::out);
 output_file << "rho,"
             << "mean_iterations,"
             << "min_iterations,"
             << "max_iterations" << std::endl;
  //             << "std_dev" << std::endl;
 
 Real current_density = rho_min;
 const unsigned n_steps = ((rho_max-rho_min)/rho_h) + 1;
 // A counter for the density steps
 unsigned i_rho_step = 0;
 
 std::cout << "rho_min: " << rho_min << std::endl;
 std::cout << "rho_max: " << rho_max << std::endl;
 std::cout << "rho_h: " << rho_h << std::endl;
 std::cout << "n_steps: " << n_steps << std::endl;
 
 // Loop over density
 do
  {
   // Averaged iteartions
   Real averaged_iterations = 0;
   // Max iterations for current density
   unsigned max_iterations = 0;
   // Min iterations for current density
   unsigned min_iterations = lattice_size*lattice_size;
   
   for (unsigned i_configuration = 0; i_configuration < n_configurations; i_configuration++)
    {
     // Instantiate a stage
     CCFloorField stage(lattice_size, lattice_size);
     
     const Real static_field_weight = 1.0;
     const Real dynamic_field_weight = 0.0;
     
     stage.k_s() = static_field_weight;
     stage.k_d() = dynamic_field_weight;
     
     // Add emergency exits
     //stage.add_emergency_exit(0, 5);
     
     stage.add_emergency_exit(0, 9);
     stage.add_emergency_exit(0, 10);
     stage.add_emergency_exit(0, 11);
     
     stage.add_emergency_exit(0, 24);
     stage.add_emergency_exit(0, 25);
     stage.add_emergency_exit(0, 26);
     
     stage.add_emergency_exit(0, 39);
     stage.add_emergency_exit(0, 40);
     stage.add_emergency_exit(0, 41);
     
     //stage.add_emergency_exit(0, 5);
     //stage.add_emergency_exit(0, 6);
     //stage.add_emergency_exit(0, 140);
     //stage.add_emergency_exit(149, 49);
     //stage.add_emergency_exit(149, 50);
     //stage.add_emergency_exit(80, 149);
     
     // Initialise stage (with doors and obstacles)
     stage.initialise();
     
     // Add people to the stage
     stage.fill_with_people(current_density);
     
     // Keep track of the number of iterations
     unsigned n_iterations_current_configuration = 0;
     
     std::ostringstream folder_name;
     folder_name << "RESLT/";
     
#ifdef OUTPUT_ONE_SINGLE_RUN
     // ---------------------------------
     // Output initial configuration
     // ---------------------------------
     unsigned index_for_files = 0;
     stage.index_files() = index_for_files;
     
     // Output static field
     stage.output_static_field(folder_name);
     
     // Output dynamic field
     stage.output_dynamic_field(folder_name);
     
     // Output occupancy matrix
     stage.output_occupancy_matrix(folder_name);
     
     // Output obstacle matrix
     stage.output_obstacle_matrix(folder_name);
#endif // #ifdef OUTPUT_ONE_SINGLE_RUN
     
     // Max iterations
     const unsigned allowed_max_iterations = lattice_size*lattice_size;
     
     // Iterate until stage is empty
     while(!stage.is_empty() && n_iterations_current_configuration < allowed_max_iterations)
      {
       // Perform a simulation step
       stage.simulation_step();
       
       // Update the fields
       stage.update();
       
       // Increase the number of iterations
       n_iterations_current_configuration++;
       
#ifdef OUTPUT_ONE_SINGLE_RUN
       // ---------------------------------
       // Output current configuration
       // ---------------------------------
       stage.index_files() = ++index_for_files;
       
       // Output dynamic field
       stage.output_dynamic_field(folder_name);
       
       // Output occupancy matrix
       stage.output_occupancy_matrix(folder_name);
#endif // #ifdef OUTPUT_ONE_SINGLE_RUN
       
      } // while(!stage.empty())
     
     // Add to the sum of iterations
     averaged_iterations+=n_iterations_current_configuration;
     
     // Update the maximum and minimum number of iterations
     if (max_iterations < n_iterations_current_configuration)
      {
       max_iterations = n_iterations_current_configuration;
      }

     if (min_iterations > n_iterations_current_configuration)
      {
       min_iterations = n_iterations_current_configuration;
      }
     
     // If this is test then break the loop
     if (test)
      {
       break;
      }
     
    } // for (i_configuration < n_configurations)
   
   // Compute the average number of iterations
   averaged_iterations=averaged_iterations/(Real)(n_configurations);
   
   // -----------------------------------------------------------------------------------------
   // Output data
   // -----------------------------------------------------------------------------------------
   output_file << current_density << ","
               << averaged_iterations << ","
               << min_iterations << ","
               << max_iterations
               << std::endl;
   
   std::cerr << "rho: " << current_density
             << "\tmeanIt: " << averaged_iterations
             << "\tminIt: " << min_iterations
             << "\tmaxIt: " << max_iterations
             << std::endl;
   
   // Increase density
   current_density+=rho_h;
   
   // Increase the density counter step
   i_rho_step++;
   
  } while (i_rho_step < n_steps && !test);
 
 // Finalise ca
 finalise_ca();
 
 return 0;
 
}

