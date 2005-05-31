# IDEB (aka BVIPER) Builder Control file
# ======================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    [ INCLUDE prereq.ctl ]

[ BLOCK <1> clean ]
#==================
    pmake -d all -h clean
    cdsay .
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=bviper

[ BLOCK . . ]
#============

cdsay <PROJDIR>
