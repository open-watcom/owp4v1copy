@echo off
REM *****************************************************************
REM SETVARS.CMD - OS/2 version
REM *****************************************************************
REM NOTE: Do not use this batch file directly, but copy it and
REM       modify it as necessary for your own use!!

REM Setup environment variables for Perforce
set P4PORT=perforce.scitechsoft.com:3488
set P4USER=YourName
set P4CLIENT=YOURCLIENT
set P4PASSWD=YourPassword

REM Change this to point to your Open Watcom source tree
set OWROOT=d:\ow

REM Change this to point to your existing Open Watcom installation
set WATCOM=c:\c\ow10

REM Change this to point to your OS/2 Toolkit directory
set OS2TKROOT=c:\Toolkit

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
set WIN95HC=
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
set BUILD_PLATFORM=os2386
set BUILDER_CTL=lang.ctl
set DEVDIR=%OWROOT%\bld
REM Subdirectory to be used for bootstrapping/prebuild binaries
set OWBINDIR=%DEVDIR%\build\binp
set DISTROOT=%OWROOT%\distrib
set RELROOT=%OWROOT%
set DWATCOM=%WATCOM%
set DOC_ROOT=%OWROOT%\docs
set INCLUDE=%WATCOM%\h;%WATCOM%\h\os2
set EDPATH=%WATCOM%\eddat
set PATH=%OWROOT%\binp;%OWBINDIR%;%OWROOT%\bat;%WATCOM%\binp;%WATCOM%\binw;%DOC_ROOT%\cmds;%OS2TKROOT%\bin;%DEFPATH%

echo Open Watcom compiler build environment

REM OS specifics

cd %DEVDIR%

REM Ensure COMSPEC points to CMD.EXE
set COMSPEC=CMD.EXE

set IPFC=%OS2TKROOT%\ipfc
set BEGINLIBPATH=%WATCOM%\binp\dll
