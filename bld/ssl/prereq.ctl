# SSL Prerequisite Tool Build Control File
# =========================================

[ BLOCK <1> dos386 ]
#================================
    cdsay <projdir>\dos386
    wmake -h
    <CPCMD> ssl.exe <relroot>\bld\build\bin\ssl.exe
    cdsay <projdir>

[ BLOCK <1> os2386 ]
#================================
    cdsay <projdir>\os2386
    wmake -h
    <CPCMD> ssl.exe <relroot>\bld\build\binp\ssl.exe
    cdsay <projdir>

[ BLOCK <1> nt386 ]
#===============================
    cdsay <projdir>\nt386
    wmake -h
    <CPCMD> ssl.exe <relroot>\bld\build\binnt\ssl.exe
    cdsay <projdir>
