@echo off
set dst=%2
if [%dst%] == [] set dst=%DEFNET%
if not exist %dst%\binw\wlink.lnk goto nonet
    set RELDIR=%1
    if [%RELDIR%] == [] set RELDIR=%DEFREL%
    set src=%relroot%\%RELDIR%
    %relroot%
    cd %src%
    dir /a:-da/s/l/b >%RAMDISK%\upd.lst
    sed -f %BATDIR%\bat\xupd.sed <%RAMDISK%\upd.lst >%RAMDISK%\upd.bat
    sed -e "s/^/attrib -a /" <%RAMDISK%\upd.lst >%RAMDISK%\upc.bat
    @echo on
    @call %RAMDISK%\upd %dst% | tee \up.log
    @echo off
    sed -e "s/copy //" -e "s/ .*$//" -e "s/^/attrib -a /" <%RAMDISK%\upd.bat >%RAMDISK%\upc.bat
    call %RAMDISK%\upc
    del %RAMDISK%\up?.*
    if [%PATPROJ%] == [] goto done
         d:
         cd \lpat
         xcopy log\*.* \\scimitar\problem\patches /s >nul
        goto done
:nonet
    echo Network not present
    goto done
:done
