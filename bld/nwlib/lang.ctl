# NWLIB Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#    <CPCMD> ntaxp/wlib.exe   <RELROOT>/rel2/axpnt/wlib.exe
#    <CPCMD> ntaxp/wlibd.dll  <RELROOT>/rel2/axpnt/wlibd.dll
    <CPCMD> os2386/wlib.exe  <RELROOT>/rel2/binp/wlib.exe
    <CPCMD> os2386/wlibd.dll <RELROOT>/rel2/binp/dll/wlibd.dll
    <CPCMD> nt386/wlib.exe   <RELROOT>/rel2/binnt/wlib.exe
    <CPCMD> nt386/wlibd.dll  <RELROOT>/rel2/binnt/wlibd.dll
    <CPCMD> dos386/wlib.exe  <RELROOT>/rel2/binw/wlib.exe
#    <CPCMD> qnx386/wlib.exe  <RELROOT>/rel2/qnx/wlib.
#    <CPCMD> qnx386/wlib.sym  <RELROOT>/rel2/qnx/wlib.sym
    <CPCMD> linux386/wlib.exe  <RELROOT>/rel2/binl/wlib
    <CPCMD> linux386/wlib.sym  <RELROOT>/rel2/binl/wlib.sym

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
