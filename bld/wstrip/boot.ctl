set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>

[ BLOCK <1> boot ]
#=================
    echo Building the wstrip bootstrap
    <MAKE> -f gnumake
    <CPCMD> <OBJDIR>/wstrip <DEVDIR>/build/bin/wstrip
    cdsay <PROJDIR>
