# Build tools Prerequisite Tool Build Control File
# =========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <projdir>\dos386
    wmake -h
    <CPCMD> wsplice.exe <relroot>\bld\build\bin\wsplice.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <projdir>\os2386
    wmake -h
    <CPCMD> wsplice.exe <relroot>\bld\build\binp\wsplice.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <projdir>\nt386
    wmake -h
    <CPCMD> wsplice.exe <relroot>\bld\build\binnt\wsplice.exe
    cdsay <projdir>
