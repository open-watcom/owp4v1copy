# WDISASM Builder Control file
# ============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#<CPCMD> <projdir>\qnx386\wdis.qnx <relroot>\rel2\qnx\binq\wdis
#<CPCMD> <projdir>\qnx386\wdis.sym <relroot>\rel2\qnx\sym\
<CPCMD> <projdir>\nt386\wdis.exe <relroot>\rel2\binnt\
<CPCMD> <projdir>\nt386\wdis.sym <relroot>\rel2\binnt\
<CPCMD> <projdir>\os2386\wdis.exe <relroot>\rel2\binp\
<CPCMD> <projdir>\os2386\wdis.sym <relroot>\rel2\binp\
#<CPCMD> <projdir>\osi386\wdis.exe <relroot>\rel2\binw\
#<CPCMD> <projdir>\osi386\wdis.sym <relroot>\rel2\binw\
<CPCMD> <projdir>\dos386\wdis.exe <relroot>\rel2\binw\
<CPCMD> <projdir>\dos386\wdis.sym <relroot>\rel2\binw\
#<CPCMD> <projdir>\ntaxp\wdis.exe <relroot>\rel2\axpnt\
#<CPCMD> <projdir>\ntaxp\wdis.sym <relroot>\rel2\axpnt\

[ BLOCK <1> clean ]
#==================
pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
