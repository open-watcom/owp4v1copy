# WHC Prerequisite Tool Build Control File
# ========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\dos386
    wmake -h
    <CPCMD> whc.exe <OWROOT>\bld\build\bin\whc.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\os2386
    wmake -h
    <CPCMD> whc.exe <OWROOT>\bld\build\binp\whc.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\nt386
    wmake -h
    <CPCMD> whc.exe <OWROOT>\bld\build\binnt\whc.exe
    cdsay <PROJDIR>
