# Add sub-folders of private developments
ADD_SUBDIRECTORY(mesh_free)
ADD_SUBDIRECTORY(lotka_volterra)
IF (SCICELLXX_USES_MPI)
   ADD_SUBDIRECTORY(mTASEP)
ENDIF (SCICELLXX_USES_MPI)
