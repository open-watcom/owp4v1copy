set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/wres/<OBJDIR>
    rm -f -r <PROJDIR>/wres/<OBJDIR>
    echo rm -f -r <PROJDIR>/rc/<OBJDIR>
    rm -f -r <PROJDIR>/rc/<OBJDIR>

[ BLOCK <1> boot ]
#=================
    echo Building the resource compiler bootstrap
    cdsay <PROJDIR>/wres
    <MAKE> -f gnumake
    cdsay ../rc
    <MAKE> -f gnumake
    <CPCMD> <OBJDIR>/rc <OWROOT>/bld/build/bin/wrc
    cdsay <PROJDIR>
