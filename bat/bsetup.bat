@echo off
call bmode
i:
call docd \setupgui\o.pls
wmake setup.exe
call docd ..\o.nt
wmake csetup.exe f77setup.exe
call docd ..\o.os2
wmake csetup.exe f77setup.exe
call docd ..\o.win
wmake csetup.exe f77setup.exe
call docd ..\mkdisk
wmake
call docd ..
if [%1] == [] goto nocopy
ifarch copy o.os2\csetup.exe %distroot%\wc\csetup\install.exe
ifarch copy o.nt\csetup.exe %distroot%\wc\csetup\setup32.exe
ifarch copy o.win\csetup.exe %distroot%\wc\csetup\setup.exe
ifarch copy o.pls\setup.exe %distroot%\wc\csetup\dossetup.exe
ifarch copy o.os2\setup.sym %distroot%\wc\csetup\install.sym
ifarch copy o.nt\setup.sym %distroot%\wc\csetup\setup32.sym
ifarch copy o.win\setup.sym %distroot%\wc\csetup\setup.sym
ifarch copy o.pls\setup.sym %distroot%\wc\csetup\dossetup.sym

ifarch copy o.os2\f77setup.exe %distroot%\wc\f77setup\install.exe
ifarch copy o.nt\f77setup.exe %distroot%\wc\f77setup\setup32.exe
ifarch copy o.win\f77setup.exe %distroot%\wc\f77setup\setup.exe
ifarch copy o.pls\setup.exe %distroot%\wc\f77setup\dossetup.exe
ifarch copy o.os2\setup.sym %distroot%\wc\f77setup\install.sym
ifarch copy o.nt\setup.sym %distroot%\wc\f77setup\setup32.sym
ifarch copy o.win\setup.sym %distroot%\wc\f77setup\setup.sym
ifarch copy o.pls\setup.sym %distroot%\wc\f77setup\dossetup.sym
ifarch copy mkdisk\*.exe %distroot%\supp
:nocopy
%blddr%
call docd .
