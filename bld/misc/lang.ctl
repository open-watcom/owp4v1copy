# MISC Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#   <CPCMD> wcc386p.exe <relroot>/rel2/binw/wcc386p.exe
#   <CPCMD> wfc386p.exe <relroot>/rel2/binw/wfc386p.exe
#   <CPCMD> wccx.exe <relroot>/rel2/binw/wccp.exe
#   <CPCMD> wfcp.exe <relroot>/rel2/binw/wfcp.exe
#   <CPCMD> wlinkp.exe <relroot>/rel2/binw/wlinkp.exe

#   <CPCMD> <DEVDIR>/misc/dos4gw.doc <relroot>/rel2/
#   <CPCMD> <DEVDIR>/misc/dos4gwj.doc <relroot>/rel2/
    <CPCMD> <DEVDIR>/misc/unicode.* <relroot>/rel2/binw/

    <CPCMD> <DEVDIR>/misc/watcom.ico <relroot>/rel2/

    <CPCMD> <DEVDIR>/watcom/lib/wresf.lib <relroot>/rel2/lib386/
    <CPCMD> <DEVDIR>/watcom/lib/wresfs.lib <relroot>/rel2/lib386/
    <CPCMD> <DEVDIR>/watcom/lib/wresm.lib <relroot>/rel2/lib286/
    <CPCMD> <DEVDIR>/watcom/lib/wresl.lib <relroot>/rel2/lib286/

    <CPCMD> <DEVDIR>/misc/unicode.* <relroot>/rel2/binl/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
