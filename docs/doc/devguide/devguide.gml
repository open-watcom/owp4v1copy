:set symbol="isbn" value="0-123-45645678-9".
.*
:INCLUDE file='LYTCHG'.
.if &e'&dohelp eq 0 .do begin
:INCLUDE file='WNOHELP'.
.do end
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
.*
:INCLUDE file='XDEFS'.
:set symbol="lang"      value="C/C++".
:INCLUDE file='DEFS'.
.*
:GDOC.
.*
.if &e'&dohelp eq 0 .do begin
.*
:FRONTM.
:TITLEP.
:TITLE stitle="Developer's Guide".&product
:TITLE.Developer's Guide
:INCLUDE file='DOCTITLE'.
:eTITLEP.
.*:ABSTRACT.
.*:INCLUDE file='COPYRITE'.
.*:INCLUDE file='DISCLAIM'.
:INCLUDE file='NEWSLETT'.
.*.pa odd
:TOC.
:FIGLIST.
.*.pa odd
.do end
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
.sepsect Documentation System
:INCLUDE file='DOCS'.
.*
.sepsect Build Architecture
:INCLUDE file='ARCH'.
.*
.if &e'&dohelp eq 0 .do begin
:BACKM.
.cd set 2
:INDEX.
.do end
.*
.cd set 1
.cntents end_of_book
:eGDOC.
