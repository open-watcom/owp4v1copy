.*
.*
:set symbol="isbn" value="".
.*
:INCLUDE file='LYTCHG'.
.if &e'&dohelp eq 0 .do begin
:INCLUDE file='WNOHELP'.
.do end
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
:INCLUDE file='XDEFS'.
.*
:GDOC.
.*
.if &e'&dohelp eq 0 .do begin
:FRONTM.
:TITLEP.
:TITLE stitle="User's Guide".&lnkname
:TITLE.User's Guide
:INCLUDE file='DOCTITLE'.
:eTITLEP.
:ABSTRACT.
:INCLUDE file='COPYRITE'.
:INCLUDE file='DISCLAIM'.
:INCLUDE file='NEWSLETT'.
:PREFACE.
:INCLUDE file='LPREF'.
.pa odd
:TOC.
.pa odd
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
.sepsect The WATCOM Linker
:INCLUDE file='WLUSAGE'.
:INCLUDE file='TUTMAIN'.
:INCLUDE file='LNKMAIN'.
:INCLUDE file='SPDOS'.
:INCLUDE file='SPELF'.
:INCLUDE file='SPNOV'.
:INCLUDE file='SPOS2'.
:INCLUDE file='SPPHAR'.
:INCLUDE file='SPQNX'.
:INCLUDE file='SPWIN16'.
:INCLUDE file='SPWIVXD'.
:INCLUDE file='SPWIN32'.
:INCLUDE file='WLERROR'.
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
