# Add source files in order of dependence, the ones with no dependency
# first then the others
SET(SRC_FILES cc_newtons_method.cpp)

# Include current directory to the path
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Create a library with the above files based on the requested library
# version
IF(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
 ADD_LIBRARY(newtons_method_lib STATIC ${SRC_FILES})
ELSEIF(${SCICELLXX_LIB_TYPE} STREQUAL SHARED)
 ADD_LIBRARY(newtons_method_lib SHARED ${SRC_FILES})
ELSE(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
 ADD_LIBRARY(newtons_method_lib ${SRC_FILES})
ENDIF(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
