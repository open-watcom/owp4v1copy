@echo %regress% off
SET TRMEM_CODE=1

if not exist ERROR.OUT  goto not_exist 
    del ERROR.OUT
:not_exist
if .%1 == . goto usage

REM ===========================
REM -- test1 - Multiple Dependents Test
REM ===========================
echo *****************************************************************
cd TEST1
:: %comspec% /c call  ... rather than call ... to isolate echo off changes
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..
    
:tst2
REM ===========================
REM -- TEST2 - Implicit Rules Test
REM ===========================
echo *****************************************************************
cd IMPLICIT
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..
    
:tst3
REM ===========================
REM -- FORTEST - FOR LOOP TEST
REM ===========================
echo *****************************************************************
cd FORTEST
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..

:tst4
REM ===========================
REM -- PRETEST - PRE COMPILER TEST 
REM ===========================
echo *****************************************************************
cd PRETEST
%comspec% /c call DOPRE.BAT %1 ..\ERROR.OUT
cd ..

:tst5
REM ===========================
REM -- UPDTEST - UPDATE TEST 
REM ===========================
echo *****************************************************************
cd UPDTEST
%comspec% /c call DOUPD.BAT %1 ..\ERROR.OUT
cd ..

:tst6
set TRMEM_CODE=3
REM ===========================
REM -- ERRTEST - ERROR TEST 
REM ===========================
echo *****************************************************************
cd ERRTEST
%comspec% /c call ERROR.BAT %1 ..\ERROR.OUT
cd ..

:tst7
set TRMEM_CODE=1
REM ===========================
REM -- INLINE TEST - 
REM ===========================
echo *****************************************************************
cd INLINE
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..

:tst8
REM ===========================
REM -- PREPROCESS TEST - 
REM ===========================
echo *****************************************************************
cd PREPROC
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..

:tst9
REM ===========================
REM -- MACROS TEST - 
REM ===========================
echo *****************************************************************
cd MACROS
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..

:tst10
REM ===========================
REM -- MISC TEST - 
REM ===========================
echo *****************************************************************
cd MISC
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..

:tst11
REM ===========================
REM -- LONG FILENAME TEST - 
REM ===========================
echo *****************************************************************
cd LONGFILE
%comspec% /c call TEST.BAT %1 ..\ERROR.OUT
cd ..


REM ===========================
REM -- End of Test 
REM ===========================
echo ************* DONE DONE DONE ********************

if exist ERROR.OUT echo !!!!!!!! ERRORS FOUND !!!!!!!!!!
if exist ERROR.OUT echo look at ERROR.OUT for listing 
goto done
:usage
echo %0 progname (e.g. %0 wmake)

:done
