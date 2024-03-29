# Indicate source files and dependencies in the files
SET(SRC_demo_basic_3_body demo_basic_3_body.cpp cc_odes_basic_3_body.cpp)

# Create executable (check whether compilation was requested or not)
IF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_basic_3_body ${SRC_demo_basic_3_body})
ELSE(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)
  ADD_EXECUTABLE(demo_basic_3_body EXCLUDE_FROM_ALL ${SRC_demo_basic_3_body})
ENDIF(${SCICELLXX_BUILD_DEMOS} STREQUAL TRUE)

# Indicate linking libraries
SET(LIB_demo_basic_3_body ${LIB_scicellxx_lib})

# ... and link againts them  
TARGET_LINK_LIBRARIES(demo_basic_3_body ${LIB_demo_basic_3_body})

# Check if the output/bin directory exists
IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  # Then create the directory
  FILE(MAKE_DIRECTORY "${bin}")
ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Set directory where to create the executables
set_target_properties( demo_basic_3_body
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )

# ===========================================
# Tests section
# ===========================================
# Run the application to check it works
ADD_TEST(NAME TEST_demo_basic_3_body_run
          COMMAND demo_basic_3_body --test)
IF (SCICELLXX_USES_DOUBLE_PRECISION)
   SET (VALIDATE_FILENAME_demo_basic_3_body "validate_3_body.dat")
ELSE (SCICELLXX_USES_DOUBLE_PRECISION)
     SET (VALIDATE_FILENAME_demo_basic_3_body "validate_3_body.dat")
ENDIF (SCICELLXX_USES_DOUBLE_PRECISION)

ADD_TEST(NAME TEST_demo_basic_3_body_check_output
         COMMAND ${PROJECT_SOURCE_DIR}/tools/development/fpdiff.py ${CMAKE_CURRENT_SOURCE_DIR}/validate/${VALIDATE_FILENAME_demo_basic_3_body} ${CMAKE_CURRENT_BINARY_DIR}/output_test.dat)

# ===========================================
# Test execution order
# ===========================================
SET_TESTS_PROPERTIES(TEST_demo_basic_3_body_check_output PROPERTIES DEPENDS TEST_demo_basic_3_body_run)
