# VI Prerequisite Build Control File
# ==================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
    set BUILD_PLATFORM=<BUILD_PLATFORM>boot

[ BLOCK <1> clean ]
#==================
    cdsay <PROJDIR>/fixtools
    pmake -d all -h clean
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=objchg
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=objfind
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=objlist
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=objxdef
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=objxref
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\fixtools\dos386
    wmake -h
    <CPCMD> *.exe <DEVDIR>\build\bin\

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\fixtools\os2386
    wmake -h
    <CPCMD> *.exe <DEVDIR>\build\binp\

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\fixtools\nt386
    wmake -h
    <CPCMD> *.exe <DEVDIR>\build\binnt\

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <PROJDIR>/fixtools/linux386
    wmake -h
    <CPCMD> objchg.exe <DEVDIR>/build/binl/objchg
    <CPCMD> objfind.exe <DEVDIR>/build/binl/objfind
    <CPCMD> objlist.exe <DEVDIR>/build/binl/objlist
    <CPCMD> objxdef.exe <DEVDIR>/build/binl/objxdef
    <CPCMD> objxref.exe <DEVDIR>/build/binl/objxref

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

cdsay <PROJDIR>
