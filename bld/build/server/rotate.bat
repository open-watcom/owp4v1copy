@echo off

rem Initialization
rem ==============
path=%path%;c:\Program Files\7-Zip
cd \www

if exist \OW\rel2 goto prerequisite_ok
echo Missing \OW\rel2. Can't continue with rotation.
goto done

:prerequisite_ok

rem Move rel2
rem =========
if exist snapshot ren snapshot snapshot.bak
if exist snapshot goto done
move \OW\rel2 snapshot

rem Build Archives
rem ==============
if exist ss.zip del ss.zip
if exist ss.7z del ss.7z
cd snapshot
zip -r ..\ss.zip *
7z a -r ..\ss.7z *
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

