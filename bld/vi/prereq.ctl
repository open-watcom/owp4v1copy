# POSIX Prerequisite Tool Build Control File
# =========================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\ctl\dos386
    wmake -h
    <CPCMD> parsectl.exe <owroot>\bld\build\bin\
    <CPCMD> parsedyn.exe <owroot>\bld\build\bin\
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\ctl\os2386
    wmake -h
    <CPCMD> parsectl.exe <owroot>\bld\build\binp\
    <CPCMD> parsedyn.exe <owroot>\bld\build\binp\
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\ctl\nt386
    wmake -h
    <CPCMD> parsectl.exe <owroot>\bld\build\binnt\
    <CPCMD> parsedyn.exe <owroot>\bld\build\binnt\
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <PROJDIR>/ctl/linux386
    wmake -h
    <CPCMD> parsectl.exe <owroot>/bld/build/binl/parsectl.
    <CPCMD> parsedyn.exe <owroot>/bld/build/binl/parsedyn.
    cdsay <PROJDIR>
