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
#ifndef CCSCICELLXX2VTK_H
#define CCSCICELLXX2VTK_H

#include "../general/general.h"

#include "../data_structures/cc_data.h"
#include "../data_structures/cc_node.h"

// Include the VTK libraries to generate the output
#include <vtkUnstructuredGrid.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>

#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkPointData.h>

namespace scicellxx
{
 
 //========================================================================
 // Wrapper to provide access to VTK ploting tools (implemented as a
 // Singleton)
 // =======================================================================
 class CCSciCellxx2VTK 
 {
 public:
  static CCSciCellxx2VTK& get_instance()
  {
   static CCSciCellxx2VTK instance;
   return instance;
  }
  
  // ==================================================================
  // Functions for VTK output
  // ==================================================================
  
  // In charge of output a set of particles at current time into a VTK
  // file. The particles are stored at particles_data parameter.
  // 
  // If having three dimensions data then it should be stored as
  // follows and set n_data_per_particle = 6
  //
  // For particle i
  // x-position of particle i, particles_data(i*6+0, 0)
  // x-velocity of particle i, particles_data(i*6+1, 0)
  // y-position of particle i, particles_data(i*6+2, 0)
  // y-velocity of particle i, particles_data(i*6+3, 0)
  // z-position of particle i, particles_data(i*6+4, 0)
  // z-velocity of particle i, particles_data(i*6+5, 0)
  //
  //
  // If having two dimensions data then it should be stored as
  // follows and set n_data_per_particle = 4
  //
  // For particle i
  // x-position of particle i, particles_data(i*4+0, 0)
  // x-velocity of particle i, particles_data(i*4+1, 0)
  // y-position of particle i, particles_data(i*4+2, 0)
  // y-velocity of particle i, particles_data(i*4+3, 0)
  // 
  // where k is the number of data per particle
  void output_particles(Real time,
                        CCData &particles_data,
                        std::ostringstream &file_name,
                        const unsigned n_data_per_particle = 6);
  
  // In charge of output the node position and attributes (velocity,
  // temperature, mass, etc) as a cloud of points
  void output_node(CCNode &node, std::ostringstream &file_name);
  
  // In charge of output the nodes positions and attributes (velocity,
  // temperature, mass, etc) as a cloud of points
  void output_nodes(std::vector<CCNode> &nodes, std::ostringstream &file_name); 
  
  // In charge of output the position and its corresponding attributes
  // (velocity, temperature, mass, etc)
  void output_position_and_attribute_data(CCData &positions,
                                          CCData &attributes,
                                          std::ostringstream &file_name);
  
  // In charge of output the vector positions and its corresponding
  // attributes (velocity, temperature, mass, etc)
  void output_position_and_attribute_datas(std::vector<CCData> &positions,
                                           std::vector<CCData> &attributes,
                                           std::ostringstream &file_name); 
  
 private:
  
  // Empty constructor
  CCSciCellxx2VTK()
   {
    
   } 
  
  /// Copy constructor (we do not want this class to be
  /// copiable). Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  CCSciCellxx2VTK(CCSciCellxx2VTK const&)
   {
    BrokenCopy::broken_copy("CCSciCellxx2VTK");
   } // Don't Implement.
  
  /// Assignment operator (we do not want this class to be
  /// copiable. Check
  /// http://www.learncpp.com/cpp-tutorial/912-shallow-vs-deep-copying/
  void operator=(CCSciCellxx2VTK const&)
   {
    BrokenCopy::broken_assign("CCSciCellxx2VTK");
   }// Don't implement
  
  // Transfer the data from the input variable time, to the
  // data_set. Inserts the associated TIME to the set of
  // particles. Called from method output_partciles()
  void add_time_to_vtk_data_set_helper(Real time,
                                       vtkSmartPointer<vtkUnstructuredGrid> &data_set); 
  
  // Transfer the data from particles_data to data_points and
  // data_set. Called from method output_partciles()
  void add_particles_to_vtk_data_set_helper(CCData &particles_data,
                                            vtkSmartPointer<vtkPoints> &data_points,
                                            vtkSmartPointer<vtkUnstructuredGrid> &data_set,
                                            const unsigned n_data_per_particle = 6);
  
  // ==================================================================
  // Transfer the data from point and attributes to data_points and
  // data_set, respectively. Called from method
  // output_position_and_attribute_data()
  // ==================================================================
  void add_data_point_and_attributes_to_vtk_data_set_helper(CCData &position,
                                                            CCData &attributes,
                                                            vtkSmartPointer<vtkPoints> &data_points,
                                                            vtkSmartPointer<vtkUnstructuredGrid> &data_set);
  
  // ==================================================================
  // Transfer the data from points and attributes to data_points and
  // data_set, respectively. Called from method
  // output_position_and_attribute_datas()
  // ==================================================================
  void add_data_points_and_attributes_to_vtk_data_set_helper(std::vector<CCData> &positions,
                                                             std::vector<CCData> &attributes,
                                                             vtkSmartPointer<vtkPoints> &data_points,
                                                             vtkSmartPointer<vtkUnstructuredGrid> &data_set);
  
 };
 
 //========================================================================
 // Single (global) instantiation of the CCSciCellxx2VTK class -- this is
 // used throughout the library to generate VTK output for Paraview
 // ========================================================================
 //extern CCSciCellxx2VTK scicellxx2vtk;

}

#endif // #ifndef CCSCICELLXX2VTK_H
