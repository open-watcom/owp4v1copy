# wcl Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>
set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../dos386.386/makefile
    <CPCMD> wcl386.exe <DEVDIR>/build/bin/bwcl386.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../os2386.386/makefile
    <CPCMD> wcl386.exe <DEVDIR>/build/binp/bwcl386.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../nt386.386/makefile
    <CPCMD> wcl386.exe <DEVDIR>/build/binnt/bwcl386.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    echo Building the wasm bootstrap
    mkdir -p <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../linux386.386/makefile
    <CPCMD> wcl386.exe <DEVDIR>/build/binl/bwcl386
    cdsay <PROJDIR>

[ BLOCK <1> clean ]
#==================
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

    rm -f -r <PROJDIR>/<OBJDIR>
    @rm -f <RELROOT>/bld/build/bin/bwcl386.*
    @rm -f <RELROOT>/bld/build/binp/bwcl386.*
    @rm -f <RELROOT>/bld/build/binnt/bwcl386.*
    @rm -f <RELROOT>/bld/build/binl/bwcl386.*
