# WRESEDIT Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
#waccel
    <CPCMD> <DEVDIR>/sdk/wresedit/waccel/wini86/wacc.dll <relroot>/rel2/binw/wacc.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/waccel/nt386/wacc.dll  <relroot>/rel2/binnt/wacc.dll
#    <CPCMD> <DEVDIR>/sdk/wresedit/waccel/ntaxp/wacc.dll  <relroot>/rel2/axpnt/wacc.dll
#wmenu
    <CPCMD> <DEVDIR>/sdk/wresedit/wmenu/wini86/wmenu.dll <relroot>/rel2/binw/wmenu.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wmenu/nt386/wmenu.dll  <relroot>/rel2/binnt/wmenu.dll
#    <CPCMD> <DEVDIR>/sdk/wresedit/wmenu/ntaxp/wmenu.dll  <relroot>/rel2/axpnt/wmenu.dll
#wstring
    <CPCMD> <DEVDIR>/sdk/wresedit/wstring/wini86/wstring.dll <relroot>/rel2/binw/wstring.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wstring/nt386/wstring.dll  <relroot>/rel2/binnt/wstring.dll
#    <CPCMD> <DEVDIR>/sdk/wresedit/wstring/ntaxp/wstring.dll  <relroot>/rel2/axpnt/wstring.dll

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
