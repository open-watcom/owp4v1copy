# WASM Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    mkdir -p <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../dos386/makefile name=bwasm
    <CPCMD> bwasm.exe <DEVDIR>/build/bin/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    mkdir -p <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../os2386/makefile name=bwasm
    <CPCMD> bwasm.exe <DEVDIR>/build/binp/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    mkdir -p <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../nt386/makefile name=bwasm
    <CPCMD> bwasm.exe <DEVDIR>/build/binnt/
    cdsay <PROJDIR>

[ BLOCK <WASMLINUXBUILD> bootstrap ]
#==================================
    echo Building the wasm bootstrap
    mkdir -p <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../linux386/makefile bootstrap=1 name=bwasm
    <CPCMD> bwasm <DEVDIR>/build/binl/bwasm
    cdsay <PROJDIR>

[ BLOCK <WASMLINUXBUILD> normal ]
#==================================
#    mkdir -p <PROJDIR>/<OBJDIR>
#    cdsay <PROJDIR>/<OBJDIR>
#    wmake -h -f ../linux386/makefile name=bwasm
#    <CPCMD> bwasm.elf <DEVDIR>/build/binl/bwasm
#    cdsay <PROJDIR>

[ BLOCK <1> clean ]
#==================
    rm -f <PROJDIR>/<OBJDIR>/*.*
    @rm -f <RELROOT>/bld/build/bin/bwasm.*
    @rm -f <RELROOT>/bld/build/binp/bwasm.*
    @rm -f <RELROOT>/bld/build/binnt/bwasm.*
    @rm -f <RELROOT>/bld/build/binl/bwasm.*
