@echo off
call bmode
echo ============================= W32 SDK ==============================
if [%1] == [] goto nocopy
set altroot=d:\w32_sdk
for %%i in (%altroot%\h\*.*) do ifarch copy %%i %relroot%\%1\h\nt
for %%i in (%altroot%\h\gl\*.*) do ifarch copy %%i %relroot%\%1\h\nt\gl
for %%i in (%altroot%\idl\*.*) do ifarch copy %%i %relroot%\%1\h\nt
for %%i in (%altroot%\lib\*.*) do ifarch copy %%i %relroot%\%1\lib386\nt
for %%i in (%altroot%\bin\*.*) do ifarch copy %%i %relroot%\%1\binnt
for %%i in (%altroot%\bin\logger32\*.*) do ifarch copy %%i %relroot%\%1\binnt\logger32
:nocopy
