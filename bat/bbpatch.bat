@echo off
call bmode
rem
call docd bdiff
wmake /h /i bpatch.exe
rem Make BPATCH.QNX for Database guys. Easier than getting them set up
wmake /h /h bpatch.qnx
cd ..

if [%1] == [] goto nocopy
ifarch copy bdiff\bpatch.exe %relroot%\%1\binw\bpatch.exe
:nocopy
