@echo off
pushd
set LABEL=open_watcom_devel_1.2.1
set PREFIX=open_watcom_devel_1.2.1
set P4OPT=-f
set ARCHIVES=\archives
cdd %ARCHIVES%
del /q %PREFIX%-src.zip >& NUL

rem ##########################################################################
rem ZIP all Open Watcom source files
rm -rf %ARCHIVES%\ow_devel_src
p4 -uanonymous -Panonymous -cOPENWATCOM_DEVEL_SRC sync %P4OPT% @%LABEL%
cd %ARCHIVES%\ow_devel_src
zip -r ..\%PREFIX%-src.zip *

popd

