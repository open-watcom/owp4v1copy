# WASM Prerequisite Tool Build Control File
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <projdir>/dos386
    wmake -h
    <CPCMD> wasm.exe <relroot>/bld/build/bin/
    <CPCMD> wasm.sym <relroot>/bld/build/bin/
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <projdir>/os2386
    wmake -h
    <CPCMD> wasm.exe <relroot>/bld/build/binp/
    <CPCMD> wasm.sym <relroot>/bld/build/binp/
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <projdir>/nt386
    wmake -h
    <CPCMD> wasm.exe <relroot>/bld/build/binnt/
    <CPCMD> wasm.sym <relroot>/bld/build/binnt/
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <projdir>/linux386
    wmake -h
    <CPCMD> wasm.exe <relroot>/bld/build/binl/
    <CPCMD> wasm.sym <relroot>/bld/build/binl/
    cdsay <projdir>
