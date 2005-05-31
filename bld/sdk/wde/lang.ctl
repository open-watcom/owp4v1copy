# WDE Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (os_win "") <2*> ]
    <CPCMD> <DEVDIR>/sdk/wde/wini86/wde.exe      <RELROOT>/rel2/binw/wde.exe

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/sdk/wde/nt386/wde.exe       <RELROOT>/rel2/binnt/wde.exe

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <DEVDIR>/sdk/wde/ntaxp/wde.exe       <RELROOT>/rel2/axpnt/wde.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
