@echo off
call bmode
call docd trap
call mkem all -h
cd ..
if [%1] == [] goto nodbgcopy
ifarch copy %devdir%\bin\bin\???.trp %relroot%\%1\binw
ifarch copy %devdir%\bin\bin\???serv.exe %relroot%\%1\binw
ifarch copy %devdir%\bin\bin\???help.ex? %relroot%\%1\binw
ifarch copy %devdir%\bin\binp\dll\???.dll %relroot%\%1\binp\dll
ifarch copy %devdir%\bin\binp\dll\std??.dll %relroot%\%1\binp\dll
ifarch copy %devdir%\bin\binp\dll\*.exe %relroot%\%1\binp\dll
ifarch copy %devdir%\bin\binp\dll\wdpmhook.dll %relroot%\%1\binp\dll
ifarch copy %devdir%\bin\binp\???serv.exe %relroot%\%1\binp
ifarch copy %devdir%\bin\binp\wdpmhelp.exe %relroot%\%1\binp
ifarch copy %devdir%\bin\binp\nmpbind.exe %relroot%\%1\binp
ifarch copy %devdir%\bin\bin\???.dll %relroot%\%1\binw
ifarch copy %devdir%\bin\bin\wint32.dll %relroot%\%1\binw
ifarch copy %devdir%\bin\???servw.exe %relroot%\%1\binw
ifarch copy %devdir%\bin\binnt\???.dll %relroot%\%1\binnt
ifarch copy %devdir%\bin\binnt\???serv.exe %relroot%\%1\binnt
ifarch copy %devdir%\bin\axpnt\???.dll %relroot%\%1\axpnt
ifarch copy %devdir%\bin\axpnt\???serv.exe %relroot%\%1\axpnt
ifarch copy %devdir%\bin\nlm\*.nlm %relroot%\%1\nlm
ifarch copy %devdir%\bin\qnx\*.trp %relroot%\%1\qnx\watcom\wd
ifarch copy %devdir%\bin\qnx\???serv.qnx %relroot%\%1\qnx\binq\*.
:nodbgcopy
