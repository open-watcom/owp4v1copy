# WSTUB Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <BUILD_PLATFORM> linux386 ]
#===============================
    <CPCMD> wstub.exe <DEVDIR>/build/binl/wstub.exe

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> wstub.exe <relroot>/rel2/binw/wstub.exe
    <CPCMD> wstubq.exe <relroot>/rel2/binw/wstubq.exe
    <CPCMD> wstub.c <relroot>/rel2/src/wstub.c

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
