# WLIB Builder Control file
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
    mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../dos386/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/bin/bwlib.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../os2386/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/binp/bwlib.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../nt386/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/binnt/bwlib.exe
    cdsay <PROJDIR>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#==================================
    echo Building the wlib bootstrap
    mkdir <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../linux386/makefile bootstrap=1
    <CPCMD> wlib <DEVDIR>/build/binl/wlib
    cdsay <PROJDIR>

[ BLOCK <OWLINUXBUILD> normal ]
#==================================
#    mkdir <PROJDIR>/<OBJDIR>
#    cdsay <PROJDIR>/<OBJDIR>
#    wmake -h -f ../linux386/makefile prebuild=1
#    <CPCMD> wlib.elf <DEVDIR>/build/binl/bwlib
#    cdsay <PROJDIR>

[ BLOCK <1> clean ]
#==================
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

    rm -f -r <PROJDIR>/<OBJDIR>
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=bwlib
