# MEGA65 User Guide
This is community effort to create a User Guide for the MEGA65 in the spirit of the original User Guide for the Commodore 64.

# Installation

## Requirements
* MingW32 (tested on MinGW-6.3.0) for Windows or GCC Compiler for Linux / Mac OS X.
* This project repo.
* The mega65-core repo: https://github.com/MEGA65/mega65-core is required for the memory tables included in the User Guide.
* A typesetting environment that supports XeLaTeX, for example: http://www.tug.org/texworks/ for Windows.

# Usage
The User Guide project is defined using a Makefile to build. Therefore if you are using an environment like TeXworks, do not use the built in Build button. That is for single file building only. For MingW32 users, just use:

`mingw32-make` in the main folder of the project to build the userguide.pdf file.

For Mac OS X and Linux users, use `make` in the main folder.
