# DMPOBJ Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay .

[ BLOCK <1> rel2 cprel2 ]
#========================
[ IFEQU os_dos <*>]
    <CPCMD> <PROJDIR>/dos386/dmpobj.exe    <RELROOT>/rel2/binw/dmpobj.exe
[ ENDIF ]
[ IFEQU os_os2 <*> ]
    <CPCMD> <PROJDIR>/os2386/dmpobj.exe    <RELROOT>/rel2/binp/dmpobj.exe
[ ENDIF ]
[ IFEQU os_nt <*> ]
    <CPCMD> <PROJDIR>/nt386/dmpobj.exe     <RELROOT>/rel2/binnt/dmpobj.exe
[ ENDIF ]
[ IFEQU os_linux <*> ]
    <CPCMD> <PROJDIR>/linux386/dmpobj.exe  <RELROOT>/rel2/binl/dmpobj
[ ENDIF ]

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
