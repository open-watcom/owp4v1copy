@echo off
call bmode
call docd f77\cg86\cp\osi386
wmake /h /i
call docd ..\osii86
wmake /h /i
call docd ..\..\..\wfl\obj
wmake /h /i
call docd ..\386
wmake /h /i
call docd ..\nti86
wmake /h /i
call docd ..\nt
wmake /h /i
call docd ..\os2i86
wmake /h /i
call docd ..\os2386
wmake /h /i

cd ..\..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\f77\cg86\cp\osii86\wfc.exe %relroot%\%1\binw\wfc.exe
ifarch copy %devdir%\f77\cg86\cp\osi386\wfc386.exe %relroot%\%1\binw\wfc386.exe

ifarch copy %devdir%\f77\wfl\obj\wfl.exe %relroot%\%1\binw\wfl.exe
ifarch copy %devdir%\f77\wfl\386\wfl386.exe %relroot%\%1\binw\wfl386.exe
ifarch copy %devdir%\f77\wfl\nti86\wfl.exe %relroot%\%1\binnt\wfl.exe
ifarch copy %devdir%\f77\wfl\nt\wfl386.exe %relroot%\%1\binnt\wfl386.exe
ifarch copy %devdir%\f77\wfl\os2i86\wfl.exe %relroot%\%1\binp\wfl.exe
ifarch copy %devdir%\f77\wfl\os2386\wfl386.exe %relroot%\%1\binp\wfl386.exe
:nocopy
