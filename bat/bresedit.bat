@echo off
call bmode

call docd sdk\rc\wres
call mk lw /i
call mk nt /i

call docd ..\..\wr\win16
if exist *.h rm *.h
wmake /h /i
call docd ..\nt
if exist *.h rm *.h
wmake /h /i

call docd ..\..\wresedit\waccel\win16
wmake /h /i
call docd ..\nt
wmake /h /i

call docd ..\..\wmenu\win16
wmake /h /i
call docd ..\nt
wmake /h /i

call docd ..\..\wstring\win16
wmake /h /i
call docd ..\nt
wmake /h /i

call docd ..\..\..\wre\win16
wmake /h /i
call docd ..\nt
wmake /h /i

call docd ..\..\..\fmedit\obj
wmake /h /i
call docd ..\nt
wmake /h /i

call docd ..\..\sdk\wde\win16
wmake /h /i
call docd ..\nt
wmake /h /i

call docd ..\..\imgedit\obj
wmake /h /i
call docd ..\objnt
wmake /h /i

cd ..\..\..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\sdk\wr\win16\wr.dll        %relroot%\%1\binw\wr.dll
ifarch copy %devdir%\sdk\wr\nt\wr.dll           %relroot%\%1\binnt\wr.dll
ifarch copy %devdir%\sdk\wresedit\waccel\win16\wacc.dll %relroot%\%1\binw\wacc.dll
ifarch copy %devdir%\sdk\wresedit\waccel\nt\wacc.dll %relroot%\%1\binnt\wacc.dll
ifarch copy %devdir%\sdk\wresedit\wmenu\win16\wmenu.dll %relroot%\%1\binw\wmenu.dll
ifarch copy %devdir%\sdk\wresedit\wmenu\nt\wmenu.dll %relroot%\%1\binnt\wmenu.dll
ifarch copy %devdir%\sdk\wresedit\wstring\win16\wstring.dll %relroot%\%1\binw\wstring.dll
ifarch copy %devdir%\sdk\wresedit\wstring\nt\wstring.dll %relroot%\%1\binnt\wstring.dll
ifarch copy %devdir%\sdk\wre\win16\wre.exe      %relroot%\%1\binw\wre.exe
ifarch copy %devdir%\sdk\wre\nt\wre.exe         %relroot%\%1\binnt\wre.exe
ifarch copy %devdir%\fmedit\obj\fmedit.dll      %relroot%\%1\binw\fmedit.dll
ifarch copy %devdir%\fmedit\nt\fmedit.dll       %relroot%\%1\binnt\fmedit.dll
ifarch copy %devdir%\sdk\wde\win16\wde.exe      %relroot%\%1\binw\wde.exe
ifarch copy %devdir%\sdk\wde\nt\wde.exe         %relroot%\%1\binnt\wde.exe
ifarch copy %devdir%\sdk\imgedit\obj\wimgedit.exe       %relroot%\%1\binw\wimgedit.exe
ifarch copy %devdir%\sdk\imgedit\objnt\wimgedit.exe     %relroot%\%1\binnt\wimgedit.exe
:nocopy
