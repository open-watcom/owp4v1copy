.* symbol "isbn" defined in grsyms
.*
.* Open Watcom C Graphics Library Reference
.* Open Watcom FORTRAN 77 Graphics Library Reference
.*
:CMT.   :set symbol='$draft' value='yes'.
:set symbol='which' value='F77'.
:set symbol='machsys' value='DOS'.
.*
:INCLUDE lytchg
:INCLUDE bantop
:INCLUDE fmtmacro
:INCLUDE symbols
:INCLUDE grsyms
:INCLUDE grmacs
:INCLUDE cmanmac
:INCLUDE libmacs
:INCLUDE libfuns
:INCLUDE liblist7
.*
:GDOC.
.*
:FRONTM.
.if &e'&dohelp eq 0 .do begin
:INCLUDE grtitle
:INCLUDE grpref
.pa odd
:TOC.
:FIGLIST.
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
.chap Graphics Library
.*
.im gfunmacs
:INCLUDE graphov
.im gfunrems
.*
.chap Graphics Library &rroutines
.*
:INCLUDE gr_fnbeg
:INCLUDE gr_index
:CMT.   .chap Graphics Examples
:CMT.   :INCLUDE gr_samp
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
