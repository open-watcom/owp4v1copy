# VI Builder Control file
# =======================
set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#========================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h bind=1

[ BLOCK <1> rel2 cprel2 ]
#========================
#   <CPCMD> <devdir>/vi/obj286/vi.exe <relroot>/rel2/binw/vi286.exe
    <CPCMD> <devdir>/vi/dos386/vi386.exe <relroot>/rel2/binw/vi.exe
    <CPCMD> <devdir>/vi/wini86/viw.exe <relroot>/rel2/binw/viw.exe
#    <CPCMD> <devdir>/vi/obj286v/viv.exe <relroot>/rel2/binw/vir.exe
    <CPCMD> <devdir>/vi/win386/viwp.exe <relroot>/rel2/binw/viwp.exe
    <CPCMD> <devdir>/vi/nt386/vint.exe <relroot>/rel2/binnt/vi.exe
    <CPCMD> <devdir>/vi/nt386.win/vintw.exe <relroot>/rel2/binnt/viw.exe
#    <CPCMD> <devdir>/vi/ntaxp/viaxp.exe <relroot>/rel2/axpnt/vi.exe
#    <CPCMD> <devdir>/vi/ntaxp.win/viaxpw.exe <relroot>/rel2/axpnt/viw.exe
#   <CPCMD> <devdir>/vi/os2i86/vi2r.exe <relroot>/rel2/binp/vi16.exe
    <CPCMD> <devdir>/vi/os2386/vi2.exe <relroot>/rel2/binp/vi.exe
#    <CPCMD> <devdir>/vi/qnxi86/viq.qnx <relroot>/rel2/qnx/binq/vi.
    <CPCMD> <devdir>/vi/linux386/vil.exe <relroot>/rel2/binl/vi.
    <CPCMD> <devdir>/vi/ctags/os2386/ctags.exe <relroot>/rel2/binp/ctags.exe
    <CPCMD> <devdir>/vi/ctags/dos386/ctags.exe <relroot>/rel2/binw/ctags.exe
    <CPCMD> <devdir>/vi/ctags/nt386/ctags.exe <relroot>/rel2/binnt/ctags.exe
#    <CPCMD> <devdir>/vi/ctags/ntaxp/ctags.exe <relroot>/rel2/axpnt/ctags.exe
    <CPCMD> <devdir>/vi/bind/dosi86/edbind.exe <relroot>/rel2/binw/edbind.exe
    <CPCMD> <devdir>/vi/bind/nt386/edbind.exe <relroot>/rel2/binnt/edbind.exe
#    <CPCMD> <devdir>/vi/bind/os2386/edbind.exe <relroot>/rel2/binp/edbind.exe
#    <CPCMD> <devdir>/vi/bind/linux386/edbind.exe <relroot>/rel2/binl/edbind.

    <CPCMD> <devdir>/vi/dat/v*.cmd <relroot>/rel2/binp/
    <CPCMD> <devdir>/vi/dat/v*.bat <relroot>/rel2/binw/
    <CPCMD> <devdir>/vi/dat/*.vi <relroot>/rel2/eddat/
    <CPCMD> <devdir>/vi/dat/*.cfg <relroot>/rel2/eddat/
    <CPCMD> <devdir>/vi/dat/*.dat <relroot>/rel2/eddat/
    <CPCMD> <devdir>/vi/dat/*.ini <relroot>/rel2/eddat/
    <CPCMD> <devdir>/vi/dat/bindvi.* <relroot>/rel2/eddat/
#   <CPCMD> <devdir>/vi/dat/weditor.ini <relroot>/rel2/binw/
#   <CPCMD> <relroot>/rel2/weditor.ini <relroot>/rel2/binnt/

    <CPCMD> <devdir>/vi/doc/*.hlp <relroot>/rel2/eddat/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
