:INCLUDE file='LYTCHG'.
.if &e'&dohelp eq 0 .do begin
:INCLUDE file='WNOHELP'.
.do end
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
:INCLUDE file='XDEFS'.
:set symbol="lang"      value="C/C++".
:set symbol="optdoc"    value="Power++ On-line Help".
:set symbol="lnkname"   value="Power++ Linker".
:INCLUDE file='DEFS'.
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
:INCLUDE file='WLERROR'.
.*
.cntents end_of_book
:eGDOC.
