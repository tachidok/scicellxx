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

// Include SciCell++ libraries
#include "../../../src/scicellxx.h"

// Include mTASEP algorithm
//#include "cc_mTASEP.h"

// Use the namespace of the framework
using namespace scicellxx;

// Used to define arguments
struct Args {
 argparse::ArgValue<unsigned> L;
 argparse::ArgValue<unsigned> N;
 argparse::ArgValue<Real> alpha_min;
 argparse::ArgValue<Real> alpha_max;
 argparse::ArgValue<Real> alpha_step;
 argparse::ArgValue<unsigned> alpha_n_points;
 argparse::ArgValue<Real> beta_min;
 argparse::ArgValue<Real> beta_max;
 argparse::ArgValue<Real> beta_step;
 argparse::ArgValue<unsigned> beta_n_points;
 argparse::ArgValue<Real> rho_min;
 argparse::ArgValue<Real> rho_max;
 argparse::ArgValue<Real> rho_step;
 argparse::ArgValue<unsigned> rho_n_points;
 argparse::ArgValue<Real> omega_in_min;
 argparse::ArgValue<Real> omega_in_max;
 argparse::ArgValue<Real> omega_in_step;
 argparse::ArgValue<unsigned> omega_in_n_points;
 argparse::ArgValue<Real> omega_out_min;
 argparse::ArgValue<Real> omega_out_max;
 argparse::ArgValue<Real> omega_out_step;
 argparse::ArgValue<unsigned> omega_out_n_points;
 argparse::ArgValue<unsigned> lateral_movement;
 argparse::ArgValue<unsigned> max_experiments;
 argparse::ArgValue<unsigned> max_simulations_per_experiment;
 argparse::ArgValue<unsigned> simulation_step_to_start_gathering_data;
 argparse::ArgValue<std::string> root_output_folder;
 argparse::ArgValue<unsigned> output_space_state_diagram;
 argparse::ArgValue<unsigned> output_microtubule_state;
};

// Output parameters to a file
void output_parameters_to_file(std::string &filename, const int argc, const char **argv,  struct Args &args)
{
  // Output file
  std::ofstream output_parameters(filename, std::ios_base::out);
  
  // Write the command line as a comment into the parameters file
  output_parameters << "#";
  for (int i = 0; i < argc-1; i++)
   {
    output_parameters << argv[i] << " ";
   }
  output_parameters << argv[argc-1] << std::endl;

  const unsigned precision_real_values = 4;
  
  output_parameters << "MPI_CORES:" << SciCellxxMPI::nprocs << std::endl;
  output_parameters << "L:" << args.L << std::endl;
  output_parameters << "N:" << args.N << std::endl;
  output_parameters << "alpha_min:" << setprecision(precision_real_values) << args.alpha_min << std::endl;
  output_parameters << "alpha_max:" << setprecision(precision_real_values) << args.alpha_max << std::endl;
  output_parameters << "alpha_step:" << setprecision(precision_real_values) << args.alpha_step << std::endl;
  output_parameters << "alpha_n_points:" << args.alpha_n_points << std::endl;
  output_parameters << "beta_min:" << setprecision(precision_real_values) << args.beta_min << std::endl;
  output_parameters << "beta_max:" << setprecision(precision_real_values) << args.beta_max << std::endl;
  output_parameters << "beta_step:" << setprecision(precision_real_values) << args.beta_step << std::endl;
  output_parameters << "beta_n_points:" << args.beta_n_points << std::endl;
  output_parameters << "rho_min:" << setprecision(precision_real_values) << args.rho_min << std::endl;
  output_parameters << "rho_max:" << setprecision(precision_real_values) << args.rho_max << std::endl;
  output_parameters << "rho_step:" << setprecision(precision_real_values) << args.rho_step << std::endl;
  output_parameters << "rho_n_points:" << args.rho_n_points << std::endl;
  output_parameters << "omega_in_min:" << setprecision(precision_real_values) << args.omega_in_min << std::endl;
  output_parameters << "omega_in_max:" << setprecision(precision_real_values) << args.omega_in_max << std::endl;
  output_parameters << "omega_in_step:" << setprecision(precision_real_values) << args.omega_in_step << std::endl;
  output_parameters << "omega_in_n_points:" << args.omega_in_n_points << std::endl;
  output_parameters << "omega_out_min:" << setprecision(precision_real_values) << args.omega_out_min << std::endl;
  output_parameters << "omega_out_max:" << setprecision(precision_real_values) << args.omega_out_max << std::endl;
  output_parameters << "omega_out_step:" << setprecision(precision_real_values) << args.omega_out_step << std::endl;
  output_parameters << "omega_out_n_points:" << args.omega_out_n_points << std::endl;
  output_parameters << "lateral_movement:" << args.lateral_movement << std::endl;
  output_parameters << "max_experiments:" << args.max_experiments << std::endl;
  output_parameters << "max_simulations_per_experiment:" << args.max_simulations_per_experiment << std::endl;
  output_parameters << "simulation_step_to_start_gathering_data:" << args.simulation_step_to_start_gathering_data << std::endl;
  output_parameters << "root_output_folder:" << args.root_output_folder << std::endl;
  output_parameters << "output_space_state_diagram:" << args.output_space_state_diagram << std::endl;
  output_parameters << "output_microtubule_state:" << args.output_microtubule_state << std::endl;
  
  // Close the parameters file
  output_parameters.close(); 
}

/// Computes the mean channels density (the mean of the density
/// along all channels/for each column)
std::vector<Real> compute_mean_channels_density(bool **m, const unsigned N, const unsigned L)
{
 /*
   Computes the mean channels density of the microtubule
   (cross-sectional density/along all channels/for each column)
 */

 // The vector storing the mean density (initialised with zeroes)
 std::vector<Real> density(L, 0);
 
 // Move along the microtubule and compute the mean density
 for (unsigned i = 0; i < L; i++)
  {
   Real sum_occupation = 0.0;
   for (unsigned k = 0; k < N; k++)
    {
     sum_occupation+=m[k][i];
    }
   // Mean of sum density
   density[i] = sum_occupation / Real(N);
  }

 return density;
 
}

/// Tries to perform a lateral movement on the particule at position
/// (k, i) in the microtubule
void try_lateral_movement(bool **m, const unsigned N, const unsigned L, const unsigned k, const unsigned i)
{
 /*
   Performs lateral movement on a TASEP model, this check for up/down
   possibilities, does not accounts for diagonal movement
 */
 
 // Double-check there is a particle at the current position
 if (m[k][i] == 0)
  {
   return;
  }

 // Store the indices for the microtubule above and below
 unsigned index_microtubule_above = k - 1;
 unsigned index_microtubule_below = k + 1;
 
 // Correct the indexes for the microtubules above and below
 // (periodic-boundary conditions)
 if (k == 0)
  {
   index_microtubule_above = N - 1;
  }
 
 if (k == N-1)
  {
   index_microtubule_below = 0;
  }
 
 // Used to get a seed for the random number engine
 std::random_device rd;
 // Standard mersenne_twister_engine seeded with rd()
 std::mt19937 gen(rd());
 
 // Use dist to generate a random number into a Real in the range
 // [0,1)
 std::uniform_real_distribution<> dis(0.0, 1.0);
 
 // Generate a random number to choose between the above or below
 // microtubule
 const Real r = dis(gen);
 
 // First choose the microtubule from above (preferred due to probability)
 if (r <= 0.5)
  {
   // If the space at the above microtubule is free then move there!
   if (m[index_microtubule_above][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_above][i] = true;
    }
   // If the space at the below microtubule is free then move there!
   else if (m[index_microtubule_below][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_below][i] = true;
    }
  } // if (r <= 0.5)
 else // First choose the microtubule from below (preferred due to
      // probability)
  {
   if (m[index_microtubule_below][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_below][i] = true;
    }
   else if (m[index_microtubule_above][i] == false)
    {
     m[k][i] = false;
     m[index_microtubule_above][i] = true;
    }
  } // else if (r <= 0.5)
 
}

/// Perform mTASEP method
void mTASEP(bool **m, const unsigned N, const unsigned L,
            const Real alpha, const Real beta, const Real rho,
            const Real omega_in, const Real omega_out,
            bool lateral_movement, Real &mean_current)
{
 /*
    Applies TASEP algorithm to a multichannel microtubule

    Return the updated microtubule as an numpy matrix

    This version implements the "(d) parallel_update" strategy as
    described in section 2.1 of "The Asymmetric Exclusion Process:
    Comparison of Update Procedures, N. Rajewsky et. al., Journal of
    Statistical Physics, Vol. 92, 1998"
 */
 
 // Used to get a seed for the random number engine
 std::random_device rd;
 // Standard mersenne_twister_engine seeded with rd()
 std::mt19937 gen(rd());
 
 // Use dist to generate a random number into a Real in the range
 // [0,1)
 std::uniform_real_distribution<> dis(0.0, 1.0);
 
 // Used to generate a random position in the microtubule (including
 // the first and the last position)
 std::uniform_int_distribution<> dis_microtubule_size(0, L-1);
 
 // Compute the current for each channel on the microtubule
 std::vector<unsigned> mean_current_per_channel(N, 0);
 
 // Perform the method for each channel
 for (unsigned k = 0; k < N; k++)
  {
   // *******************************************
   // Apply TASEP-LK rules
   // *******************************************
   
   // -------------------------------------------
   /// Attach (omega in)
   // -------------------------------------------
   
   // Keep track of the position of the particle attached by omega_in
   // because the just attached particle cannot move
   bool omega_in_attached_a_particle = false;
   unsigned r_pos_omega_in = 0;
   
   if (omega_in > 0.0)
    {
     // Choose a random position at the microtubule
     const unsigned r_pos = dis_microtubule_size(gen);
     
     // Generate a random number
     const Real r = dis(gen);
     
     // if r <= omega_in and m[k][r_pos] == 0 then add a particle to
     // the microtubule in that position
     if (r <= omega_in && m[k][r_pos] == 0)
      {
       // Attach a particle
       m[k][r_pos] = 1;
       
       // Keep track of the position of the particle attached by omega
       // in
       r_pos_omega_in = r_pos;
       omega_in_attached_a_particle = true;
      }
    }
   
   // -------------------------------------------
   /// Detach (omega out)
   // -------------------------------------------
   
   // Keep track of the position free by omega_out becasue this
   // position cannot be occupied by other particles in the current
   // simulation step
   bool omega_out_dettached_a_particle = false;
   unsigned r_pos_omega_out = 0;
   
   if (omega_out > 0.0)
    {
     // Choose a random position at the microtubule
     const unsigned r_pos = dis_microtubule_size(gen);
     
     // Generate a random number
     const Real r = dis(gen);
     
     // if r <= omega_out and m[k][r_pos] == 1 then remove the
     // particle from that position of the microtubule ALSO check that
     // the particle is not there becasue it was just attached by the
     // omega_in process
     if (r <= omega_out && m[k][r_pos] == 1 && !(omega_in_attached_a_particle && r_pos == r_pos_omega_in))
      {
       // Detach the particle
       m[k][r_pos] = 0;
       
       r_pos_omega_out = r_pos;
       omega_out_dettached_a_particle = true;
       
      }
     
    }
   
   // *******************************************
   // Update the boundaries (left and right)
   // *******************************************
   
   // -------------------------------------------
   /// LEFT END
   // -------------------------------------------
   
   // Compute a probability to add a particle at the beginning of the
   // microtubule
   const Real a = dis(gen);
   // A flag indicating whether a particle was added at the beginning
   // of the microtubule
   bool added_to_start = false;
   // Is a <= alpha and the first space is free? ALSO check whether
   // the space is free due to the dettaching process by omega_out, if
   // that is the case then we cannot add a particle to the start
   if (a <= alpha && m[k][0] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == 0))
    {
     // Add a particle to the start
     m[k][0] = 1;
     // Indicate we added a particle at the beginning of the
     // microtubule so there is no need to update its position
     added_to_start = true;
     
    }
   
   // -------------------------------------------
   /// RIGHT END
   // -------------------------------------------
   
   // Compute a probability to remove the last particle of the
   // microtubule
   const Real b = dis(gen);
   // A flag indicating whether a particle was removed from the last
   // cell of the microtubule
   bool removed_from_end = false;
   // Is b <= beta and the last space occupied, ALSO, was the particle
   // not attached by omega_in?
   if (b <= beta && m[k][L-1] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == L-1))
    {
     // Remove the particle from the microtubule
     m[k][L-1] = 0;
     // Indicate we removed a particle from the last position of the
     // microtubule so other particle should not step in this cell
     removed_from_end = true;
     
    }
   
   // *******************************************
   // Update particles in the microtubule
   // *******************************************
   
   // Update particles in the microtubule using a parallel update
   // strategy (delay movement, but keep track in a list of those
   // particles to update position)
   
   // -----------------------------------------------------------
   
   // Update the first and last indexes based on the added_to_start
   // and removed_from_end flags
   unsigned end_index = L - 2;
   if (removed_from_end)
    {
     end_index = L - 3;     
    }
   
   unsigned start_index = 0;
   if (added_to_start)
    {
     start_index = 1;
    }
   
   // Vector with particles' indexes that will move (step-forward)
   std::vector<unsigned> step_forward_particles_list;
   step_forward_particles_list.reserve(L);
      
   // Vector with particles' indexes that will try lateral movement
   std::vector<unsigned> step_lateral_particles_list;
   step_lateral_particles_list.reserve(L);
   
   // Update the microtubule from left to right
   for (unsigned i = start_index; i <= end_index; i++)
    {
     // Check whether there is a particle at the current cell space
     // and ensure that particle was not introduced by the omega_in
     // process
     if (m[k][i] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == i))
      {
       // Check whether there is a free space at the next cell space
       // and ensure that space is not there becase the omega_out
       // process
       if (m[k][i+1] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == i+1))
        {
         // Compute a probability to move to the next space
         const Real p = dis(gen);
         if (p <= rho)
          {
           // Add the particle index to the step forward list
           step_forward_particles_list.push_back(i);
          }

         // Skip the cell at the next space since there is no particle
         // there
         ++i;
         
        } // if (m[k][i+1] == 0)
       else
        {
         // Try lateral movement since there is a particle on the next
         // cell OR the particle on the next cell was just removed by
         // the omega_out process
         step_lateral_particles_list.push_back(i);
         
        } // else if (m[k][i+1] == 0 && !(omega_out_dettached_a_particle && r_pos_omega_out == i+1))
       
      } // if (m[k][i] == 1 && !(omega_in_attached_a_particle && r_pos_omega_in == i))

    } // for (i <= end_index)
        
   // Perform movement of particles based on the step forward list
   // Cache the size of the list
   const unsigned step_forward_particle_list_size = step_forward_particles_list.size();
   for (unsigned j = 0; j < step_forward_particle_list_size; j++)
    {
     // Get the index of the particle
     const unsigned i = step_forward_particles_list[j];
     m[k][i] = 0;
     m[k][i+1] = 1;
     // Add the movement to the current per channel vector
     mean_current_per_channel[k]+=1;
    }
   
   // *******************************************
   // Apply lateral movement
   // *******************************************
   if (lateral_movement)
    {
     const unsigned step_lateral_particle_list_size = step_lateral_particles_list.size();
     for (unsigned j = 0; j < step_lateral_particle_list_size; j++)
      {
       // Get the index of the particle
       const unsigned i = step_lateral_particles_list[j];
       try_lateral_movement(m, N, L, k, i);
      }
     
    }
   
  } // for (k < N)
 
 // Compute the averaged current on all the microtubules
 mean_current = 0.0;
 for (unsigned i = 0; i < N; i++)
  {
   mean_current+=mean_current_per_channel[i];
  }
 
 const Real factor = 1.0/static_cast<Real>(L*N);
 
 mean_current=mean_current*factor;
 
}

/// Output boolean matrix into a csv file
void real_matrix_to_csv_file(std::vector<std::vector<Real> > m, const unsigned nrows, const unsigned ncolumns, std::string &file_name)
{
 // Create file
 std::ofstream output_file(file_name, std::ios_base::out);

 /*
 const unsigned precision_real_values = 8;
 std::ostringstream ss;
 ss << setprecision(precision_real_values);
 */
 
 for (unsigned i = 0; i < nrows; i++)
  {
   for (unsigned j = 0; j < ncolumns-1; j++)
    {
     output_file << m[i][j] << ",";
    }
   // The last element without the ','
   output_file << m[i][ncolumns-1] << std::endl;
  }
 
 // Close the file
 output_file.close();
 
} // real_matrix_to_csv_file

/// Output boolean matrix into a csv file
void boolean_matrix_to_csv_file(bool **m, const unsigned nrows, const unsigned ncolumns, std::string &file_name)
{
 // Create file
 std::ofstream output_file(file_name, std::ios_base::out);
 
 for (unsigned i = 0; i < nrows; i++)
  {
   for (unsigned j = 0; j < ncolumns-1; j++)
    {
     output_file << m[i][j] << ",";
    }
   // The last element without the ','
   output_file << m[i][ncolumns-1] << std::endl;
  }
 
 // Close the file
 output_file.close();
 
} // boolean_matrix_to_csv_file

int main(int argc, const char** argv)
{
 // Initialise scicellxx
 initialise_scicellxx();
 
 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "mtasep-LK model for a multichannel microtubule with parallel update");
 
 // Add arguments
 
 // Optional
 parser.add_argument<unsigned>(args.L, "--L")
  .help("Size of the microtubule")
  .default_value("101");
 
 parser.add_argument<unsigned>(args.N, "--N")
  .help("Number of channels on the microtubule")
  .default_value("4");
 
 parser.add_argument<Real>(args.alpha_min, "--alpha_min")
  .help("The minimum probability to introduce a particle to the microtubule (at the first --left-- position of the microtubule)")
  .default_value("0.1");
 
 parser.add_argument<Real>(args.alpha_max, "--alpha_max")
  .help("The maximum probability to introduce a particle to the microtubule (at the first --left-- position of the microtubule)")
  .default_value("1.0");

 parser.add_argument<Real>(args.alpha_step, "--alpha_step")
  .help("The distance between alpha values")
  .default_value("0.1");
 
 parser.add_argument<unsigned>(args.alpha_n_points, "--alpha_n_points")
  .help("The number of points in the range alpha_min to alpha_max")
  .default_value("10");

 parser.add_argument<Real>(args.beta_min, "--beta_min")
  .help("The minimum probability to remove a particle from the microtubule (from the last --right-- position of the microtubule)")
  .default_value("0.1");

 parser.add_argument<Real>(args.beta_max, "--beta_max")
  .help("The maximum probability to remove a particle from the microtubule (from the last --right-- position of the microtubule)")
  .default_value("1.0");

 parser.add_argument<Real>(args.beta_step, "--beta_step")
  .help("The distance between beta values")
  .default_value("0.1");
 
 parser.add_argument<unsigned>(args.beta_n_points, "--beta_n_points")
  .help("The number of points in the range beta_min to beta_max")
  .default_value("10");
 
 parser.add_argument<Real>(args.rho_min, "--rho_min")
  .help("The minimum probability for a particle to step-forward")
  .default_value("0.8");
 
 parser.add_argument<Real>(args.rho_max, "--rho_max")
  .help("The maximum probability for a particle to step-forward")
  .default_value("1.0");

 parser.add_argument<Real>(args.rho_step, "--rho_step")
  .help("The distance between rho values")
  .default_value("0.1");
 
 parser.add_argument<unsigned>(args.rho_n_points, "--rho_n_points")
  .help("The number of points in the range rho_min to rho_max")
  .default_value("3");
 
 parser.add_argument<Real>(args.omega_in_min, "--omega_in_min")
  .help("The minimum probability for a particle to attach to a site in the microtubule")
  .default_value("0.0");
 
 parser.add_argument<Real>(args.omega_in_max, "--omega_in_max")
  .help("The maximum probability for a particle to attach to a site in the microtubule")
  .default_value("0.0");

 parser.add_argument<Real>(args.omega_in_step, "--omega_in_step")
  .help("The distance between omega_in values")
  .default_value("0.0");
 
 parser.add_argument<unsigned>(args.omega_in_n_points, "--omega_in_n_points")
  .help("The number of points in the range omega_in_min to omega_in_max")
  .default_value("1");
 
 parser.add_argument<Real>(args.omega_out_min, "--omega_out_min")
  .help("The minimum probability for a particle to detach from a site in the microtubule")
  .default_value("0.0");
 
 parser.add_argument<Real>(args.omega_out_max, "--omega_out_max")
  .help("The maximum probability for a particle to detach from a site in the microtubule")
  .default_value("0.0");

 parser.add_argument<Real>(args.omega_out_step, "--omega_out_step")
  .help("The distance between omega_out values")
  .default_value("0.0");
 
 parser.add_argument<unsigned>(args.omega_out_n_points, "--omega_out_n_points")
  .help("The number of points in the range omega_out_min to omega_out_max")
  .default_value("1");
 
 parser.add_argument<unsigned>(args.lateral_movement, "--lateral_movement")
  .help("Enables/disables lateral movement")
  .default_value("0");

 parser.add_argument<unsigned>(args.max_experiments, "--max_experiments")
  .help("Set the maximum number of experiments to perform")
  .default_value("1");
 
 parser.add_argument<unsigned>(args.max_simulations_per_experiment, "--max_simulations_per_experiment")
  .help("Set the maximum number of simulations per experiments")
  .default_value("200");
 
 parser.add_argument<unsigned>(args.simulation_step_to_start_gathering_data, "--simulation_step_to_start_gathering_data")
  .help("Set the simulation step to start gathering data (must be smaller than max_simulations_per_experiment)")
  .default_value("0");

 parser.add_argument<std::string>(args.root_output_folder, "--root_output_folder")
  .help("The root output folder")
  .default_value("RESLT");
 
 parser.add_argument<unsigned>(args.output_space_state_diagram, "--output_space_state_diagram")
  .help("Enables/disables output of the averaged space-time diagrams for all channels. Disable when performing large simulations")
  .default_value("0");
 
  parser.add_argument<unsigned>(args.output_microtubule_state, "--output_microtubule_state")
  .help("Enables/disables output of microtubule state (space-time diagrams for all channels). Disable when performing large simulations")
  .default_value("0");
  
  // Parse the input arguments
  parser.parse_args(argc, argv);

  // Configure problem
  const unsigned L = args.L; // Size of the microtubule
  const unsigned N = args.N; // Number of channels of the microtubule
  const Real alpha_min = args.alpha_min;
  const Real alpha_max = args.alpha_max;
  const Real alpha_step = args.alpha_step;
  const unsigned alpha_n_points = args.alpha_n_points;
  const Real beta_min = args.beta_min;
  const Real beta_max = args.beta_max;
  const Real beta_step = args.beta_step;
  const unsigned beta_n_points = args.beta_n_points;
  const Real rho_min = args.rho_min;
  const Real rho_max = args.rho_max;
  const Real rho_step = args.rho_step;
  const unsigned rho_n_points = args.rho_n_points;
  const Real omega_in_min = args.omega_in_min;
  const Real omega_in_max = args.omega_in_max;
  const Real omega_in_step = args.omega_in_step;
  const unsigned omega_in_n_points = args.omega_in_n_points;
  const Real omega_out_min = args.omega_out_min;
  const Real omega_out_max = args.omega_out_max;
  const Real omega_out_step = args.omega_out_step;
  const unsigned omega_out_n_points = args.omega_out_n_points;
  bool lateral_movement = false;
  if (args.lateral_movement)
   {
    lateral_movement = true;
   }
  const unsigned max_experiments = args.max_experiments;
  const unsigned max_simulations_per_experiment = args.max_simulations_per_experiment;
  const unsigned simulation_step_to_start_gathering_data = args.simulation_step_to_start_gathering_data;
  std::string root_output_folder(args.root_output_folder);
  bool output_space_state_diagram = false;
  if (args.output_space_state_diagram)
   {
    output_space_state_diagram = true;
   }
  bool output_microtubule_state = false;
  if (args.output_microtubule_state)
   {
    output_microtubule_state = true;
   }

  // Validate parameters values
  if (simulation_step_to_start_gathering_data > max_simulations_per_experiment)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "There step number to start gathering data is greater than\n"
                  << "the maximum number of simulations per experiment\n"
                  << "simulation_step_to_start_gathering_data:" << simulation_step_to_start_gathering_data
                  << "max_simulations_per_experiment:" << max_simulations_per_experiment
                  << std::endl;
    throw SciCellxxLibError(error_message.str(),
                            SCICELLXX_CURRENT_FUNCTION,
                            SCICELLXX_EXCEPTION_LOCATION);
   }

  // If the output_space_state_diagram is ENABLED check whether the
  // max_simulations_per_experiment does not exceed a MAXIMUM value
  if (output_space_state_diagram && max_simulations_per_experiment > 1000)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You enabled the output_space_state_diagram but the\n"
                  << "max_simulations_per_experiment exceeds the MAXIMUM value (1,000).\n"
                  << "This may lead to memory issues due to this large memory requirement.\n"
                  << "You can DISABLED this error check if you are sure on what you are doing!\n"
                  << std::endl;
    throw SciCellxxLibError(error_message.str(),
                            SCICELLXX_CURRENT_FUNCTION,
                            SCICELLXX_EXCEPTION_LOCATION);
   }
  
  // Output formating (files names, folders names and output to files)
  const unsigned width_number = 5;
  const char fill_char = '0';
  const unsigned precision_real_values = 4;

  // Thgis throws an error when using MPI since multile cores try to
  // create the same output folder
#ifndef SCICELLXX_USES_MPI
  // Create output directory
  SciCellxxFileSystem::create_directory(root_output_folder);
#endif // #ifdef SCICELLXX_USES_MPI
  
  // The string stream for the rank (used on output filenames)
  std::ostringstream ss_rank;
  ss_rank << SciCellxxMPI::rank;
  
  // Output parameters to a file
  std::string parameters_filename(root_output_folder + "/parameters_r" + ss_rank.str() + ".txt");
  output_parameters_to_file(parameters_filename, argc, argv, args);
  
  // ------------------------------------------------------------
  // Generate all configurations as the cartesian product of the
  // ranges of parameters
  // ------------------------------------------------------------

  // Create the "set/list" with all values for each parameter

  // Alphas
  std::vector<Real> alphas;
  SciCellxxLinearSpace::create_linear_space(alphas, alpha_min, alpha_max, alpha_step, alpha_n_points);
  // Betas
  std::vector<Real> betas;
  SciCellxxLinearSpace::create_linear_space(betas, beta_min, beta_max, beta_step, beta_n_points);
  
  // Rhos
  std::vector<Real> rhos;
  SciCellxxLinearSpace::create_linear_space(rhos, rho_min, rho_max, rho_step, rho_n_points);
  
  // Omegas_in
  std::vector<Real> omegas_in;
  SciCellxxLinearSpace::create_linear_space(omegas_in, omega_in_min, omega_in_max, omega_in_step, omega_in_n_points);
  
  // Omegas_out
  std::vector<Real> omegas_out;
  SciCellxxLinearSpace::create_linear_space(omegas_out, omega_out_min, omega_out_max, omega_out_step, omega_out_n_points);
  
  // Print the information only on the master core
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    scicellxx_output << "Linear spaces:" << std::endl;
    scicellxx_output << "Alphas:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(alphas);
    scicellxx_output << "Betas:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(betas);
    scicellxx_output << "Rhos:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(rhos);
    scicellxx_output << "Omegas_in:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(omegas_in);
    scicellxx_output << "Omegas_out:" << std::endl;
    SciCellxxLinearSpace::print_linear_space<Real>(omegas_out);
    scicellxx_output << std::endl;
   }
  
  // Create the list with the list of parameter values
  std::vector<std::vector<Real> > lists;
  lists.push_back(alphas);
  lists.push_back(betas);
  lists.push_back(rhos);
  lists.push_back(omegas_in);
  lists.push_back(omegas_out);

  // Print the information only on the master core
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    scicellxx_output << "Computing cartesian product ..." << std::endl;
   }
  // Perform cartesian product
  std::vector<std::vector<Real> > configurations = SciCellxxCartesianProduct::product(lists);
  
  // Print the information only on the master core
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    scicellxx_output << "Computing cartesian product [DONE]" << std::endl;
   }
  
  // Get the total number of configurations
  const unsigned n_all_configurations = configurations.size();

  // Print the information only on the master core
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    // Report the total number of configurations and the partitioning
    // for parallel computing
    scicellxx_output << "Total number of all configurations: " << n_all_configurations << std::endl;
    scicellxx_output << "Number of cores: " << SciCellxxMPI::nprocs << std::endl;
   }

  // Print the information only on the master core
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    // Only print the Cartesian product if PANIC mode is enabled
#ifdef SCICELLXX_PANIC_MODE
    // Report the cartesian product
    scicellxx_output << "Cartesian product:" << std::endl;
    SciCellxxCartesianProduct::print(configurations);
    scicellxx_output << std::endl;
#else
    scicellxx_output << "Cartesian product printing is DISABLED since PANIC_MODE is DISABLED" << std::endl;
#endif // #ifdef SCICELLXX_PANIC_MODE
   }
  
  // Validate that the number of cores is no larger than the number of
  // configurations
  if ((unsigned)SciCellxxMPI::nprocs > n_all_configurations)
   {
        // Error message
    std::ostringstream error_message;
    error_message << "The number of cores is larger than the number of configurations.\n"
                  << "Reduce the number of cores such that each core process at least one configuration\n"
                  << std::endl;
    throw SciCellxxLibError(error_message.str(),
                            SCICELLXX_CURRENT_FUNCTION,
                            SCICELLXX_EXCEPTION_LOCATION);
   }
  
  // Compute the number of configurations per core
  const unsigned n_configurations_per_core = n_all_configurations / SciCellxxMPI::nprocs;
  // Print the information only on the master core
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    scicellxx_output << "Number of configurations per core: " << n_configurations_per_core << std::endl;
   }
  
  // For each core get its corresponding processing configurations
  // (the indices on the all configurations vector)
  std::vector<unsigned> indices_configurations_per_core;
  indices_configurations_per_core.reserve(n_configurations_per_core + 1);
  
  for (unsigned i = SciCellxxMPI::rank; i < n_all_configurations; i+=SciCellxxMPI::nprocs)
   {
    indices_configurations_per_core.push_back(i);
   } // for (i < n_all_configurations)

  // Print the indices of configurations per core
  //for (unsigned i = 0; i < indices_configurations_per_core.size(); i++)
  // {
  //  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank,SciCellxxMPI::nprocs) << indices_configurations_per_core[i] << std::endl;
  // }
  
  // Get the real number of configuration for this core (probably
  // different from n_configurations_per_core due to rounding errors)
  const unsigned n_configurations_this_core = indices_configurations_per_core.size();
  
  // Keep track of the means, standard deviation and median of the
  // channel density space/state per configuration
  std::vector<Real> mean_density(n_configurations_this_core);
  std::vector<Real> stdev_density(n_configurations_this_core);
  std::vector<Real> median_density(n_configurations_this_core);
  
  // Keep track of the means, standard deviation and median of the
  // microtubule current per configuration
  std::vector<Real> mean_current(n_configurations_this_core);
  std::vector<Real> stdev_current(n_configurations_this_core);
  std::vector<Real> median_current(n_configurations_this_core);
  
  unsigned config_counter = 0;
  
  // Run all configurations
  for (unsigned i_config = 0; i_config < n_configurations_this_core; i_config++)
   {
    // Get the index for the corresponding configuration for this core
    const unsigned configuration_index = indices_configurations_per_core[i_config];
    // Get values for each configuration and perform the simulation
    const Real alpha = configurations[configuration_index][0];
    const Real beta = configurations[configuration_index][1];
    const Real rho = configurations[configuration_index][2];
    const Real omega_in = configurations[configuration_index][3];
    const Real omega_out = configurations[configuration_index][4];
    
    // Transform to string in case we need to generate a folder for
    // each experiment
    std::ostringstream ss_alpha;
    ss_alpha << setprecision(precision_real_values) << alpha;
    std::ostringstream ss_beta;
    ss_beta << setprecision(precision_real_values) << beta;
    std::ostringstream ss_rho;
    ss_rho << setprecision(precision_real_values) << rho;
    std::ostringstream ss_omega_in;
    ss_omega_in << setprecision(precision_real_values) << omega_in;
    std::ostringstream ss_omega_out;
    ss_omega_out << setprecision(precision_real_values) << omega_out;
    
    scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Config_id/total:" << configuration_index << "/" << n_all_configurations << "\talpha:" << ss_alpha.str() << "\tbeta:" << ss_beta.str() << "\trho:" << ss_rho.str() << "\tomega_in:" << ss_omega_in.str() << "\tomega_out:" << ss_omega_out.str() << std::endl;
    
    // Keep track of the means, standard deviation and median of the
    // channel density space/state per experiment
    std::vector<Real> mean_density_iconfig(max_experiments);
    std::vector<Real> stdev_density_iconfig(max_experiments);
    std::vector<Real> median_density_iconfig(max_experiments);

    // Keep track of the means, standard deviation and median of the
    // microtubule current per experiment
    std::vector<Real> mean_current_iconfig(max_experiments);
    std::vector<Real> stdev_current_iconfig(max_experiments);
    std::vector<Real> median_current_iconfig(max_experiments);
    
    unsigned experiment_counter = 0;
    
    // Run all experiments for the current configuration
    for (unsigned i_experiment = 0; i_experiment < max_experiments; i_experiment++)
     {
      // Experiment number to string
      std::ostringstream ss_iexperiment;
      ss_iexperiment << std::setw(width_number) << std::setfill(fill_char) << std::to_string(i_experiment);
      
      // Folder name for each experiment
      std::string experiment_folder_name(root_output_folder +
                                         std::string("/exp") + ss_iexperiment.str() +
                                         std::string("_a") + ss_alpha.str() +
                                         std::string("_b") + ss_beta.str() +
                                         std::string("_r") + ss_rho.str() +
                                         std::string("_oi") + ss_omega_in.str() +
                                         std::string("_oo") + ss_omega_out.str());
      
      // Create the folder for the output of each experiment
      if (output_microtubule_state)
       {
        // Create output directory
        SciCellxxFileSystem::create_directory(experiment_folder_name);
       }
      
      // Construct the microtubule with N channels and L cells on each channel
      bool **m = new bool*[N];
      for (unsigned i_channel = 0; i_channel < N; i_channel++)
       {
        m[i_channel] = new bool[L];
       }

      // Initialize microtubule with zeroes
      for (unsigned i_channel = 0; i_channel < N; i_channel++)
       {
        for (unsigned i_cell = 0; i_cell < L; i_cell++)
         {
          m[i_channel][i_cell] = 0;
         }
       }
      
      // Store the space/state diagram with mean density (reserve
      // memory for at least the max number of simulations per
      // experiment)
      std::vector<std::vector<Real> > mean_channels_density_space_state_all_simulations;
      if (output_space_state_diagram)
       {
        mean_channels_density_space_state_all_simulations.reserve(max_simulations_per_experiment);
       }
      
      // The number of data to collect
      const unsigned n_data_to_gather = max_simulations_per_experiment - simulation_step_to_start_gathering_data;
      
      // Keep track of the means of the mean channel density space/state
      std::vector<Real> mean_density_iexperiment(n_data_to_gather);
      // Keep track of the means of the current
      std::vector<Real> mean_current_iexperiment(n_data_to_gather);
      unsigned simulation_counter = 0;
      
      // Start simulation
      for (unsigned i_simulation_step = 0; i_simulation_step < max_simulations_per_experiment; i_simulation_step++)
       {
        // Store the current
        Real local_mean_current = 0.0;
        // Apply mTASEP
        mTASEP(m, N, L, alpha, beta, rho, omega_in, omega_out, lateral_movement, local_mean_current);
        
        // Compute the density on the microtubule
        std::vector<Real> mean_channels_density = compute_mean_channels_density(m, N, L);
        if (output_space_state_diagram)
         {
          // Add the density to the space_state diagram
          mean_channels_density_space_state_all_simulations.push_back(mean_channels_density);
         }
        
        if (i_simulation_step >= simulation_step_to_start_gathering_data)
         {
          // Compute the mean density for this simulation step
          // (density along the microtubule)
          Real local_mean_density = 0.0;
          SciCellxxStatistics::statistics_mean(mean_channels_density, local_mean_density);
          // Keep track of the means for each simulation step
          mean_density_iexperiment[simulation_counter] = local_mean_density;
          mean_current_iexperiment[simulation_counter] = local_mean_current;
          
          // Increase counter
          simulation_counter++;
          
         }
        
        // Store csv file with the microtubule state
        if (output_microtubule_state)
         {
          std::ostringstream ss;
          ss << std::setw(width_number) << std::setfill(fill_char) << std::to_string(i_simulation_step);
          
          //std::string csv_filename(root_output_folder + std::string("/example_") + std::to_string(i_simulation_step) + std::string(".csv"));
          std::string csv_filename(experiment_folder_name + std::string("/microtubule_") + ss.str() + std::string(".csv"));
          boolean_matrix_to_csv_file(m, N, L, csv_filename);
         }
        
       } // for (i_simulation_step < max_simulations_per_experiment)

      // Compute the mean, standard deviation and median for density on this experiment
      Real imean_density = 0.0;
      Real istdev_density = 0.0;
      Real imedian_density = 0.0;
      SciCellxxStatistics::statistics_mean_std_median(mean_density_iexperiment, imean_density, istdev_density, imedian_density);
      // Keep track of the mean, standard deviation and median of the density for each experiment
      mean_density_iconfig[experiment_counter] = imean_density;
      stdev_density_iconfig[experiment_counter] = istdev_density;
      median_density_iconfig[experiment_counter] = imedian_density;
      
      // Compute the mean, standard deviation and median for current on this experiment
      Real imean_current = 0.0;
      Real istdev_current = 0.0;
      Real imedian_current = 0.0;
      SciCellxxStatistics::statistics_mean_std_median(mean_current_iexperiment, imean_current, istdev_current, imedian_current);
      // Keep track of the mean, standard deviation and median of the current for each experiment
      mean_current_iconfig[experiment_counter] = imean_current;
      stdev_current_iconfig[experiment_counter] = istdev_current;
      median_current_iconfig[experiment_counter] = imedian_current;
      
      experiment_counter++;
      
      // Check whether we should output the space/state diagram
      if (output_space_state_diagram)
       {
        std::string csv_filename_space_state(experiment_folder_name + std::string(".csv"));
        real_matrix_to_csv_file(mean_channels_density_space_state_all_simulations,
                                max_simulations_per_experiment, L, csv_filename_space_state);
       }
      
      // Free memory for multichannel-microtubule
      for (unsigned i_channel = 0; i_channel < N; i_channel++)
       {
        delete [] m[i_channel];
       }
      delete [] m;
      
     } // for (i_experiment < max_experiments)
    
    // Compute the mean, standard deviation and median for density on this configuration
    Real imean_density = 0.0;
    Real istdev_density = 0.0;
    Real imedian_density = 0.0;
    SciCellxxStatistics::statistics_mean(mean_density_iconfig, imean_density);
    SciCellxxStatistics::statistics_mean(stdev_density_iconfig, istdev_density);
    SciCellxxStatistics::statistics_mean(median_density_iconfig, imedian_density);
    // Keep track of the mean, standard deviation and median of the density for each configuration
    mean_density[config_counter] = imean_density;
    stdev_density[config_counter] = istdev_density;
    median_density[config_counter] = imedian_density;
    
    // Compute the mean, standard deviation and median for current on this configuration
    Real imean_current = 0.0;
    Real istdev_current = 0.0;
    Real imedian_current = 0.0;
    SciCellxxStatistics::statistics_mean(mean_current_iconfig, imean_current);
    SciCellxxStatistics::statistics_mean(stdev_current_iconfig, istdev_current);
    SciCellxxStatistics::statistics_mean(median_current_iconfig, imedian_current);
    // Keep track of the mean, standard deviation and median of the current for each configuration
    mean_current[config_counter] = imean_current;
    stdev_current[config_counter] = istdev_current;
    median_current[config_counter] = imedian_current;
    
    config_counter++;
    
   } // for (i_config < n_configurations_this_core)
  
  // ****************************************************************************************
  // Each core reports its results into a file
  // ****************************************************************************************
  
  // Open the file
  std::string output_final_results_this_core_filename(root_output_folder + "/output_r" + ss_rank.str() + ".csv");
  std::ofstream output_final_results_this_core_file(output_final_results_this_core_filename, std::ios_base::out);
  // The header
  output_final_results_this_core_file << "id,alpha,beta,rho,omega_in,omega_out,density,std_density,median_density,current,std_current,median_current" << std::endl;
  
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Flushing results into disk ..." << std::endl;
  
  // For each configuration
  for (unsigned i_config = 0; i_config < n_configurations_this_core; i_config++)
   {
    // Get the index for the corresponding configuration for this core
    const unsigned configuration_index = indices_configurations_per_core[i_config];
    
    // Get values for each configuration
    const Real alpha = configurations[configuration_index][0];
    const Real beta = configurations[configuration_index][1];
    const Real rho = configurations[configuration_index][2];
    const Real omega_in = configurations[configuration_index][3];
    const Real omega_out = configurations[configuration_index][4];
     
    const Real imean_density = mean_density[i_config];
    const Real istdev_density = stdev_density[i_config];
    const Real imedian_density = median_density[i_config];
    
    const Real imean_current = mean_current[i_config];
    const Real istdev_current = stdev_current[i_config];
    const Real imedian_current = median_current[i_config];
    
    // Transform to string to output to file
    std::ostringstream ss_alpha;
    ss_alpha << setprecision(precision_real_values) << alpha;
    std::ostringstream ss_beta;
    ss_beta << setprecision(precision_real_values) << beta;
    std::ostringstream ss_rho;
    ss_rho << setprecision(precision_real_values) << rho;
    std::ostringstream ss_omega_in;
    ss_omega_in << setprecision(precision_real_values) << omega_in;
    std::ostringstream ss_omega_out;
    ss_omega_out << setprecision(precision_real_values) << omega_out;
    
    std::ostringstream ss_imean_density;
    ss_imean_density << setprecision(precision_real_values) << imean_density;
    std::ostringstream ss_istdev_density;
    ss_istdev_density << setprecision(precision_real_values) << istdev_density;
    std::ostringstream ss_imedian_density;
    ss_imedian_density << setprecision(precision_real_values) << imedian_density;
    
    std::ostringstream ss_imean_current;
    ss_imean_current << setprecision(precision_real_values) << imean_current;
    std::ostringstream ss_istdev_current;
    ss_istdev_current << setprecision(precision_real_values) << istdev_current;
    std::ostringstream ss_imedian_current;
    ss_imedian_current << setprecision(precision_real_values) << imedian_current;
    
    output_final_results_this_core_file << configuration_index << "," << ss_alpha.str() << "," << ss_beta.str() << "," << ss_rho.str() << "," << ss_omega_in.str() << "," << ss_omega_out.str() << "," << ss_imean_density.str() << "," << ss_istdev_density.str() << "," << ss_imedian_density.str() << "," << ss_imean_current.str() << "," << ss_istdev_current.str() << "," << ss_imedian_current.str() << std::endl;
    
    //scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "id:" << configuration_index << "\talpha:" << ss_alpha.str() << "\tbeta:" << ss_beta.str() << "\trho:" << ss_rho.str() << "\tomega_in:" << ss_omega_in.str() << "\tomega_out:" << ss_omega_out.str() << "\tdensity:" << ss_imean_density.str() << "\tdensity(std):" << ss_istdev_density.str() << "\tdensity(median):" << ss_imedian_density.str() << "\tcurrent:" << ss_imean_current.str() << "\tcurrent(std):" << ss_istdev_current.str() << "\tcurrent(median):" << ss_imedian_current.str() << std::endl;
    
   } // for (i_config < n_configurations_this_core)
  
  // Close the file
  output_final_results_this_core_file.close();
  
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Flushing results into disk [DONE]" << std::endl;
  
  // ****************************************************************************************
  // GATHER RESULTS INTO A MASTER CORE
  // ****************************************************************************************
  
  // ****************************************************************************************
  // Send the results from each core to a master core
  // ****************************************************************************************
  
  // Store the results into a vector to send it to a master node that
  // will reports results in a single file
  const unsigned n_fields_of_data_to_transfer = 12;
  // This number incluces storage for the global id
  Real *data_sent_to_master = new Real[n_fields_of_data_to_transfer*n_configurations_this_core];
  
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Gathering results into a single file ..." << std::endl;
  
  // For each configuration
  for (unsigned i_config = 0; i_config < n_configurations_this_core; i_config++)
   {
    // Get the index for the corresponding configuration for this core
    const unsigned configuration_index = indices_configurations_per_core[i_config];
    
    // Get values for each configuration
    const Real alpha = configurations[configuration_index][0];
    const Real beta = configurations[configuration_index][1];
    const Real rho = configurations[configuration_index][2];
    const Real omega_in = configurations[configuration_index][3];
    const Real omega_out = configurations[configuration_index][4];
    
    const Real imean_density = mean_density[i_config];
    const Real istdev_density = stdev_density[i_config];
    const Real imedian_density = median_density[i_config];
    
    const Real imean_current = mean_current[i_config];
    const Real istdev_current = stdev_current[i_config];
    const Real imedian_current = median_current[i_config];

    const unsigned start_index = i_config*n_fields_of_data_to_transfer;
    data_sent_to_master[start_index + 0] = configuration_index;
    data_sent_to_master[start_index + 1] = alpha;
    data_sent_to_master[start_index + 2] = beta;
    data_sent_to_master[start_index + 3] = rho;
    data_sent_to_master[start_index + 4] = omega_in;
    data_sent_to_master[start_index + 5] = omega_out;
    data_sent_to_master[start_index + 6] = imean_density;
    data_sent_to_master[start_index + 7] = istdev_density;
    data_sent_to_master[start_index + 8] = imedian_density;
    data_sent_to_master[start_index + 9] = imean_current;
    data_sent_to_master[start_index + 10] = istdev_current;
    data_sent_to_master[start_index + 11] = imedian_current;
    
   } // for (i_config < n_configurations_this_core)
  
  // The number of configurations to recieve from each core into master
  int *n_configurations_to_receive_on_master_from_each_core = new int[SciCellxxMPI::nprocs];
  
  // On a master core gather the number of configurations on each core
  MPI_Gather(&n_configurations_this_core, 1, MPI_UNSIGNED,
             n_configurations_to_receive_on_master_from_each_core, 1, MPI_INT,
             SciCellxxMPI::master_core, SciCellxxMPI::comm);

  //std::cerr << "This core configs:\n";
  //std::cerr << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << n_configurations_this_core << std::endl;
  
  unsigned all_configurations_mpi_reduce = 0;
  MPI_Reduce(&n_configurations_this_core, &all_configurations_mpi_reduce, 1, MPI_UNSIGNED, MPI_SUM,
             SciCellxxMPI::master_core, SciCellxxMPI::comm);
  
  // Validate that the sum of configurations to receive is the same as
  // the original number of total configurations
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    if (all_configurations_mpi_reduce != n_all_configurations)
     {
      // Error message
      std::ostringstream error_message;
      error_message << "The sum of configurations to receive is different than the original\n"
                    << "number of total configurations\n"
                    << "(all_configurations_mpi_reduce):" << all_configurations_mpi_reduce << std::endl
                    << "(n_all_configurations):" << n_all_configurations << std::endl;
      throw SciCellxxLibError(error_message.str(),
                              SCICELLXX_CURRENT_FUNCTION,
                              SCICELLXX_EXCEPTION_LOCATION);
     }
   }
  
  const unsigned n_data_sent_to_master = n_fields_of_data_to_transfer*n_configurations_this_core;
  //std::cerr << "N data sent to master:\n";
  //std::cerr << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << n_data_sent_to_master << std::endl;
  
  // Vector to receive data from cores
  Real *data_received_on_master = new Real[n_fields_of_data_to_transfer*n_all_configurations];
  
  // The number of data to receive on master from each core
  int *n_data_to_receive_on_master_from_each_core = new int[SciCellxxMPI::nprocs];
  
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    //std::cerr << "Master core configs:\n";
    for (int i = 0; i < SciCellxxMPI::nprocs; i++)
     {
      n_data_to_receive_on_master_from_each_core[i] = n_configurations_to_receive_on_master_from_each_core[i] * n_fields_of_data_to_transfer;
      //std::cerr << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
      //          << "[" << i << "] confgs "<< n_configurations_to_receive_on_master_from_each_core[i] << std::endl;
      //std::cerr << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs)
      //          << "[" << i << "] data "<< n_data_to_receive_on_master_from_each_core[i] << std::endl;
     }
   }
  
  // Compute the displacements vector
  int *n_displacement_on_mater_for_each_core = new int[SciCellxxMPI::nprocs];
  unsigned displ = 0;
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    for (int i = 0; i < SciCellxxMPI::nprocs; i++)
     {
      n_displacement_on_mater_for_each_core[i] = displ;
      displ+=n_data_to_receive_on_master_from_each_core[i];
      //std::cerr << "N displacement on master for each core:\n";
      //std::cerr << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "["<<i<<"]: "<< n_displacement_on_mater_for_each_core[i] << std::endl;
     }
   } // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  
  // On a master core gather the configurations from all cores
  MPI_Gatherv(data_sent_to_master, n_data_sent_to_master, MPI_SC_REAL,
              data_received_on_master, n_data_to_receive_on_master_from_each_core,
              n_displacement_on_mater_for_each_core, MPI_SC_REAL,
              SciCellxxMPI::master_core, SciCellxxMPI::comm);
  
  scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Gathering results into a single file [DONE]" << std::endl;
  
  // ****************************************************************************************
  // Generate a single output file with the results from all processors
  // ****************************************************************************************  
  if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
   {
    scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Flushing gathered results into disk ..." << std::endl;
    
    // Open the file
    std::string output_final_results_filename(root_output_folder + "/output" + ".csv");
    std::ofstream output_final_results_file(output_final_results_filename, std::ios_base::out);
    // The header
    output_final_results_file << "id,alpha,beta,rho,omega_in,omega_out,density,std_density,median_density,current,std_current,median_current" << std::endl;
    
    // For each configuration
    for (unsigned i_config = 0; i_config < n_all_configurations; i_config++)
     {
      const unsigned start_index = i_config*n_fields_of_data_to_transfer;
      
      // Get values for each configuration
      const unsigned configuration_index = static_cast<unsigned>(data_received_on_master[start_index + 0]);
      const Real alpha = data_received_on_master[start_index + 1];
      const Real beta = data_received_on_master[start_index + 2];
      const Real rho = data_received_on_master[start_index + 3];
      const Real omega_in = data_received_on_master[start_index + 4];
      const Real omega_out = data_received_on_master[start_index + 5];
      
      const Real imean_density = data_received_on_master[start_index + 6];
      const Real istdev_density = data_received_on_master[start_index + 7];
      const Real imedian_density = data_received_on_master[start_index + 8];
      
      const Real imean_current = data_received_on_master[start_index + 9];
      const Real istdev_current = data_received_on_master[start_index + 10];
      const Real imedian_current = data_received_on_master[start_index + 11];
      
      // Transform to string to output to file
      std::ostringstream ss_alpha;
      ss_alpha << setprecision(precision_real_values) << alpha;
      std::ostringstream ss_beta;
      ss_beta << setprecision(precision_real_values) << beta;
      std::ostringstream ss_rho;
      ss_rho << setprecision(precision_real_values) << rho;
      std::ostringstream ss_omega_in;
      ss_omega_in << setprecision(precision_real_values) << omega_in;
      std::ostringstream ss_omega_out;
      ss_omega_out << setprecision(precision_real_values) << omega_out;
      
      std::ostringstream ss_imean_density;
      ss_imean_density << setprecision(precision_real_values) << imean_density;
      std::ostringstream ss_istdev_density;
      ss_istdev_density << setprecision(precision_real_values) << istdev_density;
      std::ostringstream ss_imedian_density;
      ss_imedian_density << setprecision(precision_real_values) << imedian_density;
      
      std::ostringstream ss_imean_current;
      ss_imean_current << setprecision(precision_real_values) << imean_current;
      std::ostringstream ss_istdev_current;
      ss_istdev_current << setprecision(precision_real_values) << istdev_current;
      std::ostringstream ss_imedian_current;
      ss_imedian_current << setprecision(precision_real_values) << imedian_current;
      
      output_final_results_file << configuration_index << "," << ss_alpha.str() << "," << ss_beta.str() << "," << ss_rho.str() << "," << ss_omega_in.str() << "," << ss_omega_out.str() << "," << ss_imean_density.str() << "," << ss_istdev_density.str() << "," << ss_imedian_density.str() << "," << ss_imean_current.str() << "," << ss_istdev_current.str() << "," << ss_imedian_current.str() << std::endl;
      
      //scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "id:" << configuration_index << "\talpha:" << ss_alpha.str() << "\tbeta:" << ss_beta.str() << "\trho:" << ss_rho.str() << "\tomega_in:" << ss_omega_in.str() << "\tomega_out:" << ss_omega_out.str() << "\tdensity:" << ss_imean_density.str() << "\tdensity(std):" << ss_istdev_density.str() << "\tdensity(median):" << ss_imedian_density.str() << "\tcurrent:" << ss_imean_current.str() << "\tcurrent(std):" << ss_istdev_current.str() << "\tcurrent(median):" << ss_imedian_current.str() << std::endl;
      
     } // for (i_config < n_all_configurations)
    
    // Close the file
    output_final_results_file.close();
    
    scicellxx_output << MPI_RANK_NPROCS_PRINT(SciCellxxMPI::rank, SciCellxxMPI::nprocs) << "Flushing gathered results into disk [DONE]" << std::endl;
    
   } // if (SciCellxxMPI::rank == SciCellxxMPI::master_core)
  
  // Finalise chapcom
  finalise_scicellxx();
  
  return 0;
  
}

