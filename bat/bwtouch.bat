@echo off
call bmode
rem
call docd wtouch
pmake -d build -h
cd ..

if [%1] == [] goto nocopy
ifarch copy %devdir%\wtouch\os2\wtouch.exe %relroot%\%1\binw\wtouch.exe
ifarch copy %devdir%\wtouch\nt.386\wtouch.exe %relroot%\%1\binnt\wtouch.exe
ifarch copy %devdir%\wtouch\nt.axp\wtouch.exe %relroot%\%1\axpnt\wtouch.exe
:nocopy
