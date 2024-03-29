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
/// IN THIS FILE: The definition of a class to represent data

// Check whether the class has been already defined
#ifndef CCDATA_H
#define CCDATA_H

#include "../general/general.h"

namespace scicellxx
{
 
 /// Enumerator to indicate whether the associated value is pinned or
 /// not. Pinned status represents known values, ex. initial or
 /// boundary conditions
 enum Data_status {UNDEFINED, UNPINNED, PINNED};
 
 /// @class CCData cc_data.tpl.h
 
 /// Concrete class to represent data
 class CCData
 {
  
 public:
  
  /// Constructor. Allocates memory for the values. Initialise them to
  /// zero
  CCData(const unsigned n_values, const unsigned n_history_values=1);
  
  /// Constructor. Allocates memory for values and copy them from the
  /// input vector
  CCData(Real *values_pt,
         const unsigned n_values,
         const unsigned n_history_values=1);
  
  /// Copy constructor
  CCData(const CCData &copy);
  
  /// Destructor
  virtual ~CCData();
  
  /// Assignment operator
  CCData &operator=(const CCData &source_values);
  
  //// Get access using brackets as data(i). Read-only version
  inline virtual Real operator()(const unsigned &i) const
  {return value(i,0);}
  
  //// Get access using brackets as data(i). Read-write version
  inline virtual Real &operator()(const unsigned &i)
  {return value(i,0);}
  
  //// Get access using brackets as data(i,j). Read-only version
  inline virtual Real operator()(const unsigned &i, 
                                 const unsigned &t) const
  {return value(i, t);}
  
  //// Get access using brackets as data(i,j). Read-write version
  inline virtual Real &operator()(const unsigned &i, 
                                  const unsigned &t)
  {return value(i,t);}
  
  /// Transforms the input values vector to a Data class type
  void set_values(const Real *values_pt);
  
  /// Clean up for any dynamically stored data
  void clean_up();
  
  /// Free allocated memory
  void free_memory_of_values();
  
  /// Shift history values (mostly used for time integration). Move the
  /// values from index 0 the indicated number of positions to the
  /// right
  void shift_history_values(const unsigned n_shift_positions = 1);
  
  /// Get a pointer to the t-th history values
  Real *history_values_row_pt(const unsigned t = 0);
  
  /// Get the specified value (read-only)
  const Real value(const unsigned &i, const unsigned t=0) const;
  
  /// Set values (write version)
  Real &value(const unsigned &i, const unsigned t=0);
  
  /// Output the values vector (matrix)
  void output(bool output_indexes) const;
  
  /// Output the matrix
  void output(std::ofstream &outfile,
              bool output_indexes) const;
  
  /// Creates a zero Values_pt and Status_pt vectors with the given
  /// number of elements
  void create_zero_values_and_status_vectors();
  
  // Pin the corresponding i-th data (generally the i-th index
  // indicates the i-th dimensional value)
  inline void pin(const unsigned &i) {Status_pt[i]=PINNED;}
  
  // Unpin the corresponding i-th data (generally the i-th index
  // indicates the i-th dimensional value)
  inline void unpin(const unsigned &i) {Status_pt[i]=UNPINNED;}
  
  // Mark as undefined the corresponding i-th data (generally the i-th
  // index indicates the i-th dimensional value)
  inline void undefine(const unsigned &i) {Status_pt[i]=UNDEFINED;}
  
  /// Pins all the values associated with this data
  void pin_all();
  
  /// Unpins all the values associated with this data
  void unpin_all();
  
  /// Get access to the Values_pt
  inline Real *values_pt() const {return Values_pt;}
  
  /// Get access to the Status_pt
  inline Data_status *status_pt() const {return Status_pt;}
  
  inline bool is_pinned(const unsigned i) const
  {return (Status_pt[i]==PINNED);}
  
  inline bool is_unpinned(const unsigned i) const
  {return (Status_pt[i]==UNPINNED);}
  
  inline bool is_undefined(const unsigned i) const
  {return (Status_pt[i]==UNDEFINED);}
  
  /// Gets the number of values
  inline unsigned n_values() const {return N_values;}
  
  /// Gets the number of history values
  inline unsigned n_history_values() const {return N_history_values;}
  
  /// Checks whether values have been set, or allocated
  inline bool is_empty() const {return (Is_values_empty && Is_status_empty);}
  
  /// Checks whether the values storage is allowed to be deleted
  inline bool delete_values_storage() const {return Delete_values_storage;}
  
  /// Enables the deletion of values storage
  inline void enable_delete_values_storage() {Delete_values_storage=true;}
  
  /// Disables the deletion of values storage
  inline void disable_delete_values_storage() {Delete_values_storage=false;}
  
 protected:
  
  /// Flag to indicate whether values vector is empty
  bool Is_values_empty;
  /// Flag to indicate whether status vector is empty
  bool Is_status_empty;
  
  /// Flag to indicate whether to delete (free) the allocated memory
  /// for values. The deletion is true by default.
  bool Delete_values_storage;
  
 private:
  
  /// The number of values
  const unsigned N_values;
  
  /// The number of history values
  const unsigned N_history_values;
  
  /// Store the values of the i-th data at th t-th history
  /// Values_pt[t*N_values+i], the values are stored by row, i.e. the
  /// whole values at t-th history is in row t
  Real *Values_pt;
  
  /// Indicates the status of the current (0 index history) Value
  Data_status *Status_pt;
  
 };
 
}

#endif // #ifndef CCDATA_H
