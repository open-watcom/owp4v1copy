@echo off
call bmode
call docd cc\wcl
wmake /h /i
cd ..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\cc\wcl\wcl.exe %relroot%\%1\binw\wcl.exe
ifarch copy %devdir%\cc\wcl\wcl386.exe %relroot%\%1\binw\wcl386.exe
ifarch copy %devdir%\cc\wcl\wclnt.exe %relroot%\%1\binnt\wcl.exe
ifarch copy %devdir%\cc\wcl\wcl386nt.exe %relroot%\%1\binnt\wcl386.exe
rem ifarch copy %devdir%\cc\wcl\wclp16.exe %relroot%\%1\binw\wclp16.exe
:nocopy
