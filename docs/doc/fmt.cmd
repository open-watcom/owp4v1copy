@echo off
set t1=%gml%
set t2=%gmllib%
set t3=%gmlpag%
set t4=%gmlinc%
set t5=%gmlopt%
if "[%gml%]"=="[]" set gml=%doc_root%\gml\os2\wgml.exe
if "[%gmllib%]"=="[]" set gmllib=%doc_root%\gml\syslib
if "[%gmlpag%]"=="[]" set gmlpag=%TMPDIR%
if "[%gmlinc%]"=="[]" set gmlinc=cg;fg;lg;qa;cmn;gml
if "[%1]" == "[c]" set gmlinc=cl;gml;whelp
if "[%1]" == "[C]" set gmlinc=cl;gml;whelp
if "[%1]" == "[devguide]" set gmlinc=devguide;cmn;gml;whelp
if "[%1]" == "[cguide]" set gmlinc=cg;lg;cmn;plusplus;gml;whelp
if "[%1]" == "[CGUIDE]" set gmlinc=cg;lg;cmn;plusplus;gml;whelp
if "[%1]" == "[cguideq]" set gmlinc=cg;lg;cmn;plusplus;gml;whelp
if "[%1]" == "[CGUIDEQ]" set gmlinc=cg;lg;cmn;plusplus;gml;whelp
if "[%1]" == "[fuguide]" set gmlinc=fg;lg;cmn;gml;whelp
if "[%1]" == "[FUGUIDE]" set gmlinc=fg;lg;cmn;gml;whelp
if "[%1]" == "[ctools]" set gmlinc=cmn;lg;cg;gml;whelp
if "[%1]" == "[CTOOLS]" set gmlinc=cmn;lg;cg;gml;whelp
if "[%1]" == "[ftools]" set gmlinc=cmn;lg;fg;gml;whelp
if "[%1]" == "[FTOOLS]" set gmlinc=cmn;lg;fg;gml;whelp
if "[%1]" == "[guitool]" set gmlinc=gui;gui\snaps;cg;gml;whelp
if "[%1]" == "[GUITOOL]" set gmlinc=gui;gui\snaps;cg;gml;whelp
if "[%1]" == "[pguide]" set gmlinc=pg;cmn;cg;rsi;win386;gml;lr\src;whelp
if "[%1]" == "[PGUIDE]" set gmlinc=pg;cmn;cg;rsi;win386;gml;lr\src;whelp
if "[%1]" == "[fpguide]" set gmlinc=pg;cmn;fg;rsi;win386;gml;lr\src;whelp
if "[%1]" == "[FPGUIDE]" set gmlinc=pg;cmn;fg;rsi;win386;gml;lr\src;whelp
if "[%1]" == "[lguide]" set gmlinc=lg;gml;whelp
if "[%1]" == "[LGUIDE]" set gmlinc=lg;gml;whelp
if "[%1]" == "[wdbg]" set gmlinc=wd;gml;whelp
if "[%1]" == "[WDBG]" set gmlinc=wd;gml;whelp
if "[%1]" == "[f77graph]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[F77GRAPH]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[cpplib]" set gmlinc=cpplib;gml;whelp
if "[%1]" == "[CPPLIB]" set gmlinc=cpplib;gml;whelp
if "[%1]" == "[clslib]" set gmlinc=cpplib;gml;whelp
if "[%1]" == "[CLSLIB]" set gmlinc=cpplib;gml;whelp
if "[%1]" == "[clibref]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[CLIBREF]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[clib_dos]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[CLIB_DOS]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[clib_pp]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[CLIB_PP]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[clib_fox]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[CLIB_FOX]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[clib_nec]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[CLIB_NEC]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[clib_qnx]" set gmlinc=lr;lr\qnx;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[CLIB_QNX]" set gmlinc=lr;lr\qnx;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[clib_tst]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[CLIB_TST]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%1]" == "[c_readme]" set gmlinc=gs;cg;gml;whelp
if "[%1]" == "[C_README]" set gmlinc=gs;cg;gml;whelp
if "[%1]" == "[f_readme]" set gmlinc=gs;gml;whelp
if "[%1]" == "[F_README]" set gmlinc=gs;gml;whelp
if not "[%gmlopt%]" == "[]" goto OPT1
if "[%1]" == "[c_readme]" set gmlopt=nn7x9ps.opt
if "[%1]" == "[C_README]" set gmlopt=nn7x9ps.opt
if "[%1]" == "[f_readme]" set gmlopt=nn7x9ps.opt
if "[%1]" == "[F_README]" set gmlopt=nn7x9ps.opt
:OPT1
if "[%1]" == "[wedit]" set gmlinc=wed;gml
if "[%1]" == "[WEDIT]" set gmlinc=wed;gml
if "[%1]" == "[weditdoc]" set gmlinc=wed;gml;wed\gml;wed\pics
if "[%1]" == "[WEDITDOC]" set gmlinc=wed;gml;wed\gml;wed\pics
if "[%1]" == "[wfguide]" set gmlinc=wfg;fg;wed;lg;gml
if "[%1]" == "[WFGUIDE]" set gmlinc=wfg;fg;wed;lg;gml
if "[%1]" == "[wfguide3]" set gmlinc=wfg;fg;wed;lg;gml;rsi
if "[%1]" == "[WFGUIDE3]" set gmlinc=wfg;fg;wed;lg;gml;rsi
if "[%1]" == "[f77lr]" set gmlinc=fl;gml;whelp
if "[%1]" == "[F77LR]" set gmlinc=fl;gml;whelp
if "[%1]" == "[f77lr]" set gmllib=e:\gml\syslib
if "[%1]" == "[F77LR]" set gmllib=e:\gml\syslib
if not "[%gmlopt%]" == "[]" goto OPT2
if "[%1]" == "[f77lr]" set gmlopt=nbpsshad
if "[%1]" == "[F77LR]" set gmlopt=nbpsshad
:OPT2
if "[%1]" == "[os2help]" set gmlinc=lr;lr\src;lr\gr;lr\ge;lr\gp;gml;cmn
if "[%2]" == "[os2help]" set gmlinc=\doc\os2help;%gmlinc%
if "[%gmlopt%]"=="[]" set gmlopt=nb7x9ps
if "[%2]" == "[6x8]" set gmlopt=nb6x8ps
if "[%2]" == "[8x11]" set gmlopt=nbps
if "[%2]" == "[help]" set gmlopt=nbhlp
if "[%2]" == "[os2help]" set gmlopt=nbhlpos2
if "[%gmlswt%]"=="[]" set gmlswt=%2 %3 %4 %5 %6
if "[%2]" == "[6x8]" set gmlswt=%3 %4 %5 %6
if "[%2]" == "[8x11]" set gmlswt=%3 %4 %5 %6
if "[%2]" == "[help]" set gmlswt=%3 %4 %5 %6
if "[%2]" == "[os2help]" set gmlswt=%3 %4 %5 %6
if "[%1]" == "[cpplib]" set gmlswt=%gmlswt% sets target DOS
if "[%1]" == "[CPPLIB]" set gmlswt=%gmlswt% sets target DOS
rem %gml% stat.%1 (file %gmlopt% index pass 1 cpi 10 stat verb %gmlswt%
rem %gml% %1 (file %gmlopt% index pass 1 cpi 10 stat verb output (t:640) %gmlswt%|tee %1.log
%gml% %1 (file %gmlopt% index pass 1 cpi 10 stat verb output (t:100) %gmlswt%|tee %1.log
if "[%t1%]"=="[]" set gml=
if "[%t2%]"=="[]" set gmllib=
if "[%t3%]"=="[]" set gmlpag=
if "[%t4%]"=="[]" set gmlinc=
if "[%t5%]"=="[]" set gmlopt=
set gmlswt=
set t1=
set t2=
set t3=
set t4=
set t5=
