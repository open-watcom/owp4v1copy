@echo off
call bmode
call docd wstrip
wmake /h /i
cd ..

if [%1] == [] goto nocopy
ifarch copy %devdir%\bin\bin\strip.exe %relroot%\%1\binw\wstrip.exe
ifarch copy %devdir%\bin\binnt\strip.exe %relroot%\%1\binnt\wstrip.exe
ifarch copy %devdir%\bin\qnx\strip.qnx %relroot%\%1\qnx\binq\wstrip.
:nocopy
