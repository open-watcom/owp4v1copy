# EditDLL Builder Control file
# ============================

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
    <CPCMD> <devdir>\editdll\epm\wedit.dll  <relroot>\rel2\binp\dll\weditepm.dll

    <CPCMD> <devdir>\editdll\viw\wini86\wedit.dll <relroot>\rel2\binw\weditviw.dll
    <CPCMD> <devdir>\editdll\viw\nt386\wedit.dll <relroot>\rel2\binnt\weditviw.dll

    <CPCMD> <devdir>\editdll\cw\wini86\wedit.dll <relroot>\rel2\binw\weditcw.dll
    <CPCMD> <devdir>\editdll\cw\nt386\wedit.dll <relroot>\rel2\binnt\weditcw.dll

    <CPCMD> <devdir>\editdll\wedit.h <relroot>\rel2\src\editdll\wedit.h
    <CPCMD> <devdir>\editdll\wedit.doc <relroot>\rel2\src\editdll\wedit.doc

    <CPCMD> <devdir>\editdll\cw\nt386\wedit.lib <relroot>\rel2\src\editdll\nt\wedit.lib
    <CPCMD> <devdir>\editdll\cw\wini86\wedit.lib <relroot>\rel2\src\editdll\win\wedit.lib
    <CPCMD> <devdir>\editdll\epm\wedit.lib <relroot>\rel2\src\editdll\os2\wedit.lib

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>
