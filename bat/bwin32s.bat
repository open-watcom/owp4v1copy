@echo off
call bmode
if [%1] == [] goto nocopy
set _windir=i:
if exist %_windir%\win32s\tmp.* del %_windir%\win32s\tmp.*
ifarch copy %_windir%\win32s\*.* %relroot%\%1\win32s
ifarch copy %_windir%\win32s\win32s\*.* %relroot%\%1\win32s\win32s
ifarch copy %_windir%\freecell\*.* %relroot%\%1\win32s\games
set _windir=
:nocopy
