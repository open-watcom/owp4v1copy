@echo on
pushd
set LABEL=open_watcom_0.8.4
set PREFIX=open_watcom_0.8.4
set P4OPT=-f
set ARCHIVES=c:\archives
cdd %ARCHIVES%
del /q *.zip >& NUL

rem ##########################################################################
rem ZIP all Open Watcom source files
rm -rf %ARCHIVES%\ow_src
p4 -uanonymous -Panonymous -cOPENWATCOM_SRC sync %P4OPT% @%LABEL%
cd %ARCHIVES%\ow_src
zip -r ..\%PREFIX%-src.zip *

rem ##########################################################################
rem ZIP all Open Watcom documentation source files
rm -rf %ARCHIVES%\ow_docs
p4 -uanonymous -Panonymous -cOPENWATCOM_DOCS sync %P4OPT% @%LABEL%
cd %ARCHIVES%\ow_docs
zip -r ..\%PREFIX%-docs.zip *

popd

