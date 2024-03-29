Generate ``doxygen`` documentation for SciCell++
================================================

This allows you to create class diagrams and browseable documentation
directly from the source code of SciCell++.

You need to install `Doxygen <https://www.doxygen.nl/index.html>`_ and
`Latex <https://www.latex-project.org/>`_ to generate documentation
from source code.

You may check :ref:`this section
<doxygen-installation-label_installation.rst>` for doxygen
installation.

Workflow
--------
  
1. Open a command line and go to the main folder of the project.

2. In the command line type the following:
  
   .. code-block:: shell

                   ./tools/user/make_doc.sh

   Voila! The documentation will be automatically generated into the
   ``./doxy_doc/html`` folder.

3. Open the file ``./doxy_doc/html/index.html`` within your favorite
   web-browser to browse the documentation.
