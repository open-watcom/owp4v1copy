# YACC Prerequisite Tool Build Control File
# =========================================

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

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    cdsay <PROJDIR>
    <MAKE> -f gnumake
    <CPCMD> bootstrp/yacc <OWROOT>/bld/build/binl/wyacc
    cdsay <PROJDIR>

