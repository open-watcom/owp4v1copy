@echo off
call bmode
call docd emu
wmake -i -h
call docd ..\emu86
wmake -h -i
cd %devdir%
if [%1] == [] goto nolibcopy
ifarch copy %devdir%\emu\emudos.lib %relroot%\%1\lib386\dos\emu387.lib
cmp -s %devdir%\emu\emustub.lib %relroot%\%1\lib386\noemu387.lib
if not errorlevel 1 goto no387stub
copy %devdir%\emu\emustub.lib %relroot%\%1\lib386\netware\emu387.lib
copy %devdir%\emu\emustub.lib %relroot%\%1\lib386\os2\emu387.lib
copy %devdir%\emu\emustub.lib %relroot%\%1\lib386\osi\emu387.lib
copy %devdir%\emu\emustub.lib %relroot%\%1\lib386\win\emu387.lib
copy %devdir%\emu\emustub.lib %relroot%\%1\lib386\qnx\emu387.lib
copy %devdir%\emu\emustub.lib %relroot%\%1\lib386\nt\emu387.lib
copy %devdir%\emu\emustub.lib %relroot%\%1\lib386\noemu387.lib
:no387stub

ifarch copy %devdir%\emu86\emu87.lib %relroot%\%1\lib286\dos\emu87.lib
ifarch copy %devdir%\emu86\emuos2.lib %relroot%\%1\lib286\os2\emu87.lib
ifarch copy %devdir%\emu86\emuwin.lib %relroot%\%1\lib286\win\emu87.lib
cmp -s %devdir%\emu86\noemu87.lib %relroot%\%1\lib286\noemu87.lib
if not errorlevel 1 goto no87stub
copy %devdir%\emu86\noemu87.lib %relroot%\%1\lib286\qnx\emu87.lib
copy %devdir%\emu86\noemu87.lib %relroot%\%1\lib286\noemu87.lib
:no87stub
ifarch copy %devdir%\emu86\emu87*.qnx %relroot%\%1\qnx\binq\*. >nul
ifarch copy %devdir%\emu\emu387.qnx %relroot%\%1\qnx\binq\*. >nul
:nolibcopy
