# SSL Prerequisite Tool Build Control File
# =========================================

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
    set BUILD_PLATFORM=<BUILD_PLATFORM>boot

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\dos386
    wmake -h
    <CPCMD> ssl.exe <DEVDIR>\build\bin\ssl.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\os2386
    wmake -h
    <CPCMD> ssl.exe <DEVDIR>\build\binp\ssl.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\nt386
    wmake -h
    <CPCMD> ssl.exe <DEVDIR>\build\binnt\ssl.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    cdsay <PROJDIR>\ntaxp
    wmake -h
    <CPCMD> ssl.exe <DEVDIR>\build\axpnt\ssl.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    cdsay <PROJDIR>/linux386
    wmake -h
    <CPCMD> ssl.exe <DEVDIR>/build/binl/ssl
    cdsay <PROJDIR>

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=
