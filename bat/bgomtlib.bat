@echo off
call bmode
cd gomtlib
call docd .
wmake /h /i
cd ..
if [%1] == [] goto nolibcopy
ifarch copy gomtlib\*.lib %relroot%\%1\gomtlib\*.lib
:nolibcopy
