rem This setup is meant to be run on the G2 test machine which runs OS/2 Warp
rem set GML_ROOT=d:\gml\os2
set GML_ROOT=
rem set GMLLIB=d:\gml\syslib
set GMLLIB=
set WHELP=s:\jack\doc\whelp
set TEMP2=C:\TEMP
set OS2_TK=c:\wc\binp
set IPFC=s:\jack\TOOLKT2X\IPFC
for %%i in (*.whp) do wtouch %%i
for %%i in (*.ipf) do wtouch %%i
set HLPDIR=s:\jack\Hlp
if exist s:\jack\doc\os2 goto made
md s:\jack\hlp
md s:\jack\hlp\os2
:made
