@echo off
if [] == [%1] goto bad
if [] == [%2] goto bad
if [] == [%3] goto bad
rem clog packem.log
rem call %relroot%\pat\lang\setlevel %ISRC%
wmake.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipack.mak -h -n -k >jpack.bat
wmake.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipack2.mak -h -n -k >>jpack.bat
wmake.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipackhdr.mak -h -n -k >>jpack.bat
wmake.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipackf77.mak -h -n -k >>jpack.bat
wmake.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipackwf.mak -h -n -k >>jpack.bat
wmake.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipacknt.mak -h -n -k >>jpack.bat
rem clog
goto done
:bad
echo must specify REL1 or REL2, DATE(mm-dd-yy), and TIME(hh:mm:ss)
:done
