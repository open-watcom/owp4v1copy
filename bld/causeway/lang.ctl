# WDISASM Builder Control file
# ============================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    [ INCLUDE prereq.ctl ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 ]
#=================
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <PROJDIR>/cwc.exe                 <RELROOT>/binw/
    <CPCMD> <PROJDIR>/cw32/dosi86/cwstub.exe  <RELROOT>/binw/
    <CPCMD> <PROJDIR>/cw32/dosi86/cwdstub.exe <RELROOT>/binw/
    <CPCMD> <PROJDIR>/cw32/dos386/cwdll.lib   <RELROOT>/lib386/dos/
    <CPCMD> <PROJDIR>/inc/cwdllfnc.h          <RELROOT>/h/

[ BLOCK <1> clean ]
#==================
    set TMP_BUILD_PLATFORM=<BUILD_PLATFORM>
    set BUILD_PLATFORM=<BUILD_PLATFORM>clean
    [ INCLUDE prereq.ctl ]
    set BUILD_PLATFORM=<TMP_BUILD_PLATFORM>
    set TMP_BUILD_PLATFORM=
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    rm -r -f <OWBINDIR>/bcwc.exe
    rm -r -f <OWBINDIR>/bcwc

[ BLOCK . . ]
#============

cdsay <PROJDIR>
