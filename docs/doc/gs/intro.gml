.chap Introduction to &product
.*
.np
Welcome to the &product &ver..&rev development system.
Version &ver..&rev of &product is a professional, optimizing,
multi-platform
.if '&lang' eq 'C/C++' .do begin
C and C++
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
FORTRAN 77
.do end
compiler with a comprehensive suite of development tools for
developing and debugging both 16-bit and 32-bit applications for
DOS,
extended DOS,
Novell NLMs,
16-bit OS/2 1.x,
32-bit OS/2,
Windows 3.x,
Windows 95,
Win32s,
and
Windows NT (Win32).
.np
You should read the entire contents of this booklet, as it contains
information on new programs and modifications that have been made
since the previous release.
.np
.abox begin
.bd Special NOTE to users of previous versions!
.bd See the section entitled
:HDREF refid='diffs'
.bd to determine if you need to recompile your application.
.abox end
.*
.section What is in version &ver..&rev of &product?
.*
.np
Version &ver..&rev incorporates the features professional developers have
been demanding:
.begpoint $break $setptnt 5
.ix 'product overview'
.ix 'overview'
.point Open, Multi-target Integrated Development Environment
The IDE allows you to easily edit, compile, link, debug and build
applications for 16-bit systems like DOS, OS/2 1.x, and Windows 3.x
and 32-bit systems like extended DOS, Novell NLMs, OS/2,
Windows 3.x (Win32s), Windows 95, and Windows NT.
Projects can be made up of multiple targets which permit a project to
include EXEs and DLLs.
The IDE produces makefiles for the project which can be viewed and
edited with a text editor.
The IDE is hosted under Windows 3.x, Windows 95, Windows NT, and
32-bit OS/2.
.cp 10
.point The Widest Range of Intel x86 Platforms
.begnote
.note Host Platforms
.ix 'platforms supported'
.ix 'host platforms supported'
.begbull $compact
.bull
DOS (command line)
.bull
32-bit OS/2 (IDE and command line)
.bull
Windows 3.x (IDE)
.bull
Windows 95 (IDE and command line)
.bull
Windows NT (IDE and command line)
.endbull
.note 16-bit Target Platforms
.ix 'target platforms supported'
.begbull $compact
.bull
DOS
.bull
Windows 3.x
.bull
OS/2 1.x
.endbull
.note 32-bit Target Platforms
.begbull $compact
.bull
Extended DOS
.bull
Win32s
.bull
Windows 95
.bull
Windows NT
.bull
32-bit OS/2
.bull
Novell NLMs
.endbull
.endnote
.point Cross-Platform Development Tools
.ix 'cross-platform'
The core tools in the package permit cross-platform development that
allows developers to exploit the advanced features of today's popular
32-bit operating systems, including Windows 95, Windows NT, and OS/2.
Cross-platform support allows you to develop on a host development
environment for execution on a different target system.
.point Multi-Platform Debugger
The new debugger advances developer productivity. New features include
redesigned interface, ability to set breakpoints on nested function
calls, improved C++ and DLL debugging, reverse execution, and
configurable interface. Graphical versions of the debugger are
available under Windows 3.x, Windows 95, Windows NT, and 32-bit OS/2.
Character versions of the debugger are available under DOS,
Windows 3.x, Windows NT, and 32-bit OS/2.
For VIDEO fans, we have kept the command line compatibility from the
original debugger.
.if '&lang' eq 'FORTRAN 77' .do begin
:cmt. .point Class Browser
:cmt. The Browser lets you visually navigate the subroutines,
:cmt. functions, and variables of your FORTRAN 77 application.
.do end
.if '&lang' eq 'C/C++' .do begin
.point Class Browser
.ix 'class browser'
The Browser lets you visually navigate the object hierarchies,
functions, variable types, and constants of your C/C++ application.
.do end
.point Performance Analysis
.ix 'performance analysis'
The &smpname and &prfname are performance analysis tools that
locate heavily used sections of code so that you may focus your
efforts on these areas and improve your application's performance.
.point Editor
.ix 'editor'
The &edname is a context sensitive source editor, integrated into the
Windows 3.x, Windows 95 and Windows NT version of the IDE.
.if '&lang' eq 'C/C++' .do begin
.if &e'&beta eq 0 .do begin
.point &vpname
.ix '&vpname'
Blue Sky's &vpname is incorporated in the package.
&vpname is powerful visual design environment for
:CMT. Windows 3.x,
Windows 95 and Windows NT that helps you build sophisticated Windows
applications.
&vpname provides the familiar elements of the Windows user interface,
such as pull-down menus, toolbars, dialog boxes, controls, and other
elements as ready-made objects that you can incorporate into your
application instantly.
In the visual design environment, you paint your user interface
directly on the screen and attach functionality to the interface
components by visually connecting them on the screen, without writing
a single line of code.
.do end
.do end
.point Graphical Development Tools
.ix 'GUI tools'
&product includes a suite of graphical development tools to aid
development of Windows 3.x, Windows 95 and Windows NT applications.
The development tools include:
.begnote $setptnt 12
.note Resource Editors
.ix 'resource editors'
Enable you to create resources for your 16-bit and 32-bit Windows
applications. For 32-bit OS/2 PM development, &product interoperates
with IBM's OS/2 Developer's Toolkit (available from IBM).
These tools have been seamlessly integrated into the
IDE. The resource compiler allows you to incorporate these resources
into your application.
.note Resource Compiler
.ix 'resource compiler'
Produces a compiled resource file from a source file.
.note Zoom
.ix 'zoom'
Magnifies selected sections of your screen.
.note Heap Walker
.ix 'heap walker'
Displays memory usage for testing and debugging purposes.
.note Spy
.ix 'spy'
Monitors messages passed between your application and Windows.
.note DDESpy
.ix 'DDE spy'
Monitors all DDE activity occurring in the system.
.note &drwc
.ix '&drwc'
Enables you to debug your program by examining both the program and
the system after an exception occurs; monitors native applications
running under Windows 3.x, Windows 95 or Windows NT.
.endnote
.point Assembler
An assembler is now included in the package.
It is compatible with a subset of the Microsoft assembler.
.if '&lang' eq 'C/C++' .do begin
.point C++ Class Libraries
.ix 'class libraries'
&product includes container and stream class libraries.
.do end
:cmt.The container class libraries have been improved significantly and feature....
.point Royalty-free 32-bit DOS Extender
.ix 'DOS extender'
&product includes the DOS/4GW 32-bit DOS extender by Tenberry Software
with royalty-free run-time and virtual memory support up to 32MB.
.point Support for wide range of DOS Extenders
&product allows you to develop and debug applications based on the
following DOS extender technology: Tenberry Software's DOS/4G
and Phar Lap's TNT DOS Extender.
You can also develop applications using FlashTek's DOS Extender but,
currently, there is no support for debugging these applications.
.point Sample programs and applications
&product includes a large set of sample applications to demonstrate
the new integrated development environment.
.endpoint
.*
.section Technical Support and Services
.*
.np
.ix 'technical support'
We are committed to ensuring that our products perform as they were
designed.
Although a significant amount of testing has gone into this product,
you may encounter errors in the software or documentation.
Our technical support offers a variety of services to help you work
around any problems you may encounter.
&product &ver..&rev includes 60-days of free installation assistance.
Installation assistance will include installing the product and
compiling and running the samples that are included with the product
for your specific environment.
Subsequent to this 60-day period, technical assistance is available
through our fee-based support programs.
Please contact us for more information.
.if &e'&beta eq 0 .do begin
.np
In order to keep informed about product updates and announcements, we
suggest that you send in your registration card.
Product registration cards are included in the package and can be
mailed or faxed to us.
.do end
.*
.beglevel
.*
.section Resources at Your Fingertips
.*
.np
.ix 'self-help'
&product contains many resources to help you find answers to your
questions. The documentation is the first place to start. With each
release of the product, we update the manuals to answer the most
frequently asked questions. Most of this information is also
accessible through on-line help.
.np
.ix 'read-me file'
The "README" file in the main product directory contains up-to-date
information that recently became available.
.np
Answers to frequently asked questions are available on
CompuServe,
Powersoft's World Wide Web server,
Powersoft's FTP site,
Powersoft's bulletin board,
or
the Powersoft's Faxline fax back system.
These services are available 24 hours a day.
See the Powersoft
.book Customer Services Reference Guide
for more information on
.us Automated Technical Support (ATS).
.*
.section Contacting Technical Support
.*
.np
.ix 'technical support'
Our technical support is available to help resolve technical defects
in the software.
The following are ways to contact technical support.
.begnote
.*
.note Telephone
The telephone number for the Technical Support office nearest to you
is listed in the
.book Customer Services Reference Guide.
.*
.note Fax
The telephone number of the facsimile machine for the Technical
Support office nearest to you is listed in the
.book Customer Services Reference Guide.
.*
.note World Wide Web
You can also send bug reports or enhancement requests to Technical
Support using the Powersoft World Wide Web server.
Electronic forms are available for on-line completion.
.*
.note FTP
You can obtain a copies of the bug report or enhancement request forms
from the Powersoft FTP site.
.code begin
ftp://ftp.powersoft.com/pub/watcom/general/wbugrep.zip
ftp://ftp.powersoft.com/pub/watcom/general/wenhreq.zip
.code end
.*
.endnote
.np
The bug report form (WBUGREP.TXT) is available on
Compuserve,
the Powersoft BBS, and
Faxline (document #1014).
The enhancement request form (WENHREQ.TXT) is available on
Compuserve, the Powersoft BBS, and Faxline (document #1015).
.*
.section Information Technical Support Will Need to Help You
.*
.if &e'&beta eq 0 .do begin
.np
.ix 'registration number'
Your registration number will be required when you contact Technical
Support.
.do end
.np
The more information you can provide to your technical support
representative, the faster they can help you solve your problem. A
detailed description of the problem, short sample program, and a
summary of steps to duplicate the problem (including compiler and
linker options) are essential. Concise problem reports allow technical
support to quickly pinpoint the problem and offer a resolution. Here
is a list of information that will help technical support solve the
problem:
.begpoint $break
.point Contact information
We would like your name, as well as telephone and fax numbers where
you can be reached during the day.
.point Product information
Please tell us the
.if &e'&beta eq 0 .do begin
product name, version number, patch level, and software registration
number.
.do end
.el .do begin
product name and version number.
.do end
.point Hardware configuration
Please tell us what type of processor you are using (e.g., 33MHz
486SX), how much memory is present, what kind of graphics adapter you
are using, and how much memory it has.
.point Software configuration
Please tell us what operating system and version you are using.
.point Output from the TECHINFO utility
.ix 'TECHINFO'
Please provide the output from the Techinfo utility. It lists current
patch levels, environment variable settings, and the contents of your
AUTOEXEC.BAT and CONFIG.SYS files.
.point Concise problem report with short sample program
Please provide a complete description of the problem and the steps to
reproduce it. A small, self-contained program example with compile and
link options is ideal.
.endpoint
.np
An electronic form of communication is often preferable for reporting
technical problems. It provides an easy and efficient way to receive
sample code and complete problem details.
.*
.section Making Fixes Available to Developers: The Patch Utility
.*
.np
.ix 'fixes'
.ix 'patches'
We are dedicated to fixing problems in our products. We developed the
patch utility to enable timely responses to bugs in the software.
Patches are frequently made available and are designed to address
problems with the software. When you call technical support one of the
first questions asked is your current patch level.
.begpoint $break
.point How do I determine the current patch level?
.ix 'patch level'
Here are two easy ways for you to determine your current patch level:
.autonote
.note
At the command line, type the command "techinfo".  Among other information,
it displays the patch level of the various components in the package.
.note
If you are using the command line tools,
.if '&lang' eq 'C/C++' .do begin
both the C and C++ compiler display
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
the FORTRAN 77 compiler displays
.do end
a banner at startup that echo the version number with the patch level.
.endnote
.point How can I get the patches?
.ix 'patches' 'obtaining'
Patches are made available on
CompuServe,
our World Wide Web server,
our FTP site,
and
our bulletin board.
They are available 24 hours a day.
Patches are also available on the current release CD-ROM.
.point How do I apply the patches?
Once you have downloaded the patches, issue the
.kwm pkunzip
command to unzip the patch files.
A "README" file provides information on the patch.
:cmt. .np
:cmt. If you have the release &ver..&rev CD-ROM, you can apply the patches that
:cmt. are provided with it.
.if &e'&readme ne 0 .do begin
:cmt. See the section entitled
:cmt. :HDREF refid='patches'
:cmt. for more information on patching the software.
.do end
.point How can I ensure the patch applied correctly?
Running the Techinfo utility indicates the current patch level of the
tools. Ensure that the patch indication for the files is the one you
just applied.
.endpoint
.*
.endlevel
.*
.*
.section Third-party Support
.*
.np
.ix 'third-party software'
.ix 'independent software vendors'
.ix 'ISV'
A large number of third party software packages support the use of
the &product..
We have included a document on the CD-ROM which describes some of
these packages.
The information on these software packages was provided by the
developers of these packages.
The document takes the form of a help file, of which versions are
provided for DOS, Windows 3.x, Windows 95, Windows NT, and OS/2.
.*
.section Suggested Reading
.*
.np
There are a number of good books and references that can help you
answer your questions. Following is a list of some of the books and
documents we feel might be helpful. This is by no means an exhaustive
list. Contact your local bookstore for additional information.
.*
.beglevel
.* ----------------------------------------------------------------
.if '&lang' eq 'C/C++' .do begin
.*
.section C Programmers
.*
.begnote
.note The C Programming Language, 2nd Edition
.br
Brian W. Kernighan and Dennis M.Ritchie; Prentice Hall, 1988.
.*
.note C DiskTutor
.br
L. John Ribar; Osborne McGraw-Hill, 1992.
.endnote
.do end
.* ----------------------------------------------------------------
.if '&lang' eq 'C/C++' .do begin
.*
.section C++ Programmers
.*
.begnote
.*
.note C++ Primer, 2nd Edition
.br
Stanley B. Lippman; Addison-Wesley Publishing Company, 1991.
.*
.note Teach Yourself C++ in 21 Days
.br
Jesse Liberty; Sams Publishing, 1994.
.endnote
.do end
.* ----------------------------------------------------------------
.*
.section DOS Developers
.*
.begnote
.*
.note PC Interrupts, Second Edition
.br
Ralf Brown and Jim Kyle; Addison-Wesley Publishing Company, 1994.
.*
.note Relocatable Object Module Format Specification, V1.1
.br
.ix 'OMF specification'
The Intel OMF specification can be obtained from the Intel ftp site.
Here is the URL.
.code begin
ftp://ftp.intel.com/pub/tis/omf11g.zip
.code end
.np
This ZIP file contains a Postscript version of the Intel OMF V1.1
specification.
.endnote
.* ----------------------------------------------------------------
.*
.section Extended DOS Developers
.*
.begnote
.*
.note Extending DOS&mdash.A Programmer's Guide to Protected-Mode DOS, 2nd Edition
.br
Ray Duncan, et al; Addison-Wesley Publishing Company, 1992.
.*
.note DOS Protected-Mode Interface (DPMI) Specification
.br
.ix 'DPMI specification'
The DPMI 1.0 specification can be obtained from the Intel ftp site.
Here is the URL.
.code begin
ftp://ftp.intel.com/pub/IAL/software_specs/dpmiv1.zip
.code end
.np
This ZIP file contains a Postscript version of the DPMI 1.0
specification.
.endnote
.* ----------------------------------------------------------------
.*
.section Windows 3.x Developers
.*
.begnote
.*
.note Microsoft Windows Programmer's Reference
.br
Microsoft Corporation; Microsoft Press, 1990.
.*
.note Programming Windows 3.1, Third Edition
.br
Charles Petzold; Microsoft Press, 1992.
.*
.note Windows Programming Primer Plus
.br
Jim Conger; Waite Group Press, 1992.
.endnote
.* ----------------------------------------------------------------
.*
.section Windows NT Developers
.*
.begnote
.*
.note Advanced Windows NT
.br
Jeffrey Richter; Microsoft Press. 1994.
.*
.note Inside Windows NT
.br
Helen Custer; Microsoft Press. 1993.
.*
.note Microsoft Win32 Programmer's Reference, Volume One
.br
Microsoft Corporation; Microsoft Press, 1993.
.endnote
.* ----------------------------------------------------------------
.*
.section OS/2 Developers
.*
.begnote
.*
.note The Design of OS/2
.br
H.M. Deitel and M.S. Kogan; Addison-Wesley Publishing Company, 1992.
.*
.note OS/2 Warp Unleashed, Deluxe Edition
.br
David Moskowitz and David Kerr, et al; Sams Publishing, 1995.
.*
.note OS/2 Technical Library.
.br
To order the Technical Library, call one of the following numbers.
.millust begin
In Canada:            1-800-465-1234
In the United States: 1-800-426-7282 (OS/2 2.x)
                      1-800-879-2755 (OS/2 Warp)
.millust end
.np
You can also order copies of these books from an IBM authorized dealer
or IBM representative.
.endnote
.* ----------------------------------------------------------------
.if '&lang' eq 'C/C++' .do begin
.*
.section Virtual Device Driver Developers
.*
.begnote
.*
.note Writing Windows Virtual Device Drivers
.br
David Thielen and Bryan Woodruff; Addison-Wesley Publishing Company,
1994.
.endnote
.do end
.*
.endlevel
:cmt..*
:cmt..section Documentation Conventions
:cmt..*
:cmt..np
:cmt.Monospaced fonts are used to denote how type appears on the computer screen.
:cmt..np
:cmt.Bold
:cmt..np
:cmt.ALL CAPS
:cmt..np
:cmt.Italics
:cmt..np
:cmt.Throughout our manuals you may notice that for our multi-platform tools we
:cmt.have selected Windows as our default environment. Our multi-platform tools will
:cmt.appear similar when running under OS/2 unless otherwise noted.
