# vi Prerequisite Bootstrap Builder Control File
# ==============================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<OBJDIR>b
    rm -f -r <PROJDIR>/<OBJDIR>b

[ BLOCK <1> boot ]
#=================
    echo Building the vi bootstrap
    mkdir <PROJDIR>/<OBJDIR>b
    cdsay <PROJDIR>/<OBJDIR>b
    wmake -h -f ../bootmake prebuild=1
    <CPCMD> vi.exe <DEVDIR>/build/bin/bvi
    <CPCMD> vi.exe <DEVDIR>/build/binl/bvi
    cdsay <PROJDIR>
