set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
    set BUILD_PLATFORM=<BUILD_PLATFORM>boot<1>

[ BLOCK <BUILD_PLATFORM> linux386bootclean ]
#===========================================
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>
    rm -f <OWBINDIR>/wlink<CMDEXT>
    wmake -h clean
    set BUILD_PLATFORM=
    set 1=

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<PREOBJDIR>
    rm -f -r <PROJDIR>/<PREOBJDIR>
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 os2386 nt386 linux386 ]
#======================================================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wsplice -k Pwlsystem ../specs.sp wlsystem.lnk
    wmake -h -f ../<BUILD_PLATFORM>/makefile prebuild=1

[ BLOCK <BUILD_PLATFORM> linux386boot ]
#======================================
    wmake -h
    <CPCMD> wlsystem.lnk <RELROOT>/binw/wlsystem.lnk
    mkdir <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../linux386/makefile bootstrap=1
    <CPCMD> wl.exe <OWBINDIR>/wlink

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

cdsay <PROJDIR>
