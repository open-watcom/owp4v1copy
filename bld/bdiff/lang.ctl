# bpatch Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

    # Make BPATCH.QNX for Database guys. Easier than getting them set up
#    wmake -h bpatch.qnx

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <DEVDIR>/bdiff/dos386/bdiff.exe <RELROOT>/rel2/binw/bdiff.exe
    <CPCMD> <DEVDIR>/bdiff/dos386/bpatch.exe <RELROOT>/rel2/binw/bpatch.exe
  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <DEVDIR>/bdiff/os2386/bdiff.exe <RELROOT>/rel2/binp/bdiff.exe
    <CPCMD> <DEVDIR>/bdiff/os2386/bpatch.exe <RELROOT>/rel2/binp/bpatch.exe
  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/bdiff/nt386/bdiff.exe <RELROOT>/rel2/binnt/bdiff.exe
    <CPCMD> <DEVDIR>/bdiff/nt386/bpatch.exe <RELROOT>/rel2/binnt/bpatch.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
