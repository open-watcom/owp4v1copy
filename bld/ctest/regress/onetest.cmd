title C Test Stream
setlocal
set plustest_name=result
wmake -h del_log
if exist *.sav del *.sav
rem
rem figure out if any extra architectures have to be tested
if not [%OS%] == [Windows_NT] goto not_nt
if [%PROCESSOR_ARCHITECTURE%] == [x86] goto add_i86
goto extra_done
:not_nt
rem assume we want to add i86 if not Windows_NT
:add_i86
set extra_arch=i86
:extra_done
if exist local.cmd call local.cmd
cd positive
call onetest.cmd
cd ..
if exist alt_diag.cmd goto alt_diag
cd diagnose
call onetest.cmd
cd ..
goto did_diag
:alt_diag
call alt_diag.cmd
:did_diag
wmake -h clean
wmake -h
wmake -h clean
endlocal
