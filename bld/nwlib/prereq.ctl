# WLIB Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#==================================
    set BUILD_PLATFORM=<BUILD_PLATFORM>boot

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<PREOBJDIR>
    rm -f -r <PROJDIR>/<PREOBJDIR>
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=bwlib
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../dos386/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/bin/bwlib.exe

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../os2386/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/binp/bwlib.exe

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../nt386/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/binnt/bwlib.exe

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../ntaxp/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/axpnt/bwlib.exe

[ BLOCK <BUILD_PLATFORM> linux386boot ]
#==========================================
    echo Building the wlib bootstrap
    mkdir <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../linux386/makefile bootstrap=1
    <CPCMD> wlib <DEVDIR>/build/binl/bwlib

[ BLOCK <BUILD_PLATFORM> linux386 ]
#======================================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../linux386/makefile prebuild=1
    <CPCMD> wlib.exe <DEVDIR>/build/binl/bwlib

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

cdsay <PROJDIR>
