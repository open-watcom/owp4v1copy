@echo off
call bmode

call docd %devdir%\pbide\run
wmake -i -h

call docd %devdir%\pbide\dlldbg
wmake -i -h

call docd %devdir%\pbide\wig\owindll
wmake -i -h

call docd %devdir%\pbide\wig\ontdll
wmake -i -h

call docd %devdir%\viper\viper
wmake -i -h PBIDE

call docd %devdir%\pbide\fscopy
wmake -i -h

if not [%1] == [] goto copy
goto nocopy
:copy
    ifarch copy %devdir%\pbide\run\run.exe %relroot%\%1\binw\watrun.exe
    ifarch copy %devdir%\pbide\run\runnt.exe %relroot%\%1\binnt\watrun.exe
    ifarch copy %devdir%\pbide\dlldbg\dlldbg.exe %relroot%\%1\binw\dlldbg.exe
    ifarch copy %devdir%\pbide\dlldbg\pbend.dbg %relroot%\%1\binw\pbend.dbg
    ifarch copy %devdir%\pbide\dlldbg\pbstart.dbg %relroot%\%1\binw\pbstart.dbg
    ifarch copy %devdir%\pbide\dlldbg\pbwd.dbg %relroot%\%1\binw\pbwd.dbg
    ifarch copy %devdir%\pbide\dlldbg\pbwdnt.dbg %relroot%\%1\binnt\pbwdnt.dbg
    ifarch copy %devdir%\pbide\wig\owindll\pbide.dll %relroot%\%1\binw\pbide.dll
    ifarch copy %devdir%\pbide\wig\ontdll\pbide.dll %relroot%\%1\binnt\pbide.dll
    ifarch copy %devdir%\pbide\pbdll.h %relroot%\%1\h\pbdll.h
    ifarch copy %devdir%\viper\viper\win\pbide.cfg %relroot%\%1\binw\pbide.cfg
    ifarch copy %devdir%\viper\viper\nt\pbide.cfg %relroot%\%1\binnt\pbide.cfg
    ifarch copy %devdir%\viper\viper\idepb.cfg %relroot%\%1\binw\idepb.cfg
    ifarch copy %devdir%\viper\viper\idepbnt.cfg %relroot%\%1\binnt\idepbnt.cfg
    ifarch copy %devdir%\viper\viper\win\pbidex.cfg %relroot%\%1\binw\pbidex.cfg
    ifarch copy %devdir%\viper\viper\nt\pbidex.cfg %relroot%\%1\binnt\pbidex.cfg
    ifarch copy %devdir%\pbide\fscopy\wfscopy.exe %relroot%\%1\binw\wfscopy.exe
:nocopy
cd ..\..
