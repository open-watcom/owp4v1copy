@echo off
call bmode
call docd f77\cg86\rt\bcbd
wmake /h /i
call docd ..\bcbd7
wmake /h /i
call docd ..\bcsd
wmake /h /i
call docd ..\bcsd7
wmake /h /i

call docd ..\wbcbd
wmake /h /i
call docd ..\wbcbd7
wmake /h /i
call docd ..\wbcsd
wmake /h /i
call docd ..\wbcsd7
wmake /h /i

call docd ..\nbcbd
wmake /h /i
call docd ..\nbcbd7
wmake /h /i

call docd ..\2bcbd
wmake /h /i
call docd ..\2bcbd7
wmake /h /i
call docd ..\2bcsd
wmake /h /i
call docd ..\2bcsd7
wmake /h /i
call docd ..\..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\f77\cg86\rt\bcbd\flibl.lib %relroot%\%1\lib286\dos\flibl.lib
ifarch copy %devdir%\f77\cg86\rt\bcbd7\flib7l.lib %relroot%\%1\lib286\dos\flib7l.lib
ifarch copy %devdir%\f77\cg86\rt\bcsd\flibm.lib %relroot%\%1\lib286\dos\flibm.lib
ifarch copy %devdir%\f77\cg86\rt\bcsd7\flib7m.lib %relroot%\%1\lib286\dos\flib7m.lib
ifarch copy %devdir%\f77\cg86\rt\wbcbd\flibl.lib %relroot%\%1\lib286\win\flibl.lib
ifarch copy %devdir%\f77\cg86\rt\wbcbd7\flib7l.lib %relroot%\%1\lib286\win\flib7l.lib
ifarch copy %devdir%\f77\cg86\rt\wbcsd\flibm.lib %relroot%\%1\lib286\win\flibm.lib
ifarch copy %devdir%\f77\cg86\rt\wbcsd7\flib7m.lib %relroot%\%1\lib286\win\flib7m.lib
ifarch copy %devdir%\f77\cg86\rt\nbcbd\flibl.lib %relroot%\%1\lib286\dos\n98\flibl.lib
ifarch copy %devdir%\f77\cg86\rt\nbcbd7\flib7l.lib %relroot%\%1\lib286\dos\n98\flib7l.lib
ifarch copy %devdir%\f77\cg86\rt\2bcbd\flibl.lib %relroot%\%1\lib286\os2\flibl.lib
ifarch copy %devdir%\f77\cg86\rt\2bcbd7\flib7l.lib %relroot%\%1\lib286\os2\flib7l.lib
ifarch copy %devdir%\f77\cg86\rt\2bcsd\flibm.lib %relroot%\%1\lib286\os2\flibm.lib
ifarch copy %devdir%\f77\cg86\rt\2bcsd7\flib7m.lib %relroot%\%1\lib286\os2\flib7m.lib
ifarch copy %devdir%\f77\cg86\rt\errmsg.res %relroot%\%1\wf77.res
:nocopy
cd ..
