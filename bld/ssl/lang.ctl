# SSL Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
#=======================
    set old1=<1>
    set 1=<BUILD_PLATFORM>
    [ INCLUDE prereq.ctl ]
    set 1=<old1>

[ BLOCK <1> clean ]
#==================
    pmake -d all -h clean
