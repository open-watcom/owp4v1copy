# WATCOM Debugger Control file
# ============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/wv/rfx/dosi86/rfx.exe <RELROOT>/rel2/binw/rfx.exe
    <CPCMD> <DEVDIR>/wv/rfx/os2386/rfx.exe <RELROOT>/rel2/binp/rfx.exe

    <CPCMD> <DEVDIR>/wv/dsx/dos386.nec/wv.exe <RELROOT>/rel2/n98bw/wd.exe

    <CPCMD> <DEVDIR>/wv/dsx/dos386/wv.exe <RELROOT>/rel2/binw/wd.exe
    <CPCMD> <DEVDIR>/wv/dsx/dos386/wv.sym <RELROOT>/rel2/binw/wd.sym

    <CPCMD> <DEVDIR>/wv/os2/os2386/wv.exe <RELROOT>/rel2/binp/wd.exe
    <CPCMD> <DEVDIR>/wv/os2/os2386/wv.sym <RELROOT>/rel2/binp/wd.sym
    <CPCMD> <DEVDIR>/wv/os2/os2386.gui/wv.exe <RELROOT>/rel2/binp/wdw.exe
    <CPCMD> <DEVDIR>/wv/os2/os2386.gui/wv.sym <RELROOT>/rel2/binp/wdw.sym
#   <CPCMD> <DEVDIR>/wv/os2/os2i86/wv.exe <RELROOT>/rel2/binp/wd16.exe
#   <CPCMD> <DEVDIR>/wv/os2/os2i86.gui/wv.exe <RELROOT>/rel2/binp/wdw16.exe

    <CPCMD> <DEVDIR>/wv/win/winnec/wv.exe <RELROOT>/rel2/n98bw/wdc.exe
    <CPCMD> <DEVDIR>/wv/win/winnec/wv.sym <RELROOT>/rel2/n98bw/wdc.sym

    <CPCMD> <DEVDIR>/wv/win/wini86/wv.exe <RELROOT>/rel2/binw/wdc.exe
    <CPCMD> <DEVDIR>/wv/win/wini86/wv.sym <RELROOT>/rel2/binw/wdc.sym
    <CPCMD> <DEVDIR>/wv/win/wini86.gui/wv.exe <RELROOT>/rel2/binw/wdw.exe
    <CPCMD> <DEVDIR>/wv/win/wini86.gui/wv.sym <RELROOT>/rel2/binw/wdw.sym

    <CPCMD> <DEVDIR>/wv/win/nt386/wv.exe <RELROOT>/rel2/binnt/wd.exe
    <CPCMD> <DEVDIR>/wv/win/nt386/wv.sym <RELROOT>/rel2/binnt/wd.sym
    <CPCMD> <DEVDIR>/wv/win/nt386.gui/wv.exe <RELROOT>/rel2/binnt/wdw.exe
    <CPCMD> <DEVDIR>/wv/win/nt386.gui/wv.sym <RELROOT>/rel2/binnt/wdw.sym

#    <CPCMD> <DEVDIR>/wv/win/ntaxp/wv.exe <RELROOT>/rel2/axpnt/wd.exe
#    <CPCMD> <DEVDIR>/wv/win/ntaxp/wv.sym <RELROOT>/rel2/axpnt/wd.sym
#    <CPCMD> <DEVDIR>/wv/win/ntaxp.gui/wv.exe <RELROOT>/rel2/axpnt/wdw.exe
#    <CPCMD> <DEVDIR>/wv/win/ntaxp.gui/wv.sym <RELROOT>/rel2/axpnt/wdw.sym

#    <CPCMD> <DEVDIR>/wv/qnx/qnx386/wv.qnx <RELROOT>/rel2/qnx/binq/wd
#    <CPCMD> <DEVDIR>/wv/qnx/qnx386/wv.sym <RELROOT>/rel2/qnx/sym/wd.sym

    <CPCMD> <DEVDIR>/wv/linux/linux386/wv.exe <RELROOT>/rel2/binl/wd
    <CPCMD> <DEVDIR>/wv/linux/linux386/wv.sym <RELROOT>/rel2/binl/wd.sym

    <CPCMD> <DEVDIR>/wv/dbg/*.dbg <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/wv/ssl/*.prs <RELROOT>/rel2/binw/

    <CPCMD> <DEVDIR>/wv/dbg/*.dbg <RELROOT>/rel2/qnx/watcom/wd/
    <CPCMD> <DEVDIR>/wv/ssl/*.prs <RELROOT>/rel2/qnx/watcom/wd/

    <CPCMD> <DEVDIR>/wv/dbg/*.dbg <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/wv/ssl/*.prs <RELROOT>/rel2/binl/


[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
