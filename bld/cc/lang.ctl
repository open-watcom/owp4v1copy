# cfe Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
#dos stuff
    <CPCMD> dos386.386/wcc386c.exe <relroot>/rel2/binw/wcc386.exe
    <CPCMD> dos386.386/wcc386c.sym <relroot>/rel2/binw/wcc386.sym
    <CPCMD> dos386.386/wcc38601.int <relroot>/rel2/binw/wcc38601.int
    <CPCMD> dos386.i86/wcci86c.exe <relroot>/rel2/binw/wcc.exe
    <CPCMD> dos386.i86/wcci86c.sym <relroot>/rel2/binw/wcc.sym
    <CPCMD> dos386.i86/wcci8601.int <relroot>/rel2/binw/wcci8601.int
#    <CPCMD> dos386/wccaxpe.exe <relroot>/rel2/binnt/wccaxp.exe
#    <CPCMD> dosi86/wccaxpe.exe <relroot>/rel2/binnt/wccaxp.exe
#    <CPCMD> osippc/wccppcc.exe <relroot>/rel2/binw/wccppc.exe
#    <CPCMD> osiaxp/wccaxpc.exe <relroot>/rel2/binw/wccaxp.exe

#osi stuff
#    <CPCMD> osi386.386/wcc386c.exe <relroot>/rel2/binw/wcc386.exe
#    <CPCMD> osi386.386/wcc386c.sym <relroot>/rel2/binw/wcc386.sym
#    <CPCMD> osi386.386/wcc38601.int <relroot>/rel2/binw/wcc38601.int
#    <CPCMD> osi386.i86/wcci86c.exe <relroot>/rel2/binw/wcc.exe
#    <CPCMD> osi386.i86/wcci86c.sym <relroot>/rel2/binw/wcc.sym
#    <CPCMD> osi386.i86/wcci8601.int <relroot>/rel2/binw/wcci8601.int

#AXP target
    <CPCMD> nt386.axp/wccaxpe.exe <relroot>/rel2/binnt/wccaxp.exe
    <CPCMD> nt386.axp/wccaxpe.sym <relroot>/rel2/binnt/wccaxp.sym
    <CPCMD> nt386.axp/wccdaxpe.dll <relroot>/rel2/binnt/wccdaxp.dll
    <CPCMD> nt386.axp/wccdaxpe.sym <relroot>/rel2/binnt/wccdaxp.sym

    <CPCMD> os2386.axp/wccaxpe.exe <relroot>/rel2/binp/wccaxp.exe
    <CPCMD> os2386.axp/wccaxpe.sym <relroot>/rel2/binp/wccaxp.sym
    <CPCMD> os2386.axp/wccdaxpe.dll <relroot>/rel2/binp/dll/wccdaxp.dll
    <CPCMD> os2386.axp/wccdaxpe.sym <relroot>/rel2/binp/dll/wccdaxp.sym

#NT stuff
    <CPCMD> nt386.386/wcc386e.exe <relroot>/rel2/binnt/wcc386.exe
    <CPCMD> nt386.386/wcc386e.sym <relroot>/rel2/binnt/wcc386.sym
    <CPCMD> nt386.386/wccd386e.dll <relroot>/rel2/binnt/wccd386.dll
    <CPCMD> nt386.386/wccd386e.sym <relroot>/rel2/binnt/wccd386.sym
    <CPCMD> nt386.386/wcc38601.int <relroot>/rel2/binnt/wcc38601.int
    <CPCMD> nt386.i86/wcci86e.exe <relroot>/rel2/binnt/wcc.exe
    <CPCMD> nt386.i86/wcci86e.sym <relroot>/rel2/binnt/wcc.sym
    <CPCMD> nt386.i86/wccdi86e.dll <relroot>/rel2/binnt/wccd.dll
    <CPCMD> nt386.i86/wccdi86e.sym <relroot>/rel2/binnt/wccd.sym
    <CPCMD> nt386.i86/wcci8601.int <relroot>/rel2/binnt/wcci8601.int

#OS/2 stuff
    <CPCMD> os2386.386/wcc386e.exe <relroot>/rel2/binp/wcc386.exe
    <CPCMD> os2386.386/wcc386e.sym <relroot>/rel2/binp/wcc386.sym
    <CPCMD> os2386.386/wccd386e.dll <relroot>/rel2/binp/dll/wccd386.dll
    <CPCMD> os2386.386/wccd386e.sym <relroot>/rel2/binp/dll/wccd386.sym
    <CPCMD> os2386.386/wcc38601.int <relroot>/rel2/binp/dll/wcc38601.int

    <CPCMD> os2386.i86/wcci86e.exe <relroot>/rel2/binp/wcc.exe
    <CPCMD> os2386.i86/wcci86e.sym <relroot>/rel2/binp/wcc.sym
    <CPCMD> os2386.i86/wccdi86e.dll <relroot>/rel2/binp/dll/wccd.dll
    <CPCMD> os2386.i86/wccdi86e.sym <relroot>/rel2/binp/dll/wccd.sym
    <CPCMD> os2386.i86/wcci8601.int <relroot>/rel2/binp/dll/wcci8601.int

#linux stuff
    <CPCMD> linux386.386/wcc386c.elf <relroot>/rel2/binl/wcc386
    <CPCMD> linux386.386/wcc386c.sym <relroot>/rel2/binl/wcc386.sym
#    <CPCMD> linux386.386/wcc38601.int <relroot>/rel2/binl/wcc38601.int
    <CPCMD> linux386.i86/wcci86c.elf <relroot>/rel2/binl/wcc
    <CPCMD> linux386.i86/wcci86c.sym <relroot>/rel2/binl/wcc.sym
#    <CPCMD> linux386.i86/wcci8601.int <relroot>/rel2/binl/wcci8601.int

#QNX stuff
#    <CPCMD> qnx386.i86/wcci86c.qnx <relroot>/rel2/qnx/binq/wcc.
#    <CPCMD> qnx386.i86/wcci86c.sym <relroot>/rel2/qnx/sym/wcc.sym
#    <CPCMD> qnx386.386/wcc386c.qnx <relroot>/rel2/qnx/binq/wcc386.
#    <CPCMD> qnx386.386/wcc386c.sym <relroot>/rel2/qnx/sym/wcc.sym

#AXP NT host
#    <CPCMD> ntaxp.axp/wccaxpc.exe <relroot>/rel2/axpnt/wccaxp.exe
#    <CPCMD> ntaxp.axp/wccaxpc.exe <relroot>/rel2/axpnt/wccaxp.sym
#    <CPCMD> ntaxp.386/wcc386c.exe <relroot>/rel2/axpnt/wcc386.exe
#    <CPCMD> ntaxp.i86/wcci86c.exe <relroot>/rel2/axpnt/wcc.exe
#    <CPCMD> ntaxp.axp/wccaxpe.exe <relroot>/rel2/axpnt/wccaxp.exe
#    <CPCMD> ntaxp.axp/wccdaxpe.dll <relroot>/rel2/axpnt/wccdaxp.dll

#    <CPCMD> nt386.ppc/wccppcc.exe <relroot>/rel2/binnt/wccppc.exe

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
