# WMake Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/wmake/dosi86/wmk.exe    <RELROOT>/rel2/binw/wmaker.exe
    <CPCMD> <DEVDIR>/wmake/dos386/wmk.exe    <RELROOT>/rel2/binw/wmake.exe
    <CPCMD> <DEVDIR>/wmake/os2386/wmk.exe    <RELROOT>/rel2/binp/wmake.exe
    <CPCMD> <DEVDIR>/wmake/nt386/wmk.exe     <RELROOT>/rel2/binnt/wmake.exe
#    <CPCMD> <DEVDIR>/wmake/ntaxp/wmk.exe     <RELROOT>/rel2/axpnt/wmake.exe
    <CPCMD> <DEVDIR>/wmake/linux386/wmk.elf  <RELROOT>/rel2/binl/wmake
    <CPCMD> <DEVDIR>/wmake/linux386/wmk.sym  <RELROOT>/rel2/binl/wmake.sym

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
