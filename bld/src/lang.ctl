# SRC Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    cdsay ../f77/samples
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/clib/startup/a/cstrt386.asm <relroot>/rel2/src/startup/386/cstrt386.asm
    <CPCMD> <DEVDIR>/clib/startup/a/cstrtw32.asm <relroot>/rel2/src/startup/386/cstrtw32.asm
    <CPCMD> <DEVDIR>/clib/startup/a/cstrtwnt.asm <relroot>/rel2/src/startup/386/cstrtwnt.asm
#    <CPCMD> <DEVDIR>/clib/startup/a/cstrtwwt.asm <relroot>/rel2/src/startup/386/cstrtwwt.asm
    <CPCMD> <DEVDIR>/clib/startup/a/cstrtx32.asm <relroot>/rel2/src/startup/386/cstrtx32.asm
    <CPCMD> <DEVDIR>/clib/startup/c/cmain386.c <relroot>/rel2/src/startup/386/cmain386.c
    <CPCMD> <DEVDIR>/clib/startup/c/wildargv.c <relroot>/rel2/src/startup/wildargv.c
    <CPCMD> <DEVDIR>/clib/startup/a/cstrt086.asm <relroot>/rel2/src/startup/dos/cstrt086.asm
    <CPCMD> <DEVDIR>/clib/startup/a/dos16m.asm <relroot>/rel2/src/startup/dos/dos16m.asm
    <CPCMD> <DEVDIR>/clib/startup/a/dstrt386.asm <relroot>/rel2/src/startup/386/dstrt386.asm
    <CPCMD> <DEVDIR>/clib/startup/c/cmain086.c <relroot>/rel2/src/startup/dos/cmain086.c
    <CPCMD> <DEVDIR>/clib/startup/c/cmain086.c <relroot>/rel2/src/startup/os2/cmain086.c
    <CPCMD> <DEVDIR>/clib/startup/c/hpgrw086.c <relroot>/rel2/src/startup/dos/hpgrw086.c
    <CPCMD> <DEVDIR>/clib/startup/c/dmaino16.c <relroot>/rel2/src/startup/os2/dmaino16.c
    <CPCMD> <DEVDIR>/clib/startup/c/libmno16.c <relroot>/rel2/src/startup/os2/libmno16.c
    <CPCMD> <DEVDIR>/clib/startup/c/maino16.c <relroot>/rel2/src/startup/os2/maino16.c
    <CPCMD> <DEVDIR>/clib/startup/c/main2o32.c <relroot>/rel2/src/startup/386/main2o32.c
    <CPCMD> <DEVDIR>/clib/startup/c/libmno32.c <relroot>/rel2/src/startup/386/libmno32.c
    <CPCMD> <DEVDIR>/clib/startup/c/dmaino32.c <relroot>/rel2/src/startup/386/dmaino32.c
    <CPCMD> <DEVDIR>/clib/startup/c/main2wnt.c <relroot>/rel2/src/startup/386/main2wnt.c
    <CPCMD> <DEVDIR>/clib/startup/c/wmainwnt.c <relroot>/rel2/src/startup/386/wmainwnt.c
    <CPCMD> <DEVDIR>/clib/startup/c/lmainwnt.c <relroot>/rel2/src/startup/386/lmainwnt.c
    <CPCMD> <DEVDIR>/clib/startup/c/dmainwnt.c <relroot>/rel2/src/startup/386/dmainwnt.c
    <CPCMD> <DEVDIR>/clib/startup/c/lmn2wnt.c <relroot>/rel2/src/startup/386/lmn2wnt.c
    <CPCMD> <DEVDIR>/clib/startup/a/cstrto16.asm <relroot>/rel2/src/startup/os2/cstrto16.asm
    <CPCMD> <DEVDIR>/clib/startup/a/cstrto32.asm <relroot>/rel2/src/startup/386/cstrto32.asm
    <CPCMD> <DEVDIR>/clib/startup/a/cstrtw16.asm <relroot>/rel2/src/startup/win/cstrtw16.asm
    <CPCMD> <DEVDIR>/clib/startup/a/libentry.asm <relroot>/rel2/src/startup/win/libentry.asm
    <CPCMD> <DEVDIR>/clib/startup/c/8087cw.c <relroot>/rel2/src/startup/8087cw.c
    <CPCMD> <DEVDIR>/lib_misc/h/exitwmsg.h <relroot>/rel2/src/startup/os2/exitwmsg.h
    <CPCMD> <DEVDIR>/clib/startup/h/initfini.h <relroot>/rel2/src/startup/386/initfini.h
    <CPCMD> <DEVDIR>/clib/startup/h/initfini.h <relroot>/rel2/src/startup/os2/initfini.h
    <CPCMD> <DEVDIR>/watcom/h/wos2.h <relroot>/rel2/src/startup/os2/wos2.h
    <CPCMD> <DEVDIR>/watcom/h/mdef.inc <relroot>/rel2/src/startup/mdef.inc

    <CPCMD> <DEVDIR>/src/misc/*.* <relroot>/rel2/src/
    <CPCMD> <DEVDIR>/src/os2/pdd/*.*    <relroot>/rel2/src/os2/pdd/
    <CPCMD> <DEVDIR>/f77/src/*.* <relroot>/rel2/src/fortran/
    <CPCMD> <DEVDIR>/f77/src/win/*.* <relroot>/rel2/src/fortran/win/
    <CPCMD> <DEVDIR>/f77/src/dos/*.* <relroot>/rel2/src/fortran/dos/
    <CPCMD> <DEVDIR>/f77/src/os2/*.* <relroot>/rel2/src/fortran/os2/
    <CPCMD> <DEVDIR>/f77/samples/os2/*.* <relroot>/rel2/samples/fortran/os2/
    <CPCMD> <DEVDIR>/f77/samples/os2/rexx/*.* <relroot>/rel2/samples/fortran/os2/rexx/
    <CPCMD> <DEVDIR>/f77/samples/mthread/mthrdos2.for <relroot>/rel2/samples/fortran/os2/mthread.for
    <CPCMD> <DEVDIR>/f77/samples/mthread/mthrdnt.for <relroot>/rel2/samples/fortran/win32/mthread.for
    <CPCMD> <DEVDIR>/f77/samples/win/*.* <relroot>/rel2/samples/fortran/win/
    <CPCMD> <DEVDIR>/f77/samples/windll/*.* <relroot>/rel2/samples/fortran/win/dll/
    <CPCMD> <DEVDIR>/f77/samples/dll/*.* <relroot>/rel2/samples/fortran/os2/dll/
    <CPCMD> <DEVDIR>/f77/samples/dll/*.* <relroot>/rel2/samples/fortran/win32/dll/

    <CPCMD> <DEVDIR>/plusplus/cpplib/contain/cpp/*.* <relroot>/rel2/src/cpplib/contain/

    <CPCMD> <DEVDIR>/src/clibexam/*.*   <relroot>/rel2/samples/clibexam/
    <CPCMD> <DEVDIR>/src/cplbexam/*.*   <relroot>/rel2/samples/cplbexam/

#    <CPCMD> <DEVDIR>/src/cppexamp/*.*  <relroot>/rel2/samples/cppexamp/
#    <CPCMD> <DEVDIR>/src/cppexamp/excarea/*.*  <relroot>/rel2/samples/cppexamp/excarea/
#    <CPCMD> <DEVDIR>/src/cppexamp/membfun/*.*  <relroot>/rel2/samples/cppexamp/membfun/

    <CPCMD> <DEVDIR>/src/dll/*.*        <relroot>/rel2/samples/dll/
    <CPCMD> <DEVDIR>/src/goodies/*.*    <relroot>/rel2/samples/goodies/
    <CPCMD> <DEVDIR>/src/os2/*.*        <relroot>/rel2/samples/os2/
    <CPCMD> <DEVDIR>/src/os2/dll/*.*    <relroot>/rel2/samples/os2/dll/
    <CPCMD> <DEVDIR>/src/win/*.*        <relroot>/rel2/samples/win/
    <CPCMD> <DEVDIR>/src/clibexam/kanji/*.*     <relroot>/rel2/samples/clibexam/kanji/
    <CPCMD> <DEVDIR>/src/clibexam/test/*.*      <relroot>/rel2/samples/clibexam/test/
    <CPCMD> <DEVDIR>/src/cplbexam/complex/*.*   <relroot>/rel2/samples/cplbexam/complex/
    <CPCMD> <DEVDIR>/src/cplbexam/contain/*.*   <relroot>/rel2/samples/cplbexam/contain/
    <CPCMD> <DEVDIR>/src/cplbexam/fstream/*.*   <relroot>/rel2/samples/cplbexam/fstream/
    <CPCMD> <DEVDIR>/src/cplbexam/ios/*.*       <relroot>/rel2/samples/cplbexam/ios/
    <CPCMD> <DEVDIR>/src/cplbexam/iostream/*.*  <relroot>/rel2/samples/cplbexam/iostream/
    <CPCMD> <DEVDIR>/src/cplbexam/string/*.*    <relroot>/rel2/samples/cplbexam/string/
    <CPCMD> <DEVDIR>/src/cplbexam/strstrea/*.*  <relroot>/rel2/samples/cplbexam/strstrea/
    <CPCMD> <DEVDIR>/src/cplbexam/complex/friend/*.*    <relroot>/rel2/samples/cplbexam/complex/friend/
    <CPCMD> <DEVDIR>/src/cplbexam/complex/pubfun/*.*    <relroot>/rel2/samples/cplbexam/complex/pubfun/
    <CPCMD> <DEVDIR>/src/cplbexam/complex/relfun/*.*    <relroot>/rel2/samples/cplbexam/complex/relfun/
    <CPCMD> <DEVDIR>/src/cplbexam/complex/relop/*.*     <relroot>/rel2/samples/cplbexam/complex/relop/
    <CPCMD> <DEVDIR>/src/cplbexam/fstream/fstream/*.*   <relroot>/rel2/samples/cplbexam/fstream/fstream/
    <CPCMD> <DEVDIR>/src/cplbexam/fstream/ifstream/*.*  <relroot>/rel2/samples/cplbexam/fstream/ifstream/
    <CPCMD> <DEVDIR>/src/cplbexam/fstream/ofstream/*.*  <relroot>/rel2/samples/cplbexam/fstream/ofstream/
    <CPCMD> <DEVDIR>/src/cplbexam/iostream/iostream/*.* <relroot>/rel2/samples/cplbexam/iostream/iostream/
    <CPCMD> <DEVDIR>/src/cplbexam/iostream/istream/*.*  <relroot>/rel2/samples/cplbexam/iostream/istream/
    <CPCMD> <DEVDIR>/src/cplbexam/iostream/ostream/*.*  <relroot>/rel2/samples/cplbexam/iostream/ostream/
    <CPCMD> <DEVDIR>/src/cplbexam/string/friend/*.*     <relroot>/rel2/samples/cplbexam/string/friend/
    <CPCMD> <DEVDIR>/src/cplbexam/string/pubfun/*.*     <relroot>/rel2/samples/cplbexam/string/pubfun/
    <CPCMD> <DEVDIR>/src/cplbexam/strstrea/istrstre/*.* <relroot>/rel2/samples/cplbexam/strstrea/istrstre/
    <CPCMD> <DEVDIR>/src/cplbexam/strstrea/ostrstre/*.* <relroot>/rel2/samples/cplbexam/strstrea/ostrstre/
    <CPCMD> <DEVDIR>/src/cplbexam/strstrea/strstre/*.*  <relroot>/rel2/samples/cplbexam/strstrea/strstre/
    <CPCMD> <DEVDIR>/src/win/alarm/*.*  <relroot>/rel2/samples/win/alarm/
    <CPCMD> <DEVDIR>/src/win/datactl/*.*        <relroot>/rel2/samples/win/datactl/
    <CPCMD> <DEVDIR>/src/win/edit/*.*   <relroot>/rel2/samples/win/edit/
    <CPCMD> <DEVDIR>/src/win/generic/*.*        <relroot>/rel2/samples/win/generic/
    <CPCMD> <DEVDIR>/src/win/helpex/*.* <relroot>/rel2/samples/win/helpex/
    <CPCMD> <DEVDIR>/src/win/iconview/*.*       <relroot>/rel2/samples/win/iconview/
    <CPCMD> <DEVDIR>/src/win/life/*.*   <relroot>/rel2/samples/win/life/
    <CPCMD> <DEVDIR>/src/win/shootgal/*.*       <relroot>/rel2/samples/win/shootgal/
    <CPCMD> <DEVDIR>/src/win/watzee/*.* <relroot>/rel2/samples/win/watzee/
    <CPCMD> <DEVDIR>/src/win/alarm/win16/*.*    <relroot>/rel2/samples/win/alarm/win16/
    <CPCMD> <DEVDIR>/src/win/alarm/win32/*.*    <relroot>/rel2/samples/win/alarm/win32/
    <CPCMD> <DEVDIR>/src/win/alarm/win386/*.*   <relroot>/rel2/samples/win/alarm/win386/
#    <CPCMD> <DEVDIR>/src/win/alarm/winaxp/*.*  <relroot>/rel2/samples/win/alarm/winaxp/
    <CPCMD> <DEVDIR>/src/win/datactl/win16/*.*  <relroot>/rel2/samples/win/datactl/win16/
    <CPCMD> <DEVDIR>/src/win/datactl/win32/*.*  <relroot>/rel2/samples/win/datactl/win32/
    <CPCMD> <DEVDIR>/src/win/datactl/win386/*.* <relroot>/rel2/samples/win/datactl/win386/
#    <CPCMD> <DEVDIR>/src/win/datactl/winaxp/*.*        <relroot>/rel2/samples/win/datactl/winaxp/
    <CPCMD> <DEVDIR>/src/win/edit/win16/*.*     <relroot>/rel2/samples/win/edit/win16/
    <CPCMD> <DEVDIR>/src/win/edit/win32/*.*     <relroot>/rel2/samples/win/edit/win32/
    <CPCMD> <DEVDIR>/src/win/edit/win386/*.*    <relroot>/rel2/samples/win/edit/win386/
#    <CPCMD> <DEVDIR>/src/win/edit/winaxp/*.*   <relroot>/rel2/samples/win/edit/winaxp/
    <CPCMD> <DEVDIR>/src/win/generic/win16/*.*  <relroot>/rel2/samples/win/generic/win16/
    <CPCMD> <DEVDIR>/src/win/generic/win32/*.*  <relroot>/rel2/samples/win/generic/win32/
    <CPCMD> <DEVDIR>/src/win/generic/win386/*.* <relroot>/rel2/samples/win/generic/win386/
#    <CPCMD> <DEVDIR>/src/win/generic/winaxp/*.*        <relroot>/rel2/samples/win/generic/winaxp/
    <CPCMD> <DEVDIR>/src/win/helpex/win16/*.*   <relroot>/rel2/samples/win/helpex/win16/
    <CPCMD> <DEVDIR>/src/win/helpex/win32/*.*   <relroot>/rel2/samples/win/helpex/win32/
    <CPCMD> <DEVDIR>/src/win/helpex/win386/*.*  <relroot>/rel2/samples/win/helpex/win386/
#    <CPCMD> <DEVDIR>/src/win/helpex/winaxp/*.* <relroot>/rel2/samples/win/helpex/winaxp/
    <CPCMD> <DEVDIR>/src/win/iconview/win16/*.* <relroot>/rel2/samples/win/iconview/win16/
    <CPCMD> <DEVDIR>/src/win/iconview/win32/*.* <relroot>/rel2/samples/win/iconview/win32/
    <CPCMD> <DEVDIR>/src/win/iconview/win386/*.*        <relroot>/rel2/samples/win/iconview/win386/
#    <CPCMD> <DEVDIR>/src/win/iconview/winaxp/*.*       <relroot>/rel2/samples/win/iconview/winaxp/
    <CPCMD> <DEVDIR>/src/win/life/win16/*.*     <relroot>/rel2/samples/win/life/win16/
    <CPCMD> <DEVDIR>/src/win/life/win32/*.*     <relroot>/rel2/samples/win/life/win32/
    <CPCMD> <DEVDIR>/src/win/life/win386/*.*    <relroot>/rel2/samples/win/life/win386/
#    <CPCMD> <DEVDIR>/src/win/life/winaxp/*.*   <relroot>/rel2/samples/win/life/winaxp/
    <CPCMD> <DEVDIR>/src/win/shootgal/win16/*.* <relroot>/rel2/samples/win/shootgal/win16/
    <CPCMD> <DEVDIR>/src/win/shootgal/win32/*.* <relroot>/rel2/samples/win/shootgal/win32/
    <CPCMD> <DEVDIR>/src/win/shootgal/win386/*.*        <relroot>/rel2/samples/win/shootgal/win386/
#    <CPCMD> <DEVDIR>/src/win/shootgal/winaxp/*.*       <relroot>/rel2/samples/win/shootgal/winaxp/
    <CPCMD> <DEVDIR>/src/win/watzee/win16/*.*   <relroot>/rel2/samples/win/watzee/win16/
    <CPCMD> <DEVDIR>/src/win/watzee/win32/*.*   <relroot>/rel2/samples/win/watzee/win32/
    <CPCMD> <DEVDIR>/src/win/watzee/win386/*.*  <relroot>/rel2/samples/win/watzee/win386/
#    <CPCMD> <DEVDIR>/src/win/watzee/winaxp/*.* <relroot>/rel2/samples/win/watzee/winaxp/

    if exist <relroot>\rel2\src\__wprj__.del del <relroot>\rel2\src\__wprj__.del

[ BLOCK <1> clean ]
#==================
    sweep killobjs
