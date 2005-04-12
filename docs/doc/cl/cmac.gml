.*
.* This is the same as ".illust", except concatenation (.co) is not off.
.* The default font is italic.
.* This is used for illustrations where multiple fonts are used.
.*
.dm cillust begin
.  .if '&*' eq 'begin' .do begin
.  .  .keep begin
.  .  :P.
.  .  ..in +&INDillust. *
.  .  :SF font=1.
.  .do end
.  .el .if '&*' eq 'end' .do begin
.  .  :eSF.
.  .  ..in -&INDillust. *
.  .  .keep end
.  .  :P.
.  .do end
.  .el .if '&*' eq 'break' .do begin
.  .  :eSF.
.  .  ..in -&INDillust. *
.  .  .keep break
.  .  ..in +&INDillust. *
.  .  :SF font=1.
.  .do end
.  .el .do begin
.  .  :P.
.  .  .keep begin
.  .  ..in +&INDillust. *
.  .  :SF font=1.
.  .  &*
.  .  :eSF.
.  .  ..in -&INDillust. *
.  .  :P.
.  .  .keep end
.  .do end
.dm cillust end
.*
.* Inside a .cillust, skip to a new line and put out a bold "or",
.* then skip to another new line.
.*
.dm cor begin
.  :eSF.
.  .br
:HP2.&SYSRB.&SYSRB.or:eHP2.
.  .br
.  :SF font=1.
.dm cor end
.*
.* Inside a .cillust, break to a new line.
.* Required to get around a GML bug where font changes back to 0.
.*
.dm cbr begin
.  :eSF.
.  .br
.  :SF font=1.
.dm cbr end
.*
.* Inside a .cillust, skip a line.
.* Required to get around a GML bug where font changes back to 0.
.*
.dm csk begin
.  :eSF.
.  ..sk 1 c
.  :SF font=1.
.dm csk end
.*
.* This is the same as .illust, except underlining (.us) is turned on.
.*
.dm uillust begin
.if '&*' eq 'begin' .do begin
.  .  .keep begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  :SF font=1.
.  .  ..co off
.do end
.el .if '&*' eq 'end' .do begin
.  .  ..co on
.  .  :eSF.
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.  .  .keep end
.do end
.el .if '&*' eq 'break' .do begin
.  .  ..co on
.  .  :eSF.
.  .  ..in -&INDillust. *
.  .  .keep break
.  .  ..in +&INDillust. *
.  .  :SF font=1.
.  .  ..co off
.do end
.el .do begin
.  .  ..sk 1 c
.  .  ..in +&INDillust. *
.  .  :SF font=1.
.  .  ..co off
.  .  ..li &*
.  .  ..co on
.  .  :eSF.
.  .  ..in -&INDillust. *
.  .  ..sk 1 c
.do end
.dm uillust end
.*
.* Inside a .uillust, skip to a new line and put out a bold "or",
.* then skip to another new line.
.*
.dm uor begin
.  :eSF.
.  .bd  or
.  :SF font=1.
.dm uor end
.*
.* Indent text for a while.
.*
.dm discuss begin
.  ..sr *indent=&INDillust.+&INDillust.
.  ..if '&*' = 'begin' ..th ..do begin
.  .  ..in +&*indent. *
.  .  ..br
.  ..do end
.  ..el ..if '&*' = 'end' ..th ..do begin
.  .  ..in -&*indent. *
.  .  ..br
.  ..do end
.  ..el ..do begin
.  .  ..in -&*indent. *
.  .  .br
&*
.  .  ..in +&*indent. *
.  .  .br
.  ..do end
.dm discuss end
.*
.* Show a keyword in mono, and add an index reference for it.
.*
.dm kw begin
.  .ix 'keyword' '&*'
.  .mono &*
.dm kw end
.*
.dm kwix begin
.  .ix 'keyword' '&*'
.dm kwix end
.*
.dm kwfont begin
.  .mono &*
.dm kwfont end
.*
.dm kwon begin
.  .monoon
.dm kwon end
.*
.dm kwoff begin
.  .monooff
.dm kwoff end
.*
.* Show a preprocessor keyword in mono, and add an index reference for it.
.*
.dm kwpp begin
.  .ix 'preprocessor directive' '&*'
.  .mono &*
.dm kwpp end
.*
.dm ppix begin
.  .ix 'preprocessor directive' '&*'
.dm ppix end
.*
.dm ppfont begin
.  .mono &*
.dm ppfont end
.*
.* Show a constant name in mono.
.*
.dm kwconst begin
.  .mono &*
.dm kwconst end
.*
.* Show a macro in mono, and add an index reference for it.
.*
.dm mkw begin
.  .ix '&* predefined macro'
.  .ix 'macro' 'predefined' '&*'
.  .mono &*
.dm mkw end
.*
.dm mkwfont begin
.  .mono &*
.dm mkwfont end
.*
.dm mkwix begin
.  .ix 'macro' 'predefined' '&*'
.dm mkwix end
.*
.* Show a library headername, and add an index reference for it.
.*
.dm hdr begin
.  .ix 'header' '&*'
.  .mono &*
.dm hdr end
.*
.* Show a library function, and add an index reference for it.
.*
.dm libfn begin
.  .ix 'library function' '&*'
.  .mono &*
.dm libfn end
.*
.* For shaded areas of the book.
.*
.dm shade begin
.   .if '&*' eq 'begin' .do begin
.   .   .sr shadeing=1
.   .   :I1.implementation-specific behavior
.   .   ..br
.   .   .keep begin
.   .   :LQ.
.   .   .if &e'&dohelp eq 0 .do begin
.   .   .   ..sk 2
.   .   .   :SF font=15.~n:eSF.
.   .   .   ..sk -1
.   .   .do end
.   .   .el .do begin
.   .   .   .pp
.   .   .do end
.   .do end
.   .el .if '&*' eq 'end' .do begin
.   .   .sr shadeing=0
.   .   ..br
.   .   .if &e'&dohelp eq 0 .do begin
.   .   .   ..sk -1
.   .   .   :SF font=15.~n:eSF.
.   .   .do end
.   .   .el .do begin
.   .   .   .bd off
.   .   .do end
.   .   :eLQ.
.   .   .keep end
.   .do end
.dm shade end
.*
.* Redefine paragraph control macros for on-line help
.*
.if &e'&dohelp eq 1 .do begin
.*
.dm pp begin
:P.
.if '&shadeing' eq '1' .bd on
.dm pp end
.*
.dm np begin
:P.
.if '&shadeing' eq '1' .bd on
.dm np end
.*
.dm pc begin
:PC.
.if '&shadeing' eq '1' .bd on
.dm pc end
.*
.do end
.*
.dm addindent begin
.  ..in +&*. *
.dm addindent end
.*
.dm noindent begin
.  ..in -&*. *
.dm noindent end
.*
.dm bigterms begin
.  .addindent 3
.dm bigterms end
.*
.dm bigterm begin
.  .noindent 3
.  :P.
.  :HP2.
.dm bigterm end
.*
.dm bigdesc begin
.  :eHP2.
.  .addindent 3
.  :P.
.dm bigdesc end
.*
.dm endbigterms begin
.  .noindent 3
.dm endbigterms end
.*
:SET symbol='SYSRB'     value='~b'.
:SET symbol='DAGGER'    value='~D'.
.*
.dm minus begin
.  .mono -
.dm minus end
.*
:SET symbol='MINUS'     value=';.ct .minus;.ct '
.*
.dm trademark begin
.  :SF font=7.~T:eSF.
.dm trademark end
.*
:SET symbol='TRADEMARK' value=';.ct;.trademark;.ct '.
.*
.dm langle begin
.  :SF font=7.~<:eSF.
.dm langle end
.*
:SET symbol='LANGLE'   value=';.ct;.langle;.ct '.
.*
.dm rangle begin
.  :SF font=7.~>:eSF.
.dm rangle end
.*
:SET symbol='RANGLE'   value=';.ct;.rangle;.ct '.
.*
.dm @monobeg begin
.  :SF font=4.&*.
.dm @monobeg end
.*
.dm @monoend begin
.  :eSF.&*.
.dm @monoend end
.*
..gt MONO  add @monobeg continue
..gt eMONO add @monoend continue
.*
.dm @minus begin
.  :SF font=4.-:eSF.&*
.dm @minus end
.*
..gt MINUS add @minus continue
.*
.dm @msemi begin
.  :SF font=4.;:eSF.&*.
.dm @msemi end
.*
..gt MSEMI add @msemi continue
.*
.dm @italbeg begin
.  :SF font=1.&*.
.dm @italbeg end
.*
.dm @italend begin
.  :eSF.&*.
.dm @italend end
.*
..gt ITAL  add @italbeg continue
..gt eITAL add @italend continue
.*
:CMT. Fix these!
.*
.dm super begin
.  :SF font=5.(&*.):eSF.
.dm super end
.*
:SET symbol='SUPER0'   value=';.ct .super 0;.ct '.
:SET symbol='SUPER1'   value=';.ct .super 1;.ct '.
:SET symbol='SUPER2'   value=';.ct .super 2;.ct '.
:SET symbol='SUPER3'   value=';.ct .super 3;.ct '.
:SET symbol='SUPER4'   value=';.ct .super 4;.ct '.
:SET symbol='SUPER5'   value=';.ct .super 5;.ct '.
:SET symbol='SUPER6'   value=';.ct .super 6;.ct '.
:SET symbol='SUPER7'   value=';.ct .super 7;.ct '.
:SET symbol='SUPER8'   value=';.ct .super 8;.ct '.
:SET symbol='SUPER9'   value=';.ct .super 9;.ct '.
:CMT.
:CMT. These are for the "examples" chapter, to ensure that
:CMT. the mono-spaced superscripts are only 1 character wide.
:CMT.
:SET symbol='MSUPER0'  value='0'.
:SET symbol='MSUPER1'  value='1'.
:SET symbol='MSUPER2'  value='2'.
:SET symbol='MSUPER3'  value='3'.
:SET symbol='MSUPER4'  value='4'.
:SET symbol='MSUPER5'  value='5'.
:SET symbol='MSUPER6'  value='6'.
:SET symbol='MSUPER7'  value='7'.
:SET symbol='MSUPER8'  value='8'.
:SET symbol='MSUPER9'  value='9'.
.*
.dm helppref begin
.if &e'&dohelp eq 1 .do begin
.   .if '&*' ne '' .do begin
.   :helppfx pfx='&* '.
.   .se pfx$='&* '
.   .do end
.   .el .do begin
.   :helppfx.
.   .se pfx$=''
.   .do end
.   .pu 1 .helppref &*
.do end
.dm helppref end
.*
