@echo off
call bmode
set opath=%path%
set path=f:\tools\os2tools;%path%

call docd %devdir%\viper\viper\win
cd ..\..
viperb viper\win\ide.tgt mviper\win\mviper.tgt
wmake -i -h -f project.mk

call docd %devdir%\viper\viper\os2
cd ..\..
viperb viper\os2\ide.tgt mviper\os2\mviper.tgt
wmake -i -h -f project.mk

call docd %devdir%\viper\viper\nt
cd ..\..
viperb viper\nt\ide.tgt mviper\nt\mviper.tgt
wmake -i -h -f project.mk

call docd %devdir%\viper\viper
wmake -i -h IDE

call docd %devdir%\viper\vpdll\dll
wmake -i -h

cd %devdir%\viper
if [%1] == [] goto nocopy

ifarch copy getviper.bat %relroot%\%1\getviper.bat
ifarch copy viper.doc %relroot%\%1\ide.doc
ifarch copy viper\ide.cfg %relroot%\%1\binw\ide.cfg
ifarch copy viper\idedos.cfg %relroot%\%1\binw\idedos.cfg
ifarch copy viper\idewin.cfg %relroot%\%1\binw\idewin.cfg
ifarch copy viper\ideos2.cfg %relroot%\%1\binw\ideos2.cfg
ifarch copy viper\idew32.cfg %relroot%\%1\binw\idew32.cfg
ifarch copy viper\ideads.cfg %relroot%\%1\binw\ideads.cfg
ifarch copy viper\idenlm.cfg %relroot%\%1\binw\idenlm.cfg
ifarch copy viper\idemfc25.cfg %relroot%\%1\binw\idemfc25.cfg
ifarch copy viper\idemfc30.cfg %relroot%\%1\binw\idemfc30.cfg
ifarch copy viper\win\ide.exe %relroot%\%1\binw\ide.exe
ifarch copy viper\win\idex.cfg %relroot%\%1\binw\idex.cfg
ifarch copy viper\win\wsrv.pif %relroot%\%1\binw\wsrv.pif
ifarch copy viper\win\wd.pif %relroot%\%1\binw\wd.pif
ifarch copy viper\os2\ide.exe %relroot%\%1\binp\ide.exe
ifarch copy viper\os2\idex.cfg %relroot%\%1\binp\idex.cfg
ifarch copy viper\nt\ide.exe %relroot%\%1\binnt\ide.exe
ifarch copy viper\nt\idex.cfg %relroot%\%1\binnt\idex.cfg
ifarch copy vpdll\dll\vpdll.dll %relroot%\%1\binw\vpdll.dll

:nocopy
cd ..
set path=%opath%
set opath=
