@echo off
call bmode
call docd wv\trap\par\penserv
wmake /h /i w
call docd ..\pen2serv
wmake /h /i w
cd ..\..\..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\pen\wparserv.dll %relroot%\%1\pen\wparserv.dll
ifarch copy %devdir%\pen\wparsrv2.dll %relroot%\%1\pen\wparsrv2.dll
ifarch copy misc\inppdbg.bat %relroot%\%1\pen\install.bat
:nocopy
