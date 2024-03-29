# Indicate source files
SET(SRC_demo_linear_solver demo_linear_solver.cpp)

# Create executable (check whether compilation was requested or not)
IF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_linear_solver ${SRC_demo_linear_solver})
ELSE(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_linear_solver EXCLUDE_FROM_ALL ${SRC_demo_linear_solver})
ENDIF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_linear_solver ${LIB_scicellxx_lib})

# ... and link againts them  
TARGET_LINK_LIBRARIES(demo_linear_solver ${LIB_demo_linear_solver})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( demo_linear_solver
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
ADD_TEST(NAME TEST_demo_linear_solver_run
          COMMAND demo_linear_solver)
IF (SCICELLXX_USES_DOUBLE_PRECISION)
   SET (VALIDATE_FILENAME_demo_linear_solver "validate_double_demo_linear_solver.dat")
ELSE (SCICELLXX_USES_DOUBLE_PRECISION)
     SET (VALIDATE_FILENAME_demo_linear_solver "validate_demo_linear_solver.dat")
ENDIF (SCICELLXX_USES_DOUBLE_PRECISION)
ADD_TEST(NAME TEST_demo_linear_solver_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_linear_solver} ${CMAKE_CURRENT_BINARY_DIR}/output_test.dat)

# ===========================================
# Test execution order
# ===========================================
SET_TESTS_PROPERTIES(TEST_demo_linear_solver_check_output PROPERTIES DEPENDS TEST_demo_linear_solver_run)