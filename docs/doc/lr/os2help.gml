:set symbol="machsys" value="DOS".
:set symbol="isbn"    value="1-55094-036-8"
:cmt. :INCLUDE file='BANNER'.
:INCLUDE file='FMTMACRO'.
:INCLUDE file='SYMBOLS'.
:INCLUDE file='CMANMAC'.
:INCLUDE file='LIBFUNS'.
.if '&machsys' ne 'PP' .do begin
:INCLUDE file='LIBLIST'.
.do end
:GDOC.
.*
:FRONTM.
:BODY.
.sepsect WATCOM C Library Reference
.if '&machsys' ne 'TEST' .do begin
:INCLUDE file='CLIBOVER'.
.do end
.if '&machsys' eq 'DOS' .do begin
:INCLUDE file='CLIBDOS'.
.do end
:INCLUDE file='CLIBFUNS'.
:set symbol="headtxt0$" value=''.
:set symbol="headtxt1$" value=''.
:cmt. .if '&machsys' ne 'PP' .do begin
:cmt. :INCLUDE file='CREFUNS'.
:cmt. .do end
:cmt. .if '&machsys' eq 'DOS' .do begin
:cmt. :INCLUDE file='CSUMFUNS'.
:cmt. .do end
:cmt. .if '&machsys' ne 'FOX' .do begin
:cmt. .sepsect Appendices
:cmt. :APPENDIX.
:cmt. :INCLUDE file='CLIBFUNC'.
:cmt. .do end
:BACKM.
:cmt. :INDEX.
:eGDOC.
