# MAD Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
# x86
    <CPCMD> <devdir>/mad/x86/dos386/madx86.mad   <relroot>/rel2/binw/
    <CPCMD> <devdir>/mad/x86/wini86/madx86.dll   <relroot>/rel2/binw/
#    <CPCMD> <devdir>/mad/x86/os2i86/madx86.dll   <relroot>/rel2/binp/dll/
    <CPCMD> <devdir>/mad/x86/os2386/madx86.d32   <relroot>/rel2/binp/
    <CPCMD> <devdir>/mad/x86/nt386/madx86.dll    <relroot>/rel2/binnt/
#    <CPCMD> <devdir>/mad/x86/ntaxp/madx86.dll    <relroot>/rel2/axpnt/
#    <CPCMD> <devdir>/mad/x86/qnx386/madx86.mad   <relroot>/rel2/qnx/watcom/wd/
    <CPCMD> <devdir>/mad/x86/linux386/madx86.mad <relroot>/rel2/binl/
# axp
#    <CPCMD> <devdir>/mad/axp/dos386/madaxp.mad   <relroot>/rel2/binw/
#    <CPCMD> <devdir>/mad/axp/wini86/madaxp.dll   <relroot>/rel2/binw/
#    <CPCMD> <devdir>/mad/axp/os2i86/madaxp.dll   <relroot>/rel2/binp/dll/
#    <CPCMD> <devdir>/mad/axp/os2386/madaxp.d32   <relroot>/rel2/binp/
#    <CPCMD> <devdir>/mad/axp/nt386/madaxp.dll    <relroot>/rel2/binnt/
#    <CPCMD> <devdir>/mad/axp/ntaxp/madaxp.dll    <relroot>/rel2/axpnt/
#    <CPCMD> <devdir>/mad/axp/qnx386/madaxp.mad   <relroot>/rel2/qnx/watcom/wd/
#    <CPCMD> <devdir>/mad/axp/linux386/madaxp.mad <relroot>/rel2/binl/
# ppc
#    <CPCMD> <devdir>/mad/ppc/dos386/madppc.mad   <relroot>/rel2/binw/
#    <CPCMD> <devdir>/mad/ppc/wini86/madppc.dll   <relroot>/rel2/binw/
#    <CPCMD> <devdir>/mad/ppc/os2i86/madppc.dll   <relroot>/rel2/binp/dll/
#    <CPCMD> <devdir>/mad/ppc/os2386/madppc.d32   <relroot>/rel2/binp/
#    <CPCMD> <devdir>/mad/ppc/nt386/madppc.dll    <relroot>/rel2/binnt/
#    <CPCMD> <devdir>/mad/ppc/ntaxp/madppc.dll    <relroot>/rel2/axpnt/
#    <CPCMD> <devdir>/mad/ppc/qnx386/madppc.mad   <relroot>/rel2/qnx/watcom/wd/
#    <CPCMD> <devdir>/mad/ppc/linux386/madppc.mad <relroot>/rel2/binl/


[ BLOCK <1> clean ]
#==================
     pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
