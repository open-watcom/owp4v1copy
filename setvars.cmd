@echo off
REM *****************************************************************
REM SETVARS.CMD - OS/2 version
REM *****************************************************************
REM NOTE: Do not use this batch file directly, but copy it and
REM       modify it as necessary for your own use!!

REM Change this to point to your Open Watcom source tree
set OWROOT=d:\openwatcom

REM Change this to point to your existing Open Watcom installation
set WATCOM=c:\c\ow10

REM Set this variable to 1 to get debug build
set DEBUG_BUILD=0

REM Set this variable to 1 to get default windowing support in clib
set DEFAULT_WINDOWING=0

REM Set this variable to 0 to suppress documentation build
set DOC_BUILD=1

REM Setup environment variables for Perforce
set P4PORT=perforce.scitechsoft.com:3488
set P4USER=YourName
set P4CLIENT=YOURCLIENT
set P4PASSWD=YourPassword

REM Documentation related variables
REM set appropriate variables to blank for help compilers which you haven't installed
set WIN95HC=hcrtf
set WIN31HC=hc31
set OS2HC=ipfc

REM Set up default path information variable
if %DEFPATH%. == . set DEFPATH=%PATH%

REM Subdirectory to be used for bootstrapping
set OBJDIR=prebuild

REM Stuff for the Open Watcom build environment
set BUILD_PLATFORM=os2386
set BATDIR=%OWROOT%
set BLD_VER=14
set BLD_VER_STR=1.4
set BUILDER_CTL=lang.ctl
set DEFREL=rel2
set DEVDIR=%OWROOT%\bld
set DISTROOT=%OWROOT%\distrib
set RELROOT=%OWROOT%
set DWATCOM=%WATCOM%
set DOC_ROOT=%OWROOT%\docs
set LANG=%WATCOM%
set INCLUDE=%LANG%\h;%LANG%\h\win;%DEVDIR%\watcom\h
set LANG_BLD=%OWROOT%\bat
set LIB=%OWROOT%\bld\watcom\lib

echo Open Watcom compiler build environment

REM Change this to point to your OS/2 Toolkit directory
set os2tkroot=c:\Toolkit

REM split up path for easy checking.
PATH %OWROOT%\binp
PATH %PATH%;%DEVDIR%\build\binp
PATH %PATH%;%OWROOT%\bat
PATH %PATH%;%LANG%\binp
PATH %PATH%;%LANG%\binw
PATH %PATH%;%DOC_ROOT%\cmds
PATH %PATH%;%OS2TKROOT%\bin
PATH %PATH%;%DEFPATH%

set EDPATH=%LANG%\eddat
set WWINHELP=%OWROOT%\bld\online\hlp\ib

set DOS4G=quiet

%DEVDR%
cd %DEVDIR%

REM Change this to the install location of GhostScript for PDF creation
REM (optional)
set GHOSTSCRIPT=C:\gs\gs7.04

REM Ensure COMSPEC points to CMD.EXE
set COMSPEC=CMD.EXE

set LIB=%LIB%;%os2tkroot%\lib
set ipfc=%os2tkroot%\ipfc
set beginlibpath=%watcom%\binp\dll
