@echo off

REM Verify NOMINMAX works when <windows.h> is included.
REM See nominmax.cpp for further information on this issue.

REM To verify the test, disable the NOMINMAX guard in either Windef.h
REM or Win16.h; the corresponding test will show failure rather than 
REM success.

echo ============== \OW\BLD\PLUSTEST\REGRESS\OWSTL\NOMINMAX.CPP ================>>..\..\owstl.log

if not exist %WATCOM%\h\nt\windef.h goto Win16Test

REM Test windef.h, since it exists.

wpp386 nominmax.cpp -i%WATCOM%\h;%WATCOM%\h\nt -w4 -e25 -zq -od -d2 -6r -bt=nt -mf -xs -xr
wlink name nominmax d all LIBP %WATCOM%\lib386;%WATCOM%\lib386\nt SYS nt op m op maxe=25 op q FIL nominmax.obj

nominmax

if errorlevel 1 goto Win32failed

echo PASS \ow\bld\plustest\regress\owstl\nominmax.cpp: Windef.h>>..\..\owstl.log
goto Win32clean

:Win32failed
echo FAIL \ow\bld\plustest\regress\owstl\nominmax.cpp: Windef.h>>..\..\owstl.log

:Win32clean

REM Cleanup the directory.

del nominmax.exe
del nominmax.map
del nominmax.obj

:Win16Test
if not exist %WATCOM%\h\win\win16.h goto exit

REM Test win16.h, since it exists.

wpp386 nominmax.cpp -i%WATCOM%\h;%WATCOM%\h\win -w4 -e25 -zq -od -d2 -6r -bt=nt -mf -xs -xr
wlink name nominmax d all LIBP %WATCOM%\lib386;%WATCOM%\lib386\nt SYS nt op m op maxe=25 op q FIL nominmax.obj

nominmax

if errorlevel 1 goto Win386failed

echo PASS \ow\bld\plustest\regress\owstl\nominmax.cpp: Win16.h>>..\..\owstl.log
goto Win386clean

:Win386failed
echo FAIL \ow\bld\plustest\regress\owstl\nominmax.cpp: Win16.h>>..\..\owstl.log

:Win386clean

REM Cleanup the directory. 

del nominmax.exe
del nominmax.map
del nominmax.obj

:exit


