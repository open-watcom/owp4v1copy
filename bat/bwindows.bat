@echo off
call bmode
if [%1] == [] goto nocopy
call docd %devdir%\windows
set _windir=d:\bld
wmake /h /i
ifarch copy ctl3d.lib %relroot%\%1\lib286\win
ifarch copy ctl3d.dll %relroot%\%1\binw
ifarch copy windows.lib %relroot%\%1\lib286\win\windows.lib
ifarch copy %_windir%\windev31\lib\commdlg.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\windev31\lib\ddeml.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\windev31\lib\mmsystem.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\windev31\lib\olecli.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\windev31\lib\olesvr.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\windev31\lib\shell.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\windev31\lib\stress.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\windev31\lib\toolhelp.lib %relroot%\%1\lib286\win
if exist %_windir%\windev31\include\windows.h erase %_windir%\windev31\include\windows.h
ifarch copy %_windir%\windev31\include\*.h %relroot%\%1\h\win
ifarch copy %_windir%\windev31\include\*.inc %relroot%\%1\src\inc
ifarch copy %_windir%\windev31\redist\commdlg.* %relroot%\%1\binw
ifarch copy %_windir%\windev31\redist\*.dll %relroot%\%1\binw
ifarch copy %_windir%\windev31\redist\vtd.386 %relroot%\%1\binw
ifarch copy %_windir%\windev31\redist\winhelp.exe %relroot%\%1\binw
ifarch copy %_windir%\windev31\redist\winhelp.hlp %relroot%\%1\binw
ifarch copy %_windir%\windev31\redist\regload.exe %relroot%\%1\binw
ifarch copy %_windir%\windev31\redist\dib.drv %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\win31wh.hlp %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\win31mwh.hlp %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\penapiwh.hlp %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\mcistrwh.hlp %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\shed.exe %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\shed.hlp %relroot%\%1\binw
ifarch copy %_windir%\windev31\pen\msmouse.drv %relroot%\%1\binw
ifarch copy %_windir%\windev31\pen\penwin.ini %relroot%\%1\binw
ifarch copy %_windir%\windev31\pen\vgap.drv %relroot%\%1\binw
ifarch copy %_windir%\windev31\pen\yesmouse.drv %relroot%\%1\binw
ifarch copy %_windir%\windev31\pen\beditde.dll %relroot%\%1\binw
ifarch copy %_windir%\windev31\pen\heditde.dll %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\mrbc.exe %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\hc.bat %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\hc30.exe %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\hc31.exe %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\hc31.err %relroot%\%1\binw
ifarch copy %_windir%\windev31\bin\markmidi.exe %relroot%\%1\binw
ifarch copy %_windir%\msvc15\lib\compobj.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\msvc15\lib\mfcoleui.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\msvc15\lib\ole2.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\msvc15\lib\odbc.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\msvc15\lib\ole2disp.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\msvc15\lib\storage.lib %relroot%\%1\lib286\win
ifarch copy %_windir%\msvc15\redist\compobj.dll %relroot%\%1\binw
rem ifarch copy %_windir%\msvc15\redist\ctl3d.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\ctl3dv2.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\mfcoleui.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\ole2.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\ole2conv.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\ole2disp.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\ole2nls.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\ole2prox.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\storage.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\redist\typelib.dll %relroot%\%1\binw
ifarch copy %_windir%\msvc15\include\*.h %relroot%\%1\h\win
cd ..
set _windir=
:nocopy
