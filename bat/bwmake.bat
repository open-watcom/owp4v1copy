@echo off
call bmode
call docd wmake\o
wmake /h /i
call docd ..\ol
wmake /h /i
call docd ..\os2
wmake /h /i
call docd ..\nt
wmake /h /i
call docd ..\ntaxp
wmake /h /i
rem call docd ..\oq
rem wmake /h /i
cd ..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\wmake\ol\wmk.exe %relroot%\%1\binw\wmakel.exe
ifarch copy %devdir%\wmake\os2\wmk.exe %relroot%\%1\binw\wmake.exe
ifarch copy %devdir%\wmake\nt\wmk.exe %relroot%\%1\binnt\wmake.exe
ifarch copy %devdir%\wmake\ntaxp\wmk.exe %relroot%\%1\axpnt\wmake.exe
rem ifarch copy %devdir%\wmake\oq\wmk.qnx %relroot%\%1\qnx\wmake.qnx
:nocopy
