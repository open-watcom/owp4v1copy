# RCSDLL Builder Control file
# ===========================

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
    <CPCMD> <devdir>\rcsdll\objos2\rcsdll.dll <relroot>\rel2\binp\dll\rcsdll.dll
    <CPCMD> <devdir>\rcsdll\objwin\rcsdll.dll <relroot>\rel2\binw\rcsdll.dll
    <CPCMD> <devdir>\rcsdll\objnt\rcsdll.dll <relroot>\rel2\binnt\rcsdll.dll
#    <CPCMD> <devdir>\rcsdll\objaxp\rcsdll.dll <relroot>\rel2\axpnt\rcsdll.dll
    <CPCMD> <devdir>\rcsdll\bat\*.bat <relroot>\rel2\binw\
    <CPCMD> <devdir>\rcsdll\cmd\*.cmd <relroot>\rel2\binp\

[ BLOCK <1> clean ]
#==================
    sweep killobjs
