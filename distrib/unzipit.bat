@echo off
echo Unzipping '%1'...
md wise\%1
cd wise\%1
unzip ..\..\archives\%1.zip >> ..\..\mkwise.log
cd ..\..

