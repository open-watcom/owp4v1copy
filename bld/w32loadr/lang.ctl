# w32ldr Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
#   <CPCMD> os2386/os2ldr.exe <tooldr>/tools/os2ldr.exe
    <CPCMD> os2386/w32bind.exe ../build/binp/w32bind.exe
    <CPCMD> os2386/os2ldr.exe ../build/binp/os2ldr.exe
#   <CPCMD> dos386/x32run.exe <relroot>/rel2/binw/x32run.exe
#   <CPCMD> dos386/x32run.exe <relroot>/rel2/binw/w32run.exe
#   <CPCMD> dos386/d4grun.exe <relroot>/rel2/binw/d4grun.exe
#   <CPCMD> dos386/tntrun.exe <relroot>/rel2/binw/tntrun.exe
    <CPCMD> nt386/ntrunner.exe <relroot>/rel2/binnt/w32run.exe
#    these are now real NT exe's - don't copy the stub
#    <CPCMD> nt/*.exe <relroot>/rel2/binnt/

[ BLOCK <1> clean ]
#=================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>
