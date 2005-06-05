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
    <CPCMD> <DEVDIR>/sdk/rc/wres/nt386/mf_r/wres3r.lib  <RELROOT>/rel2/lib386/wresf.lib
    <CPCMD> <DEVDIR>/sdk/rc/wres/nt386/mf_s/wres3s.lib  <RELROOT>/rel2/lib386/wresfs.lib
# OSI 32-bit version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dos386/ms_r/wres3r.lib <RELROOT>/rel2/lib386/osi/wresf.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dos386/ms_s/wres3s.lib <RELROOT>/rel2/lib386/osi/wresfs.lib
# DOS 32-bit version
    <CPCMD> <DEVDIR>/sdk/rc/wres/dos386/ms_r/wres3r.lib <RELROOT>/rel2/lib386/dos/wresf.lib
    <CPCMD> <DEVDIR>/sdk/rc/wres/dos386/ms_s/wres3s.lib <RELROOT>/rel2/lib386/dos/wresfs.lib
# AXP version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/ntaxp/_s/wresaxp.lib   <RELROOT>/rel2/libaxp/
# LINUX version
    <CPCMD> <DEVDIR>/sdk/rc/wres/linux386/mf_r/wres3r.lib <RELROOT>/rel2/lib386/linux/wresf.lib
    <CPCMD> <DEVDIR>/sdk/rc/wres/linux386/mf_s/wres3s.lib <RELROOT>/rel2/lib386/linux/wresfs.lib
# QNX version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/qnx386/ms_r/wres3r.lib <RELROOT>/rel2/lib386/qnx/wresf.lib
#    <CPCMD> <DEVDIR>/sdk/rc/wres/qnx386/ms_s/wres3s.lib <RELROOT>/rel2/lib386/qnx/wresfs.lib

# DOS 16-bit version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/mm/wresm.lib    <RELROOT>/rel2/lib286/
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/ml/wresl.lib    <RELROOT>/rel2/lib286/
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/ms/wress.lib   <RELROOT>/rel2/lib286/
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/mc/wresc.lib   <RELROOT>/rel2/lib286/
#    <CPCMD> <DEVDIR>/sdk/rc/wres/dosi86/mh/wresh.lib   <RELROOT>/rel2/lib286/
# OS2 16-bit version
#    <CPCMD> <DEVDIR>/sdk/rc/wres/os2i86/ml/wresl.lib    <RELROOT>/rel2/lib286/os2/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
