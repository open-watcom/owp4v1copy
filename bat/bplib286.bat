@echo off
rem build C++ 286 libraries
call bmode
cd plusplus\cpplib\bin
call docd .
if [%2] == [nomake] goto just_copy
call mkall286.bat -i say
:just_copy
cd ..\..
if [%1] == [] goto nocopy
ifarch copy cpplib\scsd\plibs.lib %relroot%\%1\lib286
ifarch copy cpplib\scbd\plibc.lib %relroot%\%1\lib286
ifarch copy cpplib\bcsd\plibm.lib %relroot%\%1\lib286
ifarch copy cpplib\bcbd\plibl.lib %relroot%\%1\lib286
ifarch copy cpplib\bchd\plibh.lib %relroot%\%1\lib286
ifarch copy cpplib\wscsd\plibs.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wscbd\plibc.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcsd\plibm.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcbd\plibl.lib %relroot%\%1\lib286\win
ifarch copy cpplib\os2mt\plibmtl.lib %relroot%\%1\lib286\os2
ifarch copy cpplib\scsd\plibs.lib %relroot%\%1\lib286
ifarch copy cpplib\scbd\plibc.lib %relroot%\%1\lib286
ifarch copy cpplib\bcsd\plibm.lib %relroot%\%1\lib286
ifarch copy cpplib\bcbd\plibl.lib %relroot%\%1\lib286
ifarch copy cpplib\bchd\plibh.lib %relroot%\%1\lib286
ifarch copy cpplib\wscsd\plibs.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wscbd\plibc.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcsd\plibm.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcbd\plibl.lib %relroot%\%1\lib286\win
ifarch copy cpplib\os2mt\plibmtl.lib %relroot%\%1\lib286\os2
ifarch copy cpplib\scsd\plbxs.lib %relroot%\%1\lib286
ifarch copy cpplib\scbd\plbxc.lib %relroot%\%1\lib286
ifarch copy cpplib\bcsd\plbxm.lib %relroot%\%1\lib286
ifarch copy cpplib\bcbd\plbxl.lib %relroot%\%1\lib286
ifarch copy cpplib\bchd\plbxh.lib %relroot%\%1\lib286
ifarch copy cpplib\wscsd\plbxs.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wscbd\plbxc.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcsd\plbxm.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcbd\plbxl.lib %relroot%\%1\lib286\win
ifarch copy cpplib\os2mt\plbxmtl.lib %relroot%\%1\lib286\os2
ifarch copy cpplib\scsd\plbxs.lib %relroot%\%1\lib286
ifarch copy cpplib\scbd\plbxc.lib %relroot%\%1\lib286
ifarch copy cpplib\bcsd\plbxm.lib %relroot%\%1\lib286
ifarch copy cpplib\bcbd\plbxl.lib %relroot%\%1\lib286
ifarch copy cpplib\bchd\plbxh.lib %relroot%\%1\lib286
ifarch copy cpplib\wscsd\plbxs.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wscbd\plbxc.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcsd\plbxm.lib %relroot%\%1\lib286\win
ifarch copy cpplib\wbcbd\plbxl.lib %relroot%\%1\lib286\win
ifarch copy cpplib\os2mt\plbxmtl.lib %relroot%\%1\lib286\os2
ifarch copy cpplib\complex\scsd\cplxs.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\scbd\cplxc.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\bcsd\cplxm.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\bcbd\cplxl.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\bchd\cplxh.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\wscsd\cplxs.lib %relroot%\%1\lib286\win
ifarch copy cpplib\complex\wscbd\cplxc.lib %relroot%\%1\lib286\win
ifarch copy cpplib\complex\wbcsd\cplxm.lib %relroot%\%1\lib286\win
ifarch copy cpplib\complex\wbcbd\cplxl.lib %relroot%\%1\lib286\win
ifarch copy cpplib\complex\scsd7\cplx7s.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\scbd7\cplx7c.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\bcsd7\cplx7m.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\bcbd7\cplx7l.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\bchd7\cplx7h.lib %relroot%\%1\lib286
ifarch copy cpplib\complex\wscsd7\cplx7s.lib %relroot%\%1\lib286\win
ifarch copy cpplib\complex\wscbd7\cplx7c.lib %relroot%\%1\lib286\win
ifarch copy cpplib\complex\wbcsd7\cplx7m.lib %relroot%\%1\lib286\win
ifarch copy cpplib\complex\wbcbd7\cplx7l.lib %relroot%\%1\lib286\win
:nocopy
cd ..
