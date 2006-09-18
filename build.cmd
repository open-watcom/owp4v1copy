@echo off
REM *****************************************************************
REM build.cmd - build Open Watcom using selected compiler
REM
REM will build the builder, wattcp, watcom and installer
REM
REM combined OS/2 and Windows version
REM
REM If first argument is "self", uses tools in rel2 to build,
REM requiring customized devvars.cmd. Otherwise, customized
REM myvars.cmd is needed.
REM 
REM Call without parms for "builder rel2" operation -> build
REM Call with clean for "builder clean"  operation  -> build clean
REM --> requires a customized setvars.bat/cmd named myvars.cmd
REM --> set WATCOM to the existing OW 1.x installation
REM *****************************************************************
setlocal
if [%1] == [self] goto self
   set myow=myvars
   goto doneself
:SELF
   shift
   set myow=devvars
:DONESELF

if exist %myow%.cmd goto cont1
   echo Customized %myow%.cmd not found, cannot continue
   echo must be in the same dir as build.cmd
   echo copy setvars.cmd/bat to %myow%.cmd and customize
   pause
   goto eof

:CONT1
   call %myow%.cmd
   set target=%1
   if [%target%] == [] set target=rel2
rem the makefiles dont know a target rel2, so only pass target if clean
   set makeclean=
   if [%target%] == [clean] set makeclean=clean

rem NT/XP? or OS/2?
   if [%OS2_SHELL%] == [] goto noOS2
   if [%OS%] == [] goto noWIN
   echo Operating System not recognized, sorry
   goto eof

:NOOS2
rem only works if system is on c:
   if [%Systemroot%] == [C:\WINNT] goto noXP
   set builderdir=nt386
   goto bld1

:NOXP
   set builderdir=nt386
   goto bld1

:NOWIN
   set builderdir=os2386

:BLD1
rem start with the builder
   cd builder\%builderdir%
   wmake %makeclean%

REM wattcp is needed for debugger remote server tcpserv
   cd %devdir%
   cd ..\contrib\wattcp\src
   wmake %makeclean% -ms

REM now we can start the builder with 9 generations of logfiles
   cd %devdir%
   builder %target% -b 9

REM build the installer, REM next 3 lines if not wanted
   cd %devdir%
   cd setupgui
   builder %target% -b 9

:EOF
   endlocal
