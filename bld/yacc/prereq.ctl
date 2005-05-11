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
    set TARGDIR=bin

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    set TARGDIR=binl
    set EXENAME=wyacc
    set SYMNAME=wyacc.sym

[ BLOCK <BUILD_PLATFORM> nt386 ]
#================================
    set TARGDIR=binnt

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    set TARGDIR=binp

[ BLOCK <BUILD_PLATFORM> dos386 linux386 nt386 os2386 ]
#======================================================
    cdsay <PROJDIR>/<BUILD_PLATFORM>
    wmake -h
    <CPCMD> yacc.exe <OWROOT>/bld/build/<TARGDIR>/<EXENAME>
    <CPCMD> yacc.sym <OWROOT>/bld/build/<TARGDIR>/<SYMNAME>
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386boot ]
#======================================
    cdsay <PROJDIR>
    <MAKE> -f gnumake
    <CPCMD> bootstrp/yacc <OWROOT>/bld/build/binl/wyacc
    cdsay <PROJDIR>

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=
