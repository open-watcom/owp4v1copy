@echo off
call bmode

cd %devdir%\browser
rem call makeall os2 winprod nt brgdos brgnt brgos2 brgosi
call makeall os2 winprod nt brgosi

if [%1] == [] goto nocopy
ifarch copy %devdir%\bin\bin\wbrw.exe %relroot%\%1\binw\wbrw.exe
ifarch copy %devdir%\bin\binnt\wbrw.exe %relroot%\%1\binnt\wbrw.exe
ifarch copy %devdir%\bin\binp\wbrw.exe %relroot%\%1\binp\wbrw.exe
if exist %devdir%\bin\bin\wbrw.sym mv %devdir%\bin\bin\wbrw.sym %relroot%\%1\binw\wbrw.sym
if exist %devdir%\bin\binnt\wbrw.sym mv %devdir%\bin\binnt\wbrw.sym %relroot%\%1\binnt\wbrw.sym
if exist %devdir%\bin\binp\wbrw.sym mv %devdir%\bin\binp\wbrw.sym %relroot%\%1\binp\wbrw.sym
ifarch copy %devdir%\browser\brg\objosi\wbrg.exe %relroot%\%1\binw\wbrg.exe
:nocopy
cd ..
