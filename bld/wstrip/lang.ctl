# WSTRIP Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay .

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/wstrip/dosi86/strip.exe <RELROOT>/rel2/binw/wstrip.exe
    <CPCMD> <DEVDIR>/wstrip/os2386/strip.exe <RELROOT>/rel2/binp/wstrip.exe
    <CPCMD> <DEVDIR>/wstrip/nt386/strip.exe  <RELROOT>/rel2/binnt/wstrip.exe
#    <CPCMD> <DEVDIR>/wstrip/ntaxp/strip.exe <RELROOT>/rel2/axpnt/wstrip.exe
#    <CPCMD> <DEVDIR>/wstrip/qnx386/strip.exe <RELROOT>/rel2/qnx/binq/wstrip.
    <CPCMD> <DEVDIR>/wstrip/linux386/strip.exe <RELROOT>/rel2/binl/wstrip

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
