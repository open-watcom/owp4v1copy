@echo off

rem Path configuration
rem ==================
set PATH=%PATH%;c:\Program Files\7-Zip
set OWBUILDPATH=\OW\pass1
set WWWPATH=\www

rem Initialization
rem ==============
cd %WWWPATH%

if exist %OWBUILDPATH% goto prerequisite_ok
echo Missing %OWBUILDPATH%. Can't continue with rotation.
goto done

:prerequisite_ok

rem Move pass1 build
rem =================
if exist snapshot ren snapshot snapshot.bak
if exist snapshot goto done
move %OWBUILDPATH% snapshot

rem Build Archives
rem ==============
if exist ss.zip del ss.zip
if exist ss.7z del ss.7z
cd snapshot
7z a -tzip -r ..\ss.zip *
7z a -t7z -r ..\ss.7z *
cd ..

rem Move Archives
rem =============
if exist ow-snapshot.zip del ow-snapshot.zip
if exist ow-snapshot.7z del ow-snapshot.7z
ren ss.zip ow-snapshot.zip
ren ss.7z ow-snapshot.7z

rem Final Cleanup
rem =============
\OW\bld\build\binnt\rm -r snapshot.bak

:done

