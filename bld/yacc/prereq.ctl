# YACC Prerequisite Tool Build Control File
# =========================================

set PROJDIR=<CWD>

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
set BUILD_PLATFORM=<BUILD_PLATFORM>boot

[ BLOCK <1> clean ]
#==================
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\dos386
    wmake -h
    <CPCMD> yacc.exe <OWROOT>\bld\build\bin\yacc.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\os2386
    wmake -h
    <CPCMD> yacc.exe <OWROOT>\bld\build\binp\yacc.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\nt386
    wmake -h
    <CPCMD> yacc.exe <OWROOT>\bld\build\binnt\yacc.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    cdsay <PROJDIR>\ntaxp
    wmake -h
    <CPCMD> yacc.exe <OWROOT>\bld\build\axpnt\yacc.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386boot ]
#======================================
    cdsay <PROJDIR>
    <MAKE> -f gnumake
    <CPCMD> bootstrp/yacc <OWROOT>/bld/build/binl/wyacc
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    cdsay <PROJDIR>/linux386
    wmake -h
    <CPCMD> yacc.exe <OWROOT>/bld/build/binl/wyacc
    cdsay <PROJDIR>

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=
