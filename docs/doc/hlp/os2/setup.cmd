set GML_ROOT=%doc_root%\gml\os2
set GMLLIB=%doc_root%\gml\syslib
set WHELP=%doc_root%\doc\whelp
set TEMP2=%TEMP%
set HLPDIR=%devdir%\online\hlp
set WHPCVT=%doc_root%\cmds\whpcvto2
set HCDOS=%doc_root%\cmds\hcdos32
set WIN31HC=%doc_root%\helpc\hcp
set WAT31HC=%doc_root%\helpc\whc32
set WIN95HC=%doc_root%\cmds\hcrtf
rem set IPFCARTWORK=%doc_root%\doc\gui\snaps
set OS2_TK=dummy
if exist %HLPDIR%\os2 goto made1
md %HLPDIR%
:made1
if exist %HLPDIR%\os2 goto made2
md %HLPDIR%\os2
:made2
