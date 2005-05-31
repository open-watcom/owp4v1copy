set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
    set BUILD_PLATFORM=<BUILD_PLATFORM>boot

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/wres/<OBJDIR>
    rm -f -r <PROJDIR>/wres/<OBJDIR>
    echo rm -f -r <PROJDIR>/rc/<OBJDIR>
    rm -f -r <PROJDIR>/rc/<OBJDIR>
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=wrc
    set BUILD_PLATFORM=

[ BLOCK <BUILD_PLATFORM> linux386boot ]
#======================================
    echo Building the resource compiler
    cdsay <PROJDIR>/wres
    <MAKE> -f gnumake
    cdsay ../rc
    <MAKE> -f gnumake
    <CPCMD> <OBJDIR>/rc <OWROOT>/bld/build/binl/wrc

[ BLOCK . . ]
#============
set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
set TMP_BUILD_PLATFORM=

cdsay <PROJDIR>
