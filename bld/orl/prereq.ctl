set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> linux386 ]
#================================
    echo Building the object reading library
    cdsay <PROJDIR>
    <MAKE> -f gnumake
    cdsay <PROJDIR>
