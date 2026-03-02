Linux installation
==================
             
This section presents instructions for the installation of SciCell++
on a linux type system. The instructions were tested on Ubuntu 18.04
but we (hopefully) expect them to work on recent versions as well.
Once finished this section you should continue with the
:doc:`../start_scicellxx/start_scicellxx_linux` document.

Overview
--------

1. :ref:`Install Docker <install-docker-label_linux_installation.rst>`
2. :ref:`Install Git <install-git-label_linux_installation.rst>`
3. :ref:`Get your own copy of SciCell++ from GitHub
   <get_own_copy_scicellxx-label_linux_installation.rst>`

.. _install-docker-label_linux_installation.rst:
   
Install Docker
--------------

Follow the instructions in the `docker official installation website
<https://docs.docker.com/engine/install/>`_ to install Docker in your
system.

.. _install-git-label_linux_installation.rst:

Install Git
-----------

Follow the instructions in the `Git <https://git-scm.com/>`_ official
webpage for your system.

.. _get_own_copy_scicellxx-label_linux_installation.rst:

Get your own copu of SciCell++ from GitHub
------------------------------------------

1. Open a command line and type the following:

   .. code-block:: shell
   
                   git clone https://github.com/tachidok/scicellxx
                   cd scicellxx
                   git checkout -b john_cool

   The previous commands get a copy of SciCell++ from the `official
   GitHub repository <https://github.com/tachidok/scicellxx>`_ down to
   your local machine and moves into the ``scicellxx``
   folder. Finally, a new branch named ``john_cool`` is generated.

   .. note::

      Feel free to use your own name for the newly created folder.

From here on you can continue with the
:doc:`../start_scicellxx/start_scicellxx_linux` document.

.. _advanced_installation-label_linux_installation.rst:
   
Advanced installation
=====================

This type of installation gives you full customization of the software
and hardware resources in your machine.

.. note::

   Currently only Unix based systems are considerd for this type of
   installation. The steps provided in this section have been tested
   in Ubuntu 16.04 LTS and Ubuntu 18.04 LTS.

.. note::
   
   If you use `Spack <https://spack.readthedocs.io/en/latest/>`_ as
   your software package manager we provide you with our spack
   :download:`file <./spack.yaml>` (tested with spack version
   0.16.0). Feel free to use it to ease the installation process and
   skip this section.

   .. code-block:: shell

                   spack env create myenv spack.yaml
                   spack env activate -p myenv
                   spack end deactivate

Overview
--------
   
**Software packages requirements**

* `Git <https://git-scm.com/>`_ to get a copy of SciCell++ in your
  system.

* A **C++ compiler** to build the SciCell++ and the demos. We tested
  with `gcc <https://gcc.gnu.org/>`_ version 7.4.0.
  
* The `CMake <https://cmake.org/>`_ tool to configure and install
  SciCell++. We tested with CMake version 3.10.2.

* The `Python <https://www.python.org/>`_ language to run some of the
  demos and the unit test. We include Python based scripts to plot the
  result for some demos . Tested with Python version 3.7.3.

You need to manually install the previous packages, we suggest to use
the docker based installation if you are not familiar with Unix based
systems.

The following software packages are optional (but recommended)
  
* `Doxygen <https://www.doxygen.nl/index.html>`_ to build
  documentation and classes diagrams from source code for SciCell++.

* `Latex <https://www.latex-project.org/>`_ to generate *math* symbols
  in documentation generated from source code.

* A software implementation of **MPI** to support parallel features
  (openmpi/mpicc recommended - not currently supported-).

**Steps**
   
The following step guide you through the installation process:

1. Get your own copy of SciCell++

   Open a command line and type in the following:

   .. code-block:: shell
   
                   git clone https://github.com/tachidok/scicellxx
                   cd scicellxx
                   git checkout -b john_cool

   The previous commands get a copy of SciCell++ from the `official
   GitHub repository <https://github.com/tachidok/scicellxx>`_ down to
   your local machine and moves into the ``scicellxx``
   folder. Finally, a new branch named ``john_cool`` is generated.

   .. note::

      Feel free to rename the ``john_cool`` folder with your name.

That is it, now you can move to the configuration of SciCell++ section.


Add the ``bin`` folder of SciCell++ to your ``PATH`` variable
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This would allow you to execute any scripts in the ``bin`` folder of
SciCell++ without specifying the full path of the scripts.

1. Add the following line at the end of your ``.bashrc`` file from
   your home folder.

   .. code-block:: shell

      export PATH="/path/to/your/scicellxx/installation/bin/:$PATH"
   
.. _external-packages-installation-label_installation.rst:
   
External packages installation
------------------------------

If you want to get the maximum performance for SciCell++ you will need
to install some or all of the following packages:

* :ref:`Doxygen <doxygen-installation-label_installation.rst>`
* :ref:`OpenBLAS <openblas-installation-label_installation.rst>`
* :ref:`SuperLU <superlu-installation-label_installation.rst>` (requires OpenBLAS)
* :ref:`Armadillo <armadillo-installation-label_installation.rst>` (it is recommended to previously install SuperLU)
* :ref:`VTK <vtk-installation-label_installation.rst>` (for visuallisation purposes, we use it with `Paraview <https://www.paraview.org/>`_)


.. note::

   You do not need to install these packages for basic use of
   SciCell++, install them only if you require additional features.

.. note::
   
   If you used our provided spack :download:`file <./spack.yaml>` at
   the :ref:`installation
   <doxygen-installation-label_installation.rst>` step then you
   already have these ones as well. You may skip this section.

.. note:: Please note that the provided instructions were tested in
   the following distributions of Ubuntu:

   * Ubuntu 16.04 LTS 64 bits
     
   * Ubuntu 18.04.2 LTS 64 bits
     
   * Ubuntu 18.04.5 LTS 64 bits

**General requirements**

1. Before installing any of the external libraries in your system
   ensure that none of them is already installed in your system, this
   may produce crashes between versions.

   You may remove the installed packages with the following command:

   .. code-block:: shell

      sudo apt-get remove --purge <package-name>

   where `<package-name>` should be substituted by the name of the
   package you want to remove.
   
2. Install the following packages in your system.

   .. note:: We provide the version of each package that we used for
             the installation. You could check the available version
             of a package for your system by typing:

             .. code-block:: shell

                apt-cache policy <package-name>

             where `<package-name>` should be substituted by the name
             of the package which you want to check its version.
   
  **Ubuntu 16.04 LTS 64 bits**
     
     + cmake (cmake 3.5.1-1ubuntu3)
       
     + liblapack (liblapack-dev 3.6.0-2ubuntu2)
       
     + libarpack (libarpack2, libarpack2-dev 3.3.0-1build2)

   Install them by typing:

   .. code-block:: shell

      sudo apt-get install cmake liblapack-dev libarpack2 libarpack2-dev

  **Ubuntu 18.04.2 LTS 64 bits**
     
     + cmake 3.10.2
       
     + liblapack (liblapack3 3.7.1-4ubuntu1, liblapack-dev
       3.7.1-4ubuntu1)
       
     + libarpack (libarpack2 3.5.0+real-2, libarpack2-dev
       3.5.0+real-2)

   Install them by typing:

   .. code-block:: shell

      sudo apt-get install cmake liblapack3 liblapack-dev libarpack2 libarpack2-dev
       
3. Once installed follow the order below for installing the external
   packages

.. _doxygen-installation-label_installation.rst:
   
Doxygen
^^^^^^^

`Doxygen <https://www.doxygen.nl/index.html>`_ is a documentation
generator from source code. The source code of SciCell++ is documented
following Doxygen directives, if you want to create documentation from
the source code then install Doxygen and graphviz.

**Steps**

1. Open a terminal and type

.. code-block:: shell

  sudo apt-get install doxygen
  sudo apt-get install graphviz

.. note:: Tested versions with Ubuntu 18.04.2 LTS 64 bits:
          
          * doxygen 1.8.13-10
          * graphviz 2.40.1-2

.. _openblas-installation-label_installation.rst:
   
OpenBLAS
^^^^^^^^

OpenBLAS is an optimised version of the Basic Linear Algebra
Subprograms (BLAS). This section guides you through the installation
of ``OpenBLAS 0.2.20`` on the following Ubuntu distributions:

* Ubuntu 16.04 LTS 64 bits
* Ubuntu 18.04.2 LTS 64 bits
* Ubuntu 18.04.5 LTS 64 bits

.. note:: Please refer to the `OpenBLAS project original documentation
          <https://www.openblas.net/>`_ in case you have problems with
          the installation.

**Requirements**

* Double-check that no previous installation of OpenBLAS is part of
  your system. If that is the case we recommend you to uninstall them
  before continue.

  **Ubuntu 16.04 LTS 64 bits**

  Packages to install:
  
  * libblas-dev (libblas-common, libblas-dev 3.6.0-2ubuntu2)

  Open a terminal and type the following:

  .. code-block:: shell

     sudo apt-get install libblas-common libblas-dev
     
  **Ubuntu 18.04.2 LTS 64 bits**

  Packages to install:

  * libblas-dev (libblas3 3.7.1-4ubuntu1, libblas-dev 3.7.1-4ubuntu1)

  Open a terminal and type the following:

  .. code-block:: shell
  
     sudo apt-get install libblas3 libblas-dev

**Steps**

1. Extract the compressed file in
   ``scicellxx/external_src/openBLAS/OpenBLAS-0.2.20.tar.gz`` in a folder.

  .. warning:: We recommend you to extract it out of the ``scicellxx``
               project folder to avoid adding the files to the git
               repository. If you do extract it in the ``scicellxx``
               project folder then do not commit that folder within
               the project.
  
2. Open a terminal and go into the folder where you extracted the
   files, then type

   .. code-block:: shell

                   make

   .. note:: You can try with ``make -j <number_of_processors>`` to
             use more processors at compilation time.

3. Once compilation is finished type the following to start the
   installation process

   .. code-block:: shell

                   mkdir installation
                   make PREFIX=./installation install
                   
   .. note:: If you prefer you can specify a different installation
             foilder as follow:

             .. code-block:: shell

                             make PREFIX=/path/to/your/installation install
                

.. _superlu-installation-label_installation.rst:

SuperLU
^^^^^^^

SuperLU is a library for the direct solution of large, sparse,
nonsymmetric systems of linear equations. This section guides you
through the installation of ``SuperLU 5.2.0`` on the following Ubuntu
distributions:

* Ubuntu 16.04 LTS 64 bits
* Ubuntu 18.04.2 LTS 64 bits

.. note:: Please refer to the `SuperLU original documentation
          <https://portal.nersc.gov/project/sparse/superlu/>`_ in case
          you have problems with the installation.
  
**Requirements**

* Double-check that no previous installation of SuperLU is part of
  your system. If that is the case we recommend you to uninstall them
  before continue.

* You must have cmake installed in your system. Please refer to
  :ref:`that section
  <external-packages-installation-label_installation.rst>` to ensure
  its installation.

**Steps**

1. Extract the compressed file
   ``/external_src/superLU/superlu_5.2.0.tar.gz`` in a folder.

  .. warning:: We recommend you to extract it out of the ``scicellxx``
               project folder to avoid adding the files to the git
               repository. If you do extract it in the ``scicellxx``
               project folder then do not commit that folder within
               the project.

  .. note:: If you are installing Armadillo with SuperLU support then
            you need to install SuperLU with the flag ``-fPIC`` (which
            stands for `Position Independent Code`), to do so open the
            ``CMakeLists.txt`` file in the folder where you extracted
            ``SuperLU``, edit the line where ``CFLAGS`` are added (it
            should be line ``68`` for the version we supply you. It
            should look something like this.

            .. code-block:: shell
                            
               set(CMAKE_C_FLAGS "-fPIC -DPRNTlevel=0 -DAdd_ ${CMAKE_C_FLAGS}")
                  
2. Open a terminal and go into the folder where you extracted the
   files, then type

   .. code-block:: shell
                   
      mkdir build
      cd build
      cmake .. -DCMAKE_INSTALL_PREFIX=../lib
                   
   the last line indicates where to save the installation, here we use
   the ``lib`` folder of the SuperLU directory. If you have root
   privileges then you may not need to specify a value for the
   ``CMAKE_INSTALL_PREFIX`` variable.

3. Once cmake finished its configuration type

   .. code-block:: shell

      make
      make install

   .. note:: You can try with ``make -j <number_of_processors>`` to
             use more processors at compilation time.
      
**Run tests**

1. Run the following command in the same folder where you extracted
   SuperLU:

   .. code-block:: shell

      ctest
                   
   The results of the testing process can be found in the folowing
   files:

   ========================  ========================
   build/TESTING/s_test.out  single precision real
   build/TESTING/d_test.out  double precision real
   build/TESTING/c_test.out  single precision complex
   build/TESTING/z_test.out  double precision complex
   ========================  ========================
         
.. _armadillo-installation-label_installation.rst:

Armadillo
^^^^^^^^^

Armadillo is a C++ library for linear algebra and scientific
computing. This section guides you through the installation of
``Armadillo 8.300.3`` on the following Ubuntu distributions:

* Ubuntu 16.04 LTS 64 bits
* Ubuntu 18.04.2 LTS 64 bits

.. note:: Please refer to the `Armadillo original documentation
          <http://arma.sourceforge.net/>`_ in case you have problems
          with the installation.

**Requirements**

* Double-check that no previous installation of Armadillo is in your
  system. If that is the case we recommend you to uninstall them
  before continue.

* You must have cmake installed in your system. Please refer to :ref:`that section <external-packages-installation-label_installation.rst>` to ensure its installation.

* This installation assumes you have SuperLU 5.2.0 already installed
  in your system, if that is not the case then :ref:`install SuperLU
  <superlu-installation-label_installation.rst>` and return to this
  point.

.. note:: According to Armadillo's documentation, LAPACK and BLAS are
          used to work with dense matrices, meanwhile ARPACK and
          SuperLU are used to work with sparese matrices. If you want
          to use SuperLU then you need to stick to version 5.2 (check
          the README.txt file at line 146 of Armadillo
          documentation). If you want to use OpenMP then make sure you
          are using version 3.1 or newer (check the README.txt file at
          line 372 of Armadillo documentation).

**Steps**

1. Extract the compressed file ``/external_src/armadillo/armadillo-8.300.3.tar.xz`` in a folder

  .. warning:: We recommend you to extract it out of the ``scicellxx``
               project folder to avoid adding the files to the git
               repository. If you do extract it in the ``scicellxx``
               project folder then do not commit that folder within
               the project.

2. Open a terminal and go into the folder where you extracted the
   files, then type

   .. code-block:: shell

      mkdir installation
      cmake . \
      -DCMAKE_INSTALL_PREFIX=lib \
      -DSuperLU_INCLUDE_DIR=path_to_SuperLU_include_directory \
      -DSuperLU_LIBRARY=path_to_SuperLU_library \
      -Dopenblas_LIBRARY=path_to_openBLAS_library

   .. note:: Observe that you need to substitute with the paths on your particular machine. If you just followed the instructions on the previous sections then you may use the following configuration:

      .. code-block:: shell
             
         mkdir installation
         cmake . \
         -DCMAKE_INSTALL_PREFIX=installation \
         -DSuperLU_INCLUDE_DIR=../SuperLU_5.2.0/installation/include \
         -DSuperLU_LIBRARY=../SuperLU_5.2.0/installation/lib/libsuperlu.a \
         -Dopenblas_LIBRARY=../OpenBLAS-0.2.20/installation/lib/libopenblas.a
                
   .. note:: If you use ``cmake-gui`` then you can configure the above
             variables there as well.

3. Once cmake finished its configuration type

   .. code-block:: shell

      make
      make install

   .. note:: You can try with ``make -j <number_of_processors>`` to
             use more processors at compilation time.
   
   .. note:: In previous versions we required to indicate the
             installation directory at the ``make`` command , if you
             need to do it here is how, otherwise, just ignore these
             lines

             .. code-block:: shell

                make install DESTDIR=my_installation_directory

4. Add the library path to the environment variable ``LD_LIBRARY_PATH``. To do so open a terminal and type

  .. code-block:: shell

     LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/armadillo/installation/lib/folder

  .. note:: If you want to add this path "permanently" to your user
            add these lines to your ``~/.profile`` file:
  
            .. code-block:: shell

               export LD_LIBRARY_PATH=/path/to/armadillo/installation/lib/folder
                     
            and relogin.

  .. note:: Observe that you need to specify the path for your current
            machine. Here are the values I use for my personal
            computer:

            .. code-block:: shell
 
               export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/tachidok/local/working/my_stuff/armadillo-8.300.3/installation/lib/

            The following is the line I added to my ``~/.profile``
            file

            .. code-block:: shell

               export LD_LIBRARY_PATH=/home/tachidok/local/working/my_stuff/armadillo-8.300.3/installation/lib/

5. You can verify that the path is on your ``LD_LIBRARY_PATH``
   environment variable by typing:

   .. code-block:: shell

      echo $LD_LIBRARY_PATH
                   
**Run tests**

1. Open a termianl and go to folder where you extracted Armadillo,
   then open the ``Makefile`` in the ``examples`` folder.

2. Add the following lines (substitute them with the correct paths in
   your computer):

   .. code-block:: shell
                   
      INCLUDE=-I /path/to/armadillo/include/folder
      LIBS=-L /path/to/armadillo/lib/folder

3. In the same file add the ``$(INCLUDE)`` and ``$(LIBS)`` directives
   in the line:

   .. code-block:: shell

      CXXFLAGS = $(DEBUG) $(FINAL) $(OPT) $(EXTRA_OPT) $(INCLUDE) $(LIBS)
                   
4. Save and close the file.

5. Compile ``example1`` by typing

   .. code-block:: shell

      make example1
                   
   .. note:: If you got errors related with the ``pthread`` library then add the following in the ``Makefile``
             
      .. code-block:: shell

         LIB_FLAGS = -larmadillo -lpthread
         
  You should have an executable file called ``example1``.

6. Run the example as follows:

   .. code-block:: shell

      ./example1

   .. note:: If you have problems related to no shared library found
             then make sure you added the correct path for the
             armadillo libraries (/.so/) in the environment variable
             ``LD_LIBRARY_PATH``.

**Further steps**

If you require an specific configuration for Armadillo as OpenMP
support, acceleration, disabling of BLAS, LAPACK or something else
please do check `the official documentation for additional features <http://arma.sourceforge.net/docs.html#example_prog>`_.
             
.. _vtk-installation-label_installation.rst:

VTK
^^^

`VTK <https://vtk.org/>`_ is an open source Visualization Toolkit to
display scientific data in 2D and 3D. We use `Paraview
<https://www.paraview.org/>`_ , an open source visualization
application which makes use of VTK to display the plots. This section
guides you through the installation of ``VTK-8.1.1`` on the following
Ubuntu distributions:

* Ubuntu 16.04 LTS 64 bits
* Ubuntu 18.04.2 LTS 64 bits

**Steps**

1. Extract the compressed file ``/external_src/vtk/VTK-8.1.1.tar.gz``
   in a folder.

  .. warning:: We recommend you to extract it out of the ``scicellxx``
               project folder to avoid adding the files to the git
               repository. If you do extract it in the ``scicellxx``
               project folder then do not commit that folder within
               the project.

2. Open a terminal and go to the folder where you extracted the compressed file, inside the VTK folder create a new one and execute the ``cmake-gui`` as follow:

  .. code-block:: shell
     
     mkdir VTK-bin
     cd VTK-bin
     cmake-gui ../../VTK-8.1.1 -DCMAKE_INSTALL_PREFIX=./installation

  the installation will be performed in the ``installation`` directory
  of the ``VTK-bin folder``. If you have root privileges then you may
  not need to specify a value for the ``CMAKE_INSTALL_PREFIX``
  variable.

3. In the cmake gui click on the ``configure`` button, once finished
   click on the ``generate`` button.

   .. note:: Make sure that the source code and build binary directories are correctly set.

      * Source ``./VTK-8.1.1``
      * Bin ``./VTK-8.1.1/VTK-bin``

   .. note:: If you want to enable MPI then you need to set it in the
             variable ``VTK_Group_MPI``.

   .. note:: Verify that the building type you want has been set
             correctly; ``debug`` or ``release``.

4. After the configuration has finished close the cmake gui and in a
   terminal type

   .. code-block:: shell

      make
      
   .. note:: You can try with ``make -j <number_of_processors>`` to
             use more processors at compilation time.

5. Install VTK

   .. code-block:: shell

      make install

   this will install VTK into the folder specified by the build option
   ``DCMAKE_INSTALL_PREFIX``.

**Further steps**
   
* Learning VTK by examples: check `this guide <https://www.vtk.org/vtk-users-guide/>`_, specially chapters 1, 2, 3, 11 and 12.
