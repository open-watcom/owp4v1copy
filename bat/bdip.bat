@echo off
call bmode

echo call docd %devdir%\dip\%%1\%%2      >%RAMDISK%doit.bat
echo wmake -h                           >>%RAMDISK%doit.bat
if not [%1] == [] echo ifarch copy %devdir%\bin\%%3\%%1.%%4 %relroot%\%1\%%5 >>%RAMDISK%doit.bat
if not [%1] == [] echo if exist %devdir%\bin\%%3\%%1.sym mv %devdir%\bin\%%3\%%1.sym %relroot%\%1\%%6\%%1.%%7 >>%RAMDISK%doit.bat

set DIP_LIST=WATCOM EXPORT DWARF CODEVIEW
for %%i in (%DIP_LIST%) do call %RAMDISK%doit %%i os2.obj binp\dll      dll binp\dll binp\dll sym
for %%i in (%DIP_LIST%) do call %RAMDISK%doit %%i win.obj bin           dll binw     binw     sym
for %%i in (%DIP_LIST%) do call %RAMDISK%doit %%i nt.obj  binnt         dll binnt    binnt    sym
for %%i in (%DIP_LIST%) do call %RAMDISK%doit %%i dos.obj bin           dip binw     binw     dsy
for %%i in (%DIP_LIST%) do call %RAMDISK%doit %%i qnx.obj qnx           dip qnx\watcom\wd qnx\sym  sym
for %%i in (%DIP_LIST%) do call %RAMDISK%doit %%i nt.axp  axpnt         dll axpnt    axpnt      sym

call docd %devdir%
