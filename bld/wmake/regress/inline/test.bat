@echo off
echo # ===========================
echo # Start INLINE File Test
echo # ===========================

if .%2 == . goto usage

echo # ---------------------------
echo #   Inline File Test 1
echo # ---------------------------

rm tmp.out
%1 -h -f INLINE01 > tmp.out 2>&1
diff INLINE01.CMP tmp.out
if errorlevel 1 goto err1
rm tmp.out
%1 -h -f INLN01B > tmp.out 2>&1
diff INLINE01.CMP tmp.out
if errorlevel 1 goto err1
    @echo # INLINE01 successful
    goto test2
:err1
    @echo ## INLINE ## >> %2
    @echo Error: INLINE #1 unsuccessful!!! | tee -a %2

:test2

echo # ---------------------------
echo #   Inline File Test 2
echo # ---------------------------

rm tmp.out
%1 -h -f INLINE02 > tmp.out 2>&1
diff INLINE02.CMP tmp.out
if errorlevel 1 goto err2
rm tmp.out
%1 -h -f INLN02B > tmp.out 2>&1
diff INLN02B.CMP tmp.out
if errorlevel 1 goto err2
    @echo # INLINE02 successful
    goto test3
:err2
    @echo ## INLINE ## >> %2
    @echo Error: INLINE #2 unsuccessful!!! | tee -a %2

:test3

echo # ---------------------------
echo #   Inline File Test 3
echo # ---------------------------
set TRMEM_CODE=3;
rm tmp.out
%1 -h -f INLINE03 > tmp.out 2>&1
diff INLINE03.CMP tmp.out
if errorlevel 1 goto err3
    @echo # INLINE03 successful
    goto test4
:err3
    @echo ## INLINE ## >> %2
    @echo Error: INLINE #3 unsuccessful!!! | tee -a %2

:test4
set TRMEM_CODE=1;
echo # ---------------------------
echo #   Inline File Test 4
echo # ---------------------------

rm tmp.out
%1 -h -f INLINE04 > tmp.out 2>&1
diff INLINE04.CMP tmp.out
if errorlevel 1 goto err4
    @echo # INLINE04 successful
    goto test5
:err4
    @echo ## INLINE ## >> %2
    @echo Error: INLINE #4 unsuccessful!!! | tee -a %2

:test5

echo # ---------------------------
echo #   Inline File Test 5
echo # ---------------------------

rm tmp.out
%1 -h -f INLINE05 > tmp.out 2>&1
diff INLINE05.CMP tmp.out
if errorlevel 1 goto err5
    @echo # INLINE05 successful
    goto test6
:err5
    @echo ## INLINE ## >> %2
    @echo Error: INLINE #5 unsuccessful!!! | tee -a %2

:test6

echo # ---------------------------
echo #   Inline File Test 6
echo # ---------------------------

rm tmp.out
%1 -h -f INLINE06 > tmp.out 2>&1
diff INLINE06.CMP tmp.out
if errorlevel 1 goto err6
    @echo # INLINE06 successful
    goto test7
:err6
    @echo ## INLINE ## >> %2
    @echo Error: INLINE #6 unsuccessful!!! | tee -a %2

:test7

echo # ---------------------------
echo #   Inline File Test 7
echo # ---------------------------

rm tmp.out
%1 -h -f INLINE07 > tmp.out 2>&1
diff INLINE07.CMP tmp.out
if errorlevel 1 goto err7
    @echo # INLINE07 successful
    goto test8
:err7
    @echo ## INLINE ## >> %2
    @echo Error: INLINE #7 unsuccessful!!! | tee -a %2

:test8
goto done
:usage
echo usage: %0 prgname errorfile
:done
rm tmp.out
