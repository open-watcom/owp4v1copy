@echo off
call bmode
call docd wlib\osi
wmake /h /i wlib.exe
call docd ..\qnx
wmake /h /i dwlib.qnx
cd ..\nwlib\osi.
if [%1] == [] goto nocopy
ifarch copy osi\wlib.exe %relroot%\%1\binw\wlib.exe

ifarch wstrip qnx\dwlib.qnx %relroot%\%1\qnx\binq\wlib. %relroot%\%1\qnx\sym\wlib.sym
:nocopy
cd ..
