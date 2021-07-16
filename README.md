# MEGA65 User Guide
This is community effort to create a User Guide for the MEGA65 in the spirit of the original User Guide for the Commodore 64.

# Just show me the PDFs!

If you just want the typeset manuals, the latest pdf's from our build pipeline are available here:
* https://files.mega65.org/manuals-upload/

(Location will be updated to files.mega65.org at a later stage when our automation permits it)

For official release increments of the documentation, hop over to the releases page at:

https://github.com/MEGA65/mega65-user-guide/releases

# Installation

## Requirements
* MingW32 (tested on MinGW-6.3.0, and MinGW-8.1.0) for Windows or GCC Compiler for Linux / Mac OS X.
* This project repo.
* The mega65-core repo: https://github.com/MEGA65/mega65-core is required for the memory tables included in the User Guide.
* The Simple C library for the MEGA65: https://github.com/MEGA65/mega65-libc is required to create api-conio.tex
* A typesetting environment that supports XeLaTeX, for example: http://www.tug.org/texworks/ for Windows.

# Usage
The User Guide project is defined using a Makefile to build. Therefore if you are using an environment like TeXworks, do not use the built in Build button. That is for single file building only. For MingW32 users, just use:

`mingw32-make` or `mingw32-make mega65-userguide.pdf` in the main folder of the project to build the mega65-userguide.pdf file.

For linux: `make` or `make mega65-userguide.pdf`.

If you wish to build the reference manual, use: `mingw32-make referenceguide.pdf`.

If you wish to build the nexys4 setup guide, use: `mingw32-make nexys4-setup-guide.pdf`.

To build the entire book, use: `mingw32-make mega65-book.pdf`.

There is also a `sandbox.pdf` for experimenting with typesetting or components for the manuals. But it is not included in the makefile. Instead, it is able to be built directly using software like TeXworks.

The sandbox is faster to build and test changes more quickly. You can then transfer your work directly to the userguide.

The complete list of guides that can be individually built are:

| Book       | Build     |
| :------------- | :---------- |
| User Guide | make mega65-userguide.pdf |
| Chipset Reference   | make mega65-chipset-reference.pdf |
| Complete BASIC 10 Commands | make mega65-basic10-reference.pdf |
| For experimentation (don't commit) | make sandbox.pdf |
| Reference Guide | make referenceguide.pdf |
| Developer's Guide | make mega65-developer-guide.pdf |
| Guide to MEGA65 and FPGA Hardware | make hardwareguide.pdf |
| All books combined | make mega65-book.pdf |
| As above but in CYMK for printing (Ghostscript required) | make mega65-book-cmyk.pdf: |


# MAC OSX

Install the LaTeX package from:

https://www.tug.org/mactex/mactex-download.html

Or use MacPorts, which gives easy access to a huge base of Linux/UNIX software.
With MacPorts install following LaTeX packages:

```
sudo port install texlive-latex
sudo port install texlive-latex-extra
sudo port install texlive-latex-recommended
sudo port install latexmk
sudo port install latexdiff
```

The manual is created in the local repository with:

```
make clean
make mega65-book.pdf
make mega65-book.pdf
```

Calling "make" with the target twice is intentional and needed for getting all references resolved.
Of course, you can create a shell script for these three lines.

# Linux
For Linux users, the instructions are the similar, but use your favourite package manager to install a LaTeX package of your choice.

# Editors

## Visual Studio Code

Combined with the `LaTeX Workshop` extension, this is a great IDE for working with the project. However be careful of the using the built-in terminal for compiling the guides. It's a powershell-based terminal and won't work with the Makefile. Nor will Command Prompt. Use a more Unix styled terminal like Git Bash.
