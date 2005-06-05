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
    rm -f <OWBINDIR>/parsectl
    rm -f <OWBINDIR>/parsedyn
    rm -f <OWBINDIR>/parsectl.exe
    rm -f <OWBINDIR>/parsedyn.exe
    cdsay <PROJDIR>/bind
    pmake -d all -h clean
    cdsay <PROJDIR>
    echo rm -f -r <PROJDIR>/<PREOBJDIR>
    rm -f -r <PROJDIR>/<PREOBJDIR>
    rm -f <OWBINDIR>/bvi
    rm -f <OWBINDIR>/bvi.exe
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>/ctl/dos386
    wmake -h
    <CPCMD> parsectl.exe <OWBINDIR>/parsectl.exe
    <CPCMD> parsedyn.exe <OWBINDIR>/parsedyn.exe
    cdsay <PROJDIR>/bind/dos386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../dos386/makefile prebuild=1
    <CPCMD> vi.exe <OWBINDIR>/bvi.exe

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>/ctl/os2386
    wmake -h
    <CPCMD> parsectl.exe <OWBINDIR>/parsectl.exe
    <CPCMD> parsedyn.exe <OWBINDIR>/parsedyn.exe
    cdsay <PROJDIR>/bind/os2386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../os2386/makefile prebuild=1
    <CPCMD> vi.exe <OWBINDIR>/bvi.exe

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>/ctl/nt386
    wmake -h
    <CPCMD> parsectl.exe <OWBINDIR>/parsectl.exe
    <CPCMD> parsedyn.exe <OWBINDIR>/parsedyn.exe
    cdsay <PROJDIR>/bind/nt386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../nt386/makefile prebuild=1
    <CPCMD> vi.exe <OWBINDIR>/bvi.exe

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    cdsay <PROJDIR>/ctl/ntaxp
    wmake -h
    <CPCMD> parsectl.exe <OWBINDIR>/parsectl.exe
    <CPCMD> parsedyn.exe <OWBINDIR>/parsedyn.exe
    cdsay <PROJDIR>/bind/ntaxp
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../ntaxp/makefile prebuild=1
    <CPCMD> vi.exe <OWBINDIR>/bvi.exe

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    cdsay <PROJDIR>/ctl/linux386
    wmake -h
    <CPCMD> parsectl.exe <OWBINDIR>/parsectl
    <CPCMD> parsedyn.exe <OWBINDIR>/parsedyn
    cdsay <PROJDIR>/bind/linux386
    wmake -h
    cdsay <PROJDIR>
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../linux386/makefile prebuild=1
    <CPCMD> vi.exe <OWBINDIR>/bvi

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

cdsay <PROJDIR>
