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
    <CPCMD> <devdir>/wv/rfx/dosi86/rfx.exe <relroot>/rel2/binw/rfx.exe
    <CPCMD> <devdir>/wv/rfx/os2386/rfx.exe <relroot>/rel2/binp/rfx.exe

    <CPCMD> <devdir>/wv/dsx/dos386.nec/wv.exe <relroot>/rel2/n98bw/wd.exe

    <CPCMD> <devdir>/wv/dsx/dos386/wv.exe <relroot>/rel2/binw/wd.exe
    <CPCMD> <devdir>/wv/dsx/dos386/wv.sym <relroot>/rel2/binw/wd.sym

    <CPCMD> <devdir>/wv/os2/os2386/wv.exe <relroot>/rel2/binp/wd.exe
    <CPCMD> <devdir>/wv/os2/os2386/wv.sym <relroot>/rel2/binp/wd.sym
    <CPCMD> <devdir>/wv/os2/os2386.gui/wv.exe <relroot>/rel2/binp/wdw.exe
    <CPCMD> <devdir>/wv/os2/os2386.gui/wv.sym <relroot>/rel2/binp/wdw.sym
#   <CPCMD> <devdir>/wv/os2/os2i86/wv.exe <relroot>/rel2/binp/wd16.exe
#   <CPCMD> <devdir>/wv/os2/os2i86.gui/wv.exe <relroot>/rel2/binp/wdw16.exe

    <CPCMD> <devdir>/wv/win/winnec/wv.exe <relroot>/rel2/n98bw/wdc.exe
    <CPCMD> <devdir>/wv/win/winnec/wv.sym <relroot>/rel2/n98bw/wdc.sym

    <CPCMD> <devdir>/wv/win/wini86/wv.exe <relroot>/rel2/binw/wdc.exe
    <CPCMD> <devdir>/wv/win/wini86/wv.sym <relroot>/rel2/binw/wdc.sym
    <CPCMD> <devdir>/wv/win/wini86.gui/wv.exe <relroot>/rel2/binw/wdw.exe
    <CPCMD> <devdir>/wv/win/wini86.gui/wv.sym <relroot>/rel2/binw/wdw.sym

    <CPCMD> <devdir>/wv/win/nt386/wv.exe <relroot>/rel2/binnt/wd.exe
    <CPCMD> <devdir>/wv/win/nt386/wv.sym <relroot>/rel2/binnt/wd.sym
    <CPCMD> <devdir>/wv/win/nt386.gui/wv.exe <relroot>/rel2/binnt/wdw.exe
    <CPCMD> <devdir>/wv/win/nt386.gui/wv.sym <relroot>/rel2/binnt/wdw.sym

#    <CPCMD> <devdir>/wv/win/ntaxp/wv.exe <relroot>/rel2/axpnt/wd.exe
#    <CPCMD> <devdir>/wv/win/ntaxp/wv.sym <relroot>/rel2/axpnt/wd.sym
#    <CPCMD> <devdir>/wv/win/ntaxp.gui/wv.exe <relroot>/rel2/axpnt/wdw.exe
#    <CPCMD> <devdir>/wv/win/ntaxp.gui/wv.sym <relroot>/rel2/axpnt/wdw.sym

#    <CPCMD> <devdir>/wv/qnx/qnx386/wv.qnx <relroot>/rel2/qnx/binq/wd
#    <CPCMD> <devdir>/wv/qnx/qnx386/wv.sym <relroot>/rel2/qnx/sym/wd.sym

    <CPCMD> <devdir>/wv/linux/linux386/wv.exe <relroot>/rel2/binl/wd
    <CPCMD> <devdir>/wv/linux/linux386/wv.sym <relroot>/rel2/binl/wd.sym

    <CPCMD> <devdir>/wv/dbg/*.dbg <relroot>/rel2/binw/
    <CPCMD> <devdir>/wv/ssl/*.prs <relroot>/rel2/binw/

    <CPCMD> <devdir>/wv/dbg/*.dbg <relroot>/rel2/qnx/watcom/wd/
    <CPCMD> <devdir>/wv/ssl/*.prs <relroot>/rel2/qnx/watcom/wd/

    <CPCMD> <devdir>/wv/dbg/*.dbg <relroot>/rel2/binl/
    <CPCMD> <devdir>/wv/ssl/*.prs <relroot>/rel2/binl/


[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
