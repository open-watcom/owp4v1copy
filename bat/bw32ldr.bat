@echo off
call bmode
call docd %DEVDIR%\w32loadr
wmake -h
ifarch copy os2ldr.exe %tooldr%\tools\os2ldr.exe
if [%1] == [] goto nocopy
ifarch copy x32run.exe %relroot%\%1\binw\x32run.exe
ifarch copy x32run.exe %relroot%\%1\binw\w32run.exe
ifarch copy d4grun.exe %relroot%\%1\binw\d4grun.exe
ifarch copy tntrun.exe %relroot%\%1\binw\tntrun.exe
ifarch copy ntrunner.exe %relroot%\%1\binnt\w32run.exe
cd nt
ifarch copy *.exe %relroot%\%1\binnt
:nocopy
cd %DEVDIR%
