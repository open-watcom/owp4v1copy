@echo off
rem build C++ AXP libraries
call bmode
cd plusplus\cpplib\bin
call docd .
if [%2] == [nomake] goto just_copy
call mkallaxp.bat -i say
:just_copy
cd ..\..
if [%1] == [] goto nocopy
ifarch copy cpplib\ntaxp\plibnt.lib %relroot%\%1\libaxp\nt\plib.lib
ifarch copy cpplib\ntaxp\plbxnt.lib %relroot%\%1\libaxp\nt\plbx.lib
ifarch copy cpplib\complex\ntaxp\cplxnt.lib %relroot%\%1\libaxp\nt\cplx.lib
:nocopy
cd ..
