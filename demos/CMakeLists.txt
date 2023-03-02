# Add directories with demo/test cases
ADD_SUBDIRECTORY(argparse)
ADD_SUBDIRECTORY(interpolation)
ADD_SUBDIRECTORY(matrices)
ADD_SUBDIRECTORY(linear_solvers)
ADD_SUBDIRECTORY(newtons_method)
ADD_SUBDIRECTORY(odes)
IF (SCICELLXX_USES_MPI)
   ADD_SUBDIRECTORY(mpi)
ENDIF (SCICELLXX_USES_MPI)

