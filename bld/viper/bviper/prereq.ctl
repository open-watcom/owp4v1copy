# IDEB (aka BVIPER) Prerequisite Tool Build Control File
# =========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\dos386
    wmake -h
    <CPCMD> bviper.exe <owroot>\bld\build\bin\bviper.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\os2386
    wmake -h
    <CPCMD> bviper.exe <owroot>\bld\build\binp\bviper.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\nt386
    wmake -h
    <CPCMD> bviper.exe <owroot>\bld\build\binnt\bviper.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <PROJDIR>/linux386
    wmake -h
    <CPCMD> bviper.exe <owroot>/bld/build/binl/bviper.exe
    cdsay <PROJDIR>
