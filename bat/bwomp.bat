@echo off
call bmode
call docd womp\h
wmake /h /i
rem call docd ..\release
rem wmake /h /i
rem if [%1] == [] goto nocopy
rem ifarch copy womp.exe %relroot%\%1\binw\womp.exe
rem ifarch copy wompj.exe %relroot%\%1\binw\japan\womp.exe
rem :nocopy
cd ..\..
