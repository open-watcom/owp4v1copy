@echo off
call bmode
call docd graphlib
call gupdate
cd ..

if [%1] == [] goto nogrcopy
ifarch copy %devdir%\graphlib\fix\graph.lib    %relroot%\%1\lib286\dos\graph.lib
ifarch copy %devdir%\graphlib\fix\graph386.lib %relroot%\%1\lib386\dos\graph.lib
ifarch copy %devdir%\graphlib\fix\graphp.2bj   %relroot%\%1\lib286\os2\graphp.obj
ifarch copy %devdir%\graphlib\fix\graph98.lib  %relroot%\%1\lib286\dos\graph98.lib
ifarch copy %devdir%\graphlib\fix\graph983.lib %relroot%\%1\lib386\dos\graph98.lib
ifarch copy %devdir%\graphlib\fix\graphq.lib   %relroot%\%1\lib286\qnx\graph.lib
ifarch copy %devdir%\graphlib\fix\graphq3.lib  %relroot%\%1\lib386\qnx\graph3r.lib
:nogrcopy
