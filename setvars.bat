@echo off
REM *****************************************************************
REM SETVARS.BAT - Windows NT version
REM *****************************************************************
REM NOTE: Do not use this batch file directly, but copy it and
REM       modify it as necessary for your own use!!

REM Setup environment variables for Perforce
set P4PORT=perforce.scitechsoft.com:3488
set P4USER=YourName
set P4CLIENT=YOURCLIENT
set P4PASSWD=YourPassword

REM Change this to point to your Open Watcom source tree - must be an 8.3 name!
set OWROOT=d:\ow

REM Change this to point to your existing Open Watcom installation
set WATCOM=c:\c\ow10

REM placeholder
REM placeholder

REM Change this to the install location of GhostScript for PDF creation (optional)
set GHOSTSCRIPT=c:\gs\gs7.04

REM Modifications beyond this point should not be necessary

REM Set this variable to 1 to get debug build
set DEBUG_BUILD=0

REM Set this variable to 1 to get default windowing support in clib
set DEFAULT_WINDOWING=0

REM Set this variable to 0 to suppress documentation build
set DOC_BUILD=1

REM Documentation related variables
REM set appropriate variables to blank for help compilers which you haven't installed
set WIN95HC=hcrtf
set OS2HC=ipfc

REM Set up default path information variable
if "%DEFPATH%" == "" set DEFPATH=%PATH%

REM Subdirectory to be used for bootstrapping
set OBJDIR=bootstrp

REM Subdirectory to be used for building prerequisite utilities
set PREOBJDIR=prebuild

REM Set the version numbers
call version.bat

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
