# POSIX Prerequisite Tool Build Control File
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
    set BUILD_PLATFORM=<BUILD_PLATFORM>boot

[ BLOCK <1> clean ]
#==================
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f <PROJDIR>/mif/makeall.mif prebuild=1 clean
    cdsay <PROJDIR>
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>
    wmake -h -f <PROJDIR>/mif/makeall.mif prebuild=1 build_path=<OWROOT>\bld\build\bin clean_build
    wmake -h -f <PROJDIR>/mif/makeall.mif prebuild=1 build_path=<OWROOT>\bld\build\binp clean_build
    wmake -h -f <PROJDIR>/mif/makeall.mif prebuild=1 build_path=<OWROOT>\bld\build\binnt clean_build
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../systems/dos/makefile prebuild=1
    <CPCMD> *.exe <OWROOT>\bld\build\bin\
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../systems/os2386/makefile prebuild=1
    <CPCMD> *.exe <OWROOT>\bld\build\binp\
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    mkdir <PROJDIR>\<OBJDIR>
    cdsay <PROJDIR>\<OBJDIR>
    wmake -h -f ../systems/nt386/makefile prebuild=1
    <CPCMD> *.exe <OWROOT>\bld\build\binnt\
    cdsay <PROJDIR>

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=
