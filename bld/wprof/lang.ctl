# WPROF Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/wprof/os2386pm/wprof.exe    <RELROOT>/rel2/binp/wprof.exe
    <CPCMD> <DEVDIR>/wprof/os2386/wprof.exe      <RELROOT>/rel2/binp/wprofc.exe
    <CPCMD> <DEVDIR>/wprof/wini86/wprof.exe      <RELROOT>/rel2/binw/wprofw.exe
    <CPCMD> <DEVDIR>/wprof/dos386/wprof.exe      <RELROOT>/rel2/binw/wprof.exe
    <CPCMD> <DEVDIR>/wprof/nt386/wprof.exe       <RELROOT>/rel2/binnt/wprof.exe
#    <CPCMD> <DEVDIR>/wprof/ntaxp/wprof.exe       <RELROOT>/rel2/axpnt/wprof.exe
#    <CPCMD> <DEVDIR>/wprof/qnx386/wprof.qnx      <RELROOT>/rel2/qnx/binq/wprof.
#    <CPCMD> <DEVDIR>/wprof/qnx386/wprof.sym      <RELROOT>/rel2/qnx/sym/wprof.sym

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
