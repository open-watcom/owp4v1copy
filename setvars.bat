@echo off
REM *****************************************************************
REM SETVARS.BAT - Windows NT version
REM *****************************************************************
REM NOTE: Do not use this batch file directly, but copy it and
REM       modify it as necessary for your own use!!

REM Set up default path information variable
set DEFPATH=%PATH%;
set DOS4G=quiet

REM Setup environment variables for Perforce
set P4PORT=perforce.scitechsoft.com:3488
set P4USER=YourName
set P4CLIENT=YOURCLIENT
set P4PASSWD=YourPassword

REM Change this to point to your OpenWatcom tree
REM Must be an 8.3 name!
set owroot=d:\openwa~1

REM Change this to point to your Watcom 11.0c directory
set watcom=c:\c\wc11

REM temporary sub-directory for pre-build OW tools
set OBJDIR=prebuild

REM Unset this variable to get more debug info
set on_build_machine=1

REM set this variable if you want default windowing support
rem set default_windowing=1

REM Change the default command prompt
prompt $p$g

REM Ensure COMSPEC points to CMD.EXE
set COMSPEC=%SystemRoot%\system32\CMD.EXE
REM Make the window bigger
mode 80,50
cls
echo OpenWatcom compiler build environment

REM Stuff for the Open Watcom build environment
set build_platform=nt386
set batdir=%owroot%
set bld_ver=11
set builder.ctl=lang.ctl
set cge=vi.exe pagedown pageup end
set defrel=rel2
set devdir=%owroot%\bld
set distroot=%owroot%\distrib
set relroot=%owroot%
set dwatcom=%watcom%
set doc_root=%owroot%\docs
set lang=%watcom%
set include=%lang%\h;%lang%\h\win;%devdir%\watcom\h
set lang_bld=%owroot%\bat
set lc=-r
set lib=%owroot%\bld\watcom\lib
set more=-t
set path=%owroot%\bin;%devdir%\build\binnt;%owroot%\bat;%lang%\binnt;%lang%\binw;%lang%\binp;%doc_root%\cmds;%defpath%
set rm=-s
set ver=11
set watcom=%lang%
set edpath=%lang%\eddat
set wwinhelp=%owroot%\bld\online\hlp\ib
set copycmd=/y

REM Documentation related variables
set doc_root=%owroot%\docs
set gml_path=%doc_root%\gml\dos
set gmllib=%doc_root%\gml\syslib
set hcdos=%doc_root%\cmds\hcdos32
set win95hc=hcrtf
set wat31hc=whc
set win31hc=hc31

%devdr%
cd %devdir%
