@echo off
call bmode
call docd wic\osi
wmake /h
cd ..\..
if [%1] == [] goto nocopy
ifarch copy wic\osi\wic.exe %relroot%\%1\binw\wic.exe
:nocopy
