# Indicate source files and dependencies in the files
SET(SRC_demo_basic_ode_stability_analysis demo_basic_ode_stability_analysis.cpp)

# Create executable (check whether compilation was requested or not)
IF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_basic_ode_stability_analysis ${SRC_demo_basic_ode_stability_analysis})
ELSE(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_basic_ode_stability_analysis EXCLUDE_FROM_ALL ${SRC_demo_basic_ode_stability_analysis})
ENDIF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_basic_ode_stability_analysis ${LIB_scicellxx_lib})

# Check whether scicellxx is using Armadillo
IF (SCICELLXX_USES_ARMADILLO)
 LIST(APPEND LIB_demo_basic_ode_stability_analysis ${ARMADILLO_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
ENDIF (SCICELLXX_USES_ARMADILLO)

# ... and link againts them  
TARGET_LINK_LIBRARIES(demo_basic_ode_stability_analysis ${LIB_demo_basic_ode_stability_analysis})

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
set_target_properties( demo_basic_ode_stability_analysis
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
SET (VALIDATE_TEST_FOLDER_demo_basic_ode_stability_analysis "${CMAKE_CURRENT_SOURCE_DIR}/bin/")
ADD_TEST(NAME TEST_demo_basic_ode_stability_analysis_run
  COMMAND ${VALIDATE_TEST_FOLDER_demo_basic_ode_stability_analysis}demo_basic_ode_stability_analysis)
# Validate output
SET (VALIDATE_FILENAME_demo_basic_ode_stability_analysis_euler "validate_euler_stability.dat")
ADD_TEST(NAME TEST_demo_basic_ode_stability_analysis_euler_check_output
  COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_ode_stability_analysis_euler} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/euler_stability.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_basic_ode_stability_analysis_rk4 "validate_rk4_stability.dat")
ADD_TEST(NAME TEST_demo_basic_ode_stability_analysis_rk4_check_output
  COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_ode_stability_analysis_rk4} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/rk4_stability.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_basic_ode_stability_analysis_am2pc "validate_am2pc_stability.dat")
ADD_TEST(NAME TEST_demo_basic_ode_stability_analysis_am2pc_check_output
  COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_ode_stability_analysis_am2pc} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/am2pc_stability.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_basic_ode_stability_analysis_bdf1 "validate_bdf1_stability.dat")
ADD_TEST(NAME TEST_demo_basic_ode_stability_analysis_bdf1_check_output
  COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_ode_stability_analysis_bdf1} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/bdf1_stability.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_basic_ode_stability_analysis_am2 "validate_am2_stability.dat")
ADD_TEST(NAME TEST_demo_basic_ode_stability_analysis_am2_check_output
  COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_ode_stability_analysis_am2} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/am2_stability.dat)
# Validate output
SET (VALIDATE_FILENAME_demo_basic_ode_stability_analysis_bdf2 "validate_bdf2_stability.dat")
ADD_TEST(NAME TEST_demo_basic_ode_stability_analysis_bdf2_check_output
  COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_ode_stability_analysis_bdf2} ${CMAKE_CURRENT_BINARY_DIR}/RESLT/bdf2_stability.dat)

# ===========================================
# Test execution order
# ===========================================
SET_TESTS_PROPERTIES(TEST_demo_basic_ode_stability_analysis_euler_check_output TEST_demo_basic_ode_stability_analysis_rk4_check_output TEST_demo_basic_ode_stability_analysis_am2pc_check_output TEST_demo_basic_ode_stability_analysis_bdf1_check_output TEST_demo_basic_ode_stability_analysis_am2_check_output TEST_demo_basic_ode_stability_analysis_bdf2_check_output PROPERTIES DEPENDS TEST_demo_basic_ode_stability_analysis_run)
