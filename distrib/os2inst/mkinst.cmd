@echo off
copy %relroot%\docs\creadme.txt readme.txt
copy %relroot%\license.nowrap license.txt
rm owatcom.ace
ace a -s- owatcom readme.txt
ace a -s- owatcom license.txt
ace a -s- owatcom setvars.cmd
packinst owc.cfg
