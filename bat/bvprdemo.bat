@echo off
call bmode

call docd viprdemo\threed\os2
viperb -r os2_3d.tgt
wmake -i -h -f project.mk

call docd ..\win
viperb -r win_3d.tgt
wmake -i -h -f project.mk

call docd ..\nt
viperb -r nt_3d.tgt
wmake -i -h -f project.mk

call docd ..\..\src\os2
viperb -r drawos2.tgt
copy %DEVDIR%\plusplus\bin\rpp38610.exe wpp386.exe
wmake -i -h -f drawos2.mk1 %DEVDIR%\viprdemo\src\os2\box.obj
wmake -i -h -f drawos2.mk1 %DEVDIR%\viprdemo\src\os2\drawroom.obj
wmake -i -h -f drawos2.mk1 %DEVDIR%\viprdemo\src\os2\furnitu.obj
del wpp386.exe

call docd ..\win
viperb -r draw16.tgt
copy %DEVDIR%\plusplus\bin\rppi8610.exe wpp.exe
wmake -i -h -f draw16.mk1 %DEVDIR%\viprdemo\src\win\box.obj
wmake -i -h -f draw16.mk1 %DEVDIR%\viprdemo\src\win\drawroom.obj
wmake -i -h -f draw16.mk1 %DEVDIR%\viprdemo\src\win\furnitu.obj
del wpp.exe

call docd ..\win386
viperb -r draw.tgt
copy %DEVDIR%\plusplus\bin\rpp38610.exe wpp386.exe
wmake -i -h -f draw.mk1 %DEVDIR%\viprdemo\src\win386\box.obj
wmake -i -h -f draw.mk1 %DEVDIR%\viprdemo\src\win386\drawroom.obj
wmake -i -h -f draw.mk1 %DEVDIR%\viprdemo\src\win386\furnitu.obj
del wpp386.exe

call docd ..\win32
viperb -r draw32.tgt
copy %DEVDIR%\plusplus\bin\rpp38610.exe wpp386.exe
wmake -i -h -f draw32.mk1 %DEVDIR%\viprdemo\src\win32\box.obj
wmake -i -h -f draw32.mk1 %DEVDIR%\viprdemo\src\win32\drawroom.obj
wmake -i -h -f draw32.mk1 %DEVDIR%\viprdemo\src\win32\furnitu.obj
del wpp386.exe

cd ..\..

if [%1] == [] goto nocopy
cd src
ifarch copy *.* %relroot%\%1\samples\ide
ifarch copy fortran\*.* %relroot%\%1\samples\ide\fortran
ifarch copy fortran\win32\*.* %relroot%\%1\samples\ide\fortran\win32
ifarch copy fortran\os2\*.* %relroot%\%1\samples\ide\fortran\os2
ifarch copy fortran\win\*.* %relroot%\%1\samples\ide\fortran\win
ifarch copy fortran\win386\*.* %relroot%\%1\samples\ide\fortran\win386
ifarch copy win32\*.* %relroot%\%1\samples\ide\win32
ifarch copy os2\*.* %relroot%\%1\samples\ide\os2
ifarch copy win\*.* %relroot%\%1\samples\ide\win
ifarch copy win386\*.* %relroot%\%1\samples\ide\win386
cd ..
ifarch copy threed\os2\os2_3d.dll %relroot%\%1\samples\ide
ifarch copy threed\os2\os2_3d.dll %relroot%\%1\samples\ide\os2
ifarch copy threed\win\win_3d.dll %relroot%\%1\samples\ide\win
ifarch copy threed\win\win_3d.dll %relroot%\%1\samples\ide\win386
ifarch copy threed\nt\nt_3d.dll  %relroot%\%1\samples\ide\win32

ifarch copy threed\os2\os2_3d.dll   %relroot%\%1\samples\ide\fortran
ifarch copy threed\os2\os2_3d.dll   %relroot%\%1\samples\ide\fortran\os2
ifarch copy threed\win\win_3d.dll   %relroot%\%1\samples\ide\fortran\win
ifarch copy threed\win\win_3d.dll   %relroot%\%1\samples\ide\fortran\win386
ifarch copy threed\nt\nt_3d.dll     %relroot%\%1\samples\ide\fortran\win32
ifarch copy src\os2\box.obj         %relroot%\%1\samples\ide\fortran\os2
ifarch copy src\os2\drawroom.obj    %relroot%\%1\samples\ide\fortran\os2
ifarch copy src\os2\furnitu.obj     %relroot%\%1\samples\ide\fortran\os2
ifarch copy src\win\box.obj         %relroot%\%1\samples\ide\fortran\win
ifarch copy src\win\drawroom.obj    %relroot%\%1\samples\ide\fortran\win
ifarch copy src\win\furnitu.obj     %relroot%\%1\samples\ide\fortran\win
ifarch copy src\win386\box.obj      %relroot%\%1\samples\ide\fortran\win386
ifarch copy src\win386\drawroom.obj %relroot%\%1\samples\ide\fortran\win386
ifarch copy src\win386\furnitu.obj  %relroot%\%1\samples\ide\fortran\win386
ifarch copy src\win32\box.obj       %relroot%\%1\samples\ide\fortran\win32
ifarch copy src\win32\drawroom.obj  %relroot%\%1\samples\ide\fortran\win32
ifarch copy src\win32\furnitu.obj   %relroot%\%1\samples\ide\fortran\win32
:nocopy
cd ..
