# FMEDIT Builder Control file
# ===========================
# This should really be part of the SDK
# it is called from SDK\lang.ctl

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
<CPCMD> <PROJDIR>/obj/fmedit.dll <relroot>/rel2/binw/
<CPCMD> <PROJDIR>/nt/fmedit.dll <relroot>/rel2/binnt/

[ BLOCK <1> clean ]
#==================
    pmake -d all -h clean
