rem @echo off
call bmode
call docd hlp
if not [%1] == [] ifarch copy %devdir%\hlp\*.hlp %relroot%\%1\binw>nul
if not [%1] == [] ifarch copy %devdir%\hlp\watcomc.inf %relroot%\%1\binp\help>nul
cd ..
