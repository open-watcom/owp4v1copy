# TECHINFO Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <PROJDIR>\dosi86\techinfo.exe <relroot>\rel2\binw\
    <CPCMD> <PROJDIR>\os2i86\techinfo.exe <relroot>\rel2\binp\

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
