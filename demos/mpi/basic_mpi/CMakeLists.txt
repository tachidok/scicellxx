# Indicate source files
SET(SRC_demo_basic_mpi demo_basic_mpi.cpp)

# Create executable (check whether compilation was requested or not)
IF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_basic_mpi ${SRC_demo_basic_mpi})
ELSE(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_basic_mpi EXCLUDE_FROM_ALL ${SRC_demo_basic_mpi})
ENDIF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_basic_mpi ${LIB_scicellxx_lib})

# ... and link againts them
TARGET_LINK_LIBRARIES(demo_basic_mpi ${LIB_demo_basic_mpi})

# Check if the bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties(demo_basic_mpi
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
#ADD_TEST(NAME TEST_demo_basic_mpi_run
#          COMMAND mpirun -n 4 demo_basic_mpi)

SET(MPI_DEMO_NUMPROCS 4)
SET(ARGS 0)
ADD_TEST(NAME TEST_demo_basic_mpi_run
          COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${MPI_DEMO_NUMPROCS} ${MPIEXEC_PREFLAGS} $<TARGET_FILE:demo_basic_mpi> ${MPIEXEC_POSTFLAGS} ARGS)

#SET(ARGS 0)
#SET(MPI_COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUM#PROCS} ${MPIEXEC_PREFLAGS} demo_basic_mpi ${MPIEXEC_POSTFLAGS} ARGS)
#ADD_TEST(NAME TEST_demo_basic_mpi_run
#          COMMAND ${MPI_COMMAND})
          
# Validate output
IF (SCICELLXX_USES_DOUBLE_PRECISION)
   SET (VALIDATE_FILENAME_demo_basic_mpi "validate_double_demo_basic_mpi.dat")
ELSE (SCICELLXX_USES_DOUBLE_PRECISION)
     SET (VALIDATE_FILENAME_demo_basic_mpi "validate_demo_basic_mpi.dat")
ENDIF (SCICELLXX_USES_DOUBLE_PRECISION)
ADD_TEST(NAME TEST_demo_basic_mpi_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_mpi} ${CMAKE_CURRENT_BINARY_DIR}/output_test.dat)

# ===========================================
# Test execution order
# ===========================================
SET_TESTS_PROPERTIES(TEST_demo_basic_mpi_check_output PROPERTIES DEPENDS TEST_demo_basic_mpi_run)
