# wlink Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>
    wmake -h clean

[ BLOCK <1> boot ]
#=================
    echo Building the wlink bootstrap
    wmake -h
    mkdir <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../bootmake
    <CPCMD> wl.exe <DEVDIR>/build/bin/wlink
    cdsay <PROJDIR>
