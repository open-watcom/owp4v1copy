set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> linux386 ]
#================================
    echo Building the resource compiler
    cdsay <PROJDIR>/wres
    <MAKE> -f gnumake
    cdsay ../rc
    <MAKE> -f gnumake
    <CPCMD> bootstrp/rc <OWROOT>/bld/build/binl/wrc
    cdsay <PROJDIR>

