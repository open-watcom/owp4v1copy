@echo off
call bmode
call docd win386\conv
wmake /h /i
call docd ..\ext
wmake /h /i
call docd ..\dll
wmake /h /i
call docd ..\wbind
wmake /h /i
call docd ..\vxd
wmake /h /i
cd ..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\win386\ext\win386.ext %relroot%\%1\binw\win386.ext
ifarch copy %devdir%\win386\dll\w386dll.ext %relroot%\%1\binw\w386dll.ext
ifarch copy %devdir%\win386\conv\win386.lib %relroot%\%1\lib386\win\win386.lib
ifarch copy %devdir%\win386\vxd\wdebug.386 %relroot%\%1\binw\wdebug.386
ifarch copy %devdir%\win386\vxd\wemu387.386 %relroot%\%1\binw\wemu387.386
ifarch copy %devdir%\win386\wbind\wbind.exe %relroot%\%1\binw\wbind.exe
ifarch copy %devdir%\win386\wbind\wbindnt.exe %relroot%\%1\binnt\wbind.exe

:nocopy
