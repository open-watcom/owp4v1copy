# bmp2eps Builder Control file
# ============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
#=======================
    [ INCLUDE prereq.ctl ]

[ BLOCK <1> clean ]
#==================
    pmake -d all -h clean
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=bmp2eps
