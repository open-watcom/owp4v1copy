@echo off
echo Building '%1'...
cd %relroot%\rel2
comstrip %distroot%\manifest\%1 | zip %distroot%\archives\%1.zip -@ >> %distroot%\mkzips.log 2>&1
zip %distroot%\archives\%1.zip -j %relroot%\license.txt >> %distroot%\mkzips.log 2>&1
cd %distroot%
