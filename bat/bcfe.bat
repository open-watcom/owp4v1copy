@echo off
call bmode
call docd %DEVDIR%\cc\h
wmake
cd ..
pmake -b -d build -h
call tmp.bat
del tmp.bat
cd %DEVDIR%
if [%1] == [] goto nocopy
ifarch copy cc\osiaxp\wccaxpc.exe %relroot%\%1\binw\wccaxp.exe
if exist cc\osiaxp\wccaxpc.sym mv cc\osiaxp\wccaxpc.sym %relroot%\%1\binw\wccaxp.sym
ifarch copy cc\osi386\wcc386c.exe %relroot%\%1\binw\wcc386.exe
if exist cc\osi386\wcc386c.sym mv cc\osi386\wcc386c.sym %relroot%\%1\binw\wcc386.sym
ifarch copy cc\osii86\wcci86c.exe %relroot%\%1\binw\wcc.exe
if exist cc\osii86\wcci86c.sym mv cc\osii86\wcci86c.sym %relroot%\%1\binw\wcc.sym
ifarch copy cc\qnxi86\wcci86c.qnx %relroot%\%1\qnx\binq\wcc.
if exist cc\qnxi86\wcci86c.sym mv cc\qnxi86\wcci86c.sym %relroot%\%1\qnx\sym\wcc.sym
ifarch copy cc\qnx386\wcc386c.qnx %relroot%\%1\qnx\binq\wcc386.
if exist cc\qnx386\wcc386c.sym mv cc\qnx386\wcc386c.sym %relroot%\%1\qnx\sym\wcc386.sym
ifarch copy cc\ntaxp.axp\wccaxpc.exe %relroot%\%1\axpnt\wccaxp.exe
if exist cc\ntaxp.axp\wccaxpc.sym mv cc\ntaxp.axp\wccaxpc.sym %relroot%\%1\axpnt\wccaxpc.sym
:nocopy
