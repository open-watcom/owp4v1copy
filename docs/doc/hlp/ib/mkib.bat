@echo off

if _%1 == _ goto error
if %1 == win goto cont
if %1 == os2 goto cont
if %1 == ib  goto cont

:error
echo format: mkall [win/os2/ib] [clean]
goto done

:cont
if %1 == win set b1=rtf
if %1 == os2 set b1=ipf
if %1 == ib set b1=ib

set books1=ide editor
set books2=resedt resacc resdlg resimg resmnu resstr
set books3=zoom heapwalk spy ddespy drwatcom drnt
set books4=wd wprof tools lguide cguide pguide
set books5=rescomp clib cpplib wccerrs

echo @echo off > tmp.bat
echo echo building %%1 >> tmp.bat
if NOT _%2 == _clean goto noclean
echo wmake -h hbook=%%1 clean >> tmp.bat
echo wmake -h hbook=%%1 %%2   >> tmp.bat
echo wmake -h hbook=%%1 clean >> tmp.bat
:noclean
echo wmake -h hbook=%%1       >> tmp.bat

rem for %%i in (%books1%) do call tmp.bat %%i %%i.%b1%
rem for %%i in (%books2%) do call tmp.bat %%i %%i.%b1%
rem for %%i in (%books3%) do call tmp.bat %%i %%i.%b1%
for %%i in (%books4%) do call tmp.bat %%i %%i.%b1%
for %%i in (%books5%) do call tmp.bat %%i %%i.%b1%

del tmp.bat
set books1=
set books2=
set books3=
set books4=
set books5=
set b1=

:done
