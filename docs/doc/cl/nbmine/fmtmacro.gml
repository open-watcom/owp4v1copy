.*
.* Open Watcom Documentation Macros & Variables
.*
:set symbol="headtxt0$" value=" ".
:set symbol="headtxt1$" value=" ".
:set symbol="headtext$" value=" ".
:set symbol="WDWlvl"    value="10".
:set symbol="SCTlvl"    value="0".
:set symbol="INDillust" value="2".
:set symbol="NTEpr2"    value=" ".
:set symbol="NTEpr1"    value=" ".
:set symbol="NTEset"    value=" ".
:set symbol="NTEnhi"    value="termhi=3".
:set symbol="NTEphi"    value="termhi=2".
:set symbol="ANTset"    value=" ".
:set symbol="APTset"    value="compact".
.if &e'&dohelp eq 0 .do begin
:set symbol="INDxmp"    value="0.3i".
.do end
.el .do begin
:set symbol="INDxmp"    value="0.5i".
.do end
.if '&format' eq '6x8' .do begin
:set symbol="INDlvl"    value="8".
.do end
.el .if '&format' eq '8.5x11a' .do begin
:set symbol="INDlvl"    value="5".
.do end
.el .do begin
:set symbol="INDlvl"    value="7".
.do end
.*
.dm ix begin
..if &*0. eq 1 ..th ..do begin
.  :I1.&*1.
..do end
..el ..if &*0. eq 2 ..th ..do begin
.  :IH1.&*1.
.  :I2.&*2.
..do end
..el ..do begin
.  :IH1.&*1.
.  :IH2.&*2.
.  :I3.&*3.
..do end
.dm ix end
.*
.dm fn begin
.if '&*' eq 'begin' .do begin
:FN.
.do end
.el .if '&*' eq 'end' .do begin
:eFN.
.do end
.dm fn end
.*
:CMT. .dm us begin
:CMT. :SF font=1.&*:eSF.
:CMT. .dm us end
:CMT. .*
:CMT. .dm bd begin
:CMT. :SF font=2.&*:eSF.
:CMT. .dm bd end
:CMT. .*
:CMT. .dm bi begin
:CMT. :SF font=3.&*:eSF.
:CMT. .dm bi end
.*
.dm pp begin
:P.
.dm pp end
.*
.dm np begin
:P.
.dm np end
.*
.dm pc begin
:PC.
.dm pc end
.*
.dm illust begin
.  .if '&*' eq 'begin' .do begin
.  .  .keep begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  ..co off
.  .do end
.  .el .if '&*' eq 'end' .do begin
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.  .  .keep end
.  .do end
.  .el .if '&*' eq 'break' .do begin
.  .  .* revert to normal before doing break because GML sometimes
.  .  .* turns formatting back on
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  .keep break
.  .  ..in +&INDillust. *
.  .  ..co off
.  .do end
.  .el .do begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  ..li &*
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.  .do end
.dm illust end
.*
.dm millust begin
.  .if '&*' eq 'begin' .do begin
.  .  .keep begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  .monoon
.  .do end
.  .el .if '&*1' eq 'end' .do begin
.  .  .monooff
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  ..if '&*2' eq '0' ..th ..do begin
.  .  .  ..sk 0
.  .  ..do end
.  .  ..el ..do begin
.  .  .  ..sk 1 c
.  .  ..do end
.  .  .keep end
.  .do end
.  .el .if '&*' eq 'break' .do begin
.  .  .monooff
.  .  ..in -&INDillust. *
.  .  .keep break
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  .monoon
.  .do end
.  .el .do begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  .mono &*
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.  .do end
.dm millust end
.*
.dm code begin
.  .if '&*' eq 'begin' .do begin
.  .  .keep begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  .codeon
.  .do end
.  .el .if '&*' eq 'end' .do begin
.  .  .codeoff
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.  .  .keep end
.  .do end
.  .el .if '&*' eq 'break' .do begin
.  .  .* Needed because .keep break causes .co on, .codeoff, etc.
.  .  .codeoff
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  .keep break
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  .codeon
.  .do end
.  .el .if '&*' eq 'skip' .do begin
.  .  .* Needed because .sk 1 causes .co on, .codeoff, etc.
.  .  .* This skip should be conditional!
.  .  .codeoff
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  .codeon
.  .do end
.  .el .do begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  ..co off
.  .  .code &*
.  .  ..co on
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.  .do end
.dm code end
.*
.dm codeon begin
:SF font=5.
.dm codeon end
.*
.dm codeoff begin
:eSF.
.dm codeoff end
.*
.dm mono begin
:SF font=4.&*:eSF.
.dm mono end
.*
.dm monoon begin
:SF font=4.
.dm monoon end
.*
.dm monooff begin
:eSF.
.dm monooff end
.*
.dm autonote begin
.if '&*' ne '' .do begin
.* .  .sr tmplvl=&WDWlvl-3
.* .  .cp &tmplvl
.  :P.:HP1.&*:eHP1.
.do end
:OL &ANTset .
:set symbol="NTEpr2" value="&NTEpr1".
:set symbol="NTEpr1" value="&NTEset".
:set symbol="NTEset" value="ol".
.dm autonote end
.*
.dm autopoint begin
.if '&*' ne '' .do begin
.* .  .sr tmplvl=&WDWlvl-3
.* .  .cp &tmplvl
.  :P.:HP1.&*:eHP1.
.do end
:OL &APTset .
:set symbol="NTEpr2" value="&NTEpr1".
:set symbol="NTEpr1" value="&NTEset".
:set symbol="NTEset" value="ol".
.dm autopoint end
.*
.dm begnote begin
.* .sr tmplvl=&WDWlvl-3
.* .cp &tmplvl
:set symbol="NTEop1" value="".
:set symbol="NTEop2" value="".
:set symbol="NTEop3" value="".
.if '&*1' eq '$break' or '&*2' eq '$break' or '&*3' eq '$break' .do begin
:set symbol="NTEop1" value="break".
.do end
.if '&*1' eq '$compact' or '&*2' eq '$compact' or '&*3' eq '$compact' .do begin
:set symbol="NTEop2" value="compact".
.do end
.if '&*1' eq '$setptnt' .do begin
:set symbol="NTEop3" value="tsize=&*2".
.do end
.if '&*2' eq '$setptnt' .do begin
:set symbol="NTEop3" value="tsize=&*3".
.do end
.if '&*3' eq '$setptnt' .do begin
:set symbol="NTEop3" value="tsize=&*4".
.do end
:DL &NTEnhi &NTEop1 &NTEop2 &NTEop3.
.if '&NTEop1' eq '' and '&NTEop2' eq '' and '&NTEop3' eq '' .do begin
.if '&*1' ne '' .do begin
:DTHD.&*
:DDHD.~b
.do end
.do end
:set symbol="NTEpr2" value="&NTEpr1".
:set symbol="NTEpr1" value="&NTEset".
:set symbol="NTEset" value="dl".
.dm begnote end
.*
.dm begpoint begin
.* .sr tmplvl=&WDWlvl-3
.* .cp &tmplvl
:set symbol="NTEop1" value="".
:set symbol="NTEop2" value="".
:set symbol="NTEop3" value="".
.if '&*1' eq '$break' or '&*2' eq '$break' or '&*3' eq '$break' .do begin
:set symbol="NTEop1" value="break".
.do end
.if '&*1' eq '$compact' or '&*2' eq '$compact' or '&*3' eq '$compact' .do begin
:set symbol="NTEop2" value="compact".
.do end
.if '&*1' eq '$setptnt' .do begin
:set symbol="NTEop3" value="tsize=&*2".
.do end
.if '&*2' eq '$setptnt' .do begin
:set symbol="NTEop3" value="tsize=&*3".
.do end
.if '&*3' eq '$setptnt' .do begin
:set symbol="NTEop3" value="tsize=&*4".
.do end
:DL &NTEphi &NTEop1 &NTEop2 &NTEop3.
.if '&NTEop1' eq '' and '&NTEop2' eq '' and '&NTEop3' eq '' .do begin
.if '&*1' ne '' .do begin
:DTHD.&*
:DDHD.~b
.do end
.do end
:set symbol="NTEpr2" value="&NTEpr1".
:set symbol="NTEpr1" value="&NTEset".
:set symbol="NTEset" value="dl".
.dm begpoint end
.*
.dm setptnt begin
.dm setptnt end
.*
.dm endnote begin
.if '&NTEset' eq 'ol' .endOL
.el .if '&NTEset' eq 'dl' .endDL
:set symbol="NTEset" value="&NTEpr1".
:set symbol="NTEpr1" value="&NTEpr2".
:set symbol="NTEpr2" value=" ".
.dm endnote end
.*
.dm endpoint begin
.endnote
.dm endpoint end
.*
.dm endOL begin
:eOL.
.dm endOL end
.*
.dm endDL begin
:eDL.
.dm endDL end
.*
.dm note begin
.if '&NTEset' eq 'ol' .noteOL
.el .if '&NTEset' eq 'dl' .noteDL &*
.dm note end
.*
.dm point begin
.if '&NTEset' eq 'ol' .noteOL
.el .if '&NTEset' eq 'dl' .noteDL &*
.dm point end
.*
.dm noteOL begin
:LI.
.dm noteOL end
.*
.dm noteDL begin
.if '&*1' eq '.mono' :DT.&*2 &*3 &*4 &*5 &*6
.el .do begin
:DT.&*
.do end
:DD.
.dm noteDL end
.*
.dm begbull begin
.* .sr tmplvl=&WDWlvl-3
.* .cp &tmplvl
.if '&*1' eq '$compact' .do begin
:UL compact
.do end
.el .do begin
:UL
.do end
.dm begbull end
.*
.dm bull begin
:LI
.dm bull end
.*
.dm endbull begin
:eUL.
.dm endbull end
.*
..sr keeplvl = 0
.*
.dm keep begin
.  .if '&*' eq 'begin' ..th ..do begin
.  .  ..if &keeplvl. eq 0 ..th ..do begin
.  .  .  ..br
.  .  .  ..fk begin
.  .  ..do end
.  .  ..sr keeplvl = &keeplvl. + 1
.  ..do end
.  ..el ..if '&*' eq 'break' ..th ..do begin
.  .  ..fk end
.  .  ..fk dump
.  .  ..br
.  .  ..fk begin
.  ..do end
.  ..el ..if '&*' eq 'end' ..th ..do begin
.  .  ..sr keeplvl = &keeplvl. - 1
.  .  ..if &keeplvl. eq 0 ..th ..do begin
.  .  .  ..fk end
.  .  .  ..fk dump
.  .  .  ..br
.  .  ..do end
.  ..do end
.  ..el ..do begin
.  .  .termtype Error in keep parameter '&*'
.  .do end
.dm keep end
.*
.dm sepsect begin
:H0.&*
:set symbol="headtext$" value=" ".
:set symbol="headtxt0$" value="&*".
.pa odd
.dm sepsect end
.*
.dm chap begin
:H1.&*
:set symbol="SCTlvl" value="0".
:set symbol="headtext$" value="&*".
:set symbol="headtxt1$" value="&*".
.in &INDlvl
.if '&format' eq '7x9' .do begin
:BINCLUDE file='rule7x9.eps' reposition=start depth='0'.
.do end
.el .do begin
:BINCLUDE file='rule.eps' reposition=start depth='0'.
.do end
.dm chap end
.*
.dm beglevel begin
.if '&SCTlvl' eq '3' .ty too many section levels
.el .if '&SCTlvl' eq '2' :set symbol="SCTlvl" value = "3".
.el .if '&SCTlvl' eq '1' :set symbol="SCTlvl" value = "2".
.el .if '&SCTlvl' eq '0' :set symbol="SCTlvl" value = "1".
.dm beglevel end
.*
.dm endlevel begin
.if '&SCTlvl' eq '1' :set symbol="SCTlvl" value = "0".
.el .if '&SCTlvl' eq '2' :set symbol="SCTlvl" value = "1".
.el .if '&SCTlvl' eq '3' :set symbol="SCTlvl" value = "2".
.dm endlevel end
.*
.dm section begin
.* .cp &WDWlvl
.in 0
.if '&SCTlvl' eq '0' .do begin
.  :H2.&*
.  .se headtext$=&*
.do end
.el .if '&SCTlvl' eq '1' .do begin
.  :H3.&*
.do end
.el .if '&SCTlvl' eq '2' .do begin
.  :H4.&*
.do end
.el .if '&SCTlvl' eq '3' .do begin
.  :H5.&*
.do end
.in &INDlvl
.dm section end
.*
.dm cntents begin
.if &e'&dohelp eq 0 .me
.if ›&*› eq ›end_of_book› .do begin
:set symbol="SCTlvl" value = "0".
.do end
.if ›&cnt_ttl› ne › › .do begin
.   .ctxstr &cnt_pfx.&cnt_ttl.
.   .sr cnt_ttl=&'translate(›&cnt_ttl.›,'#','=')
.   .if &cnt_lvl lt &SCTlvl .do begin
.   .   .sr *sct=&cnt_lvl+2
.   .   .pu 3 &*sct &cnt_ttl.
.   .   .sr cnt_ttl='Introduction'
.   .do end
.   .el .do begin
.   .   .if &cnt_lvl eq 0 .do begin
.   .   .   .sr *sct=&cnt_lvl+2
.   .   .   .pu 3 &*sct &cnt_ttl.
.   .   .   .sr cnt_ttl='Introduction'
.   .   .do end
.   .   .el .do begin
.   .   .   .sr *sct=&cnt_lvl+1
.   .   .do end
.   .do end
.   .if &'length(&ctx_str.) gt 65 .do begin
.   .   .pu 3 &*sct &cnt_ttl.=
.   .   .pu 3 &ctx_str.
.   .do end
.   .el .do begin
.   .   .pu 3 &*sct &cnt_ttl.=&ctx_str.
.   .do end
.do end
.sr cnt_lvl=&SCTlvl
.sr cnt_pfx='&pfx$.'
.sr cnt_ttl=&*
.dm cntents end
.*
.se pfx$=''
.*
.dm npsection begin
.pa
.section &*
.dm npsection end
.*
.dm sect begin
.* .cp &WDWlvl
.in 0
.if '&SCTlvl' eq '0' .do begin
.  :P.
.  :SF font=10.&*:eSF.
.  :P.
.do end
.el .if '&SCTlvl' eq '1' .do begin
.  :P.
.  :SF font=9.&*:eSF.
.  :P.
.do end
.el .if '&SCTlvl' eq '2' .do begin
.  :P.
.  :SF font=8.&*:eSF.
.  :P.
.do end
.el .if '&SCTlvl' eq '3' .do begin
.  :P.
.  :SF font=8.&*:eSF.
.  :P.
.do end
.in &INDlvl
.dm sect end
.*
.dm npsect begin
.pa
.sect &*
.dm npsect end
.*
.dm preface begin
.in 0
:SF font=11.&*:eSF.
.if '&format' eq '7x9' .do begin
:BINCLUDE file='rule7x9.eps' reposition=start depth='0'.
.do end
.el .do begin
:BINCLUDE file='rule.eps' reposition=start depth='0'.
.do end
:P.
.in &INDlvl
.dm preface end
.*
.dm csect begin
:H6.&*
.dm csect end
.*
.dm endfmt begin
.dm endfmt end
.*
.dm rev1 begin
.dm rev1 end
.dm rev2 begin
.dm rev2 end
.dm rev3 begin
.dm rev3 end
.*
