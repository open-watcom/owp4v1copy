.dm bxt begin
.se *tmplvl=&WDWlvl-3
.if '&*1' eq 'on' .do begin
.   .if '&*2' ne '' .sr *t0=1+&*2
.   .if '&*3' ne '' .sr *t1=1+&*3
.   .if '&*4' ne '' .sr *t2=1+&*4
.   .if '&*5' ne '' .sr *t3=1+&*5
.   .if '&*6' ne '' .sr *t4=1+&*6
.   .if '&*7' ne '' .sr *t5=1+&*7
.   .if '&*8' ne '' .sr *t6=1+&*8
.   .if '&*9' ne '' .sr *t7=1+&*9
.   .if '&*10' ne '' .sr *t8=1+&*10
.   .cp &*tmplvl
.   .se $$bextrb=&*2
.   .se $$bextre=&*3
.   .xtxmp begin &*2 &*3
.   .in 0
.   .tb set \
.   .tb &*t0 &*t2 &*t3 &*t4 &*t5 &*t6 &*t7 &*t8 &*t1
:cmt..   .bx on &*2 &*4 &*5 &*6 &*7 &*8 &*9 &*10 &*3
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeon
.   .do end
.do end
.el .if '&*' eq 'off' .do begin
.   .tb set
.   .tb
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeoff
.   .do end
:cmt..   .bx off
.   .in &INDlvl
.   .xtxmp end &$$bextrb &$$bextre
.do end
.el .do begin
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeoff
.   .do end
:cmt..   .bx
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeon
.   .do end
.do end
.dm bxt end
.*
.dm box begin
.se *tmplvl=&WDWlvl-3
.if '&*1' eq 'on' .do begin
.   .if '&*2' ne '' .sr *t0=1+&*2
.   .if '&*3' ne '' .sr *t1=1+&*3
.   .if '&*4' ne '' .sr *t2=1+&*4
.   .if '&*5' ne '' .sr *t3=1+&*5
.   .if '&*6' ne '' .sr *t4=1+&*6
.   .if '&*7' ne '' .sr *t5=1+&*7
.   .if '&*8' ne '' .sr *t6=1+&*8
.   .if '&*9' ne '' .sr *t7=1+&*9
.   .if '&*10' ne '' .sr *t8=1+&*10
.   .xtnada
.   .in 0
.   .tb set \
.   .tb &*t0 &*t1 &*t2 &*t3 &*t4 &*t5 &*t6 &*t7 &*t8
:cmt..   .bx on &*2 &*3 &*4 &*5 &*6 &*7 &*8 &*9 &*10
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeon
.   .do end
.do end
.el .if '&*' eq 'off' .do begin
.   .tb set
.   .tb
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeoff
.   .do end
:cmt..   .bx off
.   .in &INDlvl
.do end
.el .do begin
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeoff
.   .do end
:cmt..   .bx
.   .if &e'&dohelp eq 1 .do begin
.   .   .codeon
.   .do end
.do end
.dm box end
.*
.dm xtnada begin
:SF font=14.~n:eSF.
.dm xtnada end
.*
.dm sf1 begin
:SF font=1.&*.
.dm sf1 end
.*
.dm sf2 begin
:SF font=2.&*.
.dm sf2 end
.*
.dm sf4 begin
:SF font=4.&*.
.dm sf4 end
.*
.dm sf7 begin
:SF font=7.&*.
.dm sf7 end
.*
.dm esf begin
:eSF.&*.
.dm esf end
.*
.dm xtxmp begin
.se shadestart=&$pagelm.+(&*2.*100)+150
.se shadeleft=&$pagelm.+(&*2.*100)+150
.se shaderight=&$pagelm.+(&*3.*100)+50
.if '&*1' eq 'begin' .do begin
.  .se shadetype=(start)
.  .se shadetop='-270'
.  .se shadebot='270'
.  .se shadels=(yes)
.do end
.el .do begin
.  .se shadetype=(end)
.  .se shadetop='-20'
.  .se shadebot='20'
.  .se shadels=(yes)
.do end
.se shadefont=(@fs0)
:cmt..oc &shadebot. &shadetop. &shadeleft. &shaderight. &shadestart. &shadefont. &shadetype. &shadels. set_shadevars
:SF font=13.~n:eSF.
.dm xtxmp end
.*
.dm illust begin
.if '&*' eq 'begin' .do begin
:XMP.:SF font=0.~b
.do end
.el .if '&*' eq 'end' .do begin
:eSF.:eXMP.
.do end
.el .if '&*' eq 'break' .do begin
:eSF.:eXMP.:XMP.:SF font=0.
.do end
.el .do begin
:XMP.:SF font=0.~b
&*
:eSF.:eXMP.:P.
.do end
.dm illust end
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
..dm boxdef begin
.  .keep begin
.  ..sr rdist  = &SYSIN.+1
.  ..sr boxcmd = &rdist.
.  ..sr tabcmd = ''
..dm boxdef end
.*
..dm boxcol begin
.  ..sr *tab   = ''
.  ..sr *width = &*1
.  ..if &*0 = 1 ..th ..do begin
.  .  ..if &rdist. ne 0 ..th ..do begin
.  .  .  ..sr *tab = &rdist.+1
.  .  ..do end
.  .  ..sr rdist = &*width.-1
.  ..do end
.  ..el ..if &*0 = 2 ..th ..do begin
.  .  ..if '&*2' = 'c' ..th ..do begin
.  .  .  ..sr *centre = (&*width.+1)/2
.  .  .  ..sr *tab    = &rdist.+&*centre.
.  .  .  ..sr *tab    = '&*tab.c'
.  .  .  ..sr rdist   = &*width.-&*centre.
.  .  ..do end
.  .  ..el ..do begin
.  .  .  ..ty Boxcol 2nd parameter error in "&*"
.  .  ..do end
.  ..do end
.  ..el ..do begin
.  .  ..ty Boxcol parameter error in "&*"
.  ..do end
.  ..sr boxcmd = '&boxcmd. +&*width.'
.  ..if '&*tab.' ne '' ..th ..do begin
.  .  ..if '&tabcmd.' eq '' ..th ..do begin
.  .  .  ..sr tabcmd = '&*tab.'
.  .  ..do end
.  .  ..el ..do begin
.  .  .  ..sr tabcmd = '&tabcmd. +&*tab.'
.  .  ..do end
.  ..do end
..dm boxcol end
.*
..dm boxbeg begin
:CMT. .  ..br
:CMT. .  ..li Final settings
:CMT. .  ..br
:CMT. .  ..li ..tb &tabcmd
:CMT. .  ..br
:CMT. .  ..li ..bx &boxcmd
:CMT. .  ..br
.  ..tb     &tabcmd
.  ..tb set &tabchar
:cmt..  ..bx on  &boxcmd
..dm boxbeg end
.*
..dm boxline begin
:cmt..  ..bx
..dm boxline end
.*
..dm boxend begin
:cmt..  ..bx off
.  ..tb
.  ..tb set
.  ..sr boxcmd off
.  ..sr tabcmd off
.  ..sr rdist  off
.  .keep end
.dm boxend end
.*
.dm smonoon begin
:SF font=5.
.dm smonoon end
.*
.dm smonooff begin
:eSF.
.dm smonooff end
.*
.dm numboxbeg begin
.  ..br
.  .if &e'&nobox eq 0 .do begin
.  .boxdef
.  .boxcol &numboxcol1.
.  .boxcol &numboxcol2.
.  .boxbeg
$Macro: :SF font=4.&*:eSF.$Value
.  .do end
.  .el .do begin
.  .millust begin
Macro: &*       Value
.  .do end
.  .boxline
.dm numboxbeg end
.*
.dm numterm begin
.  .if &e'&nobox eq 0 .do begin
$&*1.$:SF font=4.&*2:eSF.
.  .do end
.  .el .do begin
&*1             &*2
.  .do end
.dm numterm end
.*
.dm ansi begin
.  .numterm 'ISO' '&*'
.dm ansi end
.*
.dm noansi begin
.  .if &e'&nobox eq 0 .do begin
$ISO$no value specified
.  .do end
.  .el .do begin
ISO             no value specified
.  .do end
.dm noansi end
.*
.dm watcomc begin
.  ..if '&target' eq 'PC'  or  '&target' eq 'PC 370' ..th ..do begin
.  .  .numterm '&wc286.' &*1
.  ..do end
.dm watcomc end
.*
.dm c386 begin
.  ..if '&target' eq 'PC'  or  '&target' eq 'PC 370' ..th ..do begin
.  .  .numterm '&wc386.' &*1
.  ..do end
.dm c386 end
.*
.dm bothc begin
.  ..if '&target' eq 'PC'  or  '&target' eq 'PC 370' ..th ..do begin
.  .  .numterm '&wcboth.' &*1
.  ..do end
.dm bothc end
.*
.dm wlooc begin
.  ..if '&target' eq 'PC 370'  or  '&target' eq '370' ..th ..do begin
.  .  .numterm '&wlooc.' &*1
.  ..do end
.dm wlooc end
.*
.dm numboxend begin
.  .if &e'&nobox eq 0 .do begin
.  .boxend
.  .do end
.  .el .do begin
.  .millust end
.  .do end
.dm numboxend end
.*
.dm mbox begin
.se *tmplvl=&WDWlvl-3
.if '&*1' eq 'on' .do begin
.   .cp &*tmplvl
.   .xtnada
.   .bx on &*2 &*3 &*4 &*5 &*6 &*7 &*8 &*9 &*10
.   :XMP.:SF font=4.
.do end
.el .if '&*' eq 'off' .do begin
.   :eSF.:eXMP.
.   .bx off
.do end
.el .if '&*' eq 'begin' .do begin
.   .cp &*tmplvl
.   .se lmargin=&sysin+1
.   .xtnada
.   .bx on &lmargin &rmargin
.   .sk 1
.   :XMP.:SF font=6.
.do end
.el .if '&*' eq 'end' .do begin
.   :eSF.:eXMP.
.   .sk 1
.   .bx off
.do end
.el .do begin
.   .bx
.do end
.dm mbox end
.*
.dm mext begin
.se *tmplvl=&WDWlvl-3
.if '&*1' eq 'on' .do begin
.   .cp &*tmplvl
.   .se $$mextrb=&*2
.   .se $$mextre=&*3
.   .xtxmp begin &*2 &*3
.   .bx on &*2 &*4 &*5 &*6 &*7 &*8 &*9 &*10 &*3
.   :XMP.:SF font=4.
.do end
.el .if '&*' eq 'off' .do begin
.   :eSF.:eXMP.
.   .bx off
.   .xtxmp end &$$mextrb &$$mextre
.do end
.el .if '&*' eq 'begin' .do begin
.   .cp &*tmplvl
.   .se lmargin=&sysin+1
.   .se $$mextrb=&lmargin
.   .se $$mextre=&rmargin
.   .xtxmp begin &$$mextrb &$$mextre
.   .bx on &lmargin &rmargin
.   .sk 1
.   :XMP.:SF font=6.
.do end
.el .if '&*' eq 'end' .do begin
.   :eSF.:eXMP.
.   .sk 1
.   .bx off
.   .xtxmp end &$$mextrb &$$mextre
.do end
.el .do begin
.   .bx
.do end
.dm mext end
.*
.dm hint begin
.   .if '&*' eq 'begin' .do begin
.   .   :P.
.   .   .cp &WDWlvl
.   .   .se lmargin=&sysin+1
.   .   .bx on &lmargin &rmargin
.   .   .se lmargin=&sysin+2
.   .   .in +2
.   .   :SF font=3.Hint::eSF.
.  .do end
.  .el .if '&*' eq 'end' .do begin
.   .   .in -2
.   .   .bx off
.  .do end
.  .el .ty *** hint macro error ***
.dm hint end
.*
.dm remark begin
.  .if '&*' eq 'begin' .do begin
.   .   :P.
.   .   .cp &WDWlvl
.   .   .se lmargin=&sysin+1
.   .   .bx on &lmargin &rmargin
.   .   .se lmargin=&sysin+2
.   .   .in +2
.   .   :SF font=3.Note::eSF.
.  .do end
.  .el .if '&*' eq 'end' .do begin
.   .   .in -2
.   .   .bx off
.  .do end
.  .el .ty *** remark macro error ***
.dm remark end
.*
.dm warn begin
.  .if '&*' eq 'begin' .do begin
.   .   :P.
.   .   .cp &WDWlvl
.   .   .se lmargin=&sysin+1
.   .   .bx on &lmargin &rmargin
.   .   .se lmargin=&sysin+2
.   .   .in +2
.   .   :SF font=3.WARNING!:eSF.
.  .do end
.  .el .if '&*' eq 'end' .do begin
.   .   .in -2
.   .   .bx off
.  .do end
.  .el .ty *** warn macro error ***
.dm warn end
.*
.dm mkbx begin
.if &e'&dohelp eq 0 .do begin
.   .bx &*
.do end
.el .do begin
.   .abox &*
.do end
.dm mkbx end
.*
.dm mbigbox begin
.if '&*1' eq 'end' .do begin
.   .if '&format' eq '8.5x11a' .do begin
.   :eFIG.
.   .do end
.   .el .if '&format' eq 'help' .do begin
.   :eFIG.
.   .do end
.   .el .do begin
.   :eSF.:eXMP.
.   .bx off
.   .do end
.do end
.el .do begin
.   :P.
.   .se *tmplvl=&WDWlvl-3
.   .cp &*tmplvl.
.   .if '&format' eq '8.5x11a' .do begin
.   :FIG frame=box place=inline.
.   .sk 1
.   .do end
.   .el .if '&format' eq 'help' .do begin
.   :FIG frame=box place=inline.
.   .sk 1
.   .do end
.   .el .do begin
.   .se *lmargin=&sysin.+1
.   .bx on &*lmargin. &rmargin.
.   :XMP.:SF font=3.
.   .do end
.do end
.dm mbigbox end
.*
.dm embigbox begin
.if '&format' eq '8.5x11a' .do begin
:eFIG.
.do end
.el .if '&format' eq 'help' .do begin
:eFIG.
.do end
.el .do begin
:eSF.:eXMP.
.bx off
.do end
.dm embigbox end
.*
.dm ehint begin
.in -2
.bx off
.dm ehint end
.*
.dm eremark begin
.in -2
.bx off
.dm eremark end
.*
.dm ewarn begin
.in -2
.bx off
.dm ewarn end
.*

