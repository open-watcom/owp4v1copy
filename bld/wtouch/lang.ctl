# WTOUCH Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <PROJDIR>/dosi86/wtouch.exe    <RELROOT>/rel2/binw/wtouch.exe
    <CPCMD> <PROJDIR>/os2386/wtouch.exe    <RELROOT>/rel2/binp/wtouch.exe
    <CPCMD> <PROJDIR>/nt386/wtouch.exe     <RELROOT>/rel2/binnt/wtouch.exe
#    <CPCMD> <PROJDIR>/ntaxp/wtouch.exe     <RELROOT>/rel2/axpnt/wtouch.exe
    <CPCMD> <PROJDIR>/linux386/wtouch.exe  <RELROOT>/rel2/binl/wtouch

[ BLOCK <1> clean ]
#==================
    pmake -d all -h clean
