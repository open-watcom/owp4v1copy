# wcl Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
    <CPCMD> <DEVDIR>/cc/wcl/dosi86.i86/wcl.exe    <relroot>/rel2/binw/wcl.exe
    <CPCMD> <DEVDIR>/cc/wcl/dosi86.386/wcl386.exe <relroot>/rel2/binw/wcl386.exe
    <CPCMD> <DEVDIR>/cc/wcl/os2386.i86/wcl.exe    <relroot>/rel2/binp/wcl.exe
    <CPCMD> <DEVDIR>/cc/wcl/os2386.386/wcl386.exe <relroot>/rel2/binp/wcl386.exe
    <CPCMD> <DEVDIR>/cc/wcl/nt386.i86/wcl.exe     <relroot>/rel2/binnt/wcl.exe
    <CPCMD> <DEVDIR>/cc/wcl/nt386.386/wcl386.exe  <relroot>/rel2/binnt/wcl386.exe
    <CPCMD> <DEVDIR>/cc/wcl/linux386.i86/wcl.exe     <relroot>/rel2/binl/wcl
    <CPCMD> <DEVDIR>/cc/wcl/linux386.i86/wcl.sym     <relroot>/rel2/binl/wcl.sym
    <CPCMD> <DEVDIR>/cc/wcl/linux386.386/wcl386.exe  <relroot>/rel2/binl/wcl386
    <CPCMD> <DEVDIR>/cc/wcl/linux386.386/wcl386.sym  <relroot>/rel2/binl/wcl386.sym
#   <CPCMD> <DEVDIR>/cc/wcl/dos386.axp/wclaxp.exe  <relroot>/rel2/binw/wclaxp.exe
#   <CPCMD> <DEVDIR>/cc/wcl/os2386.axp/wclaxp.exe  <relroot>/rel2/binp/wclaxp.exe
#   <CPCMD> <DEVDIR>/cc/wcl/nt386.axp/wclaxp.exe  <relroot>/rel2/binnt/wclaxp.exe
#   <CPCMD> <DEVDIR>/cc/wcl/linux386.axp/wclaxp.exe  <relroot>/rel2/binl/wclaxp
#   <CPCMD> <DEVDIR>/cc/wcl/ntaxp.i86/wcl.exe     <relroot>/rel2/axpnt/wcl.exe
#   <CPCMD> <DEVDIR>/cc/wcl/ntaxp.386/wcl386.exe  <relroot>/rel2/axpnt/wcl386.exe
#   <CPCMD> <DEVDIR>/cc/wcl/ntaxp.axp/wclaxp.exe  <relroot>/rel2/axpnt/wclaxp.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
