@echo off
call bmode
call docd wsample
call mkall
if [%1] == [] goto nocopy
rem ifarch copy bin\wsampecs.exe %relroot%\%1\binw\wsampecs.exe
ifarch copy bin\wsample.exe %relroot%\%1\binw\wsample.exe
ifarch copy bin\wsamprsi.exe %relroot%\%1\binw\wsamprsi.exe
ifarch copy bin\wsamplew.exe %relroot%\%1\binw\wsamplew.exe
ifarch copy bin\wsampos2.exe %relroot%\%1\binp\wsampos2.exe
ifarch copy bin\wsmpos22.exe %relroot%\%1\binp\wsmpos22.exe
ifarch copy bin\wsamppls.exp %relroot%\%1\binw\wsamppls.exp
ifarch copy bin\wsmpnt.exe %relroot%\%1\binnt\wsample.exe
ifarch copy bin\wsample.nlm %relroot%\%1\nlm\wsample.nlm
ifarch copy bin\nsamprsi.exe %relroot%\%1\n98bw\wsamprsi.exe
ifarch copy bin\nsamppls.exp %relroot%\%1\n98bw\wsamppls.exp
:nocopy
cd ..
