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
    <CPCMD> <devdir>/cc/wcl/dosi86.i86/wcl.exe    <relroot>/rel2/binw/wcl.exe
    <CPCMD> <devdir>/cc/wcl/dosi86.386/wcl386.exe <relroot>/rel2/binw/wcl386.exe
    <CPCMD> <devdir>/cc/wcl/os2386.i86/wcl.exe    <relroot>/rel2/binp/wcl.exe
    <CPCMD> <devdir>/cc/wcl/os2386.386/wcl386.exe <relroot>/rel2/binp/wcl386.exe
    <CPCMD> <devdir>/cc/wcl/nt386.i86/wcl.exe     <relroot>/rel2/binnt/wcl.exe
    <CPCMD> <devdir>/cc/wcl/nt386.386/wcl386.exe  <relroot>/rel2/binnt/wcl386.exe
    <CPCMD> <devdir>/cc/wcl/linux386.i86/wcl.exe     <relroot>/rel2/binl/wcl
    <CPCMD> <devdir>/cc/wcl/linux386.i86/wcl.sym     <relroot>/rel2/binl/wcl.sym
    <CPCMD> <devdir>/cc/wcl/linux386.386/wcl386.exe  <relroot>/rel2/binl/wcl386
    <CPCMD> <devdir>/cc/wcl/linux386.386/wcl386.sym  <relroot>/rel2/binl/wcl386.sym
#   <CPCMD> <devdir>/cc/wcl/dos386.axp/wclaxp.exe  <relroot>/rel2/binw/wclaxp.exe
#   <CPCMD> <devdir>/cc/wcl/os2386.axp/wclaxp.exe  <relroot>/rel2/binp/wclaxp.exe
#   <CPCMD> <devdir>/cc/wcl/nt386.axp/wclaxp.exe  <relroot>/rel2/binnt/wclaxp.exe
#   <CPCMD> <devdir>/cc/wcl/linux386.axp/wclaxp.exe  <relroot>/rel2/binl/wclaxp
#   <CPCMD> <devdir>/cc/wcl/ntaxp.i86/wcl.exe     <relroot>/rel2/axpnt/wcl.exe
#   <CPCMD> <devdir>/cc/wcl/ntaxp.386/wcl386.exe  <relroot>/rel2/axpnt/wcl386.exe
#   <CPCMD> <devdir>/cc/wcl/ntaxp.axp/wclaxp.exe  <relroot>/rel2/axpnt/wclaxp.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
