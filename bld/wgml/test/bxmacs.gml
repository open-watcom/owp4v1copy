.dm abox begin
.if '&*1' eq 'begin' or '&*1' eq 'on' .do begin
.   .box
.do end
.el .if '&*1' eq 'end' or '&*1' eq 'off' .do begin
.   .ebox
.do end
.el .do begin
.   .ebox
.   .box
.do end
.dm abox end
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
.if &'lower(&syspdev) NE whelp .do begin
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
.do end
.el .do begin
.dm mbox begin
.se *tmplvl=&WDWlvl-3
.if '&*1' eq 'on' .do begin
.   .cp &*tmplvl.
.   .mkbx on &*2 &*3 &*4 &*5 &*6 &*7 &*8 &*9 &*10
.   :XMP.
.do end
.el .if '&*' eq 'off' .do begin
.   :eXMP.
.   .mkbx off
.do end
.el .if '&*' eq 'begin' .do begin
.   :P.
.   .cp &*tmplvl.
.   .se *lmargin=&sysin.+1
.   .mkbx on &*lmargin. &rmargin.
.   :XMP.
.do end
.el .if '&*' eq 'end' .do begin
.   :eXMP.
.   .mkbx off
.do end
.el .do begin
.   .mkbx
.do end
.dm mbox end
.do end
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
.dm sy begin
:SF font=1.&*:eSF.
.dm sy end
.*

