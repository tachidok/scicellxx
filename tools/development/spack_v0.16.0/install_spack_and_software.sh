git clone -c feature.manyFiles=true https://github.com/spack/spack.git
cd spack
git checkout releases/v0.18
. share/spack/setup-env.sh
spack env create myenv
spack compiler find
spack install gcc
spack install cmake
spack install git
spack install openblas threads=openmp
spack install superlu****ERROR
spack install armadillo****ERROR
