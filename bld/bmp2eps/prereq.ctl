# bmp2eps Prerequisite Tool Build Control File
# ============================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\dos386
    wmake -h
    <CPCMD> bmp2eps.exe <OWROOT>\bld\build\bin\bmp2eps.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\os2386
    wmake -h
    <CPCMD> bmp2eps.exe <OWROOT>\bld\build\binp\bmp2eps.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\nt386
    wmake -h
    <CPCMD> bmp2eps.exe <OWROOT>\bld\build\binnt\bmp2eps.exe
    cdsay <PROJDIR>

