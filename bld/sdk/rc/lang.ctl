# RC Builder Control file
# =======================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#========================
    cd wres
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/mkcdpg
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/exedmp
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>/rc
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>/sdk/rc/rc/os2386/wrc.exe <relroot>/rel2/binp/wrc.exe
    <CPCMD> <devdir>/sdk/rc/rc/nt386/wrc.exe <relroot>/rel2/binnt/wrc.exe
    <CPCMD> <devdir>/sdk/rc/rc/dos386/wrc.exe <relroot>/rel2/binw/wrc.exe
#    <CPCMD> <devdir>/sdk/rc/rc/ntaxp/wrc.exe <relroot>/rel2/axpnt/wrc.exe
#    <CPCMD> <devdir>/sdk/rc/rc/osi386/wrc.exe <relroot>/rel2/binw/wrc.exe
    <CPCMD> <devdir>/sdk/rc/rc/nt386.dll/wrc.dll <relroot>/rel2/binnt/wrc.dll
#    <CPCMD> <devdir>/sdk/rc/rc/os2386.dll/wrc.dll <relroot>/rel2/binp/dll/wrc.dll
    <CPCMD> <devdir>/sdk/rc/rc/*.uni <relroot>/rel2/binw/
    <CPCMD> <devdir>/sdk/rc/mkcdpg/nt386/mkcdpg.exe <relroot>/rel2/binnt/mkcdpg.exe
#    <CPCMD> <devdir>/sdk/rc/mkcdpg/ntaxp/mkcdpg.exe <relroot>/rel2/axpnt/mkcdpg.exe
    <CPCMD> <devdir>/sdk/rc/exedmp/nt386/exedmp.exe <relroot>/rel2/binnt/exedmp.exe
    <CPCMD> <devdir>/sdk/rc/exedmp/os2386/exedmp.exe <relroot>/rel2/binp/exedmp.exe
    <CPCMD> <devdir>/sdk/rc/rc/nt386rt.dll/wrc.dll <relroot>/rel2/binnt/rtdll/wrc.dll
# linux
    <CPCMD> <devdir>/sdk/rc/rc/linux386/wrc.exe <relroot>/rel2/binl/wrc
    <CPCMD> <devdir>/sdk/rc/rc/linux386/wrc.sym <relroot>/rel2/binl/wrc.sym
    <CPCMD> <devdir>/sdk/rc/rc/*.uni <relroot>/rel2/binl/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
