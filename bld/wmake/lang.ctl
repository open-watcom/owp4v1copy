# WMake Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <COMSPEC> c:\4nt\4nt.exe ]

    echo ******************************************
    echo *** don't build WMAKE when running 4NT ***
    echo ******************************************
    pause

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>\wmake\dosi86\wmk.exe <relroot>\rel2\binw\wmaker.exe
    <CPCMD> <devdir>\wmake\dos386\wmk.exe <relroot>\rel2\binw\wmake.exe
    <CPCMD> <devdir>\wmake\os2386\wmk.exe <relroot>\rel2\binp\wmake.exe
    <CPCMD> <devdir>\wmake\nt386\wmk.exe <relroot>\rel2\binnt\wmake.exe
#    <CPCMD> <devdir>\wmake\ntaxp\wmk.exe <relroot>\rel2\axpnt\wmake.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
