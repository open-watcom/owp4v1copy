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
.  .el .do begin
.  .  :P.
.  .  .cp &WDWlvl
.  .  .se *lmargin=&sysin.+1
.  .  .bx on &*lmargin. &rmargin.
.  .  .in +2
.  .  :SF font=3.Hint::eSF.
.  .do end
.dm hint end
.*
.dm ehint begin
.in -2
.bx off
.dm ehint end
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
.  .el  .do begin
.  .  :P.
.  .  .sr tmplvl=&WDWlvl-3
.  .  .cp &tmplvl
.  .  .se *lmargin=&sysin.+1
.  .  .bx on &*lmargin. &rmargin.
.  .  .in +2
.  .  :SF font=3.Note::eSF.
.  .do end
.dm remark end
.*
.dm eremark begin
.in -2
.bx off
.dm eremark end
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
.  .el .do begin
.  .  :P.
.  .  .cp &WDWlvl
.  .  .se *lmargin=&sysin.+1
.  .  .bx on &*lmargin. &rmargin.
.  .  .in +2
.  .  :SF font=3.WARNING!:eSF.
.  .do end
.dm warn end
.*
.dm ewarn begin
.in -2
.bx off
.dm ewarn end
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
.dm @api begin
:HP2.&*.
.dm @api end
.gt api add @api cont
.*
.dm @eapi begin
:eHP2.&*
.dm @eapi end
.gt eapi add @eapi cont
.*
.*
.dm @dsc begin
:HP0.&*.
.dm @dsc end
.gt dsc add @dsc cont
.*
.dm @edsc begin
:eHP0.&*
.dm @edsc end
.gt edsc add @edsc cont
.*
.dm cbox begin
.mbox &*
.dm cbox end
.*
.dm id begin
:SF font=4.&*:eSF.
.dm id end
.*
.dm fie begin
:SF font=4.&*.
.dm fie end
.*
.dm fi begin
.fie &*.
:eSF.
.dm fi end
.*

