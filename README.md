# MEGA65 User Guide
This is community effort to create a User Guide for the MEGA65 in the spirit of the original User Guide for the Commodore 64.

# Installation

## Requirements
* MingW32 (tested on MinGW-6.3.0, and MinGW-8.1.0) for Windows or GCC Compiler for Linux / Mac OS X.
* This project repo.
* The mega65-core repo: https://github.com/MEGA65/mega65-core is required for the memory tables included in the User Guide.
* A typesetting environment that supports XeLaTeX, for example: http://www.tug.org/texworks/ for Windows.

# Usage
The User Guide project is defined using a Makefile to build. Therefore if you are using an environment like TeXworks, do not use the built in Build button. That is for single file building only. For MingW32 users, just use:

`mingw32-make` or `mingw32-make userguide.pdf` in the main folder of the project to build the userguide.pdf file.

If you wish to build the reference manual, use: `mingw32-make referenceguide.pdf`.

If you wish to build the nexys4 setup guide, use: `mingw32-make nexys4-setup-guide.pdf`.

There is also a `sandbox.pdf` for experimenting with typesetting or components for the manuals. But it is not included in the makefile. Instead, it is able to be built directly using software like TeXworks.

The sandbox is faster to build and test changes more quickly. You can then transfer your work directly to the userguide.

# MAC OSX
It is recommended to install MacPorts, which gives easy access to a huge base of Linux/UNIX software.
With MacPorts install following LaTeX packages:

sudo port install texlive-latex

sudo port install texlive-latex-extra

sudo port install texlive-latex-recommended

sudo port install latexmk

sudo port install latexdiff
  
Then creating the complete MEGA65 documentation is done in the local repository work directory with:

make clean

make mega65-book.pdf

make mega65-book.pdf

Calling "make" with the target twice is intentional and needed for getting all references resolved.
Of course, you can create a shell script for these three lines.

# Linux
For Linux users, the instructions are the similar, but use your favourite package manager to install a LaTeX package of your choice.
