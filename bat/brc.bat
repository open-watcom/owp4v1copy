@echo off
call bmode
call docd sdk\rc\wres

pmake -d build -h

rem call mk s /i
rem call mk m /i
rem call mk c /i
rem call mk l /i
rem call mk lp /i
rem call mk lw /i
rem call mk 3r /i
rem call mk 3s /i
rem call mk nt /i
rem call mk axp /i
rem wmake /h /m /f c:\bat\make110 /f makefile obj=axp processor=axp /i

call docd %devdir%\sdk\rc\rc\osi
wmake /h /i
call docd ..\nt.axp
wmake /h /i
call docd ..\ntdll
wmake /h /i
call docd ..\..\mkcdpg
wmake /h /i
call docd
rem call docd ..\o
rem wmake /h /i
rem call docd ..\386
rem wmake /h /i
rem call docd ..\nt
rem wmake /h /i
call docd
cd ..\..
call docd

if [%1] == [] goto nocopy
rem ifarch copy %devdir%\sdk\rc\rc\o\wrc.exe %relroot%\%1\binp\wrc.exe
rem ifarch copy %devdir%\sdk\rc\rc\nt\wrc.exe %relroot%\%1\binnt\wrc.exe
rem ifarch copy %devdir%\sdk\rc\rc\386\wrcp.exe %relroot%\%1\bin\wrc.exe
ifarch copy %devdir%\sdk\rc\rc\nt.axp\wrc.exe %relroot%\%1\axpnt\wrc.exe
ifarch copy %devdir%\sdk\rc\rc\osi\wrc.exe %relroot%\%1\binw\wrc.exe
ifarch copy %devdir%\sdk\rc\rc\ntdll\wrc.dll %relroot%\%1\binnt\wrc.dll
ifarch copy %devdir%\sdk\rc\rc\kanji.uni %relroot%\%1\binw\kanji.uni
ifarch copy %devdir%\sdk\rc\rc\1252.uni %relroot%\%1\binw\1252.uni
ifarch copy %devdir%\sdk\rc\mkcdpg\mkcdpg.exe %relroot%\%1\binnt\mkcdpg.exe
:nocopy
cd ..
