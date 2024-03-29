# Indicate source files and dependencies in the files
SET(SRC_demo_adaptive_basic_ode demo_adaptive_basic_ode.cpp)

# Create executable (check whether compilation was requested or not)
IF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_adaptive_basic_ode ${SRC_demo_adaptive_basic_ode})
ELSE(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_adaptive_basic_ode EXCLUDE_FROM_ALL ${SRC_demo_adaptive_basic_ode})
ENDIF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_adaptive_basic_ode ${LIB_scicellxx_lib})

# ... and link againts them  
TARGET_LINK_LIBRARIES(demo_adaptive_basic_ode ${LIB_demo_adaptive_basic_ode})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/RESLT)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/RESLT")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/RESLT)

# Set directory where to create the executables
set_target_properties( demo_adaptive_basic_ode
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
ADD_TEST(NAME TEST_demo_adaptive_basic_ode_run
         COMMAND demo_adaptive_basic_ode)
# Validate output
SET (VALIDATE_FILENAME_demo_adaptive_basic_ode_RK45F "validate_rk45f.dat")
ADD_TEST(NAME TEST_demo_adaptive_basic_ode_RK45F_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_adaptive_basic_ode_RK45F} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/rk45f.dat)

# Validate output
SET (VALIDATE_FILENAME_demo_adaptive_basic_ode_RK45DP "validate_rk45dp.dat")
ADD_TEST(NAME TEST_demo_adaptive_basic_ode_RK45DP_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_adaptive_basic_ode_RK45DP} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/rk45dp.dat)

# ===========================================
# Test execution order
# ===========================================
SET_TESTS_PROPERTIES(TEST_demo_adaptive_basic_ode_RK45F_check_output TEST_demo_adaptive_basic_ode_RK45DP_check_output PROPERTIES DEPENDS TEST_demo_adaptive_basic_ode_run)
