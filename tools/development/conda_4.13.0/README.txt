**********************************************************
Comments on installation for openmpi on cuda
**********************************************************
(scicellxx) tachidok@tachidok-X411UN-ubuntu22:~/local/working/research/scicellxx$ conda install -c conda-forge openmpi=4.1.2
Collecting package metadata (current_repodata.json): done
Solving environment: failed with initial frozen solve. Retrying with flexible solve.
Collecting package metadata (repodata.json): done
Solving environment: done

## Package Plan ##

  environment location: /home/tachidok/anaconda3/envs/scicellxx

  added / updated specs:
    - openmpi=4.1.2


The following packages will be downloaded:

    package                    |            build
    ---------------------------|-----------------
    mpi-1.0                    |          openmpi           4 KB  conda-forge
    openmpi-4.1.2              |       hbfc84c5_0         4.3 MB  conda-forge
    ------------------------------------------------------------
                                           Total:         4.3 MB

The following NEW packages will be INSTALLED:

  mpi                conda-forge/linux-64::mpi-1.0-openmpi
  openmpi            conda-forge/linux-64::openmpi-4.1.2-hbfc84c5_0
  python_abi         conda-forge/linux-64::python_abi-3.9-2_cp39

The following packages will be UPDATED:

  ca-certificates    pkgs/main::ca-certificates-2022.4.26-~ --> conda-forge::ca-certificates-2022.5.18.1-ha878542_0

The following packages will be SUPERSEDED by a higher-priority channel:

  certifi            pkgs/main::certifi-2022.5.18.1-py39h0~ --> conda-forge::certifi-2022.5.18.1-py39hf3d152e_0
  openssl              pkgs/main::openssl-1.1.1o-h7f8727e_0 --> conda-forge::openssl-1.1.1o-h166bdaf_0


Proceed ([y]/n)? y


Downloading and Extracting Packages
mpi-1.0              | 4 KB      | ############################################################################################################### | 100% 
openmpi-4.1.2        | 4.3 MB    | ############################################################################################################### | 100% 
Preparing transaction: done
Verifying transaction: done
Executing transaction: \  
For Linux 64, Open MPI is built with CUDA awareness but this support is disabled by default.
To enable it, please set the environment variable OMPI_MCA_opal_cuda_support=true before
launching your MPI processes. Equivalently, you can set the MCA parameter in the command line:
mpiexec --mca opal_cuda_support 1 ...
 
In addition, the UCX support is also built but disabled by default.
To enable it, first install UCX (conda install -c conda-forge ucx). Then, set the environment
variables OMPI_MCA_pml="ucx" OMPI_MCA_osc="ucx" before launching your MPI processes.
Equivalently, you can set the MCA parameters in the command line:
mpiexec --mca pml ucx --mca osc ucx ...
Note that you might also need to set UCX_MEMTYPE_CACHE=n for CUDA awareness via UCX.
Please consult UCX's documentation for detail.
 

done

