@echo off
REM NOTE: Do not use this batch file directly, but copy it and
REM       modify it as necessary for your own use!!

REM: Set up default path information variable
set DEFPATH=%PATH%;
set DOS4G=quiet

REM Setup environment variables for Perforce
set P4PORT=perforce.scitechsoft.com:3488
set P4USER=YourName
set P4CLIENT=YOURCLIENT
set P4PASSWD=YourPassword

REM Change this to point to your OpenWatcom tree
set owroot=d:\openwatcom
set owdrive=d:

REM Change this to point to your Watcom 11.0c directory
set watcom=c:\c\wc11

REM Unset this variable to get more debug info
set on_build_machine=1

REM Change the default command prompt
prompt $p$g

REM Ensure COMSPEC points to CMD.EXE
set COMSPEC=C:\WINNT\system32\CMD.EXE
REM Make the window bigger
mode 80,50
cls
echo OpenWatcom compiler build environment

rem Stuff from Watcom for the build environment
set rsidir=d:\rsi
set batdir=%owdrive%
set bld_ver=110
set builder.ctl=lang.ctl
set cge=vi.exe pagedown pageup end
set defrel=rel2
set devdir=%owroot%\bld
set devdir2=%owroot%\bld
set devdr=%owdrive%
set distroot=%owroot%\distrib
set dwatcom=%watcom%
set doc_root=%owroot%\docs
set lang=%watcom%
set include=%lang%\h;%lang%\h\win;%devdir%\watcom\h
set lang_bld=%owroot%\bat
set lc=-r
set lib=%owroot%\bld\watcom\lib
set more=-t
set path=%owroot%\bin;%devdir%\build\bin;%devdr%\bat;%lang%\binnt;%lang%\binw;%devdr%\tools;%devdr%\cmds;%doc_root%\cmds;%defpath%
set relroot=%owroot%
set rm=-s
set manifest=%owroot%\manifest
set rootdir=%owdrive%\
set setupdr=%owdrive%
set shell=cmd.exe
set comspec=cmd.exe
set tntdir=d:\tools\tnt6
set tooldir=d:
set tooldr=d:
set ver=11
set watcom=%lang%
set edpath=%lang%\eddat
set wwinhelp=%owroot%\bld\online\hlp\ib
set copycmd=/y
%devdr%
cd %devdir%
