.pp
.mkwix &mkwDATE_sp.
.mkwfont &mkwDATE.
.discuss begin
The date of translation of the source file (a string literal).
The form of the date is "Mmm&SYSRB.dd&SYSRB.yyyy" where:
..if &e'&dohelp eq 1 ..in 0
:ZDL termhi=0.
:ZDT.Mmm
:ZDD.
represents the month and is one of:
.millust begin
Jan  Feb  Mar  Apr  May  Jun
Jul  Aug  Sep  Oct  Nov  Dec
.millust end
:ZDT.dd
:ZDD.
is the day of the month. The first character is a blank if the day
is less than 10.
:ZDT.yyyy
:ZDD.
is the year.
:ZeDL.
.pp
If the compiler cannot determine the current date,
another date is provided.
.*
.************************************************************************
.*
..if '&target' eq 'PC' or '&target' eq 'PC 370' ..th ..do begin
.shade begin
With
..if '&target' eq 'PC' ..th ..do begin
&wcboth.,
..do end
..el ..if '&target' eq 'PC 370' ..th .do begin
&wcall.,
..do end
the current date is always available.
.shade end
..do end
.*
.************************************************************************
.*
.discuss end
.pp
.mkwix &mkwFILE_sp.
.mkwfont &mkwFILE.
.discuss begin
The name of the current source file (a string literal). The name
may be changed using the
.kwpp #line
directive.
.discuss end
.pp
.mkwix &mkwLINE_sp.
.mkwfont &mkwLINE.
.discuss begin
The line number of the current source line (a decimal constant).
The line number may be changed using the
.kwpp #line
directive.
.discuss end
.keep begin
.pp
.mkwix &mkwSTDC_sp.
.mkwfont &mkwSTDC.
.discuss begin
The decimal constant 1, meaning that the compiler is a
.ix 'standard conforming'
standard-conforming implementation.
.discuss end
.keep end
.pp
.mkwix &mkwTIME_sp.
.mkwfont &mkwTIME.
.discuss begin
The time of translation of the source file (a string literal). The
form of the time is "hh:mm:ss", with leading zeros provided for
values less than 10.
.pp
If the compiler cannot determine the current time,
another time is provided.
.*
.************************************************************************
.*
..if '&target' eq 'PC' or '&target' eq 'PC 370' ..th ..do begin
.shade begin
With
..if '&target' eq 'PC' ..th ..do begin
&wcboth.,
..do end
..el ..if '&target' eq 'PC 370' ..th .do begin
&wcall.,
..do end
the current time is always available.
.shade end
..do end
.*
.************************************************************************
.*
.discuss end
.pp
.mkwix &mkwFNC_sp.
.mkwfont &mkwFNC.
.discuss begin
The name of the current function (a string literal).
.discuss end
