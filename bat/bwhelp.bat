@echo off
call bmode
call docd help\dos
wmake -h
cd ..\..

if [%1] == [] goto nohelpcopy
ifarch copy %devdir%\help\dos\whelp.exe %relroot%\%1\binw\whelp.exe
:nohelpcopy
