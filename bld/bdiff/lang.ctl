# bpatch Builder Control file
# ===========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

    # Make BPATCH.QNX for Database guys. Easier than getting them set up
#    wmake /h bpatch.qnx

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>\bdiff\os2386\bdiff.exe <relroot>\rel2\binp\bdiff.exe
    <CPCMD> <devdir>\bdiff\nt386\bdiff.exe <relroot>\rel2\binnt\bdiff.exe
    <CPCMD> <devdir>\bdiff\dos386\bdiff.exe <relroot>\rel2\binw\bdiff.exe
    <CPCMD> <devdir>\bdiff\os2386\bpatch.exe <relroot>\rel2\binp\bpatch.exe
    <CPCMD> <devdir>\bdiff\nt386\bpatch.exe <relroot>\rel2\binnt\bpatch.exe
    <CPCMD> <devdir>\bdiff\dos386\bpatch.exe <relroot>\rel2\binw\bpatch.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

