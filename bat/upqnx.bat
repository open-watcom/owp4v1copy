@echo off
if not exist r:\cmds\ansi.com goto nonet
    set RELDIR=%1
    if [%RELDIR%] == [] set RELDIR=%DEFREL%
    set src=%relroot%\%RELDIR%
    xcopy %src%\qnx s:\qnx /s
REM    copy  %src%\lib286\qnx\clibshar.lib s:\qnx
    goto done
:nonet
    echo Network not present
    goto done
:done
