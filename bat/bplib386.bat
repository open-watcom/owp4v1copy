@echo off
rem build C++ 386 libraries
call bmode
cd plusplus\cpplib\bin
call docd .
if [%2] == [nomake] goto just_copy
call mkall386.bat -i say
:just_copy
cd ..\..
if [%1] == [] goto nocopy
ifarch copy cpplib\3r\plib3r.lib %relroot%\%1\lib386
ifarch copy cpplib\3s\plib3s.lib %relroot%\%1\lib386
ifarch copy cpplib\3rmt\plibmt3r.lib %relroot%\%1\lib386
ifarch copy cpplib\3smt\plibmt3s.lib %relroot%\%1\lib386
ifarch copy cpplib\3r\plbx3r.lib %relroot%\%1\lib386
ifarch copy cpplib\3s\plbx3s.lib %relroot%\%1\lib386
ifarch copy cpplib\3rmt\plbxmt3r.lib %relroot%\%1\lib386
ifarch copy cpplib\3smt\plbxmt3s.lib %relroot%\%1\lib386
ifarch copy cpplib\nt3r\plbx3r.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\nt3r\plib3r.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\nt3s\plbx3s.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\nt3s\plib3s.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\nt3rmt\plbxmt3r.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\nt3rmt\plibmt3r.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\nt3smt\plbxmt3s.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\nt3smt\plibmt3s.lib %relroot%\%1\lib386\nt
ifarch copy cpplib\ntaxp\plibnt.lib %relroot%\%1\libaxp\nt\plib.lib
ifarch copy cpplib\ntaxp\plbxnt.lib %relroot%\%1\libaxp\nt\plbx.lib
ifarch copy cpplib\os23r\plbx3r.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\os23r\plib3r.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\os23s\plbx3s.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\os23s\plib3s.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\os23rmt\plbxmt3r.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\os23rmt\plibmt3r.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\os23smt\plbxmt3s.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\os23smt\plibmt3s.lib %relroot%\%1\lib386\os2
ifarch copy cpplib\complex\3r\cplx3r.lib %relroot%\%1\lib386
ifarch copy cpplib\complex\3s\cplx3s.lib %relroot%\%1\lib386
ifarch copy cpplib\complex\73r\cplx73r.lib %relroot%\%1\lib386
ifarch copy cpplib\complex\73s\cplx73s.lib %relroot%\%1\lib386
ifarch copy cpplib\complex\ntaxp\cplxnt.lib %relroot%\%1\libaxp\cplx.lib
:nocopy
cd ..
