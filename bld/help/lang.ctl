# WHELP Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/help/dos386/whelp.exe <relroot>/rel2/binw/whelp.exe
    <CPCMD> <DEVDIR>/help/os2386/whelp.exe <relroot>/rel2/binp/whelp.exe

[ BLOCK <1> clean ]
#==================
    pmake -d all -h clean
