@echo off
call bmode

rem
rem RC split out so that it can be built before the disassembler
rem
call brc.bat %1

call docd sdk
pmake -d build -h -i

call docd sdk\drwat\win
wmake /h /i
cd ..\..\..

call docd sdk\drwat\nt
wmake /h /i
cd ..\..\..

call docd sdk\drwat\chic
wmake /h /i
cd ..\..\..

rem call docd sdk\imgedit\obj
rem wmake /h /i
rem call docd ..\objnt
rem wmake /h /i
rem cd ..\..

if [%1] == [] goto nocopy
cd sdk
rem ifarch copy imgedit\obj\wimgedit.exe %relroot%\%1\binw\wimgedit.exe
rem ifarch copy imgedit\objnt\wimgedit.exe %relroot%\%1\binnt\wimgedit.exe
ifarch copy ddespy\wini86\wddespy.exe %relroot%\%1\binw\wddespy.exe
ifarch copy ddespy\nt386\wddespy.exe %relroot%\%1\binnt\wddespy.exe
ifarch copy ddespy\ntaxp\wddespy.exe %relroot%\%1\axpnt\wddespy.exe
ifarch copy heapwalk\wheapwlk.exe %relroot%\%1\binw\wheapwlk.exe
ifarch copy spy\wini86\wspy.exe %relroot%\%1\binw\wspy.exe
ifarch copy spy\wini86\wspyhk.dll %relroot%\%1\binw\wspyhk.dll
ifarch copy spy\nt386\wspy.exe %relroot%\%1\binnt\wspy.exe
ifarch copy spy\nt386\ntspyhk.dll %relroot%\%1\binnt\ntspyhk.dll
ifarch copy spy\ntaxp\wspy.exe %relroot%\%1\axpnt\wspy.exe
ifarch copy spy\ntaxp\ntspyhk.dll %relroot%\%1\axpnt\ntspyhk.dll
ifarch copy drwat\win\drwatcom.exe %relroot%\%1\binw\drwatcom.exe
ifarch copy drwat\nt\drnt.exe %relroot%\%1\binnt\drwatcom.exe
ifarch copy drwat\chic\_drwin95.exe %relroot%\%1\binnt\_drwin95.exe
ifarch copy zoom\wini86\wzoom.exe %relroot%\%1\binw\wzoom.exe
ifarch copy zoom\nt386\wzoom.exe %relroot%\%1\binnt\wzoom.exe
ifarch copy zoom\ntaxp\wzoom.exe %relroot%\%1\axpnt\wzoom.exe
cd ..
:nocopy
