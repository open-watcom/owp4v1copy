@echo off
call bmode
call docd wl

call docd ..\orl\o
wmake /h
rem call docd ..\axp
rem wmake /h
call docd ..\..\wl

rem call docd 386prod
rem wmake /h
rem cd ..

rem call docd os2dll
rem wmake /h
rem cd ..

call docd ntdll
wmake /h
cd ..

rem call docd qnx
rem wmake /h
rem cd ..

rem call docd axp
rem wmake /h
rem cd ..

rem call docd ms2wlink\o
rem wmake /h /i
rem cd ..\..

rem call docd fcenable\o
rem wmake /h /i
rem cd ..\..

rem call docd ovlldr
rem wmake /h /i wovl.lib
rem wmake /h /i wmovl.lib
rem cd ..\..

rem call docd wl95\watfor
rem wmake /h /i
rem cd ..\..

if [%1] == [] goto nocopy
rem ifarch wstrip %devdir%\wl\qnx\wl.exe %relroot%\%1\qnx\binq\wlink. %relroot%\%1\qnx\sym\wlink.sym
rem ifarch copy %devdir%\wl\ms2wlink\o\ms2wlink.exe %relroot%\%1\binw\ms2wlink.exe
rem ifarch copy %devdir%\wl\fcenable\o\fcenable.exe %relroot%\%1\binw\fcenable.exe
rem ifarch copy %devdir%\wl\ovlldr\wovl.lib %relroot%\%1\lib286\dos\wovl.lib
rem ifarch copy %devdir%\wl\ovlldr\wmovl.lib %relroot%\%1\lib286\dos\wmovl.lib
ifarch copy %devdir%\wl\wlbin.lnk %relroot%\%1\binw\wlink.lnk
ifarch copy %devdir%\wl\wlbinp.lnk %relroot%\%1\binp\wlink.lnk
ifarch copy %devdir%\wl\wlbinnt.lnk %relroot%\%1\binnt\wlink.lnk
ifarch copy %devdir%\wl\wlsystem.lnk %relroot%\%1\binw\wlsystem.lnk
rem ifarch copy %devdir%\wl\386prod\wl.exe %relroot%\%1\binw\wlink.exe
rem ifarch copy %devdir%\wl\os2dll\wlstub.exe %relroot%\%1\binp\wlink.exe
rem ifarch copy %devdir%\wl\os2dll\wl.dll %relroot%\%1\binp\dll\wlink.dll
ifarch copy %devdir%\wl\ntdll\wlstub.exe %relroot%\%1\binnt\wlink.exe
ifarch copy %devdir%\wl\ntdll\wl.dll %relroot%\%1\binnt\wlink.dll
rem ifarch copy %devdir%\wl\axp\wl.exe %relroot%\%1\axpnt\wlink.exe
rem cat %devdir%\wl\wlsystem.lnk %devdir%\wl\wlqnx.lnk >%relroot%\%1\qnx\etc\wlink.lnk
:nocopy
