@echo off
call bmode
call docd f77\lg86\obj
wmakel /h /i
call docd ..\obj87
wmakel /h /i
call docd ..\nec
wmakel /h /i
call docd ..\nec87
wmakel /h /i
call docd ..\obj386
wmakel /h /i
call docd ..\obj387
wmakel /h /i
call docd ..\nec386
wmakel /h /i
call docd ..\nec387
wmakel /h /i
call docd ..\os2_386
wmakel /h /i
call docd ..\os2_387
wmakel /h /i
call docd ..\rt\286
wmakel /h /i
call docd ..\287
wmakel /h /i
call docd ..\386
wmakel /h /i
call docd ..\387
wmakel /h /i
call docd ..\os2_386
wmakel /h /i
call docd ..\os2_387
wmakel /h /i
call docd ..\..\..

wstrip -r %devdir%\bin\bin\watfor77.exe %devdir%\bin\watfor77.jxe
wstrip -r -a %devdir%\bin\bin\watfor77.jxe . %devdir%\f77\lg86\nec\errmsg.res
wstrip -r %devdir%\bin\bin\watfor87.exe %devdir%\bin\watfor87.jxe
wstrip -r -a %devdir%\bin\bin\watfor87.jxe . %devdir%\f77\lg86\nec\errmsg.res

if [%1] == [] goto nocopy
ifarch wstrip %devdir%\bin\bin\watfor77.exe             %relroot%\%1\binw\watfor77.exe
ifarch wstrip %devdir%\bin\bin\watfor87.exe             %relroot%\%1\binw\watfor87.exe
ifarch wstrip %devdir%\bin\bin\watfor77.jxe             %relroot%\%1\binw\japan\watfor77.exe
ifarch wstrip %devdir%\bin\bin\watfor87.jxe             %relroot%\%1\binw\japan\watfor87.exe
ifarch wstrip %devdir%\bin\bin.nec\watfor77.exe          %relroot%\%1\n98bw\watfor77.exe
ifarch wstrip %devdir%\bin\bin.nec\watfor87.exe          %relroot%\%1\n98bw\watfor87.exe
ifarch wstrip %devdir%\bin\bin\wf386.exe                %relroot%\%1\binw\wf386.exe
ifarch wstrip %devdir%\bin\bin\wf387.exe                %relroot%\%1\binw\wf387.exe
ifarch wstrip %devdir%\bin\bin.nec\wf386.exe             %relroot%\%1\n98bw\wf386.exe
ifarch wstrip %devdir%\bin\bin.nec\wf387.exe             %relroot%\%1\n98bw\wf387.exe
ifarch wstrip %devdir%\bin\binp\wf386.exe               %relroot%\%1\binp\wf386.exe
ifarch wstrip %devdir%\bin\binp\wf387.exe               %relroot%\%1\binp\wf387.exe
ifarch copy %devdir%\f77\lg86\rt\286\wflib.lib      %relroot%\%1\lib286\dos\wflib.lib
ifarch copy %devdir%\f77\lg86\rt\287\wflib7.lib     %relroot%\%1\lib286\dos\wflib7.lib
ifarch copy %devdir%\f77\lg86\rt\386\wflib.lib      %relroot%\%1\lib386\dos\wflib.lib
ifarch copy %devdir%\f77\lg86\rt\387\wflib7.lib     %relroot%\%1\lib386\dos\wflib7.lib
ifarch copy %devdir%\f77\lg86\rt\os2_386\wflib.lib  %relroot%\%1\lib386\os2\wflib.lib
ifarch copy %devdir%\f77\lg86\rt\os2_387\wflib7.lib %relroot%\%1\lib386\os2\wflib7.lib
ifarch copy %devdir%\f77\lg86\rt\errmsg.res         %relroot%\%1\watfor77.res
:nocopy
cd ..
