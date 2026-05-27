#!/bin/bash
# install_dependencies.sh
# Automates the installation of SciCell++ dependencies using Spack.
# Also handles generating a mirror for offline supercomputers.

set -e

usage() {
    cat << EOF
usage: $0 [OPTIONS]

This script prepares the dependencies for SciCell++ using Spack.

OPTIONS:
   -h      Show this message
   -m      Create an offline mirror of dependencies (Run this ON an internet-connected machine)
   -o      Offline install: Install from the local mirror instead of downloading
EOF
    exit 1
}

OFFLINE_MODE=0
CREATE_MIRROR=0

while getopts "hmo" OPTION; do
    case $OPTION in
        h) usage ;;
        m) CREATE_MIRROR=1 ;;
        o) OFFLINE_MODE=1 ;;
        ?) usage ;;
    esac
done

if [[ -n "$CONDA_DEFAULT_ENV" || -n "$CONDA_PREFIX" ]]; then
    echo "[ERROR] A Conda environment ($CONDA_DEFAULT_ENV) is currently active."
    echo "Mixing Spack with Conda can lead to severe linking errors and compiler conflicts."
    echo "Please run 'conda deactivate' (possibly multiple times) to fully exit the Conda environment before running this script."
    echo ""
    exit 1
fi

echo "============================================================="
echo "        SciCell++ Dependency Manager (via Spack)"
echo "============================================================="
echo ""

# Check if spack is in the PATH
if ! command -v spack &> /dev/null; then
    echo "Spack is not in your PATH."
    if [ ! -d "spack" ]; then
        if [ "$OFFLINE_MODE" -eq 1 ]; then
            echo "[ERROR] You are in offline mode, but spack is not cloned into ./spack/"
            exit 1
        fi
        echo "Cloning Spack (latest version)..."
        git clone -c feature.manyFiles=true https://github.com/spack/spack.git
    fi
    echo "Loading Spack..."
    source spack/share/spack/setup-env.sh
fi

echo "Spack initialized: $(spack --version)"

# Create and sync the environment from the spack.yaml
echo "Activating Spack environment in the current directory..."
spack env activate -p .

if [ "$CREATE_MIRROR" -eq 1 ]; then
    echo "============================================================="
    echo "Creating offline mirror in ./spack-mirror ..."
    echo "This will download all tarballs needed without compiling them."
    echo "============================================================="
    spack mirror create -d ./spack-mirror --all
    echo "Mirror created! You can now copy this entire directory to your"
    echo "supercomputer and run: ./install_dependencies.sh -o"
    exit 0
fi

if [ "$OFFLINE_MODE" -eq 1 ]; then
    echo "============================================================="
    echo "Offline Mode: Adding local mirror ..."
    echo "============================================================="
    if [ ! -d "./spack-mirror" ]; then
        echo "[ERROR] ./spack-mirror directory not found."
        echo "Please run './install_dependencies.sh -m' on a connected machine first, then copy it here."
        exit 1
    fi
    # Add the local directory as a spack mirror
    spack mirror add --scope env:. local_offline_mirror file://$(pwd)/spack-mirror
fi

echo "============================================================="
echo "Concretizing and Installing Dependencies..."
echo "============================================================="
spack concretize -f
spack install

echo "============================================================="
echo "Updating configuration for autogen.sh ..."
echo "============================================================="
# Find the view directory where everything is linked
VIEW_DIR="$(pwd)/spack-view"

CONFIG_FILE="./configs/current"
echo "Creating/Updating $CONFIG_FILE ..."

cat << EOF > $CONFIG_FILE
SCICELLXX_LIB_TYPE=STATIC
SCICELLXX_RANGE_CHECK=TRUE
SCICELLXX_USES_DOUBLE_PRECISION=TRUE

SCICELLXX_USES_ARMADILLO=TRUE
SCICELLXX_AUTO_FIND_ARMADILLO_PATHS=TRUE
ARMADILLO_AUTO_FIND_FOLDER=$VIEW_DIR
ARMADILLO_INCLUDE_DIRS=$VIEW_DIR/include
# The .so name may differ depending on Spack version, but pointing to the view allows CMake to find it.

SCICELLXX_USES_VTK=TRUE
SCICELLXX_AUTO_FIND_VTK_PATHS=TRUE
VTK_AUTO_FIND_FOLDER=$VIEW_DIR

SCICELLXX_PANIC_MODE=TRUE
SCICELLXX_USES_MPI=TRUE
SCICELLXX_AUTO_FIND_MPI_PATHS=TRUE
MPI_AUTO_FIND_FOLDER=$VIEW_DIR
EOF

echo "Done! The configuration file configs/current has been pointed to the Spack View."
echo "Dependencies like numerical_recipes & argparse will still be built from external_src."
echo "You can now run: ./autogen.sh"
