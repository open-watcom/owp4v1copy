@echo off
call bmode
call docd f77\cg86\rt\386
wmake /h /i
call docd ..\387
wmake /h /i
call docd ..\386s
wmake /h /i
call docd ..\387s
wmake /h /i

call docd ..\w386
wmake /h /i
call docd ..\w387
wmake /h /i
call docd ..\w386s
wmake /h /i
call docd ..\w387s
wmake /h /i

call docd ..\23r
wmake /h /i
call docd ..\23r7
wmake /h /i
call docd ..\23s
wmake /h /i
call docd ..\23s7
wmake /h /i

call docd ..\ntr
wmake /h /i
call docd ..\ntr7
wmake /h /i
call docd ..\nts
wmake /h /i
call docd ..\nts7
wmake /h /i

call docd ..\nec3r
wmake /h /i
call docd ..\nec3r7
wmake /h /i
call docd ..\nec3s
wmake /h /i
call docd ..\nec3s7
wmake /h /i

call docd ..\net386s
wmake /h /i
call docd ..\net387s
wmake /h /i
call docd ..\..\..

if [%1] == [] goto nocopy
ifarch copy %devdir%\f77\cg86\rt\386\flib.lib %relroot%\%1\lib386\dos\flib.lib
ifarch copy %devdir%\f77\cg86\rt\387\flib7.lib %relroot%\%1\lib386\dos\flib7.lib
ifarch copy %devdir%\f77\cg86\rt\386s\flibs.lib %relroot%\%1\lib386\dos\flibs.lib
ifarch copy %devdir%\f77\cg86\rt\387s\flib7s.lib %relroot%\%1\lib386\dos\flib7s.lib
ifarch copy %devdir%\f77\cg86\rt\w386\flib.lib %relroot%\%1\lib386\win\flib.lib
ifarch copy %devdir%\f77\cg86\rt\w387\flib7.lib %relroot%\%1\lib386\win\flib7.lib
ifarch copy %devdir%\f77\cg86\rt\w386s\flibs.lib %relroot%\%1\lib386\win\flibs.lib
ifarch copy %devdir%\f77\cg86\rt\w387s\flib7s.lib %relroot%\%1\lib386\win\flib7s.lib
ifarch copy %devdir%\f77\cg86\rt\23r\flib.lib %relroot%\%1\lib386\os2\flib.lib
ifarch copy %devdir%\f77\cg86\rt\23r7\flib7.lib %relroot%\%1\lib386\os2\flib7.lib
ifarch copy %devdir%\f77\cg86\rt\23s\flibs.lib %relroot%\%1\lib386\os2\flibs.lib
ifarch copy %devdir%\f77\cg86\rt\23s7\flib7s.lib %relroot%\%1\lib386\os2\flib7s.lib
ifarch copy %devdir%\f77\cg86\rt\ntr\flib.lib %relroot%\%1\lib386\nt\flib.lib
ifarch copy %devdir%\f77\cg86\rt\ntr7\flib7.lib %relroot%\%1\lib386\nt\flib7.lib
ifarch copy %devdir%\f77\cg86\rt\nts\flibs.lib %relroot%\%1\lib386\nt\flibs.lib
ifarch copy %devdir%\f77\cg86\rt\nts7\flib7s.lib %relroot%\%1\lib386\nt\flib7s.lib
ifarch copy %devdir%\f77\cg86\rt\nec3r\flib.lib %relroot%\%1\lib386\dos\n98\flib.lib
ifarch copy %devdir%\f77\cg86\rt\nec3r7\flib7.lib %relroot%\%1\lib386\dos\n98\flib7.lib
ifarch copy %devdir%\f77\cg86\rt\nec3s\flibs.lib %relroot%\%1\lib386\dos\n98\flibs.lib
ifarch copy %devdir%\f77\cg86\rt\nec3s7\flib7s.lib %relroot%\%1\lib386\dos\n98\flib7s.lib
ifarch copy %devdir%\f77\cg86\rt\net386s\flibs.lib %relroot%\%1\lib386\netware\flibs.lib
ifarch copy %devdir%\f77\cg86\rt\net387s\flib7s.lib %relroot%\%1\lib386\netware\flib7s.lib
ifarch copy %devdir%\f77\cg86\rt\errmsg.res %relroot%\%1\wf77.res
:nocopy
cd ..
