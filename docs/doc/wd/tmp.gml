:INCLUDE file='LYTCHG'.
:INCLUDE file='FMTMACRO'.
:INCLUDE file='GMLMACS'.

.dm begmenu begin
.begnote $break
.dm begmenu end
.dm menuitem begin
.ix 'menu' '&*'
.note &*
.dm menuitem end
.dm endmenu begin
.endnote
.dm endmenu end
.*
.dm refalso begin
See :HDREF refid='&*'..
.dm refalso end

.dm mi begin
.ix 'menu' '&*'
.ct
:SF font=3.&*:eSF.
.dm mi end

.dm mm begin
.ix '&*'
.ct
:SF font=3.&*:eSF.
.dm mm end

.dm menuref begin
.mi &*1
from the
.mi &*2
.ix 'menu' '&*2'
menu
.dm menuref end

.dm popup begin
.mi &*
.ix 'menu' '&*'
from the pop-up menu
.dm popup end

.dm dot begin
.ct .li .
.dm dot end

.dm cmddef begin
.section &*
.ix 'command' '&*'
.dm cmddef end

.dm wnddef begin
&*
.ix 'window' '&*'
.ix '&*' 'window'
.dm wnddef end

.dm optdef begin
.note &*
.ix 'options' '&*'
.ix '&* option'
.dm optdef end

:INCLUDE file='XMACS'.
:cmt. :INCLUDE file='CMACS'.
:cmt. :INCLUDE file='PMACS'.

.if &e'&dohelp eq 1 .do begin
.dm sepsect begin
.dm sepsect end
.do end

:GDOC.
.*
:BODY.
.sepsect Remote Debugging
:INCLUDE file='wdbgrem.gml'.
.sepsect Debugging Techniques
.sepsect Interrupting A Running Program
:INCLUDE file='wdbgintp.gml'.
.sepsect Operating System specifics
:INCLUDE file='wdbgos.gml'.
.sepsect Expressions
:INCLUDE file='wdbgexpr.gml'.
:eGDOC.
