# Browser Builder Control file
# ============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]

cdsay <PROJDIR>/contain/lib/objwin
wmake -i -h
cdsay <PROJDIR>/contain/lib/obj32
wmake -i -h
cdsay <PROJDIR>/dlgprs/o
wmake -i -h
cdsay <PROJDIR>/gen
wmake -i -h
cd <PROJDIR>
pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>/browser/wini86/wbrw.exe <relroot>/rel2/binw/wbrw.exe
    <CPCMD> <devdir>/browser/nt386/wbrw.exe  <relroot>/rel2/binnt/wbrw.exe
#    <CPCMD> <devdir>/browser/ntaxp/wbrw.exe  <relroot>/rel2/axpnt/wbrw.exe
    <CPCMD> <devdir>/browser/os2386/wbrw.exe <relroot>/rel2/binp/wbrw.exe
    <CPCMD> <devdir>/browser/brg/dos386/wbrg.exe <relroot>/rel2/binw/wbrg.exe
    <CPCMD> <devdir>/browser/brg/os2386/wbrg.exe <relroot>/rel2/binp/wbrg.exe
    <CPCMD> <devdir>/browser/brg/nt386/wbrg.exe  <relroot>/rel2/binnt/wbrg.exe
#    <CPCMD> <devdir>/browser/brg/ntaxp/wbrg.exe  <relroot>/rel2/axpnt/wbrg.exe

[ BLOCK <1> clean ]
#==================
   cdsay <PROJDIR>/contain/lib/objwin
   wmake -i -h clean
   cdsay <PROJDIR>/contain/lib/obj32
   wmake -i -h clean
   cdsay <PROJDIR>/dlgprs/o
   wmake -i -h clean
   cd <PROJDIR>
   pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
