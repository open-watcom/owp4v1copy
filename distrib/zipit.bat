@echo off
echo Building '%1'...
comstrip %distroot%\manifest\%1 %relroot%\%defrel%\ | zip %distroot%\archives\%1.zip -@ >> mkzips.log
rem zip %distroot%\archives\%1.zip license.txt >> mkzips.log
