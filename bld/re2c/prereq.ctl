# RE2C Prerequisite Tool Build Control File
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
    set BUILD_PLATFORM=<BUILD_PLATFORM>boot

[ BLOCK <1> clean ]
#==================
    rm -f -r <PROJDIR>/<PREOBJDIR>
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=re2c
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../dos386/makefile prebuild=1
    <CPCMD> re2c.exe <DEVDIR>\build\bin\re2c.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../os2386/makefile prebuild=1
    <CPCMD> re2c.exe <DEVDIR>\build\binp\re2c.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../nt386/makefile prebuild=1
    <CPCMD> re2c.exe <DEVDIR>\build\binnt\re2c.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../ntaxp/makefile prebuild=1
    <CPCMD> re2c.exe <DEVDIR>\build\axpnt\re2c.exe
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../linux386/makefile prebuild=1
    <CPCMD> re2c.exe <DEVDIR>/build/binl/re2c
    cdsay <PROJDIR>

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=
