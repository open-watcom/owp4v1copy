@echo off
set odir=
if [%1] == [3] set odir=386
set exe=exe
if [%1] == [3] set exe=exp
md ..\o1
echo cd ..\o1
cd ..\o1
call ..\test\all ..\test\incest%1 o%odir%
call ..\test\all ..\test\asm%1
call ..\test\link%1
md ..\o2
echo cd ..\o2
cd ..\o2
call ..\test\all ..\test\incest%1 o1
call ..\test\all ..\test\asm%1
call ..\test\link%1
comp wdisasm.%exe% ..\o1
cd ..\test
set odir=
set exe=
