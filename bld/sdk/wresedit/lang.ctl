# WRESEDIT Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (os_win "") <2*> ]
    <CPCMD> <DEVDIR>/sdk/wresedit/waccel/wini86/wacc.dll     <RELROOT>/binw/wacc.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wmenu/wini86/wmenu.dll     <RELROOT>/binw/wmenu.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wstring/wini86/wstring.dll <RELROOT>/binw/wstring.dll

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/sdk/wresedit/waccel/nt386/wacc.dll      <RELROOT>/binnt/wacc.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wmenu/nt386/wmenu.dll      <RELROOT>/binnt/wmenu.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wstring/nt386/wstring.dll  <RELROOT>/binnt/wstring.dll

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <DEVDIR>/sdk/wresedit/waccel/ntaxp/wacc.dll      <RELROOT>/axpnt/wacc.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wmenu/ntaxp/wmenu.dll      <RELROOT>/axpnt/wmenu.dll
    <CPCMD> <DEVDIR>/sdk/wresedit/wstring/ntaxp/wstring.dll  <RELROOT>/axpnt/wstring.dll

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
