# WASM Prerequisite Tool Build Control File
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>/dos386
    wmake -h
    <CPCMD> wasm.exe <RELROOT>/bld/build/bin/
    <CPCMD> wasm.sym <RELROOT>/bld/build/bin/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>/os2386
    wmake -h
    <CPCMD> wasm.exe <RELROOT>/bld/build/binp/
    <CPCMD> wasm.sym <RELROOT>/bld/build/binp/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>/nt386
    wmake -h
    <CPCMD> wasm.exe <RELROOT>/bld/build/binnt/
    <CPCMD> wasm.sym <RELROOT>/bld/build/binnt/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <PROJDIR>/linux386
    wmake -h
    <CPCMD> wasm.exe <RELROOT>/bld/build/binl/wasm
    <CPCMD> wasm.sym <RELROOT>/bld/build/binl/wasm.sym
    cdsay <PROJDIR>
