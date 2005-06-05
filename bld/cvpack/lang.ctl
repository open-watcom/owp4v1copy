# cvpack Builder Control file
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
    <CPCMD> <PROJDIR>/dos386/cvpack.exe    <RELROOT>/rel2/binw/cvpack.exe

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <PROJDIR>/os2386/cvpack.exe    <RELROOT>/rel2/binp/cvpack.exe

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <PROJDIR>/nt386/cvpack.exe     <RELROOT>/rel2/binnt/cvpack.exe

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <PROJDIR>/ntaxp/cvpack.exe     <RELROOT>/rel2/axpnt/cvpack.exe

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> <PROJDIR>/qnx386/cvpack.exe    <RELROOT>/rel2/qnx/binq/cvpack

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <PROJDIR>/linux386/cvpack.exe  <RELROOT>/rel2/binl/cvpack

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
