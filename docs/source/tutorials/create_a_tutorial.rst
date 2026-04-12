Create a tutorial
=================

How to create documentation
---------------------------

Python
------

Sphinx
------

Installation
------------

Install Sphinx using conda

``conda install sphinx``

or via the conda-forge channel

``conda install -c conda-forge sphinx``

check the official documentation at:
https://www.sphinx-doc.org/en/master/usage/installation.html

Setting up
----------
Go into the docs folder and run the command ``sphinx-quickstart`` which
will ask a few questions.

Latest used version was ``9.1.0``

Check the official documentation at:
https://www.sphinx-doc.org/en/master/usage/quickstart.html

Compiling documentation
-----------------------
Use the command

``sphinx-build -M html source_dir output_dir``

where ``source_dir`` is the source directory with the ``.rst`` files
and the ``output_dir`` is where the documentation will be build. The
``-M`` option selects the builder.

If you want a live version of the documentation use

``sphinx-autobuild source-dir output-dir``

The ``sphinx-quickstart`` script creates a ``Makefile`` and a
``make.bat`` file thus you can build the documentation using the
``make html`` command. Use ``make`` without arguments to check what
options are available.

Install the extension
---------------------

``pip install sphinx_rtd_theme``

and include

extensions = [
    ...
    'sphinx_rtd_theme',
]

html_theme = "sphinx_rtd_theme"

in the ``conf.py`` file.


Check the official documentation at:
https://www.sphinx-doc.org/en/master/usage/quickstart.html

Web tutorials
-------------
