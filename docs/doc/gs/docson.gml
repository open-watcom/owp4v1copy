.chap Documentation
.*
.np
The following manuals comprise the &product documentation set.
Printed copies of this documentation can be ordered from &company..
.if '&lang' eq 'C/C++' .do begin
When you install the software, portions of the documentation set are
provided as on-line help files.
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
When you install the software, the &product documentation set is
provided as on-line help files.
.do end
Subsequent sections describe how to access this on-line help.
.np
The following describes the titles in the &product documentation set.
.begpoint $break
.*
.point &product User's Guide
.if &e'&readme ne 0 .do begin
.if '&lang' eq 'C/C++' .do begin
This manual describes how to use &product..
It contains an introduction to the compiler and a tutorial section.
It also describes
compiler options,
precompiled header files,
libraries,
memory models,
calling conventions,
pragmas,
in-line assembly,
ROM based applications,
and environment variables.
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
This manual describes how to use &product..
It contains an introduction to the compiler and a tutorial section.
It also describes
compiler options,
include file processing,
libraries,
memory models,
calling conventions,
pragmas,
in-line assembly,
and environment variables.
.do end
.do end
.el .do begin
(This book)
.do end
.*
.point &product Tools User's Guide
This manual describes the command line oriented tools including the
compile and link utility,
library manager,
object file disassembler,
far call optimization tool,
assembler,
patch utility,
strip utility,
make utility,
and touch utility.
.*
.point &company Graphical Tools User's Guide
This manual describes &company's Windows and OS/2 graphical tools
including the &ide, &br., &drwc., Spy, DDE Spy, Image Editor,
Resource Editor, Sampler/Profiler, Resource Compiler, Heap Walker,
Zoom, and Editor.
.*
.point &product Programmer's Guide
This manual includes 5 major sections each of which describes
operating system specific development issues. The operating systems
covered include extended DOS, OS/2, Windows 3.x, Windows NT/Win32s,
32-bit Windows 3.x (using &company's Supervisor technology), AutoCAD
ADS and Novell NLMs. Topics include creating a sample program,
operating system specific error messages, and debugging techniques.
.*
.*
.if '&lang' eq 'FORTRAN 77' .do begin
.point &product Language Reference
This manual describes the ANSI FORTRAN 77 programming language and
extensions which are supported by &product..
.do end
.*
.if '&lang' eq 'FORTRAN 77' .do begin
.point &product Graphics Library Reference
This manual describes the graphics libraries supported by &product..
The graphics functions are used to display graphical images such as
lines and circles upon the screen.
Functions are also provided for displaying text.
.do end
.*
.if '&lang' eq 'C/C++' .do begin
.point &cmpcname Language Reference
This manual describes the ANSI C programming language and extensions
which are supported by &cmpcname..
.do end
.*
.if '&lang' eq 'C/C++' .do begin
.point &cmpcname Library Reference, Volumes 1 and 2
These manuals describe the C and graphics libraries supported by
&product..
.do end
.*
.if '&lang' eq 'C/C++' .do begin
.point &cmppname Class Library Reference
This manual provides a comprehensive reference to the C++ class
libraries provided with &product..
.do end
.*
.point &dbgname User's Guide
This manual describes the &dbgname and discusses advanced debugging
techniques.
.*
.point &lnkname User's Guide
This manual describes how to use the &lnkname to generate executables
for target systems such as extended DOS, Windows 3.x, Windows 95,
Windows NT, OS/2, and Novell NLMs.
.*
.if '&lang' eq 'C/C++' .do begin
.point Visual Programmer User's Guide
This manual describes how to use Blue Sky's Visual Programmer, an easy
to use Prototyper and C/C++ Code Generator for Windows, Win32 and
Windows NT.
.*
.point MFC Switch-It Module User's Guide
Blue Sky's MFC Switch-It Module (SIM) allows you to generate Microsoft
Foundation Classes (MFC) C++ code for your application design(s). You
prototype and test your application using Visual Programmer, then
generate the code using the MFC SIM.
.do end
.*
:cmt..point Comprehensive Index
:cmt.Comprehensive Index for entire package
.*
.endpoint
.if '&lang' eq 'C/C++' .do begin
.np
The following book is included in the printed documentation set.
It is not available as an on-line document.
.begpoint $break
.*
.point The C++ Programming Language by Bjarne Stroustrup
This book is an industry recognized authoritative standard on C++
programming. This manual begins with a tutorial to introduce the C++
concepts. The second section consists of an in-depth C++ reference
guide.
.*
.endpoint
.do end
.*
.section Accessing On-line Documentation
.*
.np
The following sections describe how to access the on-line help that is
available for DOS, Windows 3.x, Windows 95, Windows NT and OS/2.
.*
.beglevel
.*
.section On-line Documentation under DOS
.*
.np
The &company Help program,
.kw WHELP,
may be used under DOS to access on-line documentation.
The &company Help command line syntax is:
.millust begin
WHELP help_file [topic_name]
.millust end
.autonote Notes:
.note
If
.us help_file
is specified without an extension then ".IHP" is assumed.
.note
The
.us topic_name
parameter is optional.
.note
If
.us topic_name
is not specified, the default topic is "Table of Contents".
.note
If
.us topic_name
contains spaces then it must be enclosed in quotes.
.endnote
.np
The following help files are available:
.begnote
.if '&lang' eq 'C/C++' .do begin
.note CGUIDE
.book &product User's Guide
(excludes C and C++ Diagnostic Messages appendices which are available
as separate help files)
.do end
.note CLIB
.book &cmpcname Library Reference
.if '&lang' eq 'C/C++' .do begin
.note CLR
.book &cmpcname Language Reference
.do end
.if '&lang' eq 'C/C++' .do begin
.note CMIX
.book &product Master Index
.note CPPLIB
.book &cmppname Class Library Reference
.note ISVCPP
.book &product &ver..&rev Add-In Tools Guide
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.note FMIX
.book &product Master Index
.note FPGUIDE
.book &product Programmer's Guide
.note FTOOLS
.book &product Tools User's Guide
.note FUGUIDE
.book &product User's Guide
(excludes Diagnostic Messages appendix which is available
as a separate help file)
.note F77LR
.book &product Language Reference
.note F77GRAPH
.book &product Graphics Library Reference
.note ISVFOR
.book &product &ver..&rev Add-In Tools Guide
.do end
.note LGUIDE
.book &lnkname User's Guide
.if '&lang' eq 'C/C++' .do begin
.note PGUIDE
.book &product Programmer's Guide
.do end
.if '&lang' eq 'C/C++' .do begin
.note C_README
.book &product Getting Started manual
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.note F_README
.book &product Getting Started manual
.do end
.note RESCOMP
Documentation for the &wrcname for Windows
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.if '&lang' eq 'C/C++' .do begin
.note TOOLS
.book &product Tools User's Guide
.do end
.note WD
.book &dbgname User's Guide
.note WPROF
Documentation for the &smpname and &prfname
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.if '&lang' eq 'C/C++' .do begin
.note WCCERRS
Documentation for the &cmpcname Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct ).
:cmt. This file was inadvertantly omitted from the CD-ROM but can be
:cmt. down-loaded from the &company BBS, CompuServe or FTP'ed from the
:cmt. &company FTP site.
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
.note WFCERRS
Documentation for the &cmpname Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct ).
.do end
.if '&lang' eq 'C/C++' .do begin
.note WPPERRS
Documentation for the &cmppname Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct ).
:cmt. This file was inadvertantly omitted from the CD-ROM but can be
:cmt. down-loaded from the &company BBS, CompuServe or FTP'ed from the
:cmt. &company FTP site.
.do end
.endnote
.*
.section On-line Documentation under Windows 3.x, 95 and NT
.*
.np
On-line documentation is presented in the form of Windows Help
files (".HLP" files).
When the software is installed under Windows 3.x, Windows 95 or
Windows NT, a number of program groups are created.
You can access the on-line document by opening a program group and
double-clicking on a help icon.
.begnote $setptnt 15
.notehd1 &product Group
.notehd2 ~b
.note Getting Started
.book &product Getting Started
.endnote
.*
.begnote $setptnt 15
.notehd1 &product Tools Help Group
.notehd2 ~b
.note Accelerator Editor Help
Documentation for the Accelerator Editor
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.if '&lang' eq 'C/C++' .do begin
.note C Error Messages
Documentation for the &cmpcname Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct )
.note C++ Error Messages
Documentation for the &cmppname Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct )
.note C Language Reference
.book &cmpcname Language Reference
.do end
.note C Library Reference
.book &cmpcname Library Reference
.if '&lang' eq 'C/C++' .do begin
.note C++ Library Reference
.book &cmppname Class Library Reference
.note &lang Master Index
The master index for all of the &product on-line help
.do end
.note DDE Spy Help
Documentation for the DDE Spy utility
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Debugger Help
.book &dbgname User's Guide
.note Dialog Editor Help
Documentation for the Dialogue Editor
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note &drwc Help
Documentation for &drwc
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Editor Help
Documentation for the &edname
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.if '&lang' eq 'FORTRAN 77' .do begin
.note &lang Error Messages
Documentation for the &cmpname Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct )
.note &lang Master Index
The master index for all of the &product on-line help
.do end
.note Heap Walker Help
Documentation for the Heap Walker utility
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note IDE Help
Documentation for the Interactive Development Environment
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Image Editor Help
Documentation for the Image Editor
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Linker Guide
.book &lnkname User's Guide
.note Menu Editor Help
Documentation for the Menu Editor
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Profiler Help
Documentation for the &smpname and &prfname
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Programmer's Guide
.book &product Programmer's Guide
.note Resource Compiler Help
Documentation for the Resource Compiler
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Resource Editor Help
Documentation for the Resource Editor
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.if '&lang' eq 'C/C++' .do begin
.note Source Browser Help
Documentation for the Browser
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.do end
.note Spy Help
Documentation for the Spy utility
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note String Editor Help
Documentation for the String Editor
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Tools Guide
.book &product Tools User's Guide
.note User's Guide
.book &product User's Guide
.if '&lang' eq 'C/C++' .do begin
(excludes C and C++ Diagnostic Messages appendices which are available
as separate help files)
.do end
.if '&lang' eq 'C/C++' .do begin
.note Visual Programmer Help
.book Visual Programmer User's Guide
.note Visual Programmer MFC Help
.book MFC Switch-it Module User's Guide
.do end
.note Zoom Help
Documentation for the Zoom utility
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.endnote
.np
The second set of help files, listed below, forms part of the
Microsoft Windows SDK.
.begnote $setptnt 15
.notehd1 &product Additional Help Group
.notehd2 ~b
.note Add-In Tools Guide
A guide to third-party software tools/libraries
.note Hotspot Editor Help
.note MCI Command Strings
.note &mfc32 Library Help
.note &mfc32 Sample Program Help
.note &mfc32 Technical Notes
.note &mfc16 Library Help
.note &mfc16 Sample Program Help
.note &mfc16 Technical Notes
.note Multimedia Reference
.note Pen API Reference
.note Win32 API Help
.note Windows API Reference
.endnote
.*
.section On-line Documentation under OS/2
.*
.np
On-line documentation is presented in the form of OS/2 Information
files (".INF" files).
When the software is installed under OS/2, the &product folder
is created.
You can access the on-line document by opening the &product
folder and double-clicking on a help icon.
.begnote $setptnt 15
.if '&lang' eq 'C/C++' .do begin
.note C Error Messages
Documentation for the C Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct )
.note C++ Error Messages
Documentation for the C++ Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct )
.note C Language Reference
.book &cmpcname Language Reference
.do end
.note C Library Reference
.book &cmpcname Library Reference
.if '&lang' eq 'C/C++' .do begin
.note C++ Library Reference
.book &cmppname Class Library Reference
.note &lang Master Index
The master index for all of the &product on-line help
.do end
.note CP Reference
OS/2 Control Program Reference
.note Debugger Help
.book &dbgname User's Guide
.if '&lang' eq 'FORTRAN 77' .do begin
.note &lang Error Messages
Documentation for the &cmpname Diagnostic Messages
(excerpt from the
.book &product User's Guide
.ct )
.note &lang Master Index
The master index for all of the &product on-line help
.do end
.note Getting Started
.book &product Getting Started
.note IDE Help
Documentation for the Interactive Development Environment
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note IPFC Reference
OS/2 IPF Compiler Reference
.note Linker Guide
.book &lnkname User's Guide
.note PM Reference
OS/2 Presentation Manager Reference
.note Profiler Help
Documentation for the &smpname and &prfname
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.note Programmer's Guide
.book &product Programmer's Guide
.note REXX Reference
OS/2 REXX Reference
.if '&lang' eq 'C/C++' .do begin
.note SOM Reference
OS/2 System Object Model Reference
.do end
.if '&lang' eq 'C/C++' .do begin
.note Source Browser Help
Documentation for the &brname
(excerpt from the
.book &company Graphical Tools User's Guide
.ct )
.do end
.note Tools Guide
.book &product Tools User's Guide
.note Tools Reference
OS/2 Toolkit Reference
.note User's Guide
.book &product User's Guide
.if '&lang' eq 'C/C++' .do begin
(excludes C and C++ Diagnostic Messages appendices which are available
as separate help files)
.do end
.endnote
.*
.endlevel
