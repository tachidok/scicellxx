# Indicate source files and dependencies in the files
#SET(SRC_demo_mTASEP_LK cc_mTASEP.cpp demo_mTASEP_LK.cpp)
SET(SRC_demo_mTASEP_LK demo_mTASEP_LK.cpp)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(julio_demo_mTASEP_LK EXCLUDE_FROM_ALL ${SRC_demo_mTASEP_LK})

# Indicate linking libraries
SET(LIB_demo_mTASEP_LK ${LIB_scicellxx_lib})

# ... and link againts them  
#TARGET_LINK_LIBRARIES(julio_demo_mTASEP_LK ${LIB_demo_mTASEP_LK})
# Temporal solution for MPI
# when linking a target use the following (supported in CMake 3.9+)
#target_link_libraries(MyTarget PUBLIC MPI::MPI_CXX)

TARGET_LINK_LIBRARIES(julio_demo_mTASEP_LK PUBLIC ${LIB_demo_mTASEP_LK} MPI::MPI_CXX)

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( julio_demo_mTASEP_LK
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

