# MSTOOLS Builder Control file
# ============================
set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
#    <CPCMD> <DEVDIR>/mstools/asaxp/nt386/asaxp.exe <relroot>/rel2/binnt/asaxp.exe
#    <CPCMD> <DEVDIR>/mstools/asaxp/ntaxp/asaxp.exe <relroot>/rel2/axpnt/asaxp.exe
    <CPCMD> <DEVDIR>/mstools/cl/nt386/cl.exe <relroot>/rel2/binnt/cl.exe
    <CPCMD> <DEVDIR>/mstools/cl/nt386/cl386.exe <relroot>/rel2/binnt/cl386.exe
#    <CPCMD> <DEVDIR>/mstools/cl/nt386/claxp.exe <relroot>/rel2/binnt/claxp.exe
#    <CPCMD> <DEVDIR>/mstools/cl/ntaxp/cl.exe <relroot>/rel2/axpnt/cl.exe
#    <CPCMD> <DEVDIR>/mstools/cl/ntaxp/cl386.exe <relroot>/rel2/axpnt/cl386.exe
#    <CPCMD> <DEVDIR>/mstools/cl/ntaxp/claxp.exe <relroot>/rel2/axpnt/claxp.exe
    <CPCMD> <DEVDIR>/mstools/cvtres/nt386/cvtres.exe <relroot>/rel2/binnt/cvtres.exe
#    <CPCMD> <DEVDIR>/mstools/cvtres/ntaxp/cvtres.exe <relroot>/rel2/axpnt/cvtres.exe
    <CPCMD> <DEVDIR>/mstools/lib/nt386/lib.exe <relroot>/rel2/binnt/lib.exe
    <CPCMD> <DEVDIR>/mstools/lib/nt386/lib386.exe <relroot>/rel2/binnt/lib386.exe
#    <CPCMD> <DEVDIR>/mstools/lib/nt386/libaxp.exe <relroot>/rel2/binnt/libaxp.exe
#    <CPCMD> <DEVDIR>/mstools/lib/ntaxp/lib.exe <relroot>/rel2/axpnt/lib.exe
#    <CPCMD> <DEVDIR>/mstools/lib/ntaxp/lib386.exe <relroot>/rel2/axpnt/lib386.exe
#    <CPCMD> <DEVDIR>/mstools/lib/ntaxp/libaxp.exe <relroot>/rel2/axpnt/libaxp.exe
    <CPCMD> <DEVDIR>/mstools/link/nt386/link.exe <relroot>/rel2/binnt/link.exe
    <CPCMD> <DEVDIR>/mstools/link/nt386/link386.exe <relroot>/rel2/binnt/link386.exe
#    <CPCMD> <DEVDIR>/mstools/link/nt386/linkaxp.exe <relroot>/rel2/binnt/linkaxp.exe
#    <CPCMD> <DEVDIR>/mstools/link/ntaxp/link.exe <relroot>/rel2/axpnt/link.exe
#    <CPCMD> <DEVDIR>/mstools/link/ntaxp/link386.exe <relroot>/rel2/axpnt/link386.exe
#    <CPCMD> <DEVDIR>/mstools/link/ntaxp/linkaxp.exe <relroot>/rel2/axpnt/linkaxp.exe
    <CPCMD> <DEVDIR>/mstools/nmake/nt386/nmake.exe <relroot>/rel2/binnt/nmake.exe
#    <CPCMD> <DEVDIR>/mstools/nmake/ntaxp/nmake.exe <relroot>/rel2/axpnt/nmake.exe
    <CPCMD> <DEVDIR>/mstools/rc/nt386/rc.exe <relroot>/rel2/binnt/rc.exe
#    <CPCMD> <DEVDIR>/mstools/rc/ntaxp/rc.exe <relroot>/rel2/axpnt/rc.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

