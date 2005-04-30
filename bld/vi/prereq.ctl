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
    cdsay <PROJDIR>/ctl
    pmake -d all -h clean
    cdsay <PROJDIR>/ctl
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=parsectl
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=parsedyn
    cdsay <PROJDIR>/bind
    pmake -d all -h clean
    cdsay <PROJDIR>
    echo rm -f -r <PROJDIR>/<PREOBJDIR>
    rm -f -r <PROJDIR>/<PREOBJDIR>
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=bvi
    cdsay .
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\ctl\dos386
    wmake -h
    <CPCMD> parsectl.exe <DEVDIR>\build\bin\
    <CPCMD> parsedyn.exe <DEVDIR>\build\bin\
    cdsay <PROJDIR>\bind\dos386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../dos386/makefile prebuild=1
    <CPCMD> vi.exe <DEVDIR>/build/bin/bvi.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\ctl\os2386
    wmake -h
    <CPCMD> parsectl.exe <DEVDIR>\build\binp\
    <CPCMD> parsedyn.exe <DEVDIR>\build\binp\
    cdsay <PROJDIR>\bind\os2386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../os2386/makefile prebuild=1
    <CPCMD> vi.exe <DEVDIR>/build/binp/bvi.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\ctl\nt386
    wmake -h
    <CPCMD> parsectl.exe <DEVDIR>\build\binnt\
    <CPCMD> parsedyn.exe <DEVDIR>\build\binnt\
    cdsay <PROJDIR>\bind\nt386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../nt386/makefile prebuild=1
    <CPCMD> vi.exe <DEVDIR>/build/binnt/bvi.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    cdsay <PROJDIR>\ctl\ntaxp
    wmake -h
    <CPCMD> parsectl.exe <DEVDIR>\build\axpnt\
    <CPCMD> parsedyn.exe <DEVDIR>\build\axpnt\
    cdsay <PROJDIR>\bind\ntaxp
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>\<PREOBJDIR>
    cdsay <PROJDIR>\<PREOBJDIR>
    wmake -h -f ../ntaxp/makefile prebuild=1
    <CPCMD> vi.exe <DEVDIR>/build/axpnt/bvi.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <PROJDIR>/ctl/linux386
    wmake -h
    <CPCMD> parsectl.exe <DEVDIR>/build/binl/parsectl
    <CPCMD> parsedyn.exe <DEVDIR>/build/binl/parsedyn
    cdsay <PROJDIR>/bind/linux386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../linux386/makefile prebuild=1
    <CPCMD> vi.exe <DEVDIR>/build/binl/bvi
    cdsay <PROJDIR>

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=
