# SSL Prerequisite Tool Build Control File
# =========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\dos386
    wmake -h
    <CPCMD> ssl.exe <owroot>\bld\build\bin\ssl.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\os2386
    wmake -h
    <CPCMD> ssl.exe <owroot>\bld\build\binp\ssl.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\nt386
    wmake -h
    <CPCMD> ssl.exe <owroot>\bld\build\binnt\ssl.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    cdsay <PROJDIR>/linux386
    wmake -h
    <CPCMD> ssl.exe <owroot>/bld/build/binl/ssl
    cdsay <PROJDIR>
