:INCLUDE file='LYTCHG'.
.if &e'&dohelp eq 0 .do begin
:INCLUDE file='WNOHELP'.
.do end
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.
:INCLUDE file='XDEFS'.
:GDOC.
.*
.if &e'&dohelp eq 0 .do begin
.*
:FRONTM.
:TITLEP.
:TITLE stitle="Watcom FORTRAN 77 Add-In Tools Guide".Watcom FORTRAN 77 Add-In Tools Guide
:set symbol="isbn" value="".
:graphic depth='5.0i' xoff='-1.5i' yoff='0.0i' scale=100 file='pwrs.eps'.
:eTITLEP.
:ABSTRACT.
:INCLUDE file='COPYRITE'.
:INCLUDE file='DISCLAIM'.
:INCLUDE file='NEWSLETT'.
:PREFACE.
:INCLUDE file='PREFACE'.
.pa odd
:TOC.
:cmt.   :FIGLIST.
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
.chap Introduction
.*
.np
Welcome to the Watcom FORTRAN 77 &ver..&rev Add-In Tools Guide.  This guide
contains information from a wide variety of third-party vendors
with tools that support and enhance the Watcom FORTRAN 77 &ver..&rev
development system.  Please feel free to contact the companies in
this listing to obtain more information on their products and
their Watcom support.  Note that not all products support all of
the platforms supported by Watcom FORTRAN 77.
.np
Note that this may not be a complete list.  Other vendors may
currently be developing support for Watcom FORTRAN 77 &ver..&rev..
An up-to-date list is always available on our BBS,
Powersoft's World Wide Web site, and CompuServe forum.
.np
Disclaimer: Publication in this directory is not an endorsement
by &pcompany of any product or company.  &pcompany makes no warranty,
implied or otherwise, for the quality, reliability or performance
of these products.  The publisher is not responsible for
typographical or other errors.  Please confirm all information
with the individual vendors before making purchasing decisions.
.np
Publish date:  &pubdate.
.*
:include file='listf'.
.*
.if &e'&dohelp eq 0 .do begin
:BACKM.
:INDEX.
.do end
:eGDOC.
