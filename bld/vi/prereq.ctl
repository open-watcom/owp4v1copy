# VI Prerequisite Build Control File
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> clean ]
#==================
set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>
set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\ctl\dos386
    wmake -h
    <CPCMD> parsectl.exe <owroot>\bld\build\bin\
    <CPCMD> parsedyn.exe <owroot>\bld\build\bin\
    cdsay <PROJDIR>
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../dos386/makefile bname=bvi
    <CPCMD> bvi.exe <DEVDIR>/build/bin/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\ctl\os2386
    wmake -h
    <CPCMD> parsectl.exe <owroot>\bld\build\binp\
    <CPCMD> parsedyn.exe <owroot>\bld\build\binp\
    cdsay <PROJDIR>
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../os2386/makefile bname=bvi
    <CPCMD> bvi.exe <DEVDIR>/build/binp/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\ctl\nt386
    wmake -h
    <CPCMD> parsectl.exe <owroot>\bld\build\binnt\
    <CPCMD> parsedyn.exe <owroot>\bld\build\binnt\
    cdsay <PROJDIR>
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../nt386/makefile bname=bvi
    <CPCMD> bvi.exe <DEVDIR>/build/binnt/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <PROJDIR>/ctl/linux386
    wmake -h
    <CPCMD> parsectl.exe <owroot>/bld/build/binl/parsectl.
    <CPCMD> parsedyn.exe <owroot>/bld/build/binl/parsedyn.
    cdsay <PROJDIR>
    mkdir -p <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../linux386/makefile bname=bvi
    <CPCMD> bvi.exe <DEVDIR>/build/binl/bvi.
    cdsay <PROJDIR>

[ BLOCK <1> clean ]
#==================
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=
    rm -f -r <PROJDIR>/<OBJDIR>
    @rm -f <RELROOT>/bld/build/bin/bvi.*
    @rm -f <RELROOT>/bld/build/binp/bvi.*
    @rm -f <RELROOT>/bld/build/binnt/bvi.*
    @rm -f <RELROOT>/bld/build/binl/bvi.*
