#! /bin/sh

# conda to docker image
# https://blog.ceshine.net/post/replicate-conda-environment-in-docker/

conda update -n base -c defaults conda
conda create --name scicellxx python=3.9.12
conda activate scicellxx
----conda install --yes -c conda-forge gxx=11.2.0
conda install --yes -c conda-forge gcc=9.4.0 gxx=9.4.0
conda install --yes -c conda-forge cmake=3.23.2
conda install --yes -c conda-forge git=2.35.3
conda install --yes -c conda-forge openblas=0.3.20=openmp_h53a8fd6_0
conda install --yes -c conda-forge openmpi=4.1.2
conda install --yes -c conda-forge superlu=5.2.2
conda install --yes -c conda-forge superlu_dist=7.2.0
conda install --yes -c conda-forge armadillo=11.1.1
conda install --yes -c conda-forge trilinos=12.18.1
conda install --yes -c conda-forge libgcc-ng=11.2.0

# Remove unused packages and tarfiles
conda clean --all

******** DOES NOT WORK
Checking this
https://stackoverflow.com/questions/31170869/cmake-could-not-find-opengl-in-ubuntu/47926071#47926071
sudo apt-get install libegl1-mesa-dev
sudo apt-get install libgl1-mesa-dev 
sudo apt-get install mesa-common-dev
******** DOES NOT WORK


https://github.com/pmh47/dirt/issues/47
sudo apt-get install libglu1-mesa-dev
sudo apt-get libgl1-mesa-dev
sudo apt-get install mesa-common-dev


Need to check this
----conda install --yes -c conda-forge freeglut
----conda install --yes -c conda-forge vtk=9.1.0
DOES NOT WORK

----conda install --yes -c conda-forge vtk=9.0.0

Search for packages and info
conda search -c conda-forge freeglut --info


VTK UNINSTALLED
- Requieres to check for errors with opengl lib



There is a problem when running CMake it does not find openGL libs required for VTK, it seems that this is a well known issue for conda

https://github.com/mantidproject/conda-recipes/issues/43

The suggestion is to install freeglut and check what happens
conda install --yes -c conda-forge freeglut

*****************************************
Try installing Qt5
pip install PtQt5
** DOES NOT WORK
*****************************************



CMake Error at /home/tachidok/anaconda3/envs/scicellxx/share/cmake-3.23/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find OpenGL (missing: OPENGL_opengl_LIBRARY OPENGL_egl_LIBRARY
  OpenGL EGL)
Call Stack (most recent call first):
  /home/tachidok/anaconda3/envs/scicellxx/share/cmake-3.23/Modules/FindPackageHandleStandardArgs.cmake:594 (_FPHSA_FAILURE_MESSAGE)
  /home/tachidok/anaconda3/envs/scicellxx/lib/cmake/vtk-9.1/patches/99/FindOpenGL.cmake:495 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
  /home/tachidok/anaconda3/envs/scicellxx/lib/cmake/vtk-9.1/VTK-vtk-module-find-packages.cmake:162 (find_package)
  /home/tachidok/anaconda3/envs/scicellxx/lib/cmake/vtk-9.1/vtk-config.cmake:150 (include)
  CMakeLists.txt:108 (FIND_PACKAGE)
  
  
  CMake Error at /usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find OpenGL (missing: OPENGL_opengl_LIBRARY OPENGL_egl_LIBRARY
  OpenGL EGL)
Call Stack (most recent call first):
  /usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:594 (_FPHSA_FAILURE_MESSAGE)
  /home/tachidok/anaconda3/envs/scicellxx/lib/cmake/vtk-9.1/patches/99/FindOpenGL.cmake:495 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
  /home/tachidok/anaconda3/envs/scicellxx/lib/cmake/vtk-9.1/VTK-vtk-module-find-packages.cmake:162 (find_package)
  /home/tachidok/anaconda3/envs/scicellxx/lib/cmake/vtk-9.1/vtk-config.cmake:150 (include)
  CMakeLists.txt:108 (FIND_PACKAGE)

  

