# EMU86 Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>\emu86\dosi86\emu87.lib <relroot>\rel2\lib286\dos\emu87.lib
    <CPCMD> <devdir>\emu86\os2i86\emu87.lib <relroot>\rel2\lib286\os2\emu87.lib
    <CPCMD> <devdir>\emu86\wini86\emu87.lib <relroot>\rel2\lib286\win\emu87.lib
#    <CPCMD> <devdir>\emu86\stubi86\noemu87.lib <relroot>\rel2\lib286\qnx\emu87.lib
    <CPCMD> <devdir>\emu86\stubi86\noemu87.lib <relroot>\rel2\lib286\noemu87.lib
#    <CPCMD> <devdir>\emu86\qnxi86\emu86    <relroot>\rel2\qnx\binq\emu86
#    <CPCMD> <devdir>\emu86\qnxi86\emu86_16 <relroot>\rel2\qnx\binq\emu86_16
#    <CPCMD> <devdir>\emu86\qnxi86\emu86_32 <relroot>\rel2\qnx\binq\emu86_32

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
