@echo off
call bmode

call docd idebatch\os2
wmake -i -h

call docd ..\nt
wmake -i -h

call docd ..\dos
wmake -i -h

cd ..

if [%1] == [] goto nocopy

ifarch copy os2\batserv.exe %relroot%\%1\binp\batserv.exe
ifarch copy nt\batserv.exe %relroot%\%1\binnt\batserv.exe
ifarch wstrip dos\int.exe %relroot%\%1\binw\dosserv.exe
ifarch copy dos\batchbox.pif %relroot%\%1\binw\batchbox.pif

:nocopy
cd ..
