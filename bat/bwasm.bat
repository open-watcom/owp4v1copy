@echo off
call bmode
call docd wasm\h
wmake /h
rem call docd ..\o
rem wmake /h wasm.exe
rem call docd ..\x
rem wmake /h wasmp.exe
call docd ..\q
wmake /h
call docd ..\osi
wmake /h
rem call docd ..\n
rem wmake /h
cd ..\..
if [%1] == [] goto nocopy
rem ifarch copy wasm\o\wasm.exe %relroot%\%1\binp
rem ifarch copy wasm\x\wasmp.exe %relroot%\%1\binw\wasm.exe
ifarch copy wasm\osi\wasm.exe %relroot%\%1\binw\wasm.exe
if exist wasm\osi\wasm.sym mv wasm\osi\wasm.sym %relroot%\%1\binw\wasm.sym

if exist wasm\q\wasm.qnx mv wasm\q\wasm.qnx %relroot%\%1\qnx\wasm.
if exist wasm\q\wasm.sym mv wasm\q\wasm.sym %relroot%\%1\qnx\sym

rem ifarch copy wasm\n\wasm.exe %relroot%\%1\binnt
:nocopy
