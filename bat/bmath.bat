@echo off
call bmode
cd mathlib
call docd .
if [%2] == [] goto ordinary_make
if [%2] == [nomake] goto just_copy

    pmake.exe -d %2 %3 %4 %5 %6 %7 %8 %9 -h
    goto done_make

:ordinary_make
    pmake.exe -d all -h

:done_make
cd %devdir%\mathlib

:just_copy
if [%1] == [] goto nolibcopy
if not exist %relroot%\%1\lib286 goto nolibcopy
ifarch copy %devdir%\mathlib\library\msdos.286\mc\mathc.lib %relroot%\%1\lib286\mathc.lib
ifarch copy %devdir%\mathlib\library\msdos.286\mh\mathh.lib %relroot%\%1\lib286\mathh.lib
ifarch copy %devdir%\mathlib\library\msdos.286\ml\mathl.lib %relroot%\%1\lib286\mathl.lib
ifarch copy %devdir%\mathlib\library\msdos.286\mm\mathm.lib %relroot%\%1\lib286\mathm.lib
ifarch copy %devdir%\mathlib\library\msdos.286\ms\maths.lib %relroot%\%1\lib286\maths.lib
ifarch copy %devdir%\mathlib\library\msdos.287\mc\mathc.lib %relroot%\%1\lib286\math87c.lib
ifarch copy %devdir%\mathlib\library\msdos.287\mh\mathh.lib %relroot%\%1\lib286\math87h.lib
ifarch copy %devdir%\mathlib\library\msdos.287\ml\mathl.lib %relroot%\%1\lib286\math87l.lib
ifarch copy %devdir%\mathlib\library\msdos.287\mm\mathm.lib %relroot%\%1\lib286\math87m.lib
ifarch copy %devdir%\mathlib\library\msdos.287\ms\maths.lib %relroot%\%1\lib286\math87s.lib
ifarch copy %devdir%\mathlib\library\msdos.386\ms_r\maths.lib %relroot%\%1\lib386\math3r.lib
ifarch copy %devdir%\mathlib\library\msdos.386\ms_s\maths.lib %relroot%\%1\lib386\math3s.lib
ifarch copy %devdir%\mathlib\library\msdos.387\ms_r\maths.lib %relroot%\%1\lib386\math387r.lib
ifarch copy %devdir%\mathlib\library\msdos.387\ms_s\maths.lib %relroot%\%1\lib386\math387s.lib
ifarch copy %devdir%\mathlib\library\windows.286\mc\mathc.lib %relroot%\%1\lib286\win\mathc.lib
ifarch copy %devdir%\mathlib\library\windows.286\ml\mathl.lib %relroot%\%1\lib286\win\mathl.lib
ifarch copy %devdir%\mathlib\library\windows.287\mc\mathc.lib %relroot%\%1\lib286\win\math87c.lib
ifarch copy %devdir%\mathlib\library\windows.287\ml\mathl.lib %relroot%\%1\lib286\win\math87l.lib
:nolibcopy
cd %devdir%
