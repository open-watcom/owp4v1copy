@echo off
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
set owroot=d:\openwatcom
set owdrive=d:

REM Change this to point to your Watcom 11.0c directory
set watcom=c:\c\wc11

REM Change this to point to your OS/2 Toolkit directory
set os2tkroot=c:\Toolkit

REM Unset this variable to get more debug info
set on_build_machine=1

REM Change the default command prompt
prompt $p$g

REM Ensure COMSPEC points to CMD.EXE
set COMSPEC=C:\OS2\CMD.EXE
REM Make the window bigger
mode 80,50
cls
echo OpenWatcom compiler build environment

REM Stuff for the Open Watcom build environment
set build_platform=os2386
set rsidir=d:\rsi
set batdir=%owroot%
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
set path=%owroot%\binp;%devdir%\build\binp;%owroot%\bat;%lang%\binp;%lang%\binw;%devdir%\tools;%devdir%\cmds;%doc_root%\cmds;%defpath%
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
set beginlibpath=%watcom%\binp\dll

REM Documentation related variables
set doc_root=%owroot%\docs
set whelp=%doc_root%\doc\whelp
set gml_root=%doc_root%\gml\os2
set gmllib=%doc_root%\gml\syslib
set whpcvt=%doc_root%\cmds\whpcvto2
set hcdos=%doc_root%\cmds\hcdosos2
set hlpdir=%devdir%\online\hlp
set ipfc=%os2tkroot%\ipfc

%devdr%
cd %devdir%
