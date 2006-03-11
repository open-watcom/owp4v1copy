# wlink Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>
    echo rm -f wlsystem.lnk
    rm -f wlsystem.lnk

[ BLOCK <1> boot ]
#=================
    echo Building the wlib bootstrap
    wsplice -k Pwlsystem specs.sp wlsystem.lnk
    mkdir <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../bootmake
    <CPCMD> wl.exe <DEVDIR>/build/bin/wlink
    cdsay <PROJDIR>
