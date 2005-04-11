# MAD Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
# x86
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <DEVDIR>/mad/x86/dos386/madx86.mad   <RELROOT>/rel2/binw/
#    <CPCMD> <DEVDIR>/mad/axp/dos386/madaxp.mad   <RELROOT>/rel2/binw/
#    <CPCMD> <DEVDIR>/mad/ppc/dos386/madppc.mad   <RELROOT>/rel2/binw/

  [ IFDEF (os_win "") <2*> ]
    <CPCMD> <DEVDIR>/mad/x86/wini86/madx86.dll   <RELROOT>/rel2/binw/
#    <CPCMD> <DEVDIR>/mad/axp/wini86/madaxp.dll   <RELROOT>/rel2/binw/
#    <CPCMD> <DEVDIR>/mad/ppc/wini86/madppc.dll   <RELROOT>/rel2/binw/

  [ IFDEF (os_os2 "") <2*> ]
#    <CPCMD> <DEVDIR>/mad/x86/os2i86/madx86.dll   <RELROOT>/rel2/binp/dll/
#    <CPCMD> <DEVDIR>/mad/axp/os2i86/madaxp.dll   <RELROOT>/rel2/binp/dll/
#    <CPCMD> <DEVDIR>/mad/ppc/os2i86/madppc.dll   <RELROOT>/rel2/binp/dll/
    <CPCMD> <DEVDIR>/mad/x86/os2386/madx86.d32   <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/mad/x86/os2386/madx86.sym   <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/mad/axp/os2386/madaxp.d32   <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/mad/axp/os2386/madaxp.sym   <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/mad/ppc/os2386/madppc.d32   <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/mad/ppc/os2386/madppc.sym   <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/mad/mips/os2386/madmips.d32 <RELROOT>/rel2/binp/
    <CPCMD> <DEVDIR>/mad/mips/os2386/madmips.sym <RELROOT>/rel2/binp/

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/mad/x86/nt386/madx86.dll    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/mad/x86/nt386/madx86.sym    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/mad/axp/nt386/madaxp.dll    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/mad/axp/nt386/madaxp.sym    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/mad/ppc/nt386/madppc.dll    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/mad/ppc/nt386/madppc.sym    <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/mad/mips/nt386/madmips.dll  <RELROOT>/rel2/binnt/
    <CPCMD> <DEVDIR>/mad/mips/nt386/madmips.sym  <RELROOT>/rel2/binnt/
#    <CPCMD> <DEVDIR>/mad/x86/ntaxp/madx86.dll    <RELROOT>/rel2/axpnt/
#    <CPCMD> <DEVDIR>/mad/axp/ntaxp/madaxp.dll    <RELROOT>/rel2/axpnt/
#    <CPCMD> <DEVDIR>/mad/ppc/ntaxp/madppc.dll    <RELROOT>/rel2/axpnt/
#    <CPCMD> <DEVDIR>/mad/mips/ntaxp/madmips.dll  <RELROOT>/rel2/axpnt/

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> <DEVDIR>/mad/x86/qnx386/madx86.mad   <RELROOT>/rel2/qnx/watcom/wd/
#    <CPCMD> <DEVDIR>/mad/axp/qnx386/madaxp.mad   <RELROOT>/rel2/qnx/watcom/wd/
#    <CPCMD> <DEVDIR>/mad/ppc/qnx386/madppc.mad   <RELROOT>/rel2/qnx/watcom/wd/

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <DEVDIR>/mad/x86/linux386/madx86.mad    <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/mad/x86/linux386/madx86.sym    <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/mad/axp/linux386/madaxp.mad    <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/mad/axp/linux386/madaxp.sym    <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/mad/ppc/linux386/madppc.mad    <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/mad/ppc/linux386/madppc.sym    <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/mad/mips/linux386/madmips.mad  <RELROOT>/rel2/binl/
    <CPCMD> <DEVDIR>/mad/mips/linux386/madmips.sym  <RELROOT>/rel2/binl/


[ BLOCK <1> clean ]
#==================
     pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
     cdsay <PROJDIR>

