@echo %verbose% off
echo # ===========================
echo # Start Preprocessor Test
echo # ===========================

if .%2 == . goto usage

echo # ---------------------------
echo #   PreProcess Test 1
echo # ---------------------------

rm tmp.out
%1 -h -f prep01 > tmp.out 2>&1
diff -b prep01.cmp tmp.out
if errorlevel 1 goto err1
    @echo # prep01 successful
    goto test2
:err1
    @echo ## PREPROCESS ## >> %2
    @echo Error: PREPROCESS #1 unsuccessful!!! | tee -a %2

:test2

echo # ---------------------------
echo #   PreProcess Test 2
echo # ---------------------------

rm tmp.out
%1 -h -f prep02 -ms -m > tmp.out 2>&1
diff prep02.cmp tmp.out
if errorlevel 1 goto err2
    @echo # prep02 successful
    goto test3
:err2
    @echo ## PREPROCESS ## >> %2
    @echo Error: PREPROCESS #2 unsuccessful!!! | tee -a %2

:test3

goto done
:usage
echo usage: %0 prgname errorfile
:done
if exist tmp.out rm tmp.out
