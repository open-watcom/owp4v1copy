# Post Script Documentation Builder Control file
# =======================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#    <CPCMD> <devdir>\docs\targs\ps\cl.ps <relroot>\rel2\binp\wrc.exe
#    <CPCMD> <devdir>\sdk\rc\rc\nt\wrc.exe <relroot>\rel2\binnt\wrc.exe
#    <CPCMD> <devdir>\sdk\rc\rc\386\wrc.exe <relroot>\rel2\binw\wrc.exe
#    <CPCMD> <devdir>\sdk\rc\rc\ntdll\wrc.dll <relroot>\rel2\binnt\wrc.dll
#    <CPCMD> <devdir>\sdk\rc\rc\kanji.uni <relroot>\rel2\binw\kanji.uni
#    <CPCMD> <devdir>\sdk\rc\rc\936.uni <relroot>\rel2\binw\936.uni
#    <CPCMD> <devdir>\sdk\rc\rc\949.uni <relroot>\rel2\binw\949.uni
#    <CPCMD> <devdir>\sdk\rc\rc\1252.uni <relroot>\rel2\binw\1250.uni
#    <CPCMD> <devdir>\sdk\rc\rc\1250.uni <relroot>\rel2\binw\1251.uni
#    <CPCMD> <devdir>\sdk\rc\rc\1251.uni <relroot>\rel2\binw\1252.uni
#    <CPCMD> <devdir>\sdk\rc\mkcdpg\nt386\mkcdpg.exe <relroot>\rel2\binnt\mkcdpg.exe
#    <CPCMD> <devdir>\sdk\rc\mkcdpg\ntaxp\mkcdpg.exe <relroot>\rel2\axpnt\mkcdpg.exe
#    <CPCMD> <devdir>\sdk\rc\rc\ntrtdll\wrc.dll <relroot>\rel2\binnt\rtdll\wrc.dll

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean

