:TITLEP.
:TITLE &company &lang
:TITLE Graphics Library Reference
.if '&machsys' eq 'QNX' .do begin
:TITLE.for QNX
.do end
:INCLUDE file='DOCTITLE'.
:eTITLEP.
:ABSTRACT.
:INCLUDE copyrite
:INCLUDE disclaim
.if '&machsys' ne 'QNX' .do begin
:INCLUDE newslett
.do end
