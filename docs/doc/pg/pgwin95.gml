.chap *refid=win95 Windows 95 SDK Notes
.*
.np
.ix 'Win95 SDK'
.ix 'Windows 95 SDK'
The Windows 95 SDK is available from Microsoft.
At present, the SDK header files and sample programs contain Microsoft
Visual C++ dependencies and some errors.
If you wish to use the Windows 95 SDK for application development or
if you want to build the sample programs using &product,
you should read the following notes on changes required to the SDK
source code.
These notes apply to the BETA 2 release of the Microsoft Windows 95 SDK.
If you have a later release, you may find some differences in the
source code as we describe it below.
.*
.section *refid=win95a Windows 95 SDK Header Files
.*
.np
.ix 'Windows 95 SDK' 'header files'
The following changes are required to the Windows 95 SDK header files
(located in
.fi \SDK\INC32
.ct ).
.*
.sect WINDOWS.H
.*
.np
.ix 'Windows 95 SDK' 'WINDOWS.H'
Modify the
.fi WINDOWS.H
header file as follows:
.autonote
.note
After the following line:
.millust begin
#define _WINDOWS_
.millust end
.pc
add:
.millust begin
#ifdef __WATCOMC__

    // Temporary stuff for win95

    #ifndef WIN32
    #define WIN32 100
    #endif

    #ifndef _WIN32
    #define _WIN32
    #endif

    #ifndef _MSC_VER
    #define _MSC_VER 800
    #endif

.millust break
#ifdef __cplusplus
#define _CFRONT_PASS_

#ifndef _WCHAR_T_DEFINED_
#define _WCHAR_T_DEFINED_
#define _WCHAR_T_DEFINED
typedef long char wchar_t;
#endif
#else
#ifndef _WCHAR_T_DEFINED_
#define _WCHAR_T_DEFINED_
#define _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#endif
#endif

.millust break
#undef cdecl
#define i386
#define _X86_
#define I386_HOST       1
#ifndef _M_IX86
#define _M_IX86         300
#endif
struct _EXCEPTION_RECORD;
struct _CONTEXT;
struct _RTL_CRITICAL_SECTION;
struct tagCREATESTRUCTA;
struct tagCREATESTRUCTW;
#endif
.millust end
.note
Comment out the lines containing
.mono #include &lc.excpt.h&rc.
and the
.mono #include &lc.winperf.h&rc.
and the
.mono #include &lc.winsvc.h&rc..
These files are not included in the BETA 2 SDK.
.endnote
.*
.sect WINNT.H
.*
.np
.ix 'Windows 95 SDK' 'WINNT.H'
Locate the section in this header file marked by the lines that say:
.millust begin
#if defined( MIDL_PASS ) || defined( RC_INVOKED )
.millust end
.pc
and
.millust begin
#elif defined( _M_MRX000 )
.millust end
.np
Add the following new section just before the line that contains:
.millust begin
#elif defined( _M_MRX000 )
.millust end
.np
The source code for this new section can be found in &company's
Windows NT version of
.fi WINNT.H.
.millust begin
#elif defined(__WATCOMC__)

LONGLONG
Int32x32To64 (
    LONG Multiplier,
    LONG Multiplicand
    );

.millust break
DWORDLONG
UInt32x32To64 (
    DWORD Multiplier,
    DWORD Multiplicand
    );

.millust break
DWORDLONG
Int64ShllMod32 (
    DWORDLONG Value,
    DWORD ShiftCount
    );

.millust break
LONGLONG
Int64ShraMod32 (
    LONGLONG Value,
    DWORD ShiftCount
    );

.millust break
DWORDLONG
Int64ShrlMod32 (
    DWORDLONG Value,
    DWORD ShiftCount
    );

.millust break
#pragma aux Int32x32To64 = \
    "imul edx" \
    parm [eax] [edx] \
    value no8087 [eax edx];

.millust break
#pragma aux UInt32x32To64 = \
    "mul edx" \
    parm [eax] [edx] \
    value no8087 [eax edx];

.millust break
#pragma aux Int64ShllMod32 = \
    "shld edx,eax,cl" \
    "shl eax,cl" \
    parm [eax edx] [ecx] \
    modify exact [eax edx] \
    value no8087 [eax edx];

.millust break
#pragma aux Int64ShraMod32 = \
    "shrd eax,edx,cl" \
    "sar edx,cl" \
    parm [eax edx] [ecx] \
    modify exact [eax edx] \
    value no8087 [eax edx];

.millust break
#pragma aux Int64ShrlMod32 = \
    "shrd eax,edx,cl" \
    "shr edx,cl" \
    parm [eax edx] [ecx] \
    modify exact [eax edx] \
    value no8087 [eax edx];
.millust end
.*
.sect RPC.H
.*
.np
.ix 'Windows 95 SDK' 'RPC.H'
Comment out the line:
.mono #include &lc.excpt.h&rc..
This file is not included in the BETA 2 SDK.
:cmt. .*
:cmt. .sect OLE header files
:cmt. .*
:cmt. .np
:cmt. .ix 'Windows 95 SDK' 'OLE headers'
:cmt. There are also errors in some of the OLE header files, where
:cmt. functions were defined as being "__stdcall __stdcall".
:cmt. Remove one of the "_stdcall" declaratives.
.*
.section General Notes on Microsoft - &company Compatibility
.*
.np
.ix 'Windows 95 SDK' 'compatibility issues'
The following sections describe issues with Microsoft - &company
compatibility.
.beglevel
.*
.section *refid=win95b Makefile Syntax
.*
.np
.ix 'Windows 95 SDK' 'makefile syntax'
If an application comes with a makefile, there may be some syntax
changes required.
The following are some areas where makefile content or syntax differs.
.millust begin
1.  Microsoft                   &company.
    ---------                   ------
    CC = cl                     CC = wcc386
    CPP = cl                    CPP = wpp386
    CXX = cl                    CXX = wpp386
    RC = rc                     RC = &wrccmd
.millust end
.millust begin
2.  Microsoft                   &company.
    ---------                   ------
    !if "$(DEBUG)" == "1"       !ifeq DEBUG 1
.millust end
.millust begin
3.  Microsoft                   &company.
    ---------                   ------
    !if [if exist X.Y del X.Y]  .before
    !endif                          if exist X.Y do del X.Y
.millust end
.millust begin
4.  Microsoft                   &company.
    ---------                   ------
    OBJS = ALOCFREE.OBJ \       OBJS = ALOCFREE.OBJ &
            CODEPAGE.OBJ \              CODEPAGE.OBJ &
            CONINFO.OBJ \               CONINFO.OBJ &
            CONMODE.OBJ \               CONMODE.OBJ &
            CONSOLE.OBJ \               CONSOLE.OBJ &
            CONTITLE.OBJ \              CONTITLE.OBJ &
            CREATE.OBJ                  CREATE.OBJ
.millust end
.code begin
5.  Microsoft
    ---------
    $(PROJ).EXE:    MOD1.OBJ MOD2.OBJ MOD3.OBJ $(OBJS_EXT)
       echo &rc.NUL @&lc.&lc.$(PROJ).CRF
    $(LFLAGS)
    MOD1.OBJ
    MOD2.OBJ
    MOD3.OBJ
    $(OBJS_EXT)
    -OUT:$(PROJ).EXE
    $(MAPFILE_OPTION)
    $(LIBS)
    $(DEFFILE_OPTION) -implib:$(PROJ).lib
    &lc.&lc.
        link @$(PROJ).CRF


    &company.
    ------
    OBJS = MOD1.OBJ MOD2.OBJ MOD3.OBJ

    $(PROJ).EXE : $(OBJS)
        @%create $(PROJ).CRF
        @%append $(PROJ).CRF $(LFLAGS)
        @for %f in ($(OBJS)) do @%append $(PROJ).CRF file %f
        @for %f in ($(OBJS_EXT)) do @%append $(PROJ).CRF file %f
        @%append $(PROJ).CRF NAME $(PROJ).EXE
        @%append $(PROJ).CRF $(MAPFILE_OPTION)
        @%append $(PROJ).CRF LIBRARY $(LIBS)
        wlink @$(PROJ).CRF
.code end
.*
.section Compiler Flags
.*
.np
.ix 'Windows 95 SDK' 'compiler flags'
If an application comes with a makefile, the compiler flags will have
to be changed.
.np
When doing Win32 programming, you should use the /ei and /zp4 options
to compile C and C++ code with the &company compilers since this
adjusts the compilers to match the default Microsoft compiler
behaviour.
Some Microsoft software examples rely on the default behaviour of
their own compiler regarding the treatment of enums and structure
packing alignment.
.np
Here is a list of Microsoft compiler options and the approximate
equivalent for &company's compilers.
If there is no approximate equivalent, we have recorded "..." (not
applicable) in the &company column.
.code begin
Microsoft   &company.
---------   ------
/c          ... compile only (.c -&rc. .obj)
/Dxxx       /Dxxx
/D "xxx"    /Dxxx
/DEBUG      /d3
/FA[c|s]    ... .ASM listing control
/Fa         ... listing file name control
/Fd         ... program database file name control
/Fm         ... .MAP file control
/Fo         /fo=
/Fp         /fh=
/FR         /DB
/FRname     /DB
/Fr         ...
/G3         /3
/G4         /4
/G5         /5
/GB         ... blend of 3/4/5
/Gd         ... use __cdecl for all non-C++ functions
/Gr         ... use __fastcall for all non-C++ functions
/Gz         ... use __stdcall for all non-C++ functions
/GX         /xs (C++ only)
/GX-        /xd
/Gy         /zm
/Gf         ... eliminate duplicate strings (.EXE-wide)
/I          /i=
/LD         /bd create dll
/MD         /bm /bd create multi-threaded .DLL
/ML         default create single-threaded app
/MT         /bm create multi-threaded app
/NOLOGO     /zq
/O1         ... smaller code optimizations
/O2         ... faster code optimizations
/Oa         /oa
/Ob[0,1,2]  /oe[=num]
/Od         /od
/Og         default optimize entire functions
/Oi         /oi
/Op         /op
/Os         /os
/Ot         /ot
/Ow         ... function calls don't modify memory or use aliases
/Ox         /ox
/Oy         by default (/of[+] to turn off)
/u          ... no pre-defined macros
/Uxxx       /uxxx
/W0         /w0
/W1         /w1
/W2         /w2
/W3         /w3
/W4         /w4
/WX         /we
/X          ... ignore standard include paths (must use -I)
/YX[file]   /fh[q][=file]
/Yc[file]   /fh[q][=file]
/Yu[file]   /fh[q][=file]
/Za         /za
/Ze         /ze
/Zd         /d2
/Zi         /d1
/Zn         ... include unreferenced symbols in browser info
/Zp1        /zp1
/Zp2        /zp2
/Zp4        /zp4
/Zp8        /zp8
/Zp16       /zp16
.code end
.np
When in doubt, delete the option.
:CMT. .np
:CMT. Most command-line #define's (/d) can be left alone.
:CMT. One exception that should be deleted is:  /D"__X86__".
:CMT. It causes problems with &lc.excpt.h&rc..
.np
If you are using C instead of C++, add
.mono /dDBG=0
to the command line.
The header file
.fi objbase.h
contains the line
.millust begin
#if DBG == 1
.millust end
.pc
which will result in an error when
.mono DBG
is not defined.
.np
If you are starting without a makefile, try this command line:
.millust begin
wcc386 /w4/zq/bt=nt /dDBG=0 ...
.millust end
:CMT. .np
:CMT. Add
:CMT. .millust begin
:CMT. /dWIN32=100 /d_WIN32 /d_MSC_VER=800
:CMT. .millust end
:CMT. .pc
:CMT. to all of the above.
:CMT. Also, see :HDREF refid='win95c'. for other changes to header files in
:CMT. the SDK.
.*
.section Linker Flags
.*
.np
.ix 'Windows 95 SDK' 'linker flags'
&company's linker command-line format is very different from
Microsoft's, but here are a few tips:
.begbull
.bull
Change "/NOLOGO" to "option quiet".
.bull
Change "/DEBUG /DEBUGTYPE:cv" to "debug all".
Make sure that "debug all" appears near the top of the linker command
file before any object files are listed.
.bull
Change "/SUBSYSTEM:console" to "sys nt", and change
"/SUBSYSTEM:windows [...]" to "sys nt_win" or "sys nt_dll" as
appropriate.
.bull
Change "/OUT:&lc.name&rc." to "name &lc.name&rc.".
.bull
Change "/MAP:&lc.name&rc." to "option map=&lc.name&rc.".
.bull
Change "/IMPLIB:&lc.name&rc." to "option implib".  This is DLL specific.
.bull
Change "/ENTRY:&lc.symbol&rc." to "option start=&lc.symbol&rc.".
Note that "&lc.symbol&rc." must be converted to an assembly language symbol,
so tack on underscores and COFF symbology where appropriate.
.bull
Do not forget to separate the object files and libraries into a
"file" list and a "library" list.
Add "libpath" commands if necessary.
.bull
Remove everything related to .SBR files, .BSC files, and .PDB files.
(These are related to the Visual C++ IDE.)
.bull
Remove .RES files from the object file list, since &company's linker
does not do that.
Instead, use "&wrccmd" to link the .RES file to the resulting
executable.
.bull
If you do not recognize a Microsoft linker option, you should probably
just remove it.
.endbull
.np
Sometimes Microsoft's
.kw nmake
creates a temporary file to hold linker options by doing this in a
command list:
.millust begin
link @&lc.&lc. [linker options] &lc.&lc.
.millust end
.np
&makname does not support this syntax, so use the following:
.millust begin
@%write somefile.lnk [linker options]
wlink @somefile.lnk
.millust end
.np
Also, see :HDREF refid='win95b'. for another example of makefile
syntax relating to creation of linker command files.
.*
.section Exports from a DLL and COFF symbols
.*
.np
.ix 'Windows 95 SDK' 'exports'
.ix 'Windows 95 SDK' 'COFF'
.ix 'Windows 95 SDK' '.DEF files'
Microsoft uses .DEF files a lot of the time instead of the _export and
_declspec(dllexport) keywords.
The short way to fix this is to add the _export keyword by hand and
forget about the .DEF file, but you cannot always do this (for
instance, when your function will be imported by Windows itself, and
the header for the function is defined in a Windows header file
without the _export keyword).
The long way to fix it is:
.begbull
.bull
If you want to preserve the .DEF file,
copy it to something with a .WDF extension.
.bull
Edit the .WDF file and remove everything except the "EXPORTS"
section.
.bull
Change the "EXPORTS" keyword to "EXPORT" to form a &lnkname keyword.
.bull
Change each entry in the "exports" section from:
.millust begin
&lc.symbol name&rc. @n     &lc.- Microsoft format
.millust end
.pc
to
.millust begin
&lc.symbol name&rc..n      &lc.- &company format
.millust end
.bull
Include commas at the end of all but the last entry.
.bull
Replace each function name with its corresponding assembler symbol.
If you cannot figure out what the assembler symbol will be, link the
application once to create a map file ("option map"); the symbols will
be in the map file.
.bull
COFF symbols contain the character "@".
These symbols must be placed inside of apostrophes:
.exam begin
'_COFF_stdcall_function@n' (where n is some number)
.exam end
.bull
Finally, include the .WDF file on the linker command line:
.millust begin
&lnkcmd @myapp.lnk @myapp.wdf
.millust end
.endbull
.*
.section *refid=win95c General Source Code Changes
.*
.ix 'Windows 95 SDK' 'source code changes'
.begbull
.bull
&product does not support Microsoft's "__inline" keyword.
Use of this keyword should be deleted.
.np
Note: This keyword is used in the
.fi winnt.h
header file; however conditional code was added that avoids the
segment of the source code where this keyword is used.
See :HDREF refid='win95a'. for a discussion of changes to the SDK
header files.
.bull
The &wrcname, &wrccmdup, does not support certain Microsoft RC
keywords, namely RCINCLUDE.
Replace these with "#include" statements.
.exam begin
Microsoft                   &company.
---------                   ------
RCINCLUDE about.dlg         #include "about.dlg"
.exam end
.bull
&wrccmdup sometimes complains about .RC files which give Microsoft RC
no trouble.
If this happens, try replacing each comma in the .RC file with a
space; this should fix it.
.np
The problem is that &wrccmdup uses a grammar while Microsoft RC does
not.
Thus RC accepts things that are not grammatical (such as commas just
before an "END").
.bull
There is an incompatibility between Microsoft and &company compilers
in the support of the try/except/finally mechanism;
Microsoft now supports two underscores.
Unfortunately, every RPC sample in the SDK seems to use it.
.np
There is one possible solution:
Modify the
.fi rpc.h
header file to remove the extra underscore used in these keywords.
.exam begin
Microsoft                   &company.
---------                   ------
#define RpcTry __try {      #define RpcTry _try {
.exam end
.endbull
.*
.section Creating Libraries (other than import libraries)
.*
.np
.ix 'Windows 95 SDK' 'libraries'
Some examples in the SDK create static libraries; changing the
makefile to use &libcmdup is a trivial matter.
.exam begin
&libcmd applib +applib +othermod +thirdmod
.exam end
.*
.section Standard Makefiles
.*
.np
.ix 'Windows 95 SDK' 'makefiles'
If a makefile includes a "generic" makefile like
.fi ntwin32.mak,
remove the include statement.
The "generic" makefiles are too problematic to bother converting to
&makname format.
.np
Instead, just use the command-line arguments suggested above.
As for referenced libraries, the following is a fairly complete list:
.code begin
KERNEL32,USER32,GDI32,ADVAPI32,COMDLG32,WINSPOOL,OLE32,OLEAUT32,UUID
.code end
.pc
If that is too long for your taste, compile your application once to
create a map file and then go back and remove any libraries not
mentioned in the map file.
Most applications do not use anything but the first three,
and possibly OLE32.
.*
.section IDL compilation
.*
.np
.ix 'Windows 95 SDK' 'IDL'
You can use &company's compiler (or rather, &company's preprocessor)
with Microsoft's "midl" compiler. Instead of specifying
.millust begin
midl ... /cpp_cmd cl ...
.millust end
.pc
try the following:
.millust begin
midl ... /cpp_cmd wcc386 /cpp_opt "/pw=0" ...
.millust end
.*
.endlevel
.*
.section Specific Notes on the SDK Samples
.*
.ix 'Windows 95 SDK' 'sample notes'
.begnote $break
.note \sdk\samples\win32\existing\console
.*
The source code must be compiled with /zp4 to use the console
functions correctly.
.*
.note \sdk\samples\win32\existing\fontview
.*
Add the line
.mono "return TRUE;"
before line 94 of
.fi status.c
and line 92 of
.fi tools.c.
.*
.note \sdk\samples\win32\existing\getsys
.*
Add the line
.mono "return TRUE;"
to end of function
.mono MainDlgProc
in
.fi getsys.c
to silence the compiler warning.
.*
.note \sdk\samples\win32\existing\icmtest
.*
In
.fi dib.c
at lines 309 and 316, change
.mono "(DWORD) pRgb += ..."
to
.mono "pRgb += (DWORD) ...".
.np
This application uses a function from
.fi winspool.lib
(an import library for
.fi winspool.drv
.ct ); however, &company's linker links the executable to
.fi winspool.dll.
We are working on a solution to this; in the meantime copy
.fi winspool.drv
to
.fi winspool.dll.
.np
This example is just buggy, but the remaining bugs are in the source
code;
the Microsoft compiler does no better.
We did not have time to track down the problems.
.*
.note \sdk\samples\win32\frmwork\editsdi
.*
This example uses
.mono __inline
keywords in the following files:
.millust begin
GLOBALS.H       lines 91 and 95
EDITSDI.C       line 528
FILE.C          line 125
.millust end
.*
.note \sdk\samples\win32\frmwork\gdidib
.*
This example causes two linker warnings about redefinition of the
symbols
.mono _szBuffer
and
.mono _rcExamples.
However, there are no problems when executing the application.
.*
.note \sdk\samples\win32\frmwork\gdiinput
.*
Same linker warning as above.
.*
.note \sdk\samples\win32\frmwork\gdimeta
.*
This example has
.mono __inline
keywords in the following files:
.millust begin
GLOBALS.H       line 93
FILE.C          line 64
.millust end
.np
In
.fi client.c
at line 792, change the initialization of "rc" to:
.millust begin
Rect rc = {0,0,0,0}; rc.right=cxPict; rc.bottom=cyPict;
.millust end
.np
Same linker warning as above.
.*
.note \sdk\samples\win32\frmwork\iathread
.*
Add the WINAPI keyword to declarations of
.mono ChildThreadProc
in
.fi mdichild.c
and
.fi globals.h.
This declarative is required for any function whose address is passed
to Microsoft's CreateThread.
.np
Programs using Microsoft's API thread functions should be compiled
with the /bm option.
.*
.note \sdk\samples\win32\frmwork\listctrl
.*
When compiled as is, the list control is stuck in Details mode and
cannot be changed.
The call to SetWindowLong, which is supposed to change it, does not
generate the proper events.
Here is a fix:
.np
In
.fi listview.c
at line 510, change:
.code begin
SetWindowLong(hwndLV, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | dwView);
.code end
.pc
to
.code begin
STYLESTRUCT ss;
SetWindowLong(hwndLV, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | dwView);
ss.styleOld = dwStyle;
ss.styleNew = (dwStyle & ~LVS_TYPEMASK) | dwView;
SendMessage( hwndLV, WM_STYLECHANGING, GWL_STYLE, (LPARAM) &ss );
SendMessage( hwndLV, WM_STYLECHANGED, GWL_STYLE, (LPARAM) &ss );
.code end
.*
.note \sdk\samples\win32\frmwork\multipad
.*
This example has
.mono __inline
keywords in:
.millust begin
GLOBALS.H       line 120
FILE.C          line 90.
.millust end
.*
.note \sdk\samples\win32\frmwork\ole2\simpcntr
.*
In
.fi site.cpp"
at line 56, change
.mono OLESTR("Object")
to something like
.mono OLESTR("Object\0\0").
The reason is, the call to
.mono IStorage::CreateStorage
eventually calls
.mono wcslen,
a double-byte char
.mono strlen
function.
If you do not have two nulls at the end of your string,
.mono wcslen
returns an absurd number and the program crashes.
.np
This is a problem in the Microsoft libraries.
The only reason this code does not crash under Microsoft's run-time
system is that the Microsoft compiler does not pack their string data
as tightly, leaving strings of null bytes all over memory.
In the example,
.mono wcslen
still does not return a sensible value; it just returns a value low
enough to keep the program from crashing.
.*
.note \sdk\samples\win32\frmwork\ole2\simpsvr
.*
Place
.millust begin
#ifndef STRICT

#endif
.millust end
.pc
statements around the
.mono #define STRICT 1
statement in
.fi simpui.h
at line 13.
.*
.note \sdk\samples\win32\frmwork\pview95
.*
In
.fi pview95.c
at line 382, change the initialization of
.mono pt
to
.millust begin
POINT pt; pt.x = LOWORD(lparam); pt.y = HIWORD(lparam);
.millust end
.*
.note \sdk\samples\win32\frmwork\propshet
.*
In the
.mono DoPropertySheet
function of
.fi propshet.c,
use
.mono memset
to fill the property sheet structures with zeroes before initializing
the structure fields.
.code begin
memset( psp, 0, 2*sizeof(PROPSHEETPAGE) );
psp[0].dwSize = sizeof(PROPSHEETPAGE);
    .
    .
    .
memset( &psh, 0, sizeof(PROPSHEETHEADER) );
psh.dwSize = sizeof(PROPSHEETHEADER);
.code end
.*
.note \sdk\samples\win32\frmwork\threads
.*
Add a
.mono NULL
as the second argument to
.mono _beginthread
in
.fi threads.c
at line 535 to match the prototype of the &company version of that
function.
.np
Compile the application source code with the /bm option.
.*
.note \sdk\samples\win32\frmwork\rtfedit
.*
Compile the application source code with the /zp4 option.
.*
.note \sdk\samples\win32\frmwork\writepad
.*
Remove the
.mono __inline
keywords from:
.millust begin
GLOBALS.H       line 127
FILE.C          line 91
.millust end
.np
Compile the application source code with the /zp4 option.
.*
.note \sdk\samples\win32\frmwork\mixtree
.*
The linker gives a number of warnings re: redefinition of symbols.
.*
.note \sdk\samples\win32\newshell\dropext
.*
In
.fi dropext.c
at line 355, the initialization of
.mono fmte
is not standard C.
Initialize each field separately after the declaration.
.code begin
FORMATETC fmte;
    .
    .
    .
fmte.cfFormat = g_cfNetResource? g_cfNetResource
    : (g_cfNetResource=RegisterClipboardFormat("Net Resource"));
fmte.ptd = (DVTARGETDEVICE FAR *) NULL;
fmte.dwAspect = DVASPECT_CONTENT;
fmte.lindex = -1;
fmte.tymed = TYMED_HGLOBAL;
.code end
.*
.note \sdk\samples\win32\newshell\shesamp
.*
In
.fi shesamp.c
at line 422, the initialization of
.mono fmte
is not standard C.
Initialize each field separately after the declaration.
.code begin
FORMATETC fmte;
    .
    .
    .
fmte.cfFormat = g_cfNetResource ? g_cfNetResource :
        (g_cfNetResource = RegisterClipboardFormat(CFSTR_NETRESOURCES));
fmte.ptd = NULL;
fmte.dwAspect = DVASPECT_CONTENT;
fmte.lindex = -1;
fmte.tymed = TYMED_HGLOBAL;
.code end
.*
.note \sdk\samples\win32\newshell\wizard
.*
In
.fi wizard.c
above line 270, add the line:
.millust begin
return TRUE;
.millust end
.*
.note \sdk\samples\fileview\fparser
.*
In
.fi vsctop.h
at line 621, cast the RHS to
.mono (VOID(SO_ENTRYMOD *)()).
.*
.note \sdk\samples\ole\winhlprs
.*
In
.fi cwindow.h
at line 84, remove the "::".
It is unnecessary, possibly illegal in standard C++, and confuses the
&company compiler.
.*
.note \sdk\samples\ole\ole2ui
.*
In
.fi ..\include\ole2ui.h,
add
.mono #undef IDHELP
just before line 203 to prevent a name conflict.
.np
In
.fi insobj.c
at line 869, change
.mono _getcwd
to
.mono getcwd.
.*
.note \sdk\samples\ole\mfract
.*
To permit OLE to connect to the DLL's used as engines in this sample,
the symbols in the DLL must be exported in an explicit way.
.np
Any DLL attempting to be an OLE server should be linked with the
following flags:
.millust begin
option start='_DllMain@12'
export DllCanUnloadNow.1='_DllCanUnloadNow@0' ,
       DllGetClassObject.2='_DllGetClassObject@12'
.millust end
.np
You cannot just use "_export" or "_declspec(dllimport)"
because the prototypes for
.mono DllGetClassObject
and
.mono DllCanUnloadNow
are defined in the Windows header files without such keywords.
Also, OLE looks up these functions by ordinal, and &company's linker
gets the ordinals backwards by default.
So you have to perform the export explicitly.
.np
Add the
.mono WINAPI
keyword to the definition of any function called
.mono GraphicsThread.
Thread functions used with the
.mono CreateThread
library function must be defined
.mono WINAPI
or
.mono __stdcall
in order to receive their arguments properly.
.np
At the time of writing, DLL's with entry points defined by "option
start" cannot seem to call standard library functions like "sprintf"
and "srand" without crashing.
.*
.bi I've sent Greg Bentz mail on this, but it was 6 hours before his
.bi vacation :-) so this may have to wait for the new year.
.*
.note \sdk\mmedia\samples\mciapp
.*
In
.fi appport.h,
at lines 34-37, change
.mono _stdcall
to
.mono __stdcall.
.np
Define
.mono /dDEBUG=NO
on the command line, so that the
.mono DPF
macro will compile properly.
.np
In
.fi appinit.c
at line 769, cast the RHS to
.mono (PENWINREGISTERPROC)
instead of the LHS.
.np
When calling a Microsoft va_args function such as
.mono wvsprintf(),
the va_list argument must be changed somewhat; i.e. change
.millust begin
wvsprintf( s1, s2, va )
.millust end
.pc
to
.millust begin
wvsprintf( s1, s2, (va_list) *va )
.millust end
.np
The "*" is to deal with the difference between &company's and
Microsoft's definition of a
.mono va_list;
&company's va_list is a (char **) while Microsoft's libraries expect a
(char *).
The cast is to keep &company's compiler happy.
.endnote
.np
Obviously this list is not comprehensive. Many OLE samples remain
untried, and most of the RPC, multimedia, and pen samples were
untouched. However, we tested all of the samples in the "samples\win32"
directory; if a function from that directory is not listed above, it
did not contain any problems not mentioned in the "general" or "C++"
sections.
.*
.section C++ Notes
.*
.ix 'Windows 95 SDK' 'C++ notes'
.begbull
.bull
Some of the examples in the SDK, most notably the OLE samples, are
written in C++. Most are written in C.
.bull
Of the examples written in C, most will NOT compile cleanly under C++
largely due to pointer and function pointer type mismatches (it
appears that Microsoft is not careful about this).
If they do compile, it will probably be with a large number of
warnings.
The RPC examples (which use _try/_except/_finally) will probably never
compile under &company's C++ compiler.
.np
One such type mismatch came up frequently: in many of the samples in
.fi samples\win32\frmwork
(which share a common source base for the most part) the file
.fi about.c
will contain a call to
.mono VerQueryValue
somewhere around line 201.
The third argument to that function has to be cast to
.mono (LPVOID *)
explicitly.
.bull
Of course, there is no reason that you would really want to compile
all of the C source files as C++ if you did not have to.
.endbull
