@echo off
set GMLINC=.;%DOC_ROOT%\doc\whelp;%DOC_ROOT%\doc\gml
set GMLLIB=%DOC_ROOT%\gml\syslib
if not %build_platform% == dos386 goto next1
set wgml=%DOC_ROOT%\gml\dos\wgml
goto compile
:next1
if not %build_platform% == os2386 goto next2
set wgml=%DOC_ROOT%\gml\os2\wgml
goto compile
:next2
if not %build_platform% == nt386 goto error1
set wgml=%DOC_ROOT%\gml\dos\wgml
:compile
%wgml% cgdoc.gml (file cgdps
goto end

:error1
@echo !!!!! Unsupported building platform !!!!!!

:end
