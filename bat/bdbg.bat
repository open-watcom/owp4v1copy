@echo off
call bmode ram
if errorlevel 1 goto done
set wlink=option quiet
set optasm=/t
%devdr%:
call docd %devdir%
if not [%2] == [] clog #50 %2
if     [%2] == [] clog #50 bdbg.log
call bwpi       %1
rem call bsdk   %1
call bui        %1
call blui       %1
call bdui       %1
call bdip       %1
call btrap      %1
call bvid       %1
clog
