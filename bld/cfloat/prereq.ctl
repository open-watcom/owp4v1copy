set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> linux386 ]
#================================
    echo Building the cfloat lib
    mkdir -p <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../o.386/makefile bootstrap=1
    cdsay <PROJDIR>
