set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    echo Building the codegen bootstrap
    mkdir -p <PROJDIR>/intel/386/<OBJDIR>
    cdsay <PROJDIR>/intel/386/<OBJDIR>
    wmake -h -f ../linux386/makefile bootstrap=1
    cdsay <PROJDIR>
