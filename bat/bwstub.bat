@echo off
call bmode
call docd wstub
wmake /h /i wstub.exe
wmake /h /i wstubq.exe
cd ..

if [%1] == [] goto nocopy
ifarch copy wstub\wstub.exe %relroot%\%1\binw\wstub.exe
ifarch copy wstub\wstubq.exe %relroot%\%1\binw\wstubq.exe
ifarch copy wstub\wstub.c %relroot%\%1\src\wstub.c
:nocopy
