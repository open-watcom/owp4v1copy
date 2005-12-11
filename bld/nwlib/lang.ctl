# NWLIB Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> dos386/wlib.exe  <RELROOT>/rel2/binw/
    <CPCMD> dos386/wlib.sym  <RELROOT>/rel2/binw/

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> os2386/wlib.exe  <RELROOT>/rel2/binp/
    <CPCMD> os2386/wlibd.dll <RELROOT>/rel2/binp/dll/
    <CPCMD> os2386/wlibd.sym <RELROOT>/rel2/binp/dll/

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> nt386/wlib.exe   <RELROOT>/rel2/binnt/
    <CPCMD> nt386/wlibd.dll  <RELROOT>/rel2/binnt/
    <CPCMD> nt386/wlibd.sym  <RELROOT>/rel2/binnt/

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> ntaxp/wlib.exe   <RELROOT>/rel2/axpnt/
    <CPCMD> ntaxp/wlibd.dll  <RELROOT>/rel2/axpnt/
    <CPCMD> ntaxp/wlibd.sym  <RELROOT>/rel2/axpnt/

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> qnx386/wlib.exe  <RELROOT>/rel2/qnx/wlib.
    <CPCMD> qnx386/wlib.sym  <RELROOT>/rel2/qnx/wlib.sym

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> linux386/wlib.exe  <RELROOT>/rel2/binl/wlib
    <CPCMD> linux386/wlib.sym  <RELROOT>/rel2/binl/wlib.sym

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
