@echo off
echo call %tooldr%\tools\setenv %lang% %ramd%:\ > __reset.bat
call unset
call __reset
erase __reset.bat
if not [%1] == [ram] goto done
    call ramit %watcom%\binb wcc.exe wcc386.exe
    call ramit %watcom%\bin  w32run.exe
    call ramit %watcom%\bin  wlink.exe
    call ramit %watcom%\binb wmake.exe
    call ramit %watcom%\binb wasm.exe
:done
