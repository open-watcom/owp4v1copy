@echo off
pushd
set LABEL=open_watcom_0.8.0
set PREFIX=open_watcom_0.8.0
set P4OPT=-f
set ARCHIVES=c:\archives
cdd %ARCHIVES%
del /q *.zip

rem ##########################################################################
rem ZIP all Open Watcom source files
rm -rf %ARCHIVES%\openwatcom
p4 -uanonymous -Panonymous -cOPENWATCOM_SRC sync %P4OPT% @%LABEL%
cd %ARCHIVES%\openwatcom
zip -r ..\%PREFIX%-src.zip *

popd

