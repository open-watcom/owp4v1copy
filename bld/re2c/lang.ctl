# RE2C Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> build rel2 cprel2 ]
#==============================

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>
