@echo off
call bmode
cd rtdll
call docd .
if [%2] == [nomake] goto just_copy

cd clib
pmake.exe -d -b build %2 %3 %4 %5 %6 %7 %8 %9 -h
call tmp.bat
erase tmp.bat
cd ..

cd mathlib
pmake.exe -d -b build %2 %3 %4 %5 %6 %7 %8 %9 -h
call tmp.bat
erase tmp.bat
cd ..

cd cpplib
pmake.exe -d -b build %2 %3 %4 %5 %6 %7 %8 %9 -h
call tmp.bat
erase tmp.bat
cd ..

:just_copy
if [%1] == [] goto nolibcopy
if not exist %relroot%\%1\lib386 goto nolibcopy

ifarch copy clib\os2.386\ms_r\clibs.lib %relroot%\%1\lib386\os2\clbrdll.lib
ifarch copy clib\os2.386\ms_r\clbr107.dll %relroot%\%1\binp\dll\clbr107.dll
ifarch copy clib\os2.386\ms_r\clbr107.sym %relroot%\%1\binp\dll\clbr107.sym
ifarch copy clib\os2.386\ms_s\clibs.lib %relroot%\%1\lib386\os2\clbsdll.lib
ifarch copy clib\os2.386\ms_s\clbs107.dll %relroot%\%1\binp\dll\clbs107.dll
ifarch copy clib\os2.386\ms_s\clbs107.sym %relroot%\%1\binp\dll\clbs107.sym
ifarch copy clib\winnt.386\ms_r\clibs.lib %relroot%\%1\lib386\nt\clbrdll.lib
ifarch copy clib\winnt.386\ms_r\clbr107.dll %relroot%\%1\binnt\clbr107.dll
ifarch copy clib\winnt.386\ms_r\clbr107.sym %relroot%\%1\binnt\clbr107.sym
ifarch copy clib\winnt.386\ms_s\clibs.lib %relroot%\%1\lib386\nt\clbsdll.lib
ifarch copy clib\winnt.386\ms_s\clbs107.dll %relroot%\%1\binnt\clbs107.dll
ifarch copy clib\winnt.386\ms_s\clbs107.sym %relroot%\%1\binnt\clbs107.sym

ifarch copy mathlib\os2.386\ms_r\clibs.lib %relroot%\%1\lib386\os2\mthrdll.lib
ifarch copy mathlib\os2.386\ms_r\mthr107.dll %relroot%\%1\binp\dll\mthr107.dll
ifarch copy mathlib\os2.386\ms_r\mthr107.sym %relroot%\%1\binp\dll\mthr107.sym
ifarch copy mathlib\os2.387\ms_r\clibs.lib %relroot%\%1\lib386\os2\mt7rdll.lib
ifarch copy mathlib\os2.387\ms_r\mt7r107.dll %relroot%\%1\binp\dll\mt7r107.dll
ifarch copy mathlib\os2.387\ms_r\mt7r107.sym %relroot%\%1\binp\dll\mt7r107.sym
ifarch copy mathlib\os2.386\ms_s\clibs.lib %relroot%\%1\lib386\os2\mthsdll.lib
ifarch copy mathlib\os2.386\ms_s\mths107.dll %relroot%\%1\binp\dll\mths107.dll
ifarch copy mathlib\os2.386\ms_s\mths107.sym %relroot%\%1\binp\dll\mths107.sym
ifarch copy mathlib\os2.387\ms_s\clibs.lib %relroot%\%1\lib386\os2\mt7sdll.lib
ifarch copy mathlib\os2.387\ms_s\mt7s107.dll %relroot%\%1\binp\dll\mt7s107.dll
ifarch copy mathlib\os2.387\ms_s\mt7s107.sym %relroot%\%1\binp\dll\mt7s107.sym
ifarch copy mathlib\winnt.386\ms_r\clibs.lib %relroot%\%1\lib386\nt\mthrdll.lib
ifarch copy mathlib\winnt.386\ms_r\mthr107.dll %relroot%\%1\binnt\mthr107.dll
ifarch copy mathlib\winnt.386\ms_r\mthr107.sym %relroot%\%1\binnt\mthr107.sym
ifarch copy mathlib\winnt.387\ms_r\clibs.lib %relroot%\%1\lib386\nt\mt7rdll.lib
ifarch copy mathlib\winnt.387\ms_r\mt7r107.dll %relroot%\%1\binnt\mt7r107.dll
ifarch copy mathlib\winnt.387\ms_r\mt7r107.sym %relroot%\%1\binnt\mt7r107.sym
ifarch copy mathlib\winnt.386\ms_s\clibs.lib %relroot%\%1\lib386\nt\mthsdll.lib
ifarch copy mathlib\winnt.386\ms_s\mths107.dll %relroot%\%1\binnt\mths107.dll
ifarch copy mathlib\winnt.386\ms_s\mths107.sym %relroot%\%1\binnt\mths107.sym
ifarch copy mathlib\winnt.387\ms_s\clibs.lib %relroot%\%1\lib386\nt\mt7sdll.lib
ifarch copy mathlib\winnt.387\ms_s\mt7s107.dll %relroot%\%1\binnt\mt7s107.dll
ifarch copy mathlib\winnt.387\ms_s\mt7s107.sym %relroot%\%1\binnt\mt7s107.sym

ifarch copy cpplib\os2.386\ms_r\clibs.lib %relroot%\%1\lib386\os2\plbrdll.lib
cmp -s %relroot%\%1\lib386\os2\plbrdll.lib %relroot%\%1\lib386\os2\plbrdllx.lib
if errorlevel 1 copy %relroot%\%1\lib386\os2\plbrdll.lib %relroot%\%1\lib386\os2\plbrdllx.lib
ifarch copy cpplib\os2.386\ms_r\plbr107.dll %relroot%\%1\binp\dll\plbr107.dll
ifarch copy cpplib\os2.386\ms_r\plbr107.sym %relroot%\%1\binp\dll\plbr107.sym
ifarch copy cpplib\os2.386\ms_s\clibs.lib %relroot%\%1\lib386\os2\plbsdll.lib
cmp -s %relroot%\%1\lib386\os2\plbsdll.lib %relroot%\%1\lib386\os2\plbsdllx.lib
if errorlevel 1 copy %relroot%\%1\lib386\os2\plbsdll.lib %relroot%\%1\lib386\os2\plbsdllx.lib
ifarch copy cpplib\os2.386\ms_s\plbs107.dll %relroot%\%1\binp\dll\plbs107.dll
ifarch copy cpplib\os2.386\ms_s\plbs107.sym %relroot%\%1\binp\dll\plbs107.sym
ifarch copy cpplib\winnt.386\ms_r\clibs.lib %relroot%\%1\lib386\nt\plbrdll.lib
cmp -s %relroot%\%1\lib386\nt\plbrdll.lib %relroot%\%1\lib386\nt\plbrdllx.lib
if errorlevel 1 copy %relroot%\%1\lib386\nt\plbrdll.lib %relroot%\%1\lib386\nt\plbrdllx.lib
ifarch copy cpplib\winnt.386\ms_r\plbr107.dll %relroot%\%1\binnt\plbr107.dll
ifarch copy cpplib\winnt.386\ms_r\plbr107.sym %relroot%\%1\binnt\plbr107.sym
ifarch copy cpplib\winnt.386\ms_s\clibs.lib %relroot%\%1\lib386\nt\plbsdll.lib
cmp -s %relroot%\%1\lib386\nt\plbsdll.lib %relroot%\%1\lib386\nt\plbsdllx.lib
if errorlevel 1 copy %relroot%\%1\lib386\nt\plbsdll.lib %relroot%\%1\lib386\nt\plbsdllx.lib
ifarch copy cpplib\winnt.386\ms_s\plbs107.dll %relroot%\%1\binnt\plbs107.dll
ifarch copy cpplib\winnt.386\ms_s\plbs107.sym %relroot%\%1\binnt\plbs107.sym

:nolibcopy
cd %devdir%
