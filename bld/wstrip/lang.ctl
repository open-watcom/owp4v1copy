# WSTRIP Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <DEVDIR>/wstrip/dosi86/strip.exe    <RELROOT>/rel2/binw/wstrip.exe

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <DEVDIR>/wstrip/os2386/strip.exe    <RELROOT>/rel2/binp/wstrip.exe

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/wstrip/nt386/strip.exe     <RELROOT>/rel2/binnt/wstrip.exe

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <DEVDIR>/wstrip/linux386/strip.exe  <RELROOT>/rel2/binl/wstrip

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <DEVDIR>/wstrip/ntaxp/strip.exe     <RELROOT>/rel2/axpnt/wstrip.exe

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> <DEVDIR>/wstrip/qnx386/strip.exe    <RELROOT>/rel2/qnx/binq/wstrip.

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
