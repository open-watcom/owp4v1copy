# SAMP Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
#   <CPCMD> bin/wsampecs.exe <relroot>/rel2/binw/wsampecs.exe
    <CPCMD> dos386/wsample.exe <relroot>/rel2/binw/wsample.exe
    <CPCMD> rsii86/wsamprsi.exe <relroot>/rel2/binw/wsamprsi.exe
    <CPCMD> pls386/wsamppls.exp <relroot>/rel2/binw/wsamppls.exp
    <CPCMD> wini86/wsamplew.exe <relroot>/rel2/binw/wsamplew.exe
#    <CPCMD> os2i86/wsampos2.exe <relroot>/rel2/binp/wsampos2.exe
    <CPCMD> os2386/wsmpos22.exe <relroot>/rel2/binp/wsmpos22.exe
    <CPCMD> nt386/wsmpnt.exe <relroot>/rel2/binnt/wsample.exe
#    <CPCMD> ntaxp/wsmpaxp.exe <relroot>/rel2/axpnt/wsample.exe
    <CPCMD> nlm/wsample.nlm <relroot>/rel2/nlm/wsample.nlm
#    <CPCMD> dosi86.nec/wsample.exe <relroot>/rel2/n98bw/wsample.exe
    <CPCMD> rsii86.nec/wsamprsi.exe <relroot>/rel2/n98bw/wsamprsi.exe
    <CPCMD> pls386.nec/wsamppls.exp <relroot>/rel2/n98bw/wsamppls.exp

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>
