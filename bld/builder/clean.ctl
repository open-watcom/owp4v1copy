# Build tools cleaning Control File
# =================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> clean ]
#==================
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=langdat
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=mksetup
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=cdsay
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=sweep
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=wsplice
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=diff
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=comstrip
    wmake -h -f <DEVDIR>/build/mif/cleanp.mif platform=<BUILD_PLATFORM> file=rm

[ BLOCK . . ]
#============

