@echo off
call bmode
call docd wv\dsx\obj
wmake /h /i w
call docd ..\..\dsx\necobj
wmake /h /i w
call docd ..\..\dos\obj
wmake /h /i remmisc.obj remfile.obj dbgfile.obj dosfilio.obj doslkup.obj dostrap.obj
call docd ..\..\os2\obj
wmake /h /i w
call docd ..\..\os2\objpm
wmake /h /i w
call docd ..\..\qnx\obj
wmake /h /i d
call docd ..\..\win\objc
wmake /h /i w
call docd ..\..\win\obj
wmake /h /i w
call docd ..\..\win\necobj
wmake /h /i w
call docd ..\..\win\objntc
wmake /h /i w
call docd ..\..\win\objnt
wmake /h /i w
call docd ..\..\win\objntc.axp
wmake /h /i w
call docd ..\..\win\objnt.axp
wmake /h /i w
call docd ..\..\ssl
wmake /h
ECHO.
ECHO RFX make suppressed - Literal string problems
ECHO.
rem call docd ..\rfx\dos
rem wmake /h /i w
rem call docd ..\os2
rem wmake /h /i w
cd ..\..\..

if [%1] == [] goto nodbgcopy
ifarch copy %devdir%\bin\bin\rfx.exe %relroot%\%1\binw\rfx.exe
ifarch copy %devdir%\bin\bin.nec\wv.exe %relroot%\%1\n98bw\wd.exe
ifarch copy %devdir%\bin\bin\wv.exe %relroot%\%1\binw\wd.exe
if exist %devdir%\bin\bin\wv.sym mv %devdir%\bin\bin\wv.sym %relroot%\%1\binw\wd.sym

ifarch copy %devdir%\bin\binp\rfx.exe %relroot%\%1\binp\rfx.exe
ifarch copy %devdir%\bin\binp\wv.exe %relroot%\%1\binp\wd.exe
ifarch copy %devdir%\bin\binp\wvw.exe %relroot%\%1\binp\wdw.exe
if exist %devdir%\bin\binp\wv.sym mv %devdir%\bin\binp\wv.sym %relroot%\%1\binp\wd.sym
if exist %devdir%\bin\binp\wvw.sym mv %devdir%\bin\binp\wvw.sym %relroot%\%1\binp\wdw.sym

ifarch copy %devdir%\bin\bin.nec\wvc.exe %relroot%\%1\n98bw\wdc.exe
ifarch copy %devdir%\bin\bin\wvc.exe %relroot%\%1\binw\wdc.exe
ifarch copy %devdir%\bin\bin\wvw.exe %relroot%\%1\binw\wdw.exe
if exist %devdir%\bin\bin\wvc.sym mv %devdir%\bin\bin\wvc.sym %relroot%\%1\binw\wdc.sym
if exist %devdir%\bin\bin\wvw.sym mv %devdir%\bin\bin\wvw.sym %relroot%\%1\binw\wdw.sym

ifarch copy %devdir%\bin\binnt\wv.exe %relroot%\%1\binnt\wd.exe
ifarch copy %devdir%\bin\binnt\wvw.exe %relroot%\%1\binnt\wdw.exe
if exist %devdir%\bin\binnt\wv.sym mv %devdir%\bin\binnt\wv.sym %relroot%\%1\binnt\wd.sym
if exist %devdir%\bin\binnt\wvw.sym mv %devdir%\bin\binnt\wvw.sym %relroot%\%1\binnt\wdw.sym
ifarch copy %devdir%\bin\axpnt\wv.exe %relroot%\%1\axpnt\wd.exe
ifarch copy %devdir%\bin\axpnt\wvw.exe %relroot%\%1\axpnt\wdw.exe
if exist %devdir%\bin\axpnt\wv.sym mv %devdir%\bin\axpnt\wv.sym %relroot%\%1\axpnt\wd.sym
if exist %devdir%\bin\axpnt\wvw.sym mv %devdir%\bin\axpnt\wvw.sym %relroot%\%1\axpnt\wdw.sym

if exist %devdir%\bin\qnx\dv.sym mv %devdir%\bin\qnx\dv.sym %relroot%\%1\qnx\sym\wd.sym
ifarch copy %devdir%\bin\qnx\dv.qnx %relroot%\%1\qnx\binq\wd.

ifarch -a copy %devdir%\wv\dbg\*.dbg %relroot%\%1\binw
ifarch -a copy %devdir%\wv\ssl\*.prs %relroot%\%1\binw

ifarch copy %devdir%\wv\dbg\*.dbg %relroot%\%1\qnx\watcom\wd
ifarch copy %devdir%\wv\ssl\*.prs %relroot%\%1\qnx\watcom\wd
:nodbgcopy
