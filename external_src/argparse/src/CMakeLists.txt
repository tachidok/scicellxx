# Add source files in order of dependence, the ones with no dependency
# first then the others
SET(SRC_FILES argparse.cpp argparse_formatter.cpp argparse_util.cpp)

# Create a library with the above files based on the requested library
# version
IF(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
  ADD_LIBRARY(argparse_lib STATIC ${SRC_FILES})
ELSEIF(${SCICELLXX_LIB_TYPE} STREQUAL SHARED)
  ADD_LIBRARY(argparse_lib SHARED ${SRC_FILES})
ELSE(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
  ADD_LIBRARY(argparse_lib ${SRC_FILES})
ENDIF(${SCICELLXX_LIB_TYPE} STREQUAL STATIC)
