@echo off
echo Building '%1'...
cd %relroot%\%defrel%
comstrip %distroot%\manifest\%1 | zip %distroot%\archives\%1.zip -@ >> %distroot%\mkzips.log 2>&1
cd %distroot%
