set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> linux386 ]
#================================
    echo Building the dwarf write library
    mkdir <PROJDIR>/dw/<OBJDIR>
    cdsay <PROJDIR>/dw/<OBJDIR>
    wmake -h -f ../o/makefile bootstrap=1
    cdsay <PROJDIR>
