@echo off
echo call %tooldr%\tools\setenv %relroot%\rel2 %ramd%:\ > __reset.bat
call unset
call __reset
erase __reset.bat
set testmode=1
if exist %ramdisk%*.exe erase %ramdisk%*.exe
