# cvpack Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/cvpack/dos386/cvpack.exe  <RELROOT>/rel2/binw/cvpack.exe
    <CPCMD> <DEVDIR>/cvpack/os2386/cvpack.exe  <RELROOT>/rel2/binp/cvpack.exe
    <CPCMD> <DEVDIR>/cvpack/nt386/cvpack.exe   <RELROOT>/rel2/binnt/cvpack.exe
#    <CPCMD> <DEVDIR>/cvpack/ntaxp/cvpack.exe   <RELROOT>/rel2/axpnt/cvpack.exe
#    <CPCMD> <DEVDIR>/cvpack/qnx386/cvpack.exe  <RELROOT>/rel2/qnx/binq/cvpack

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK <1> relink ]
#===================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h killnonobj
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
