:INCLUDE file='BOOKLAY'.
:INCLUDE file='BANNER'.
:INCLUDE file='FMTMACRO'.
:INCLUDE file='SYMBOLS'.
:INCLUDE file='CMANMAC'.
:GDOC.
.*
:FRONTM.
:TITLEP.
:TITLE stitle="Library Reference".WATCOM C Library Reference
.if '&machsys' eq 'QNX' .do begin
:TITLE.for QNX
.do end
.if '&machsys' eq 'DOS' .do begin
:TITLE.3rd Edition
.do end
:cmt. :DOCNUM.ISBN 0-921329-74-1
:cmt. :DATE.&date.
:INCLUDE file='AUTHORS'.
:ADDRESS.
:ALINE.WATCOM Publications Limited
:ALINE.Waterloo, Ontario, Canada
:eADDRESS.
:cmt. :graphic depth='0.7i' xoff='-2.55i' yoff='0.0i' scale=50 file='watcom.eps'.
:eTITLEP.
:ABSTRACT.
:INCLUDE file='COPYRITE'.
:INCLUDE file='DISCLAIM'.
:INCLUDE file='NEWSLETT'.
:PREFACE.
:INCLUDE file='LIBPREF'.
:INCLUDE file='CLIBINDX'.
.pa odd
:TOC.
:FIGLIST.
:BODY.
.if '&machsys' eq 'QNX' .do begin
.sepsect WATCOM C Library Reference Volume 1
.do end
.el .do begin
.sepsect WATCOM C Library Reference
.do end
.chap Overview
:INCLUDE file='CLIBOVER'.
.if '&machsys' ne 'QNX' .do begin
.chap DOS Considerations
:INCLUDE file='CLIBDOS'.
.do end
.chap Library Functions and Macros
:INCLUDE file='CLIBFUNS'.
.chap Re-entrant Functions
:INCLUDE file='CREFUNS'.
.if '&machsys' ne 'QNX' .do begin
.chap Missing Functions
:INCLUDE file='CSUMFUNS'.
.do end
.sepsect Appendices
:APPENDIX.
:INCLUDE file='PCBUGFRM'.
:BACKM.
:INDEX.
:eGDOC.
