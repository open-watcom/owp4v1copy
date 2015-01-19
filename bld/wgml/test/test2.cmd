@echo off
REM *****************************************************************
REM test.cmd - process the specified GML file with wgml4.0 and new WGML
REM
REM adapted from %owroot%\build.cmd for determining operating system
REM
REM combined OS/2, eCS and Windows version
REM
REM Needed argument:
REM     1   name of gml file without extension (.gml always used)
REM
REM Example call             device PS is always used
REM         test ttag
REM
REM will create logfile ttag.old and output file ttag.ops with wgml 4
REM         and logfile ttag.new and output file ttag.nps with WGML new
REM
REM *****************************************************************

setlocal
if [%OWROOT%] == [] goto NO_OWROOT
if [%1] == [] goto parmmiss

rem del previous test rseults for same file
rm -fs %1.ops %1.nps %1.ntr %1.old %1.new
rm -fs %1_dos.pdf %1_new.pdf

rem set the wgml cmd line parameters
rem these are the options used to build devguide.gml
rem set options=file nb7x9ps lay to85x11pass 2 cpinch 10 verbose warn stat incl index setsymbol dotarget ps setsymbol book_ver 1.9 

rem these are comon simple parameters to build the local gml test files
rem set options=set date 'April 28, 2009' set time '21:11:02' wscript dev ps pass 1 verbose warn stat incl
rem set options=wscript device ps pass 1 verbose warn stat incl

rem as the dos wgml won't parse set or setsymbol using the above single cmd line
rem use the next line instead to use cmd line parameters defined in the external file wgmlopts.opt
set options=file wgmlopts.opt


set ow=%owroot%
set gmllib=%ow%\docs\gml\syslib;
rem set gmlinc=%ow%\docs\doc\devguide;%ow%\docs\doc\gml;%ow%\docs\doc\whelp;%ow%\docs\doc\hlp
rem I ignore the files in the doc\hlp directory, appear to be test modifications of those in doc\gml
rem devguide dir is included so as to build the devguide document to show how much the new wgml already does :)
set gmlinc=%ow%\docs\doc\cmn;%ow%\docs\doc\gml;%ow%\docs\doc\whelp;%ow%\docs\doc\devguide;

rem the following lines used by dosbox when running dos wgml.exe on vista and win7 platforms 
set ow=C:
set dosgmllib="set gmllib=%ow%\docs\gml\syslib"
set dosgmlinc="set gmlinc=%ow%\docs\doc\cmn;%ow%\docs\doc\gml;%ow%\docs\doc\whelp;%ow%\docs\doc\devguide;"

rem NT/XP? or OS/2 or eCS?
  if [%OS2_SHELL%] == [] goto notOS2
  if [%OS%] == [] goto is_OS2
  if [%OS%] == [ecs] goto is_OS2
:UNKNOWN
  @echo Operating System not recognized, sorry
  goto eof

:NOTOS2
  if [%Systemroot%] == [] goto unknown
  set wgml_dir=nt386

  if [%ProgramData%] == [] goto NT_X86
    @echo we have NT 64bit so must use DOSBOX to run the 16bit dos wgml.exe
    if [%OWDOSBOX%] == [] goto NO_DOSBOX
    %OWDOSBOX% -noautoexec -c "mount c %owroot%" -c "mount d ."  -c %dosgmllib% -c %dosgmlinc% -c "D:" -c "C:\docs\gml\dos\wgml.exe %1.gml ( %options% out %1.ops %2 %3 %4 %5 %6 %7 %8 %9>%1.old" -c "exit" -noconsole
    goto bld1

:NT_X86
  @echo we have a winOS which can still run the 16bit dos wgml.exe
  %owroot%\docs\gml\dos\wgml.exe %1.gml ( %options% out %1.ops %2 %3 %4 %5 %6 %7 %8 %9 >%1.old
  goto bld1

:IS_OS2
  set wgml_dir=os2386
  @echo run the os2 version of wgml.exe
  %owroot%\docs\gml\os2\wgml.exe %1.gml ( %options% out %1.ops %2 %3 %4 %5 %6 %7 %8 %9 >%1.old

:BLD1
  rem use Ghostscript to convert to pdf if desired
  if exist %1.ops cmd /c ps2pdf12 -r600 %1.ops %1_dos.pdf
  rem pause

  @echo now running the new wgml.exe	
  rem set new wgml flag for very very verbose output
  set research=-r
  %owroot%\bld\wgml\%wgml_dir%\wgml     %1.gml  %research% ( %options% out %1.nps %2 %3 %4 %5 %6 %7 %8 %9 >%1.new

  rem use Ghostscript to convert to pdf if desired	
  if exist %1.nps cmd /c ps2pdf12 -r600 %1.nps %1_new.pdf
  REM pause

  REM wdw %owroot%\bld\wgml\%wgml_dir%\wgml %1.gml  -r ( %options% out %1.nps %2 %3 %4 %5 %6 %7 %8 %9
goto eof

:NO_OWROOT
  @echo Cannot find build - did you run your setvars.bat?
  goto eof

:NO_DOSBOX
  @echo Cannot find dosbox.exe - check if defined in your setvars.bat?
  goto eof

:PARMMISS
  @echo Parameter missing
  @echo usage: test gmlfile

:EOF
  endlocal
