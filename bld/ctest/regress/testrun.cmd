setlocal
set ctest_name=result
wmake -h del_log
if exist *.sav del *.sav
cd positive
call testrun.cmd
cd ..
cd diagnose
call testrun.cmd
cd ..
endlocal
