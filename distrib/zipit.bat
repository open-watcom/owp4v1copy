@echo off
echo Building '%1'...
cd %relroot%\%defrel%
comstrip %distroot%\manifest\%1 | zip %distroot%\archives\%1.zip -@ >> mkzips.log
rem zip %distroot%\archives\%1.zip license.txt >> mkzips.log
cd %distroot%
