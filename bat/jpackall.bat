@echo off
if [] == [%1] goto bad
if [] == [%2] goto bad
if [] == [%3] goto bad
rem clog packem.log
rem call %relroot%\pat\lang\setlevel %ISRC%
wmakel.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipack.mak -a -h -n -k >jpack.bat
wmakel.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipack2.mak -a -h -n -k >>jpack.bat
wmakel.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipackhdr.mak -a -h -n -k >>jpack.bat
wmakel.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipackf77.mak -a -h -n -k >>jpack.bat
wmakel.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipackwf.mak -a -h -n -k >>jpack.bat
wmakel.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipacknt.mak -a -h -n -k >>jpack.bat
wmakel.exe src_dir=%1 date=%2 time=%3 -f %tooldr%\tools\ipacksdk.mak -a -h -n -k >>jpack.bat
rem clog
goto done
:bad
echo must specify REL1 or REL2, DATE(mm-dd-yy), and TIME(hh:mm:ss)
:done
