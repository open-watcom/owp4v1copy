.* Online Help Files for library marcros
.*
.* .ty Do NOT produce a new HELP file.  The Library Reference is out-of-date,
.* .ty contains numerous spelling mistakes, and other formatting glitches
.* .ty for help text (e.g., references to other sections which don't exist).
.* .ty These problems have been corrected manually (hours and hours of work)
.* .ty in the WEDITC.HLP file.
.* .ty
.dm func begin
. . .sk 1
. . .sr func = &*
. . .su ::::&'upper(&func)
. . .fo off
.dm func end
.*
.dm func2 begin
.dm func2 end
.*
.dm ix delete
.dm ix begin
.dm ix end
.*
.dm funcend begin
. . .sk 1
. . .fo on
.dm funcend end
.*
.dm desc begin
. . .if '&*' = 'end' .th .do begin
. . . .me .me
. . .do end
. . .el .if '&*' = 'begin' .th .do begin
. . .do end
. . .el .ty 'ERROR: missing description begin - end'
.dm desc end
.*
.dm return begin
:frontm
. .me .me
.dm return end
.*
.dm see begin
. .me .me
.dm see end
.*
.dm seelist begin
. .me .me
.dm seelist end
.*
.dm exmp begin
. .sk 1
. .fo off
. .me .me
.dm exmp end
.*
.dm kw begin
. .su &*
.dm kw end
.*
.dm arg begin
.  .sr strlen = &'length( '&*' )
.  .sr lastchar = &'substr( '&*', &strlen )
.  .if 1 = &'verify('&lastchar','&DELIMS') .do begin
.  .  .su "&*"
.  .do end
.  .el .do begin
.  .  .sr strlen = &strlen-1
.  .  .su "&'substr( '&*', 1, &strlen )"
.  .  .ct .li
&lastchar
.  .do end
.dm arg end
.*
.dm us begin
. .su &*
.dm us end
.*
.dm pp begin
:P.
.dm pp end
.*
.dm np begin
:P.
.dm np end
.*
.dm ixfunc begin
.dm ixfunc end
.*
.dm ixfunc2 begin
.dm ixfunc2 end
.*
.dm filename begin
. .su &*
.dm filename end
.*
.dm hdrfile begin
. .su <&*>
.dm hdrfile end
.*
.dm mono begin
. .if '&*' ne 'begin' and '&*' ne 'end' .th .do begin
. . .su &*
. .do end
.dm mono end
.*
.dm millust begin
. . .if &* = 'begin' .th .do begin
. . . .fo off
. . . .sk 1
. . . .in +3
. . .do end
. . .el .if &* = 'end' .th .do begin
. . . .fo on
. . . .sk 1
. . . .in -3
. . .do end
. . .el .do begin
. . . .fo off
. . . .sk 1
. . . .in +3
. . . .su &*
. . . .in -3
. . . .sk 1
. . . .fo on
. . .do end
.dm millust end
.*
.dm begterm begin
.if '&*1' ne '' .do begin
:DL tsize=&*1.
.do end
.el .do begin
:DL tsize=15.
.do end
.dm begterm end
.*
.dm term begin
:DT.&*
:DD.
.dm term end
.*
.dm endterm begin
:eDL.
.dm endterm end
.*
.dm beglist begin
. .fo off
. .sk 1
. .tb set $
. .tb 3+&*
.dm beglist end
.*
.dm endlist begin
. .fo on
. .tb set
. .tb
.dm endlist end
.*
.dm blkcode begin
.  .if '&*' eq 'begin' .do begin
.  .  .in +5
.  .  .fo off
.  .do end
.  .el .if '&*' eq 'end' .do begin
.  .  .in -5
.  .  .fo on
.  .do end
.  .el .do begin
.  .  .if '&*' = '' .th .termtype BLKCODE macro error
.  .do end
.dm blkcode end
.*
.dm blktext begin
.  .if '&*' eq 'begin' .do begin
.  .  .fo on
.  .do end
.  .el .if '&*' eq 'end' .do begin
.  .  .fo on
.  .do end
.  .el .do begin
.  .  .if '&*' = '' .th .termtype BLKTEXT macro error
.  .do end
.dm blktext end
.*
.dm doscmd begin
DOS command &'upper(&*)
.dm doscmd end
.*
.dm file begin
. .su &*
.dm file end
.*
.dm newtext begin
.sk
.fo on
&*
.sk
.in +8
.dm newtext end
.*
.dm oldtext begin
.  .in
.  .newkeep 0 1
.  .fo on
.dm oldtext end
.*
.ti ª ›
.sr sepchar = &syscw
.dc cw off
.sr DELIMS = '().,:;'
.dc cw &sepchar
.*
.ll 78
.sr bullet = '*'
.sr justify = 0
.sr parain = 0
.dc tb $
.dc ti @
.pl 9999
.* .im helpfuns
.* .im grmacs
.* .im graffuns
.* .im newhelp
.im newhelp2
