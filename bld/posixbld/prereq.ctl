# POSIX Prerequisite Tool Build Control File
# =========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <projdir>\dos386
    wmake -h
    <CPCMD> *.exe <relroot>\bld\build\bin\
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <projdir>\os2386
    wmake -h
    <CPCMD> *.exe <relroot>\bld\build\binp\
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <projdir>\nt386
    wmake -h
    <CPCMD> *.exe <relroot>\bld\build\binnt\
    cdsay <projdir>
