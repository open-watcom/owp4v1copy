# WIN386 Builder Control file
# ===========================

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
    <CPCMD> ext/win386.ext <relroot>/rel2/binw/win386.ext
    <CPCMD> dll/w386dll.ext <relroot>/rel2/binw/w386dll.ext
    <CPCMD> conv/obj/win386.lib <relroot>/rel2/lib386/win/win386.lib
    <CPCMD> vxd/wdebug.386 <relroot>/rel2/binw/wdebug.386
    <CPCMD> vxd/wemu387.386 <relroot>/rel2/binw/wemu387.386
    <CPCMD> wbind/dosi86/wbind.exe <relroot>/rel2/binw/wbind.exe
    <CPCMD> wbind/os2i86/wbind.exe <relroot>/rel2/binp/wbind.exe
    <CPCMD> wbind/nt386/wbind.exe <relroot>/rel2/binnt/wbind.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    @rm -f asm/dllthk.asm c/dllthunk.c asm/winglue.asm h/winglue.inc
    @rm -f ext/win386.ext dll/w386dll.ext
    @rm -f vxd/wdebug.386 vxd/wemu387.386
