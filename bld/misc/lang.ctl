# MISC Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#   <CPCMD> wcc386p.exe <RELROOT>/rel2/binw/wcc386p.exe
#   <CPCMD> wfc386p.exe <RELROOT>/rel2/binw/wfc386p.exe
#   <CPCMD> wccx.exe <RELROOT>/rel2/binw/wccp.exe
#   <CPCMD> wfcp.exe <RELROOT>/rel2/binw/wfcp.exe
#   <CPCMD> wlinkp.exe <RELROOT>/rel2/binw/wlinkp.exe

#   <CPCMD> <DEVDIR>/misc/dos4gw.doc <RELROOT>/rel2/
#   <CPCMD> <DEVDIR>/misc/dos4gwj.doc <RELROOT>/rel2/
    <CPCMD> <DEVDIR>/misc/unicode.* <RELROOT>/rel2/binw/

    <CPCMD> <DEVDIR>/misc/watcom.ico <RELROOT>/rel2/

    <CPCMD> <DEVDIR>/sdk/rc/wres/lib/wresf.lib <RELROOT>/rel2/lib386/
    <CPCMD> <DEVDIR>/sdk/rc/wres/lib/wresfs.lib <RELROOT>/rel2/lib386/
    <CPCMD> <DEVDIR>/sdk/rc/wres/lib/wresm.lib <RELROOT>/rel2/lib286/
    <CPCMD> <DEVDIR>/sdk/rc/wres/lib/wresl.lib <RELROOT>/rel2/lib286/

    <CPCMD> <DEVDIR>/misc/unicode.* <RELROOT>/rel2/binl/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
