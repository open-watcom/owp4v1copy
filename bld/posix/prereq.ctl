# POSIX Prerequisite Tool Build Control File
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../systems/dos/makefile prebuild=1
    <CPCMD> *.exe <owroot>\bld\build\bin\
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../systems/os2386/makefile prebuild=1
    <CPCMD> *.exe <owroot>\bld\build\binp\
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    if not exist <PROJDIR>\<OBJDIR> mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../systems/nt386/makefile prebuild=1
    <CPCMD> *.exe <owroot>\bld\build\binnt\
    cdsay <PROJDIR>

[ BLOCK <1> clean ]
#==================
    rm -f -r <PROJDIR>/<OBJDIR>
