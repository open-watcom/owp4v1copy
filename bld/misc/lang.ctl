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
    <CPCMD> <DEVDIR>/misc/unicode.* <RELROOT>/rel2/binl/

# NT, OS2 32-bit version
    <CPCMD> <DEVDIR>/sdk/rc/wres/flat386/mf_r/wres.lib  <RELROOT>/rel2/lib386/wresf.lib
    <CPCMD> <DEVDIR>/sdk/rc/wres/flat386/mf_s/wres.lib  <RELROOT>/rel2/lib386/wresfs.lib
# OSI 32-bit version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/small386/ms_r/wres.lib <RELROOT>/rel2/lib386/osi/wresf.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/small386/ms_s/wres.lib <RELROOT>/rel2/lib386/osi/wresfs.lib
# DOS 32-bit version
    <CPCMD> <DEVDIR>/sdk/rc/wres/small386/ms_r/wres.lib <RELROOT>/rel2/lib386/dos/wresf.lib
    <CPCMD> <DEVDIR>/sdk/rc/wres/small386/ms_s/wres.lib <RELROOT>/rel2/lib386/dos/wresfs.lib
# AXP version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/ntaxp/_s/wres.lib   <RELROOT>/rel2/libaxp/
# LINUX version
    <CPCMD> <DEVDIR>/sdk/rc/wres/linux386/mf_r/wres.lib <RELROOT>/rel2/lib386/linux/wresf.lib
    <CPCMD> <DEVDIR>/sdk/rc/wres/linux386/mf_s/wres.lib <RELROOT>/rel2/lib386/linux/wresfs.lib
# QNX version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/qnx386/ms_r/wres.lib <RELROOT>/rel2/lib386/qnx/wresf.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/qnx386/ms_s/wres.lib <RELROOT>/rel2/lib386/qnx/wresfs.lib

# DOS 16-bit version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/mm/wres.lib    <RELROOT>/rel2/lib286/wresm.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/ml/wres.lib    <RELROOT>/rel2/lib286/wresl.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/ms/wres.lib   <RELROOT>/rel2/lib286/wress.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/mc/wres.lib   <RELROOT>/rel2/lib286/wresc.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/mh/wres.lib   <RELROOT>/rel2/lib286/wresh.lib
# OS2 16-bit version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/os2i86/ml/wres.lib    <RELROOT>/rel2/lib286/os2/wresl.lib

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
