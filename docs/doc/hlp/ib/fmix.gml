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
.ixbook WATCOM FORTRAN 77 User's Guide
.ixhelp fuguide
.ixbook WATCOM FORTRAN 77 Programmer's Guide
.ixhelp fpguide
.ixbook WATCOM FORTRAN 77 Language Reference
.ixhelp f77lr
.ixbook WATCOM FORTRAN 77 Tools User's Guide
.ixhelp ftools
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
.ixbook WATCOM FORTRAN 77 Graphics Library Reference
.ixhelp f77graph
.* .ixbook WATCOM FORTRAN 77 Diagnostic Messages On-line Help
.* .ixhelp wfcerrs
.*
:eGDOC.
