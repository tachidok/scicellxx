# Add source files in order of dependence, the ones with no dependency
# first then the others
#SET(SRC_FILES ac_interpolator.cpp cc_newton_interpolator.cpp cc_newtons_method.tpl.cpp ac_problem.cpp ac_ivp_for_odes.cpp ac_mesh_free_problem.tpl.cpp)
SET(SRC_FILES ac_problem.cpp ac_ibvp.tpl.cpp)

# Include current directory to the path
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library with the above files based on the requested library
# version
IF(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
 ADD_LIBRARY(problem_lib STATIC ${SRC_FILES})
ELSEIF(${SCICELLXX_LIB_TYPE} STREQUAL SHARED)
 ADD_LIBRARY(problem_lib SHARED ${SRC_FILES})
ELSE(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
 ADD_LIBRARY(problem_lib ${SRC_FILES})
ENDIF(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
