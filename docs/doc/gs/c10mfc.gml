.if '&lang' eq 'C/C++' .do begin
.*
.chap Microsoft Foundation Classes
.*
.np
Two versions of MFC are included in this release of the compiler.
Use version &mfc32 to create 32-bit applications.
Use version &mfc16 to create 16-bit applications.
MFC help is available by clicking on the MFC help icon in the &product
folder.
MFC programs will not run under the &company 32-bit Windows Extender.
.*
.section &mfc32
.*
.np
&mfc32 is used to create 32-bit MFC applications that run under the
Win32 API (Windows NT, Windows 95) and Win32s.
There is a batch file that creates the &mfc32 sample programs.
.if '&mfc32' ne 'MFC 4.1' .ty +++ERROR+++ the path for &mfc32 needs to be changed
.code begin
C:&pathnamup.\SAMPLES\MFC\V41\MKSAMPLE.BAT
.code end
.*
.section &mfc16
.*
.np
&mfc16 is used to create 16-bit MFC applications that run under the
Windows API.
There is a batch file that creates the &mfc16 sample programs.
.if '&mfc16' ne 'MFC 2.52b' .ty +++ERROR+++ the path for &mfc16 needs to be changed
.code begin
C:&pathnamup.\SAMPLES\MFC\V252\MKSAMPLE.BAT
.code end
.*
.section Debugging MFC Applications
.*
.np
&product contains compiled libraries for MFC.
For each library that is included, there is a production library and a
"DEBUG" library.
The debug library usually has the same name as the production library
but with a "D" suffix on the file name.
For example,
.fi NRFXCC.LIB
and
.fi NRFXCCD.LIB
are the production and debug versions of one of the libraries included
in the package.
The debug library is compiled with line number debugging information
("d1") and with debugging "assertions" enabled.
.np
If you wish to have a version of the library that contains "d2" type
debugging information then you must recompiled the MFC source code
that is provided.
Makefiles tailored for &cmppname are provided for building the MFC
libraries.
.np
To debug an MFC application, you must ensure the following:
.autopoint
.point
The debug libraries must be installed.
.point
If you want to see the MFC source code when debugging an MFC
function, you must have installed the source code.
You must also use the debugger's "set source path" to set the source
path explicitly in the debugger for the MFC source (when using
libraries that were compiled by &company).
.point
If you want to use the &brname to see MFC classes, you must select the
&cmppname "db" option when compiling your source code.
.endpoint
.np
Please note that if you wish to profile the execution of you MFC
application with the &prfname, you will not be able to drill down to
MFC library source code unless you have recompiled the source code on
your own system.
.*
.do end
