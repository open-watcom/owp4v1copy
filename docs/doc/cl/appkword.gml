.pp
The following topics are discussed:
.begbull
.bull
Standard Keywords
.bull
&wcboth. Keywords
.endbull
.*
.section Standard Keywords
.*
.pp
The following is the list of keywords reserved by the
C language:
..sk 1 c
.im kwlist
.*
.************************************************************************
.*
..if '&target' eq 'PC' or '&target' eq 'PC 370' ..th ..do begin
.*
.section &wcboth. Keywords
.*
.pp
The &wcboth. compilers also reserve the following keywords:
..sk 1
.im kwwatcom
.pc
The &wc386. compiler also reserves the following keywords:
..sk 1
.im kw386
.pc
The keywords
.kwix &kwbased_sp.
.kwfont &kwbased.
..ct ,
.kwix &kwsegm_sp.
.kwfont &kwsegment.
..ct ,
.kwix &kwsegnm_sp.
.kwfont &kwsegname.
and
.kwix &kwself_sp.
.kwfont &kwself.
are described in the section "&basedptr.".
.*
&wcboth. provide the predefined macro
.mkw _based
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwbased_sp.
.kwfont &kwbased.
..ct ..li .
.*
&wcboth. provide the predefined macro
.mkw _segment
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwsegm_sp.
.kwfont &kwsegment.
..ct ..li .
.*
&wcboth. provide the predefined macro
.mkw _segname
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwsegnm_sp.
.kwfont &kwsegname.
..ct ..li .
.*
&wcboth. provide the predefined macro
.mkw _self
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwself_sp.
.kwfont &kwself.
..ct ..li .
.pp
The keywords
.kwix &kwfar_sp.
.kwfont &kwfar.
..ct ,
.kwix &kwhuge_sp.
.kwfont &kwhuge.
and
.kwix &kwnear_sp.
.kwfont &kwnear.
are described in the sections "&ptr86." and "&ptr386.".
.*
&wcboth. provide the predefined macros
.mkw far
and
.mkw _far
for convenience and compatibility with the Microsoft C compiler.
&wc286. provides the predefined macro
.mkw SOMDLINK
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwfar_sp.
.kwfont &kwfar.
..ct ..li .
.*
&wcboth. provide the predefined macros
.mkw huge
and
.mkw _huge
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwhuge_sp.
.kwfont &kwhuge.
..ct ..li .
.*
&wcboth. provide the predefined macros
.mkw near
and
.mkw _near
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwnear_sp.
.kwfont &kwnear.
..ct ..li .
.pp
The keywords
.kwix &kwfar16_sp.
.kwfont &kwfar16.
and
.kwix &kwseg16_sp.
.kwfont &kwseg16.
are described in the section "&ptr386.".
.*
&wcboth. provide the predefined macros
.mkw _far16
and
.mkw _Far16
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwfar16_sp.
.kwfont &kwfar16.
..ct ..li .
.pp
The
.kwix &kwpackd_sp.
.kwfont &kwpacked.
keyword is described in the section
"&struct".
.pp
The
.kwix &kwcdecl_sp.
.kwfont &kwcdecl.
keyword may be used with function definitions, and indicates that the
calling convention for the function is the same as that used by
Microsoft C.
All parameters are pushed onto the stack, instead of being passed
in registers.
This calling convention may be controlled by a
.kwpp #pragma
directive.
See the &userguide..
&wcboth. provide the predefined macros
.mkw cdecl,
.mkw _cdecl
and
.mkw _Cdecl
for convenience and compatibility with the Microsoft C compiler.
&wc286. provides the predefined macro
.mkw SOMLINK
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwcdecl_sp.
.kwfont &kwcdecl.
..ct ..li .
.pp
The
.kwix &kwfortr_sp.
.kwfont &kwfortran.
keyword
may be used with function definitions, and indicates that the
calling convention for the function
is suitable for calling a function written in FORTRAN.
By default, this keyword has no effect.
This calling convention may be controlled by a
.kwpp #pragma
directive.
See the &userguide..
&wcboth. provide the predefined macros
.mkw fortran
and
.mkw _fortran
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwfortr_sp.
.kwfont &kwfortran.
..ct ..li .
.pp
The
.kwix &kwpascl_sp.
.kwfont &kwpascal.
keyword
may be used with function definitions, and indicates that the
calling convention for the function is suitable for
calling a function written in Pascal.
All parameters are pushed onto the stack, but in reverse order to the
order specified by
.kwfont &kwcdecl.
..ct ..li .
This calling convention may be controlled by a
.kwpp #pragma
directive.
See the &userguide..
&wcboth. provide the predefined macros
.mkw pascal
and
.mkw _pascal
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwpascl_sp.
.kwfont &kwpascal.
..ct ..li .
.pp
.ix 'OS/2 convention'
The
.kwix &kwsyscl_sp.
.kwfont &kwsyscall.
keyword may be used with function definitions, and indicates that the
calling convention used is compatible with OS/2 (version 2.0 or
higher).
This calling convention may be controlled by a
.kwpp #pragma
directive.
See the &userguide..
&wcboth. provide the predefined macros
.mkw _syscall,
and
.mkw _System
for convenience and compatibility with the Microsoft C compiler.
&wc386. provides the predefined macros
.mkw SOMLINK
and
.mkw SOMDLINK
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwsyscl_sp.
.kwfont &kwsyscall.
..ct ..li .
.pp
.ix 'Win32 convention'
The
.kwix &kwstdcl_sp.
.kwfont &kwstdcall.
keyword may be used with function definitions, and indicates that the
calling convention used is compatible with Win32.
This calling convention may be controlled by a
.kwpp #pragma
directive.
See the &userguide..
.pp
.ix 'storage duration' 'static'
.ix 'static storage duration'
The
.kwix &kwexprt_sp.
.kwfont &kwexport.
keyword may be used with objects with static storage duration (global
objects) and with functions, and describes that object or function as
being a known object or entry point within a Dynamic Link Library in
OS/2 or Microsoft Windows.
.ix 'linkage' 'external'
.ix 'external linkage'
The object or function must also be declared as having external
linkage (using the
.kw extern
keyword).
.ix 'function' 'call back'
.ix 'call back function'
In addition, any
.ul call back
function whose address is passed to Windows (and which Windows will
"call back") must be defined with the
.kwfont &kwexport.
keyword, otherwise the call will fail and cause unpredictable results.
The
.kwfont &kwexport.
keyword may be omitted if the object or function is exported by an
option specified using the linker.
See the &linkref..
&wcboth. provide the predefined macro
.mkw _export
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwexprt_sp.
.kwfont &kwexport.
..ct ..li .
.pp
The
.kwix &kwintr_sp.
.kwfont &kwintrpt.
keyword may be used with function definitions for functions that
handle computer interrupts.
All registers are saved before the function begins execution and
restored prior to returning from the interrupt.
The machine language return instruction for the function is changed to
.mono iret
(interrupt return).
Functions written using
.kwfont &kwintrpt.
are suitable for attaching to the
.ix 'interrupt'
interrupt vector using the library
function
.libfn _dos_setvect
..ct ..li .
&wcboth. provide the predefined macros
.mkw interrupt
and
.mkw _interrupt
for convenience and compatibility with the Microsoft C compiler.
They may be used in place of
.kwix &kwintr_sp.
.kwfont &kwintrpt.
..ct ..li .
.pp
The
.kwix &kwlodds_sp.
.kwfont &kwloadds.
keyword may be used with functions, and causes the compiler to
generate code that will force the DS register to be set to the default
data segment (DGROUP) so that near pointers will refer to that
segment.
This keyword is normally used with functions written for Dynamic Link
Libraries in Windows and OS/2.
&wcboth. provide the predefined macro
.mkw _loadds
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwlodds_sp.
.kwfont &kwloadds.
..ct ..li .
.pp
The
.kwix &kwsvreg_sp.
.kwfont &kwsaveregs.
keyword may be used with functions.
It is provided for compatibility with Microsoft C, and has no effect
in &wcboth..
&wcboth. provide the predefined macro
.mkw _saveregs
for convenience and compatibility with the Microsoft C compiler.
It may be used in place of
.kwix &kwsvreg_sp.
.kwfont &kwsaveregs.
..ct ..li .
..do end
.*
.************************************************************************
.*
