@echo off
call bmode
rem build support tools
call docd plusplus\hash\src
wmake /h /k
cd ..
rem build C++ helpfiles
rem call docd ..\helptext
rem wmake /h /k wpperrs.ipf wpp.hlp
rem build C++ compilers
rem call docd ..\dosi86
rem wmake /h /k
rem call docd ..\dos386
rem wmake /h /k
rem call docd ..\os2i86
rem wmake /h /k
rem call docd ..\os2386
rem wmake /h /k
call docd ..\nti86
wmake /h /k
if [%2] == [reference] wmake /h /k reference
call docd ..\nt386
wmake /h /k
if [%2] == [reference] wmake /h /k reference
rem call docd ..\osii86
rem wmake /h /k
rem call docd ..\osi386
rem wmake /h /k
rem call docd ..\qnxi86
rem wmake /h /k
rem call docd ..\qnx386
rem wmake /h /k
rem call docd ..\ntaxp
rem wmake /h /k
rem if [%2] == [reference] wmake /h /k reference
rem call docd ..\osiaxp
rem wmake /h /k
rem call docd ..\ntaxp.axp
rem wmake /h /k
cd ..
rem plusplus project is on a joined drive, and ifarch doesn't work over joins
if [%1] == [] goto nocopy
rem
rem Win32 .DLL compilers
rem
ifarch copy nti86\wppd.dll %relroot%\%1\binnt\wppd.dll
ifarch copy nt386\wppd386.dll %relroot%\%1\binnt\wppd386.dll
ifarch copy nti86\wppd.sym %relroot%\%1\binnt\wppd.sym
ifarch copy nt386\wppd386.sym %relroot%\%1\binnt\wppd386.sym
ifarch copy nti86\wcppi86.exe %relroot%\%1\binnt\wpp.exe
ifarch copy nt386\wcpp386.exe %relroot%\%1\binnt\wpp386.exe
rem
rem OSI compilers
rem
rem ifarch copy osii86\wcppi86.exe %relroot%\%1\binw\wpp.exe
rem ifarch copy osi386\wcpp386.exe %relroot%\%1\binw\wpp386.exe
rem if exist osii86\wcppi86.sym mv osii86\wcppi86.sym %relroot%\%1\binw\wpp.sym
rem if exist osi386\wcpp386.sym mv osi386\wcpp386.sym %relroot%\%1\binw\wpp386.sym
rem
rem QNX compilers
rem
rem ifarch copy qnxi86\wcppi86.qnx %relroot%\%1\qnx\binq\wpp.
rem ifarch copy qnx386\wcpp386.qnx %relroot%\%1\qnx\binq\wpp386.
rem if exist qnxi86\wcppi86.sym mv qnxi86\wcppi86.sym %relroot%\%1\qnx\sym\wpp.sym
rem if exist qnx386\wcpp386.sym mv qnx386\wcpp386.sym %relroot%\%1\qnx\sym\wpp386.sym
rem
rem AXP compilers
rem ifarch copy osiaxp\wcppaxp.exe %relroot%\%1\binw\wppaxp.exe
rem ifarch copy ntaxp.axp\wcppaxp.exe %relroot%\%1\axpnt\wppaxp.exe
rem if exist ntaxp.axp\wcppaxp.sym mv ntaxp.axp\wcppaxp.sym %relroot%\%1\axpnt\wppaxp.sym
rem
:nocopy
if [%2] == [updlang] call updlang
cd ..
