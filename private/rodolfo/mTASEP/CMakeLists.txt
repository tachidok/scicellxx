# Indicate source files and dependencies in the files
find_package(CUDAToolkit REQUIRED)

if(CUDAToolkit_FOUND)
    message(STATUS "CUDAToolkit encontrado: ${CUDAToolkit_INCLUDE_DIRS}")
endif()

SET(SRC_demo_mTASEP_LK demo_mTASEP_LK_CUDA.cu)

# Do not create executable but add them to the 'make' list
ADD_EXECUTABLE(rodolfo_demo_mTASEP_LK EXCLUDE_FROM_ALL ${SRC_demo_mTASEP_LK})

SET(LIB_demo_mTASEP_LK ${LIB_scicellxx_lib})

# GPU Architecture
set_target_properties(rodolfo_demo_mTASEP_LK PROPERTIES CUDA_ARCHITECTURES 86)

# Suppress "type qualifier on return type is meaningless" warning (815-D)
target_compile_options(rodolfo_demo_mTASEP_LK PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-Xcudafe --diag_suppress=815>)

# Include directories
include_directories(${CUDAToolkit_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

# Linking
target_link_libraries(rodolfo_demo_mTASEP_LK ${CUDAToolkit_LIBRARIES})
TARGET_LINK_LIBRARIES(rodolfo_demo_mTASEP_LK PUBLIC ${LIB_demo_mTASEP_LK})
target_link_libraries(rodolfo_demo_mTASEP_LK PRIVATE CUDA::cudart CUDA::curand)

IF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
ENDIF()

set_target_properties( rodolfo_demo_mTASEP_LK
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
  )
