@echo off
call bmode
call docd as
pmake -d build -h

if [%1] == [] goto nocopy
ifarch copy %devdir%\as\alpha\os2386\wasaxp.exe %relroot%\%1\binp\wasaxp.exe
ifarch copy %devdir%\as\alpha\nt386\wasaxp.exe %relroot%\%1\binnt\wasaxp.exe
ifarch copy %devdir%\as\alpha\ntaxp\wasaxp.exe %relroot%\%1\axpnt\wasaxp.exe
:nocopy
