@echo off
call bmode
rem
call docd cmdedit
wmake /h /i
if [%1] == [] goto nocopy
ifarch copy cmdedit.exe %relroot%\%1\binw\cmdedit.exe
ifarch copy cmdedit.txt %relroot%\%1\binw\cmdedit.txt
ifarch copy os2edit.exe %relroot%\%1\binp\os2edit.exe
ifarch copy os22edit.exe %relroot%\%1\binp\os22edit.exe
ifarch copy os2edit.dll %relroot%\%1\binp\dll\os2edit.dll
:nocopy
cd ..
