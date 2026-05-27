.. _dependency_management:

Dependency Management with Spack
================================

SciCell++ relies on several external C++ libraries (such as VTK, Armadillo, and OpenBLAS) to function properly. To make the setup seamless, SciCell++ provides a fully automated package management workflow using **Spack**.

Spack builds dependencies directly from source, allowing for deep hardware optimizations tailored to your specific CPU architecture (like AVX-512). It also supports completely offline deployments for air-gapped High-Performance Computing (HPC) clusters.

Conda environments are no longer supported for managing SciCell++ dependencies. If you have any active Conda environments, they must be fully deactivated before using the Spack-based setup.

HPC and Local Deployment using Spack
-------------------------------------

We provide an automated wrapper script, ``install_dependencies.sh``, to handle Spack installation, environment activation, concretization, and path configuration.

**Important:** Before starting, ensure that any Conda environment is deactivated!
   
.. code-block:: bash

   conda deactivate

The automated build scripts will abort execution if they detect an active Conda environment, to avoid compilation and linking conflicts.

1. Online Installation (With Internet Connection)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If your system has an internet connection, run the wrapper script from the project root:

.. code-block:: bash

   ./install_dependencies.sh

This script will automatically:
1. Clone Spack (if missing).
2. Load Spack's environment.
3. Activate the local environment configuration in ``spack.yaml``.
4. Concretize and install the dependencies (Armadillo, OpenBLAS, SuperLU, VTK, lcov).
5. Generate a symlinked view folder at ``spack-view/``.
6. Automatically create/update the ``configs/current`` path configurations.

Once completed, verify your configuration and build the framework:

.. code-block:: bash

   cat configs/current
   ./autogen.sh -t STATIC -b RELEASE

2. Offline Installation (Air-Gapped Clusters)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If the target compute/login nodes do not have internet access, you can use Spack's Mirror feature.

*Step 1: On a machine with Internet access*
Run the mirror creation option. This downloads all package tarballs without compiling them:

.. code-block:: bash

   ./install_dependencies.sh -m

This creates a folder called ``spack-mirror/`` containing the downloaded source files.

*Step 2: Transfer files*
Copy the entire ``scicellxx`` directory (including the generated ``spack-mirror/`` folder) to the offline machine or supercomputer via SCP, FTP, or storage drive.

*Step 3: On the offline machine / Supercomputer*
Run the offline installation option. Spack will ingest the local mirror and compile the libraries tailored to the supercomputer's hardware:

.. code-block:: bash

   ./install_dependencies.sh -o

Finally, compile the framework:

.. code-block:: bash

   ./autogen.sh -t STATIC -b RELEASE
