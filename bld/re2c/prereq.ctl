# RE2C Prerequisite Tool Build Control File
# =========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <projdir>\dos386
    wmake -h
    <CPCMD> re2c.exe <relroot>\bld\build\bin\re2c.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <projdir>\os2386
    wmake -h
    <CPCMD> re2c.exe <relroot>\bld\build\binp\re2c.exe
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <projdir>\nt386
    wmake -h
    <CPCMD> re2c.exe <relroot>\bld\build\binnt\re2c.exe
    cdsay <projdir>
