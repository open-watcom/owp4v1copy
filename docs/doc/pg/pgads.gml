.if '&lang' eq 'FORTRAN 77' .do begin
:set symbol="slib" value="FLIB7S".
.do end
.*
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
:set symbol="slib" value="CLIB3S".
.do end
.*
:set symbol="acaddir"   value="\acad".
:set symbol="acaddirup" value="\ACAD".
:set symbol="dbgprompt" value="".
.*
.chap Creating AutoCAD Applications
.*
.np
.ix 'AutoCAD support'
.ix 'ADS support' 'DOS version'
.ix 'ADS support' 'release 13'
&product supports the DOS version of the AutoCAD Development System
(ADS) from Autodesk
.if '&lang' eq 'FORTRAN 77' .do begin
(release 12 and earlier versions).
.do end
.el .do begin
(release 13 and earlier versions).
.do end
ADS is a set of &header files and a library.
The &header files (files with extension "&hxt") are located in the
.if '&lang' eq 'FORTRAN 77' .do begin
.fi &pathnamup.\SRC\FORTRAN\ADS
and
.fi &pathnamup.\SRC\FORTRAN\ADS\REL12
directories (assuming you installed &product in the "&pathnamup"
directory).
.do end
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
.fi &acaddirup.\ADS
directory (assuming you installed AutoCAD in the "&acaddirup"
directory).
.do end
Files in this directory with extension ".&langsuffup" are sample ADS
applications.
The ADS library is called
.fi WCADS.LIB
and is located in the
.fi &acaddirup.\ADS
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
directory.
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
directory (assuming you installed AutoCAD in the "&acaddirup"
directory).
.do end
.np
The ADS library
.fi WCADS.LIB
is in a format that is specific to the Phar Lap development tools and
must be converted to a standard form so that the &lnkname can read the
library dictionary.
This is achieved by issuing the following two commands.
.autonote
.note
Protect the old ADS library by renaming it.
.exam begin
&prompt.ren wcads.lib owcads.lib
.exam end
.note
Convert the library by issuing the following command.
.exam begin
&prompt.&libcmd wcads +owcads.lib
.exam end
.endnote
.*
.section Compiling an ADS Application
.*
.np
.ix 'ADS application' 'compiling'
As an example, we will create the ADS application contained in the
file
.fi FACT.&langsuffup..
In order to compile this file, we must set the
.ev &incvarup
environment variable to the
.if '&lang' eq 'FORTRAN 77' .do begin
path that contains the necessary &header files.
.exam begin
&prompt.&setcmd &incvar=&pathnam.&hdrdir.\ads
.exam end
.do end
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
paths that contain the necessary &header files.
.exam begin
&prompt.&setcmd. &incvar.=&pathnam.&hdrdir.&ps.&acaddir.\ads
.exam end
.do end
.pc
We can now compile
.fi FACT.&langsuffup
by issuing the following command.
.exam begin
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
&prompt.&ccmd32 /fpi87 /3s fact
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
&prompt.&ccmd32 /fpi87 /3 /sc fact
.do end
.exam end
.pc
The "fpi87" option tells the compiler to generate in-line 80x87
floating-point instructions.
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
The "3s" option selects 80387 instruction timings and the stack-based
calling convention.
The stack-based calling convention is required for AutoCAD
applications.
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
The "3" option selects 80387 instruction timings.
The "sc" option selects the stack-based calling convention required
for AutoCAD applications.
.do end
.*
.section Linking an ADS Application
.*
.np
.ix 'ADS application' 'linking'
Each ADS application requires a special version of the startup module
that is contained in the &product run-time library
.fi &slib..LIB.
This special version,
.fi ADSSTART.OBJ,
is located in the
.fi &pathnamup.&libdirup32.\DOS
directory.
It is automatically included when you use the
.mono system ads
directive shown below.
.np
For ease of use, create the following linker directive file and name
it
.fi FACT.LNK.
.millust begin
system ads
file fact
library &acaddir.\ads\wcads
.millust end
.pc
We can now link our ADS application by issuing the following command.
.exam begin
&prompt.&lnkcmd @fact
.exam end
.*
.section One-Step Compiling and Linking
.*
.np
.ix 'ADS application' 'compile and link'
For simple applications, the above steps can be combined into a single
command as follows:
.exam begin
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
&prompt.&wclcmd32 /l=ads /fpi87 /3s &acaddir.\ads\wcads.lib fact
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
&prompt.&wclcmd32 /l=ads /fpi87 /3 /sc &acaddir.\ads\wcads.lib fact
.do end
.exam end
.np
The &wclcmdup utility will automatically generate the appropriate
linker directive file.
.*
.section Debugging an ADS Application
.*
.np
.ix 'ADS application' 'debugging'
If you wish to debug your ADS application with the &dbgname, you must
specify the "d1" or "d2" option when compiling the source code.
The "d1" compiler option generates only line numbering information;
the "d2" compiler option generates full debugging information that
includes symbolic information for all variables.
.exam begin
.if '&lang' eq 'C' or '&lang' eq 'C/C++' .do begin
&prompt.&ccmd32 /fpi87 /3s /d2 fact
.do end
.if '&lang' eq 'FORTRAN 77' .do begin
&prompt.&ccmd32 /fpi87 /3 /sc /d2 fact
.do end
.exam end
.pc
When we link our application we must inform the &lnkname to create an
executable file that contains the debugging information generated by
the compiler.
This is done by adding the "DEBUG ALL" directive to our directive file.
.millust begin
debug all
system ads
file fact
library &acaddir.\ads\wcads
.millust end
.pc
We must link our application again.
.exam begin
&prompt.&lnkcmd @fact
.exam end
.np
.ix 'ACAD.ADS'
Before running the &dbgname, add the following line to your
.fi ACAD.ADS
file.
.millust begin
&pathnam.\binw\adshelp.exp
.millust end
.pc
The
.fi ACAD.ADS
file contains a list of ADS applications that are loaded by AutoCAD
when AutoCAD is loaded.
.ix 'ADSHELP.EXP'
The
.fi ADSHELP.EXP
file is an ADS application that is required by the &dbgname for
debugging ADS applications.
.np
An earlier version of the &dbgname required that you set the
.ev DOSX
environment variable to PRIVILEGED
.mono (DOSX=-priv)
for AutoCAD Release 12 since AutoCAD was linked as UNPRIVILEGED.
Starting with version 10 of the debugger, this is no longer required
and, in fact, will cause unpredictable results for release 13 if it is
set.
.np
To invoke the &dbgname, enter the following command.
.exam begin
&prompt.&dbgcmd. /tr=ads /swap
.exam end
.pc
If you have a two-monitor setup, you may omit the "swap" option.
.np
.ix 'ADS.TRP'
Note that we did not specify the AutoCAD executable file; the debugger
trap file,
.fi ADS.TRP,
will load AutoCAD automatically.
You should now be in the &dbgname..
At this point, enter the following debugger command.
.exam begin
&dbgprompt.ads fact.exp
.exam end
.np
You should now be in AutoCAD.
When you load your ADS application from AutoLISP, the debugger will be
entered and source for your program should be displayed in the source
window.
.ix 'ADS.DBG'
The
.fi ADS.DBG
file contains a sequence of debugger commands that starts AutoCAD,
loads the debugging information from the executable file you specify,
and relocates address information based on the code and data selector
values for your application.
You are now ready to debug your ADS application.
.np
For large ADS applications, you may get an error when the "ADS"
debugger command file is invoked indicating that the debugger was
unable to load the debugging information from the executable file
because of memory constraints.
If the error message
.us "no memory for debugging information"
or
.us "no memory for debugging information - increase dynamic memory"
is issued, use the debugger "dynamic" option to increase the amount of
dynamic memory (the default is 40k).
The following example increases the amount of dynamic memory to 60k.
.exam begin
&prompt.&dbgcmd. /tr=ads /swap /dynamic=60k
.exam end
