:INCLUDE file='LYTCHG'.
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
:INCLUDE file='XDEFS'.
:set symbol="lang"       value="FORTRAN 77".
:INCLUDE file='DEFS'.
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
.ixbook &product User's Guide
.ixhelp fuguide
.ixbook &product Programmer's Guide
.ixhelp fpguide
.ixbook &product Language Reference
.ixhelp f77lr
.ixbook &product Tools User's Guide
.ixhelp ftools
.* .ixbook &company Graphical Tools User's Guide
.* .ixhelp ide
.* .ixhelp wbrw
.* .ixhelp wprof
.* .* .ixhelp editor
.* .* .ixhelp resedt
.* .* .ixhelp resacc
.* .* .ixhelp resdlg
.* .* .ixhelp resimg
.* .* .ixhelp resmnu
.* .* .ixhelp resstr
.* .ixhelp rescomp
.* .* .ixhelp zoom
.* .* .ixhelp heapwalk
.* .* .ixhelp spy
.* .* .ixhelp ddespy
.* .* .ixhelp drwatcom
.* .* .ixhelp drnt
.ixbook &lnkname User's Guide
.ixhelp lguide
.ixbook &dbgname User's Guide
.ixhelp wd
.ixbook &product Graphics Library Reference
.ixhelp f77graph
.* .ixbook &product Diagnostic Messages Online Help
.* .ixhelp wfcerrs
.*
:eGDOC.
