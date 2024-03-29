# Indicate source files and dependencies in the files
SET(SRC_demo_lotka_volterra demo_lotka_volterra.cpp cc_lotka_volterra_odes.cpp)

# Create executable (check whether compilation was requested or not)
IF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_lotka_volterra ${SRC_demo_lotka_volterra})
ELSE(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_lotka_volterra EXCLUDE_FROM_ALL ${SRC_demo_lotka_volterra})
ENDIF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_lotka_volterra ${LIB_scicellxx_lib})

# ... and link againts them  
TARGET_LINK_LIBRARIES(demo_lotka_volterra ${LIB_demo_lotka_volterra})

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
set_target_properties( demo_lotka_volterra
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
ADD_TEST(NAME TEST_demo_lotka_volterra_run
         COMMAND demo_lotka_volterra)
# Validate output
SET (VALIDATE_FILENAME_demo_lotka_volterra_euler "validate_euler.dat")
ADD_TEST(NAME TEST_demo_lotka_volterra_euler_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_lotka_volterra_euler} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/euler.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_lotka_volterra_rk4 "validate_rk4.dat")
ADD_TEST(NAME TEST_demo_lotka_volterra_rk4_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_lotka_volterra_rk4} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/rk4.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_lotka_volterra_am2pc "validate_am2pc.dat")
ADD_TEST(NAME TEST_demo_lotka_volterra_am2pc_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_lotka_volterra_am2pc} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/am2pc.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_lotka_volterra_bdf1 "validate_bdf1.dat")
ADD_TEST(NAME TEST_demo_lotka_volterra_bdf1_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_lotka_volterra_bdf1} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/bdf1.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_lotka_volterra_am2 "validate_am2.dat")
ADD_TEST(NAME TEST_demo_lotka_volterra_am2_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_lotka_volterra_am2} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/am2.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_lotka_volterra_bdf2 "validate_bdf2.dat")
ADD_TEST(NAME TEST_demo_lotka_volterra_bdf2_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_lotka_volterra_bdf2} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/bdf2.dat)

# ===========================================
# Test execution order
# ===========================================
SET_TESTS_PROPERTIES(TEST_demo_lotka_volterra_euler_check_output TEST_demo_lotka_volterra_rk4_check_output TEST_demo_lotka_volterra_am2pc_check_output TEST_demo_lotka_volterra_bdf1_check_output TEST_demo_lotka_volterra_am2_check_output TEST_demo_lotka_volterra_bdf2_check_output PROPERTIES DEPENDS TEST_demo_lotka_volterra_run)
