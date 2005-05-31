# Build tools cleaning Control File
# =================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> clean ]
#==================
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=langdat
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=mksetup
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=cdsay
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=sweep
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=wsplice
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=diff
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=comstrip
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif file=rm

[ BLOCK . . ]
#============

