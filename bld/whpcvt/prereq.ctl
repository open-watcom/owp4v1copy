# WHPCVT Prerequisite Tool Build Control File
# =========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\dos386
    wmake -h
    <CPCMD> whpcvt.exe <owroot>\bld\build\bin\whpcvt.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\os2386
    wmake -h
    <CPCMD> whpcvt.exe <owroot>\bld\build\binp\whpcvt.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\nt386
    wmake -h
    <CPCMD> whpcvt.exe <owroot>\bld\build\binnt\whpcvt.exe
    cdsay <PROJDIR>
