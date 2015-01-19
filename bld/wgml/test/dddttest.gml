:CMT. test for :DD & :DT used in begnote,note and step macros
.*
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
.*
The main subdirectories in the Open Watcom root are the following:
.begnote
.note bat
currently contains mostly useless batch files and several useful ones.
One extremely important file lives here:
.us makeinit.
This file controls the operation of wmake and is the key to understanding
of the build process. Since wmake looks for
.us makeinit
along the
.id PATH,
the
.us bat
directory should be placed at or near the start of your
.id PATH
environment variable.
.note bin, binp
contains miscellaneous binaries used in the build process. The binp directory
contains OS/2 executables, bin contains DOS or Win32 executables.
.note bld
is
.us the
directory where it's at. It contains all the Open Watcom source code. It is
so important (and huge) that it deserves its own section.
.note docs
contains source files for the Open Watcom documentation as well as binaries
needed to translate the sources into PostScript, HTML or various online
help formats. The source files of this document are stored under this directory.
For more information please refer the the chapter entitled Documentation
later in this manual.
.note rel2
is the "release" directory is where the binaries and other files produced
in the course of the build process end up. The structure of this directory
mirrors the
.us WATCOM
directory of a typical Open Watcom installation.
.endnote
.*
.begstep
.step Start the &editor..
.step Choose New
from the File menu.
.result
An empty text window appears.
.step Select New
from the File menu.
.result
Another empty text window appears.
.endstep
.*





