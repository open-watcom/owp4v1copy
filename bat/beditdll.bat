@echo off
call bmode

call docd editdll\epm
wmake /h /i

call docd ..\cw
wmake /h /i

call docd ..\viw\objwin
wmake /h /i

call docd ..\objnt
wmake /h /i

cd ..\..\..

if [%1] == [] goto nocopy
ifarch copy editdll\epm\epmlink.dll  %relroot%\%1\binp\dll
ifarch copy editdll\viw\objwin\weditviw.dll %relroot%\%1\binw\weditviw.dll
ifarch copy editdll\viw\objnt\weditviw.dll %relroot%\%1\binnt\weditviw.dll
ifarch copy editdll\cw\weditcw.dll %relroot%\%1\binw\weditcw.dll
ifarch copy editdll\cw\weditcw.c %relroot%\%1\src\editdll\weditcw.c
ifarch copy editdll\cw\weditcw.h %relroot%\%1\src\editdll\weditcw.h
ifarch copy editdll\cw\weditcw.lnk %relroot%\%1\src\editdll\weditcw.lnk
ifarch copy editdll\cw\makefile %relroot%\%1\src\editdll\makefile
ifarch copy editdll\wedit.h %relroot%\%1\src\editdll\wedit.h
ifarch copy editdll\wedit.doc %relroot%\%1\src\editdll\wedit.doc
:nocopy
