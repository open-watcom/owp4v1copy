set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    echo Building the WATCOM support library
    <MAKE> -f gnumake
    cdsay <PROJDIR>
