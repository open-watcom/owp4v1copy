# WHC Prerequisite Tool Build Control File
# ========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <projdir>\dos386
    wmake -h
    <CPCMD> whc.exe <owroot>\bld\build\bin\whc.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <projdir>\os2386
    wmake -h
    <CPCMD> whc.exe <owroot>\bld\build\binp\whc.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <projdir>\nt386
    wmake -h
    <CPCMD> whc.exe <owroot>\bld\build\binnt\whc.exe
    cdsay <projdir>
