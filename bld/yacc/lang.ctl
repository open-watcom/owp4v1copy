# YACC Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <projdir>\dos386
    wmake -h
    <CPCMD> yacc.exe <relroot>\bld\build\bin\yacc.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <projdir>\os2386
    wmake -h
    <CPCMD> yacc.exe <relroot>\bld\build\binp\yacc.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <projdir>\nt386
    wmake -h
    <CPCMD> yacc.exe <relroot>\bld\build\binnt\yacc.exe
    cdsay <projdir>

[ BLOCK <1> clean ]
#==================
    pmake -d all -h clean
