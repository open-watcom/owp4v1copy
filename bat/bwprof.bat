@echo off
call bmode
call docd wprof
call mkall say

if [%1] == [] goto nocopy
ifarch copy %devdir%\wprof\pm\wprof.exe %relroot%\%1\binp\wprof.exe
ifarch copy %devdir%\wprof\win\wprofw.exe %relroot%\%1\binw\wprofw.exe
ifarch copy %devdir%\wprof\dos\wprof.exe %relroot%\%1\binw\wprof.exe
ifarch copy %devdir%\wprof\dos\wprofnec.exe %relroot%\%1\n98bw\wprof.exe
ifarch copy %devdir%\wprof\nt\wprof.exe %relroot%\%1\binnt\wprof.exe
ifarch copy %devdir%\wprof\qnx\wprof.qnx %relroot%\%1\qnx\binq\wprof.
if exist %devdir%\wprof\qnx\wprof.sym mv %devdir%\wprof\qnx\wprof.sym %relroot%\%1\qnx\sym\wprof.sym
:nocopy
