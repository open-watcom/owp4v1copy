@echo off
echo Building 'w32api'...
cd %relroot%\%defrel%
comstrip %distroot%\manifest\w32api | zip %distroot%\archives\w32api.zip -@ >> %distroot%\mkzips.log 2>&1
cd %distroot%
