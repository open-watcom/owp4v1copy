# cfloat Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>

[ BLOCK <1> boot ]
#=================
    echo Building the pmake bootstrap
    mkdir <PROJDIR>/<OBJDIR>
    <MAKE> -f gnumake
    <CPCMD> <OBJDIR>/pmake <DEVDIR>/build/bin/pmake
    cdsay <PROJDIR>
