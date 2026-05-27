Initial steps
=============

.. toctree::
   :maxdepth: 2
   :numbered:

   initial_steps/installation/dependency_management.rst
   initial_steps/installation/linux_installation.rst
   initial_steps/installation/windows_installation.rst

   initial_steps/start_scicellxx_linux.rst
   initial_steps/start_scicellxx_windows.rst

   
This document shows you how to :ref:`install
<installation-label_initial_steps.rst>`, :ref:`start
<starting_scicellxx_-label_initial_steps.rst>` and :ref:`configure
<configuration-label_initial_steps.rst>` SciCell++ on Linux and
Windows systems.

1. The default installation strategy is based on containers so that
   all software dependencies are preinstalled and ready to use by
   SciCell++. The :ref:`installation
   <installation-label_initial_steps.rst>` section will show you how
   to install the container software for your operating system.

2. Then continue with the :ref:`starting SciCell++
   <starting_scicellxx_-label_initial_steps.rst>` section that shows
   you how to start SciCell++ using a container.

3. Once running the container with SciCell++ the :ref:`configuration
   <configuration-label_initial_steps.rst>` section will show you how
   to compile and enable advanced SciCell++'s features.
 
.. _installation-label_initial_steps.rst:

Installation
------------

The default installations use a container strategy such that all
software dependencies are preinstalled and ready to use for SciCell++.

:doc:`initial_steps/installation/linux_installation` For Linux systems
     users. Use this if you are not an experience Linux user. If you
     feel like challenging yourself then try our non-container based
     installation instructions and set aside enough sparse time.

     :ref:`Advanced installation
     <advanced_installation-label_linux_installation.rst>`: Use this
     installation only if you are familiar with Unix based
     systems. This installation provides you with full control over
     the versions of the third-part packages used by SciCell++.

:doc:`initial_steps/installation/windows_installation` For Windows
     systems users. Use this instructions only if you are not able to
     install a Linux distribution in your local machine.
     
.. _starting_scicellxx_-label_initial_steps.rst:
     
Starting SciCell++
------------------

Start the container that provides you with all packages required by
SciCell++ and let it ready for the configuration step.
     
:doc:`initial_steps/start_scicellxx/start_scicellxx_linux` Use this
     option to run SciCell++ by either using the container based
     installation or the advanced installation.

:doc:`initial_steps/start_scicellxx/start_scicellxx_windows` Use this
     option to run SciCell++ using the container based installation.
   
.. _configuration-label_initial_steps.rst:

Configuration
-------------

This section guides you through the configuration process of
SciCell++.

 .. important::

    At this point you should have successfully installed and executued
    SciCell++ on either Linux or Windows. Also you have started the
    docker container.

The configuration is performed with help of the ``autogen.sh`` script
which lives in the main SciCell++ folder.

1. In the running terminal make sure you are in the ``scicellxx``
   folder.
2. Execute the automatic generator script by typing:

   .. code-block:: shell

                   ./autogen.sh

   This command executes a full compilation of SciCell++ and runs all
   the demos and tests to make sure you are working with an stable
   copy. If you want a full list of available parameters for this
   script then add the ``-h`` parameter and review the
   :ref:`additional options for autogen.sh
   <autogen.sh-options-label_initial_steps.rst>` section.
      
   A summary of the compilation and testing processes is shown once
   they have finished. If no errors were reported then SciCell++ is
   ready to go.

   .. note::

      If you require to enable advanced features then check the
      configuration files at the ``./configs`` folder and have a look
      at the :ref:`options for these files
      <options_for_the_configuration_file-label_initial_steps.rst>` to
      enabled/disable advanced features.

   .. note::

      If you are NOT running SciCell++ within a container then use the
      appropiate config files in the ``./configs/advanced/`` folder.

   If this is the first time you compiled SciCell++ then you need to
   create your own private folder, check the :doc:`workflows` section
   on how to do this and many other common uses for SciCell++. Also
   check the :doc:`tutorials` and :doc:`demos` documents.

.. _autogen.sh-options-label_initial_steps.rst:
        
Additional options for ``autogen.sh``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Activate the interactive mode for full configuration by passing the
``-i`` parameter.

   .. code-block:: shell

                   ./autogen.sh -i
                   
We encourage you to check the full list of options by passing the
``-h`` parameter, the following may not reflect the full list of
options for the script.

This script builds [and runs the test suite of] SciCell++

      OPTIONS:
         -h      Show this message
         -t      Indicates to generate a 'STATIC' or 'SHARED' version of library files
         -b      Build version 'DEBUG' or 'RELEASE'
         -c      Configuration file for additional building tools
         -n      Number of processors to build the framework
         -d      Number of processors to run demos (set to '0' to skip demos testing)
         -i      Interative mode, launches the interactive mode to prompt for FULL configuration options (any other parameters are ignored)
         -r      Generate code coverage report
         -v      Verbose

.. _options_for_the_configuration_file-label_initial_steps.rst:
  
Options for the configuration file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* SCICELLXX_LIB_TYPE

  Specify the building type of the library ``STATIC`` or ``SHARED``.

  Example:

  .. code-block:: shell

     SCICELLXX_LIB_TYPE=STATIC
  
* SCICELLXX_RANGE_CHECK

  Specify whether to check for out-of-range in vectors or not. This
  significantly increase the running time of your application. We
  recommend you to enable this option only for developing purposes to
  ease you finding errors in the code. When using this option consider
  to compile with debugging options when prompted by the
  ``autogen.sh`` file. Once you move into the release mode disable
  this option and compile with full optimisation when prompted by the
  ``autogen.sh`` file.
  
  Example:
  
  .. code-block:: shell

     SCICELLXX_RANGE_CHECK=TRUE
  
* SCICELLXX_USES_DOUBLE_PRECISION

  Specify whether to use single (``float``) or double (``double``)
  precision at running time.
  
  Example:
  
  .. code-block:: shell

     SCICELLXX_USES_DOUBLE_PRECISION=TRUE
  
* SCICELLXX_USES_ARMADILLO

  Specify whether to use the external library Armadillo for linear
  algebra. You would need to :ref:`install Armadillo
  <armadillo-installation-label_installation.rst>` to enable this
  option.
  
  Example:
  
  .. code-block:: shell

     SCICELLXX_USES_ARMADILLO=FALSE
  
* SCICELLXX_AUTO_FIND_ARMADILLO_PATHS

  Use this option to automatically find the corresponding installation
  folder of Armadillo. Enable this option only if the option
  ``SCICELLXX_USES_ARMADILLO`` was set to ``TRUE``.
  
  Example:
  
  .. code-block:: shell

     SCICELLXX_AUTO_FIND_ARMADILLO_PATHS=TRUE
     
* ARMADILLO_AUTO_FIND_FOLDER

  If you want ot use Armadillo and you set the variable
  ``SCICELLXX_AUTO_FIND_ARMADILLO_PATHS`` to ``TRUE`` you may indicate
  an starting folder for the automatic finding of the Armadillo
  installation.

  Example:
  
  .. code-block:: shell
 
     ARMADILLO_AUTO_FIND_FOLDER=/home/tachidok/local/working/research/armadillo-8.300.3

* ARMADILLO_INCLUDE_DIRS

  If you want to use Armadillo but set the variable
  ``SCICELLXX_AUTO_FIND_ARMADILLO_PATHS`` to ``FALSE`` you must
  indicate the ``include`` directory in this option.
  
  Example:
  
  .. code-block:: shell

     ARMADILLO_INCLUDE_DIRS=/home/tachidok/local/working/research/armadillo-8.300.3/installation/include

* ARMADILLO_LIBRARIES

  If you want to use Armadillo but set the variable
  ``SCICELLXX_AUTO_FIND_ARMADILLO_PATHS`` to ``FALSE`` you must
  indicate the ``lib`` directory in this option.
  
  Example:
  
  .. code-block:: shell

     ARMADILLO_LIBRARIES=/home/tachidok/local/working/research/armadillo-8.300.3/installation/lib/libarmadillo.so.8.300.3
     
* SCICELLXX_USES_VTK

  Specify whether to use the external library VTK for results
  visualization. Some demos require VTK to generate output. You would
  need to :ref:`install VTK <vtk-installation-label_installation.rst>`
  to enable this option.

  Example:

  .. code-block:: shell

     SCICELLXX_USES_VTK=FALSE
  
* SCICELLXX_AUTO_FIND_VTK_PATHS

  Use this option to automatically find the corresponding installation
  folder of VTK. Enable this option only if the option
  ``SCICELLXX_USES_VTK`` was set to ``TRUE``.
  
  Example:

  .. code-block:: shell

     SCICELLXX_AUTO_FIND_VTK_PATHS=TRUE

* VTK_AUTO_FIND_FOLDER

  If you want ot use VTK and you set the variable
  ``SCICELLXX_AUTO_FIND_VTK_PATHS`` to ``TRUE`` you may indicate an
  starting folder for the automatic finding of the VTK installation.
  
  Example:

  .. code-block:: shell

     VTK_AUTO_FIND_FOLDER=/home/tachidok/local/working/research/VTK-8.1.1/VTK-bin
                  
* VTK_INCLUDE_DIRS

  If you want to use VTK but set the variable
  ``SCICELLXX_AUTO_FIND_VTK_PATHS`` to ``FALSE`` you must indicate the
  ``include`` directory in this option.
  
  Example:

  .. code-block:: shell

     VTK_INCLUDE_DIRS=/home/tachidok/local/working/research/VTK-8.1.1/VTK-bin/installation/include/vtk-8.1
     
* VTK_LIBRARIES

  If you want to use VTK but set the variable
  ``SCICELLXX_AUTO_FIND_VTK_PATHS`` to ``FALSE`` you must indicate the
  ``lib`` directory in this option.
  
  Example:

  .. code-block:: shell

     VTK_LIBRARIES=/home/tachidok/local/working/research/VTK-8.1.1/VTK-bin/installation/lib/libvtkalglib-8.1.so.1
     
* SCICELLXX_PANIC_MODE

  This option enables a large number of validations at running time,
  it also enables error messages that may help you to identify
  problems in your code. However, this considerably increase the
  running time of your application. Use this option only at developing
  time. Deactivate this function when runnig on release mode, also
  make sure to activate full optimisation at compilation time when
  prompted by the ``autogen.sh`` script.
  
  Example:

  .. code-block:: shell

     SCICELLXX_PANIC_MODE=TRUE   
