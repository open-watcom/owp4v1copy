@echo off
call bmode
call docd wpack\o
wmake /h /i
cd ..\..
if [%1] == [] goto nocopy
ifarch copy wpack\o\wpack.exe %distroot%\supp
:nocopy
