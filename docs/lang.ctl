# Documentation Builder Control file
# ==================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <doc_root>\dos\*.ihp <relroot>\rel2\binw\
    <CPCMD> <doc_root>\win\*.hlp <relroot>\rel2\binw\
    <CPCMD> <doc_root>\os2\*.inf <relroot>\rel2\binp\help\
    <CPCMD> <doc_root>\os2\*.hlp <relroot>\rel2\binp\help\
    <CPCMD> <doc_root>\nt\*.hlp  <relroot>\rel2\binnt\
    <CPCMD> <doc_root>\nt\*.cnt  <relroot>\rel2\binnt\

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
