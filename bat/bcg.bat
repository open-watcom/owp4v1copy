@echo off
call bmode
cd \bld\cg
pmake -b -d build -h
rem pmake seems to be leaving us down in object directories - why?
cd \bld\cg
call tmp.bat
del tmp.bat
cd \bld
