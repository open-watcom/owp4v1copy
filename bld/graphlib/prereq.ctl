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
    pmake -d fixtools <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    rm -f <OWBINDIR>/objchg
    rm -f <OWBINDIR>/objfind
    rm -f <OWBINDIR>/objlist
    rm -f <OWBINDIR>/objxdef
    rm -f <OWBINDIR>/objxref
    rm -f <OWBINDIR>/objchg.exe
    rm -f <OWBINDIR>/objfind.exe
    rm -f <OWBINDIR>/objlist.exe
    rm -f <OWBINDIR>/objxdef.exe
    rm -f <OWBINDIR>/objxref.exe
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>/fixtools/dos386
    wmake -h
    <CPCMD> *.exe <OWBINDIR>/

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>/fixtools/os2386
    wmake -h
    <CPCMD> *.exe <OWBINDIR>/

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>/fixtools/nt386
    wmake -h
    <CPCMD> *.exe <OWBINDIR>/

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    cdsay <PROJDIR>/fixtools/linux386
    wmake -h
    <CPCMD> objchg.exe <OWBINDIR>/objchg
    <CPCMD> objfind.exe <OWBINDIR>/objfind
    <CPCMD> objlist.exe <OWBINDIR>/objlist
    <CPCMD> objxdef.exe <OWBINDIR>/objxdef
    <CPCMD> objxref.exe <OWBINDIR>/objxref

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

cdsay <PROJDIR>
