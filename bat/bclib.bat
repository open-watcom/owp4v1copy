@echo off
call bmode
cd clib
call docd .
if [%2] == [] goto ordinary_make
if [%2] == [nomake] goto just_copy

        pmake.exe -d %2 %3 %4 %5 %6 %7 %8 %9 -h
        goto done_make

:ordinary_make
        pmake.exe -d all -h

:done_make
cd %devdir%\clib

:just_copy
if [%1] == [] goto nolibcopy
if not exist %relroot%\%1\lib386 goto nolibcopy
ifarch copy library\msdos.086\mc\clibc.lib %relroot%\%1\lib286\dos\clibc.lib
ifarch copy library\msdos.086\mh\clibh.lib %relroot%\%1\lib286\dos\clibh.lib
ifarch copy library\msdos.086\ml\clibl.lib %relroot%\%1\lib286\dos\clibl.lib
ifarch copy startup\library\msdos.086\ml\dos16m.obj %relroot%\%1\lib286\dos\dos16m.obj
ifarch copy library\msdos.086\ml_ov\clibl.lib %relroot%\%1\lib286\dos\clibol.lib
ifarch copy library\msdos.086\mm\clibm.lib %relroot%\%1\lib286\dos\clibm.lib
ifarch copy library\msdos.086\mm_ov\clibm.lib %relroot%\%1\lib286\dos\clibom.lib
ifarch copy library\msdos.086\ms\clibs.lib %relroot%\%1\lib286\dos\clibs.lib
ifarch copy startup\library\msdos.086\ms\cstrtiny.obj %relroot%\%1\lib286\dos\cstart_t.obj
ifarch copy library\msdos.286\mc\clibc.lib %relroot%\%1\lib286\os2\dospmc.lib
ifarch copy library\msdos.286\mh\clibh.lib %relroot%\%1\lib286\os2\dospmh.lib
ifarch copy library\msdos.286\ml\clibl.lib %relroot%\%1\lib286\os2\dospml.lib
ifarch copy library\msdos.286\mm\clibm.lib %relroot%\%1\lib286\os2\dospmm.lib
ifarch copy library\msdos.286\ms\clibs.lib %relroot%\%1\lib286\os2\dospms.lib
ifarch copy library\msdos.386\ms_r\clibs.lib %relroot%\%1\lib386\dos\clib3r.lib
ifarch copy library\msdos.386\ms_s\clibs.lib %relroot%\%1\lib386\dos\clib3s.lib
ifarch copy startup\library\msdos.386\ms_r\cstrtx32.obj %relroot%\%1\lib386\dos\cstrtx3r.obj
ifarch copy startup\library\msdos.386\ms_s\cstrtx32.obj %relroot%\%1\lib386\dos\cstrtx3s.obj
ifarch copy startup\library\msdos.386\ms_s\adsstart.obj %relroot%\%1\lib386\dos\adsstart.obj
ifarch copy startup\library\msdos.386\ms_s\adiestrt.obj %relroot%\%1\lib386\dos\adiestrt.obj
ifarch copy startup\library\msdos.386\ms_s\adifstrt.obj %relroot%\%1\lib386\dos\adifstrt.obj
ifarch copy library\netware.386\ms_s\clibs.lib %relroot%\%1\lib386\netware\clib3s.lib
ifarch copy library\os2.286\mc\clibc.lib %relroot%\%1\lib286\os2\clibc.lib
ifarch copy library\os2.286\mh\clibh.lib %relroot%\%1\lib286\os2\clibh.lib
ifarch copy library\os2.286\ml\clibl.lib %relroot%\%1\lib286\os2\clibl.lib
ifarch copy library\os2.286\ml_dll\clibl.lib %relroot%\%1\lib286\os2\clibdll.lib
ifarch copy library\os2.286\ml_mt\clibl.lib %relroot%\%1\lib286\os2\clibmtl.lib
ifarch copy library\os2.286\mm\clibm.lib %relroot%\%1\lib286\os2\clibm.lib
ifarch copy library\os2.286\ms\clibs.lib %relroot%\%1\lib286\os2\clibs.lib
ifarch copy library\os2.386\ms_r\clibs.lib %relroot%\%1\lib386\os2\clib3r.lib
ifarch copy library\os2.386\ms_s\clibs.lib %relroot%\%1\lib386\os2\clib3s.lib
ifarch copy library\osi.386\ms_r\clibs.lib %relroot%\%1\lib386\osi\clib3r.lib
rem
rem penpoint is dead!!!
rem
rem ifarch copy library\penpoint.386\mf_s\clibf.lib %relroot%\%1\gomtlib\clib3s.prd
rem ifarch copy library\penpoint.386\mf_sdb\clibf.lib %relroot%\%1\gomtlib\clib3s.dbg
ifarch copy library\qnx.286\mc\clibc.lib %relroot%\%1\lib286\qnx\clibc.lib
ifarch copy library\qnx.286\mh\clibh.lib %relroot%\%1\lib286\qnx\clibh.lib
ifarch copy library\qnx.286\ml\clibl.lib %relroot%\%1\lib286\qnx\clibl.lib
ifarch copy library\qnx.286\ml_sh\clibl.lib %relroot%\%1\lib286\qnx\clibshar.lib
ifarch copy library\qnx.286\mm\clibm.lib %relroot%\%1\lib286\qnx\clibm.lib
ifarch copy library\qnx.286\ms\clibs.lib %relroot%\%1\lib286\qnx\clibs.lib
ifarch copy library\qnx.386\ms_r\clibs.lib %relroot%\%1\lib386\qnx\clib3r.lib
ifarch copy library\qnx.386\ms_s\clibs.lib %relroot%\%1\lib386\qnx\clib3s.lib
ifarch copy library\windows.086\mc\clibc.lib %relroot%\%1\lib286\win\clibc.lib
ifarch copy library\windows.086\ml\clibl.lib %relroot%\%1\lib286\win\clibl.lib
ifarch copy startup\library\windows.086\ml\libentry.obj %relroot%\%1\lib286\win\libentry.obj
ifarch copy library\windows.086\mm\clibm.lib %relroot%\%1\lib286\win\clibm.lib
ifarch copy library\windows.086\ms\clibs.lib %relroot%\%1\lib286\win\clibs.lib
ifarch copy library\windows.386\mf_r\clibf.lib %relroot%\%1\lib386\win\clib3r.lib
ifarch copy library\windows.386\mf_s\clibf.lib %relroot%\%1\lib386\win\clib3s.lib
ifarch copy library\winnt.386\ms_r\clibs.lib %relroot%\%1\lib386\nt\clib3r.lib
ifarch copy library\winnt.386\ms_s\clibs.lib %relroot%\%1\lib386\nt\clib3s.lib
:nolibcopy
cd %devdir%
