@echo off
call bmode
call docd %devdir%\misc\stubs
wmake -h
if [%1] == [] goto nocopy
rem ifarch copy wcc386p.exe %relroot%\%1\binw\wcc386p.exe
rem ifarch copy wfc386p.exe %relroot%\%1\binw\wfc386p.exe
rem ifarch copy wccx.exe %relroot%\%1\binw\wccp.exe
rem ifarch copy wfcp.exe %relroot%\%1\binw\wfcp.exe
rem ifarch copy wlinkp.exe %relroot%\%1\binw\wlinkp.exe
call docd %devdir%\misc
ifarch copy cvpack.nt %relroot%\%1\binnt\cvpack.exe
ifarch copy cvpack.os2 %relroot%\%1\binp\cvpack.exe
ifarch copy *.sex %relroot%\%1\binw\*.exe
ifarch copy dos4gw.doc %relroot%\%1
ifarch copy weditf.hlp %relroot%\%1\binw
ifarch copy dbglib.rex %relroot%\%1\binw
ifarch copy unicode.* %relroot%\%1\binw
rem ifarch copy api32wh.hlp %relroot%\%1\binw
rem ifarch copy api32wh.ind %relroot%\%1\binw
ifarch copy nt.lib %relroot%\%1\lib386\nt
ifarch copy watcom.ico %relroot%\%1
call docd win
ifarch copy *.dll %relroot%\%1\binw
call docd ..\..\techinfo
wmake -h
ifarch copy techinfo.exe %relroot%\%1\binw
cd ..
ifarch copy watcom\lib\wresf.lib %relroot%\%1\lib386
ifarch copy watcom\lib\wresfs.lib %relroot%\%1\lib386
ifarch copy watcom\lib\wresm.lib %relroot%\%1\lib286
ifarch copy watcom\lib\wresl.lib %relroot%\%1\lib286
:nocopy
