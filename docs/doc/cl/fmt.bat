@echo off
echo Formatting for PostScript ...
set optfile=nb7x9ps
rem set optfile=nbps
set gmlinc=.;.\nbmine;.\nb
set gmllib=%gmlinc%;.\syslib
set gmltmp=c:\tmp
wgml %1 (file %optfile% %2 %3 %4 %5 %6 %7 %8 %9
set gmllib=
set gmlinc=
set gmltmp=
set optfile=
call setcol
:end
