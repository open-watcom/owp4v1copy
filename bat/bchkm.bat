@echo off
call bmode
call docd chkmouse
wmake /h /i
if [%1] == [] goto nocopy
ifarch copy %devdir%\bin\chkmouse.com %relroot%\%1\binw\chkmouse.com
:nocopy
cd ..
