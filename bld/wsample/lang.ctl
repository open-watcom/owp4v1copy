# SAMP Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]
    
[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
# fixme: not all done yet
    cdsay <projdir>\DOS
    wmake -h 
    cdsay <projdir>\NEC
    wmake -h 
    cdsay <projdir>\NLM
    wmake -h 
    cdsay <projdir>\PLS
    wmake -h 
    cdsay <projdir>\RSI
    wmake -h 
    cdsay <projdir>\WIN
    wmake -h 
    cdsay <projdir>

[ BLOCK <1> rel2 cprel2 ]
#========================
#   <CPCMD> bin\wsampecs.exe <relroot>\rel2\binw\wsampecs.exe
    <CPCMD> bin\wsample.exe <relroot>\rel2\binw\wsample.exe
    <CPCMD> bin\wsamprsi.exe <relroot>\rel2\binw\wsamprsi.exe
    <CPCMD> bin\wsamplew.exe <relroot>\rel2\binw\wsamplew.exe
	<CPCMD> bin\wsampos2.exe <relroot>\rel2\binp\wsampos2.exe
	<CPCMD> bin\wsmpos22.exe <relroot>\rel2\binp\wsmpos22.exe
	<CPCMD> bin\wsmpnt.exe <relroot>\rel2\binnt\wsample.exe
#	<CPCMD> bin\wsmpaxp.exe <relroot>\rel2\axpnt\wsample.exe
    <CPCMD> bin\wsamppls.exp <relroot>\rel2\binw\wsamppls.exp
    <CPCMD> bin\wsample.nlm <relroot>\rel2\nlm\wsample.nlm
    <CPCMD> bin\nsamprsi.exe <relroot>\rel2\n98bw\wsamprsi.exe
    <CPCMD> bin\nsamppls.exp <relroot>\rel2\n98bw\wsamppls.exp
    
[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <projdir>\DOS
    wmake -h clean
    cdsay <projdir>\NEC
    wmake -h clean 
    cdsay <projdir>\NLM
    wmake -h clean 
    cdsay <projdir>\PLS
    wmake -h clean 
    cdsay <projdir>\RSI
    wmake -h clean 
    cdsay <projdir>\WIN
    wmake -h clean 
    cdsay <projdir>
