# MISC Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    cdsay stubs
    wmake -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
#   <CPCMD> wcc386p.exe <relroot>\rel2\binw\wcc386p.exe
#   <CPCMD> wfc386p.exe <relroot>\rel2\binw\wfc386p.exe
#   <CPCMD> wccx.exe <relroot>\rel2\binw\wccp.exe
#   <CPCMD> wfcp.exe <relroot>\rel2\binw\wfcp.exe
#   <CPCMD> wlinkp.exe <relroot>\rel2\binw\wlinkp.exe

#   <CPCMD> <devdir>\misc\dos4gw.doc <relroot>\rel2\
#   <CPCMD> <devdir>\misc\dos4gwj.doc <relroot>\rel2\
    <CPCMD> <devdir>\misc\unicode.* <relroot>\rel2\binw\

    <CPCMD> <devdir>\misc\watcom.ico <relroot>\rel2\

    <CPCMD> <devdir>\watcom\lib\wresf.lib <relroot>\rel2\lib386\
    <CPCMD> <devdir>\watcom\lib\wresfs.lib <relroot>\rel2\lib386\
    <CPCMD> <devdir>\watcom\lib\wresm.lib <relroot>\rel2\lib286\
    <CPCMD> <devdir>\watcom\lib\wresl.lib <relroot>\rel2\lib286\

[ BLOCK <1> clean ]
#==================
    sweep killobjs
