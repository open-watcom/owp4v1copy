@echo off
call bmode
cdsay f77\samples\windll
wmake /h /i cover16.obj
cdsay ..\..\..\src
if [%1] == [] goto nocopy
sweep if exist *.sex ren *.sex *.exe
sweep if exist *.sdl ren *.sdl *.dll
ifarch copy %devdir%\clib\startup\a\cstrt386.asm %relroot%\%1\src\startup\386\cstrt386.asm
ifarch copy %devdir%\clib\startup\a\cstrtw32.asm %relroot%\%1\src\startup\386\cstrtw32.asm
ifarch copy %devdir%\clib\startup\a\cstrtwnt.asm %relroot%\%1\src\startup\386\cstrtwnt.asm
ifarch copy %devdir%\clib\startup\a\cstrtwwt.asm %relroot%\%1\src\startup\386\cstrtwwt.asm
ifarch copy %devdir%\clib\startup\a\cstrtx32.asm %relroot%\%1\src\startup\386\cstrtx32.asm
ifarch copy %devdir%\clib\startup\c\cmain386.c %relroot%\%1\src\startup\386\cmain386.c
ifarch copy %devdir%\clib\startup\c\wildargv.c %relroot%\%1\src\startup\wildargv.c
ifarch copy %devdir%\clib\startup\a\cstrt086.asm %relroot%\%1\src\startup\dos\cstrt086.asm
ifarch copy %devdir%\clib\startup\a\dos16m.asm %relroot%\%1\src\startup\dos\dos16m.asm
ifarch copy %devdir%\clib\startup\a\dstrt386.asm %relroot%\%1\src\startup\386\dstrt386.asm
cmp -s %devdir%\clib\startup\c\cmain086.c %relroot%\%1\src\startup\os2\cmain086.c
if not errorlevel 1 goto cmain086
copy %devdir%\clib\startup\c\cmain086.c %relroot%\%1\src\startup\dos\cmain086.c
copy %devdir%\clib\startup\c\cmain086.c %relroot%\%1\src\startup\os2\cmain086.c
:cmain086
ifarch copy %devdir%\clib\startup\c\hpgrw086.c %relroot%\%1\src\startup\dos\hpgrw086.c
ifarch copy %devdir%\clib\startup\c\dmaino16.c %relroot%\%1\src\startup\os2\dmaino16.c
ifarch copy %devdir%\clib\startup\c\libmno16.c %relroot%\%1\src\startup\os2\libmno16.c
ifarch copy %devdir%\clib\startup\c\maino16.c %relroot%\%1\src\startup\os2\maino16.c
ifarch copy %devdir%\clib\startup\c\main2o32.c %relroot%\%1\src\startup\386\main2o32.c
ifarch copy %devdir%\clib\startup\c\libmno32.c %relroot%\%1\src\startup\386\libmno32.c
ifarch copy %devdir%\clib\startup\c\dmaino32.c %relroot%\%1\src\startup\386\dmaino32.c
ifarch copy %devdir%\clib\startup\c\main2wnt.c %relroot%\%1\src\startup\386\main2wnt.c
ifarch copy %devdir%\clib\startup\c\wmainwnt.c %relroot%\%1\src\startup\386\wmainwnt.c
ifarch copy %devdir%\clib\startup\c\lmainwnt.c %relroot%\%1\src\startup\386\lmainwnt.c
ifarch copy %devdir%\clib\startup\c\dmainwnt.c %relroot%\%1\src\startup\386\dmainwnt.c
ifarch copy %devdir%\clib\startup\c\lmn2wnt.c %relroot%\%1\src\startup\386\lmn2wnt.c
ifarch copy %devdir%\clib\startup\a\cstrto16.asm %relroot%\%1\src\startup\os2\cstrto16.asm
ifarch copy %devdir%\clib\startup\a\cstrto32.asm %relroot%\%1\src\startup\386\cstrto32.asm
ifarch copy %devdir%\clib\startup\a\cstrtw16.asm %relroot%\%1\src\startup\win\cstrtw16.asm
ifarch copy %devdir%\clib\startup\a\libentry.asm %relroot%\%1\src\startup\win\libentry.asm
ifarch copy %devdir%\clib\startup\c\8087cw.c %relroot%\%1\src\startup\8087cw.c
ifarch copy %devdir%\clib\startup\a\adstart.asm %relroot%\%1\src\startup\ads\adstart.asm
ifarch copy %devdir%\clib\h\exitwmsg.h %relroot%\%1\src\startup\os2\exitwmsg.h
cmp -s %devdir%\clib\startup\h\initfini.h %relroot%\%1\src\startup\os2\initfini.h
if not errorlevel 1 goto initfini
copy %devdir%\clib\startup\h\initfini.h %relroot%\%1\src\startup\386\initfini.h
copy %devdir%\clib\startup\h\initfini.h %relroot%\%1\src\startup\os2\initfini.h
:initfini
ifarch copy %devdir%\watcom\h\wos2.h %relroot%\%1\src\startup\os2\wos2.h
ifarch copy %devdir%\src\startup\dos\mdef.inc %relroot%\%1\src\startup\dos\mdef.inc
ifarch copy %devdir%\src\startup\win\mdef.inc %relroot%\%1\src\startup\win\mdef.inc
ifarch copy %devdir%\src\startup\os2\mdef.inc %relroot%\%1\src\startup\os2\mdef.inc

ifarch copy %devdir%\src\misc\*.* %relroot%\%1\src
ifarch copy %devdir%\f77\src\*.* %relroot%\%1\src\fortran
ifarch copy %devdir%\f77\src\win\*.* %relroot%\%1\src\fortran\win
ifarch copy %devdir%\f77\src\dos\*.* %relroot%\%1\src\fortran\dos
ifarch copy %devdir%\f77\src\ads\*.* %relroot%\%1\src\fortran\ads
ifarch copy %devdir%\f77\src\ads\rel12\*.* %relroot%\%1\src\fortran\ads\rel12
ifarch copy %devdir%\f77\src\os2\*.* %relroot%\%1\src\fortran\os2
ifarch copy %devdir%\f77\samples\os2\*.* %relroot%\%1\samples\fortran\os2
ifarch copy %devdir%\f77\samples\os2\rexx\*.* %relroot%\%1\samples\fortran\os2\rexx
ifarch copy %devdir%\f77\samples\mthread\mthrdos2.for %relroot%\%1\samples\fortran\os2\mthread.for
ifarch copy %devdir%\f77\samples\mthread\mthrdnt.for %relroot%\%1\samples\fortran\win32\mthread.for
ifarch copy %devdir%\f77\samples\win\*.* %relroot%\%1\samples\fortran\win
ifarch copy %devdir%\f77\samples\windll\*.* %relroot%\%1\samples\fortran\win\dll
ifarch copy %devdir%\f77\samples\dll\*.* %relroot%\%1\samples\fortran\os2\dll
ifarch copy %devdir%\f77\samples\dll\*.* %relroot%\%1\samples\fortran\win32\dll

ifarch copy %devdir%\plusplus\cpplib\contain\cpp\*.* %relroot%\%1\src\cpplib\contain

cd mswin
sweep -a ifarch copy %%l %relroot%\%1\samples\win\%%f
cd ..\dll
sweep -a ifarch copy %%l %relroot%\%1\samples\dll\%%f
cd ..\clibexam
sweep -a ifarch copy %%l %relroot%\%1\samples\clibexam\%%f
cd ..\goodies
sweep -a ifarch copy %%l %relroot%\%1\samples\goodies\%%f
cd ..\cplbexam
sweep -a ifarch copy %%l %relroot%\%1\samples\cplbexam\%%f
cd ..\os2
sweep -a ifarch copy %%l %relroot%\%1\samples\os2\%%f
cd ..

if exist %relroot%\%1\src\__wprj__.del del %relroot%\%1\src\__wprj__.del
sweep if exist *.exe ren *.exe *.sex
sweep if exist *.dll ren *.dll *.sdl
:nocopy
cd ..
