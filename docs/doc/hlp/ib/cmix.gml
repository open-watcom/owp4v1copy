:INCLUDE file='LYTCHG'.
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
.*
.if &e'&dohelp eq 1 .do begin
.dm sepsect begin
.dm sepsect end
.do end
.*
:GDOC.
.*
:BODY.
.*
.if &e'&dohelp eq 1 .do begin
:exhelp
:include file='&book..idx'
:include file='&book..tbl'
:include file='&book..kw'
.do end
.*
.ixbook WATCOM C/C++ User's Guide
.ixhelp cguide
.ixbook WATCOM C/C++ Programmer's Guide
.ixhelp pguide
.ixbook WATCOM C/C++ Tools User's Guide
.ixhelp tools
.ixbook WATCOM Graphical Tools User's Guide
.* .ixhelp ide
.* .ixhelp wbrw
.ixhelp wprof
.* .ixhelp editor
.* .ixhelp resedt
.* .ixhelp resacc
.* .ixhelp resdlg
.* .ixhelp resimg
.* .ixhelp resmnu
.* .ixhelp resstr
.ixhelp rescomp
.* .ixhelp zoom
.* .ixhelp heapwalk
.* .ixhelp spy
.* .ixhelp ddespy
.* .ixhelp drwatcom
.* .ixhelp drnt
.ixbook WATCOM Linker User's Guide
.ixhelp lguide
.ixbook WATCOM Debugger User's Guide
.ixhelp wd
.ixbook WATCOM C Language Reference
.ixhelp clr
.ixbook WATCOM C Library Reference
.ixhelp clib
.ixbook WATCOM C++ Class Library Reference
.ixhelp cpplib
.* .ixbook WATCOM C Diagnostic Messages On-line Help
.* .ixhelp wccerrs
.* .ixbook WATCOM C++ Diagnostic Messages On-line Help
.* .ixhelp wpperrs
.*
:eGDOC.
