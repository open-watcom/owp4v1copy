@echo off
echo Building '%1'...
cd %relroot%\%defrel%
comstrip %distroot%\manifest\%1 | zip %distroot%\archives\%1.zip -@ >> mkzips.log
zip %distroot%\archives\%1.zip -j %relroot%\license.txt >> mkzips.log
cd %distroot%
