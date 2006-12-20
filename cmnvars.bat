@echo off
REM *****************************************************************
REM CMNVARS.BAT - common environment variables
REM *****************************************************************
REM NOTE: All batch files to set the environment must call this batch
REM       file at the end.

REM Set the version numbers
set BLD_VER=17
set BLD_VER_STR=1.7

REM Set up default path information variable
if "%DEFPATH%" == "" set DEFPATH=%PATH%

REM Stuff for the Open Watcom build environment
set BUILD_PLATFORM=nt386
set BUILDER_CTL=lang.ctl
set DEVDIR=%OWROOT%\bld
REM Subdirectory to be used for bootstrapping/prebuild binaries
set OWBINDIR=%DEVDIR%\build\binnt
set DISTROOT=%OWROOT%\distrib
set RELROOT=%OWROOT%
set DWATCOM=%WATCOM%
set DOC_ROOT=%OWROOT%\docs
set INCLUDE=%WATCOM%\h;%WATCOM%\h\nt
set EDPATH=%WATCOM%\eddat
set PATH=%OWROOT%\bin;%OWBINDIR%;%OWROOT%\bat;%WATCOM%\binnt;%WATCOM%\binw;%WATCOM%\binp;%DOC_ROOT%\cmds;%DEFPATH%

echo Open Watcom compiler build environment

REM OS specifics

cd %DEVDIR%

set DOS4G=quiet

REM setup right COMSPEC for non-standard COMSPEC setting on NT based systems
if '%OS%' == 'Windows_NT' set COMSPEC=%windir%\system32\cmd.exe

set COPYCMD=/y
