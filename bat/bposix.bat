@echo off
call bmode

cd posix\systems\dos
call docd .
wmake/h

cd ..\pharlap
call docd .
wmake/h

cd ..\dosos2
call docd .
wmake/h

cd ..\os2v2
call docd .
wmake/h

cd ..\nt
call docd .
wmake/h

cd ..\qnx
call docd .
wmake/h

cd ..\qnx16
call docd .
wmake/h

cd ..\..\..
