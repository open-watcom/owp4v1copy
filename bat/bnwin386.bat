@echo off
call bmode
call docd nwin386\conv
wmake /h /i
call docd ..\ext9
wmake /h /i
call docd ..\dll9
wmake /h /i
cd ..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\nwin386\ext9\win386.ext %relroot%\%1\binw\win386b.ext
ifarch copy %devdir%\nwin386\dll9\w386dll.ext %relroot%\%1\binw\w386dllb.ext
ifarch copy %devdir%\nwin386\conv\wstubs\win386.lib %relroot%\%1\lib386\win\win386b.lib

:nocopy
