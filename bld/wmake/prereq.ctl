set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    echo Building the wmake bootstrap
    cdsay <PROJDIR>
    <MAKE> -f gnumake
    <CPCMD> <OBJDIR>/wmake <devdir>/build/binl/wmake
    cdsay <PROJDIR>
