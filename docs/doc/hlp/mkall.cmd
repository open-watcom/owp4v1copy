@echo off

if _%1 == _ goto error
if %1 == win goto cont
if %1 == os2 goto cont
if %1 == ib  goto cont
if %1 == html  goto cont

:error
echo format: mkall [win/os2/ib/html] [clean]
goto done

:cont
if %1 == win set b1=rtf
if %1 == os2 set b1=ipf
if %1 == ib set b1=ib
if %1 == html set b1=htm
cd %1

if %1 == win goto WINBOOKS
if %1 == os2 goto OS2BOOKS
if %1 == ib goto IBBOOKS
if %1 == html goto HTMLBOOKS
:WINBOOKS
set books1=c_readme ide wbrw wprof editor
set books2=resedt resacc resdlg resimg resmnu resstr
set books3=zoom heapwalk spy ddespy drwatcom drnt
set books4=clr cguide pguide lguide wlnkerrs tools wd rescomp
set books5=clib cpplib wccerrs wpperrs wrcerrs cmix isvcpp isvfor
set books6=f_readme fuguide fpguide ftools f77graph f77lr wfcerrs fmix
goto GOTBOOKS
:OS2BOOKS
set books1=c_readme ide wbrw wprof
set books2=cguide pguide lguide
set books3=tools wd
set books4=clr rescomp
set books5=clib cpplib wccerrs wpperrs cmix isvcpp isvfor
set books6=f_readme fuguide fpguide ftools f77graph f77lr wfcerrs fmix
goto GOTBOOKS
:IBBOOKS
set books1=c_readme wprof
set books2=cguide pguide lguide
set books3=tools wd
set books4=clr rescomp
set books5=clib cpplib wccerrs wpperrs cmix isvcpp isvfor
set books6=f_readme fuguide fpguide ftools f77graph f77lr wfcerrs fmix
goto GOTBOOKS
:HTMLBOOKS
set books1=c_readme
set books2=cguideq
set books3=wd
set books4=clr
set books5=clibqnx cpplib wccerrs wpperrs
set books6=
:GOTBOOKS

echo @echo off > $$$tmp.cmd
echo echo building %%1 >> $$$tmp.cmd
if NOT _%2 == _clean goto noclean
echo wmake -h hbook=%%1 clean >> $$$tmp.cmd
echo wmake -h hbook=%%1 %%2   >> $$$tmp.cmd
echo wmake -h hbook=%%1 clean >> $$$tmp.cmd
:noclean
echo wmake -h hbook=%%1       >> $$$tmp.cmd

for %%i in (%books1%) do call $$$tmp.cmd %%i %%i.%b1%
for %%i in (%books2%) do call $$$tmp.cmd %%i %%i.%b1%
for %%i in (%books3%) do call $$$tmp.cmd %%i %%i.%b1%
for %%i in (%books4%) do call $$$tmp.cmd %%i %%i.%b1%
for %%i in (%books5%) do call $$$tmp.cmd %%i %%i.%b1%
for %%i in (%books6%) do call $$$tmp.cmd %%i %%i.%b1%

del $$$tmp.cmd
set books1=
set books2=
set books3=
set books4=
set books5=
set books6=
set b1=

:done
