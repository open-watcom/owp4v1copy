@echo off
call bmode

call docd online\hlp\win
rem call mkall

call docd ..\os2
rem call mkall

call docd ..\ib
rem call mkall

cd ..\..\..

if [%1] == [] goto nocopy
set _onlinedir=d:
for %%i in (%_onlinedir%\online\hlp\win\*.hlp) do ifarch copy %%i %relroot%\%1\binw
for %%i in (%_onlinedir%\online\hlp\os2\*.inf) do ifarch copy %%i %relroot%\%1\binp\help
for %%i in (%_onlinedir%\online\hlp\os2\*.hlp) do ifarch copy %%i %relroot%\%1\binp\help
for %%i in (%_onlinedir%\online\hlp\ib\*.ihp) do ifarch copy %%i %relroot%\%1\binw
set _onlinedir=
:nocopy
