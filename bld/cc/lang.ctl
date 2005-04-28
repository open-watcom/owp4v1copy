# cfe Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> dos386.386/wcc386c.exe <RELROOT>/rel2/binw/wcc386.exe
    <CPCMD> dos386.386/wcc386c.sym <RELROOT>/rel2/binw/wcc386.sym
    <CPCMD> dos386.386/wcc38601.int <RELROOT>/rel2/binw/wcc38601.int
    <CPCMD> dos386.i86/wcci86c.exe <RELROOT>/rel2/binw/wcc.exe
    <CPCMD> dos386.i86/wcci86c.sym <RELROOT>/rel2/binw/wcc.sym
    <CPCMD> dos386.i86/wcci8601.int <RELROOT>/rel2/binw/wcci8601.int
#    <CPCMD> dos386.axp/wccaxpe.exe <RELROOT>/rel2/binnt/wccaxp.exe

#  [ IFDEF (os_osi "") <2*> ]    #osi stuff
#    <CPCMD> osi386.386/wcc386c.exe <RELROOT>/rel2/binw/wcc386.exe
#    <CPCMD> osi386.386/wcc386c.sym <RELROOT>/rel2/binw/wcc386.sym
#    <CPCMD> osi386.386/wcc38601.int <RELROOT>/rel2/binw/wcc38601.int
#    <CPCMD> osi386.i86/wcci86c.exe <RELROOT>/rel2/binw/wcc.exe
#    <CPCMD> osi386.i86/wcci86c.sym <RELROOT>/rel2/binw/wcc.sym
#    <CPCMD> osi386.i86/wcci8601.int <RELROOT>/rel2/binw/wcci8601.int
#    <CPCMD> osi386.axp/wccaxpc.exe <RELROOT>/rel2/binw/wccaxp.exe
#    <CPCMD> osi386.ppc/wccppcc.exe <RELROOT>/rel2/binw/wccppc.exe

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> os2386.386/wcc386e.exe <RELROOT>/rel2/binp/wcc386.exe
    <CPCMD> os2386.386/wcc386e.sym <RELROOT>/rel2/binp/wcc386.sym
    <CPCMD> os2386.386/wccd386e.dll <RELROOT>/rel2/binp/dll/wccd386.dll
    <CPCMD> os2386.386/wccd386e.sym <RELROOT>/rel2/binp/dll/wccd386.sym
    <CPCMD> os2386.386/wcc38601.int <RELROOT>/rel2/binp/dll/wcc38601.int
    <CPCMD> os2386.i86/wcci86e.exe <RELROOT>/rel2/binp/wcc.exe
    <CPCMD> os2386.i86/wcci86e.sym <RELROOT>/rel2/binp/wcc.sym
    <CPCMD> os2386.i86/wccdi86e.dll <RELROOT>/rel2/binp/dll/wccd.dll
    <CPCMD> os2386.i86/wccdi86e.sym <RELROOT>/rel2/binp/dll/wccd.sym
    <CPCMD> os2386.i86/wcci8601.int <RELROOT>/rel2/binp/dll/wcci8601.int
    <CPCMD> os2386.axp/wccaxpe.exe <RELROOT>/rel2/binp/wccaxp.exe
    <CPCMD> os2386.axp/wccaxpe.sym <RELROOT>/rel2/binp/wccaxp.sym
    <CPCMD> os2386.axp/wccdaxpe.dll <RELROOT>/rel2/binp/dll/wccdaxp.dll
    <CPCMD> os2386.axp/wccdaxpe.sym <RELROOT>/rel2/binp/dll/wccdaxp.sym
    <CPCMD> os2386.axp/wccaxp01.int <RELROOT>/rel2/binp/wccaxp01.int
    <CPCMD> os2386.ppc/wccppce.exe <RELROOT>/rel2/binp/wccppc.exe
    <CPCMD> os2386.ppc/wccppce.sym <RELROOT>/rel2/binp/wccppc.sym
    <CPCMD> os2386.ppc/wccdppce.dll <RELROOT>/rel2/binp/dll/wccdppc.dll
    <CPCMD> os2386.ppc/wccdppce.sym <RELROOT>/rel2/binp/dll/wccdppc.sym
    <CPCMD> os2386.ppc/wccppc01.int <RELROOT>/rel2/binp/wccppc01.int
    <CPCMD> os2386.mps/wccmpse.exe <RELROOT>/rel2/binp/wccmps.exe
    <CPCMD> os2386.mps/wccmpse.sym <RELROOT>/rel2/binp/wccmps.sym
    <CPCMD> os2386.mps/wccdmpse.dll <RELROOT>/rel2/binp/dll/wccdmps.dll
    <CPCMD> os2386.mps/wccdmpse.sym <RELROOT>/rel2/binp/dll/wccdmps.sym
    <CPCMD> os2386.mps/wccmps01.int <RELROOT>/rel2/binp/wccmps01.int

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> nt386.386/wcc386e.exe <RELROOT>/rel2/binnt/wcc386.exe
    <CPCMD> nt386.386/wcc386e.sym <RELROOT>/rel2/binnt/wcc386.sym
    <CPCMD> nt386.386/wccd386e.dll <RELROOT>/rel2/binnt/wccd386.dll
    <CPCMD> nt386.386/wccd386e.sym <RELROOT>/rel2/binnt/wccd386.sym
    <CPCMD> nt386.386/wcc38601.int <RELROOT>/rel2/binnt/wcc38601.int
    <CPCMD> nt386.i86/wcci86e.exe <RELROOT>/rel2/binnt/wcc.exe
    <CPCMD> nt386.i86/wcci86e.sym <RELROOT>/rel2/binnt/wcc.sym
    <CPCMD> nt386.i86/wccdi86e.dll <RELROOT>/rel2/binnt/wccd.dll
    <CPCMD> nt386.i86/wccdi86e.sym <RELROOT>/rel2/binnt/wccd.sym
    <CPCMD> nt386.i86/wcci8601.int <RELROOT>/rel2/binnt/wcci8601.int
    <CPCMD> nt386.axp/wccaxpe.exe <RELROOT>/rel2/binnt/wccaxp.exe
    <CPCMD> nt386.axp/wccaxpe.sym <RELROOT>/rel2/binnt/wccaxp.sym
    <CPCMD> nt386.axp/wccdaxpe.dll <RELROOT>/rel2/binnt/wccdaxp.dll
    <CPCMD> nt386.axp/wccdaxpe.sym <RELROOT>/rel2/binnt/wccdaxp.sym
    <CPCMD> nt386.axp/wccaxp01.int <RELROOT>/rel2/binnt/wccaxp01.int
    <CPCMD> nt386.ppc/wccppce.exe <RELROOT>/rel2/binnt/wccppc.exe
    <CPCMD> nt386.ppc/wccppce.sym <RELROOT>/rel2/binnt/wccppc.sym
    <CPCMD> nt386.ppc/wccdppce.dll <RELROOT>/rel2/binnt/wccdppc.dll
    <CPCMD> nt386.ppc/wccdppce.sym <RELROOT>/rel2/binnt/wccdppc.sym
    <CPCMD> nt386.ppc/wccppc01.int <RELROOT>/rel2/binnt/wccppc01.int

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> linux386.386/wcc386c.elf <RELROOT>/rel2/binl/wcc386
    <CPCMD> linux386.386/wcc386c.sym <RELROOT>/rel2/binl/wcc386.sym
    <CPCMD> linux386.386/wcc38601.int <RELROOT>/rel2/binl/wcc38601.int
    <CPCMD> linux386.i86/wcci86c.elf <RELROOT>/rel2/binl/wcc
    <CPCMD> linux386.i86/wcci86c.sym <RELROOT>/rel2/binl/wcc.sym
    <CPCMD> linux386.i86/wcci8601.int <RELROOT>/rel2/binl/wcci8601.int
  [ ENDIF ]

  [ IFDEF (cpu_axp) <2*> ]          # AXP target from 386 hosts
    <CPCMD> nt386.axp/wccaxpe.exe   <RELROOT>/rel2/binnt/wccaxp.exe
    <CPCMD> nt386.axp/wccaxpe.sym   <RELROOT>/rel2/binnt/wccaxp.sym
    <CPCMD> nt386.axp/wccdaxpe.dll  <RELROOT>/rel2/binnt/wccdaxp.dll
    <CPCMD> nt386.axp/wccdaxpe.sym  <RELROOT>/rel2/binnt/wccdaxp.sym
    <CPCMD> nt386.axp/wccaxp01.int  <RELROOT>/rel2/binnt/wccaxp01.int
    <CPCMD> os2386.axp/wccaxpe.exe  <RELROOT>/rel2/binp/wccaxp.exe
    <CPCMD> os2386.axp/wccaxpe.sym  <RELROOT>/rel2/binp/wccaxp.sym
    <CPCMD> os2386.axp/wccdaxpe.dll <RELROOT>/rel2/binp/dll/wccdaxp.dll
    <CPCMD> os2386.axp/wccdaxpe.sym <RELROOT>/rel2/binp/dll/wccdaxp.sym
    <CPCMD> os2386.axp/wccaxp01.int <RELROOT>/rel2/binp/wccaxp01.int
#    <CPCMD> ntaxp.axp/wccaxpc.exe  <RELROOT>/rel2/axpnt/wccaxp.exe
#    <CPCMD> ntaxp.axp/wccaxpc.exe  <RELROOT>/rel2/axpnt/wccaxp.sym
#    <CPCMD> ntaxp.axp/wccaxpe.exe  <RELROOT>/rel2/axpnt/wccaxp.exe
#    <CPCMD> ntaxp.axp/wccdaxpe.dll <RELROOT>/rel2/axpnt/wccdaxp.dll
#    <CPCMD> ntaxp.axp/wccaxp01.int <RELROOT>/rel2/axpnt/wccaxp01.int

  [ IFDEF (cpu_ppc) <2*> ]          # PPC target from 386 hosts
    <CPCMD> nt386.ppc/wccppce.exe   <RELROOT>/rel2/binnt/wccppc.exe
    <CPCMD> nt386.ppc/wccppce.sym   <RELROOT>/rel2/binnt/wccppc.sym
    <CPCMD> nt386.ppc/wccdppce.dll  <RELROOT>/rel2/binnt/wccdppc.dll
    <CPCMD> nt386.ppc/wccdppce.sym  <RELROOT>/rel2/binnt/wccdppc.sym
    <CPCMD> nt386.ppc/wccppc01.int  <RELROOT>/rel2/binnt/wccppc01.int
    <CPCMD> os2386.ppc/wccppce.exe  <RELROOT>/rel2/binp/wccppc.exe
    <CPCMD> os2386.ppc/wccppce.sym  <RELROOT>/rel2/binp/wccppc.sym
    <CPCMD> os2386.ppc/wccdppce.dll <RELROOT>/rel2/binp/dll/wccdppc.dll
    <CPCMD> os2386.ppc/wccdppce.sym <RELROOT>/rel2/binp/dll/wccdppc.sym
#    <CPCMD> ntppc.ppc/wccppce.exe  <RELROOT>/rel2/ppcnt/wccppc.exe
#    <CPCMD> ntppc.ppc/wccppce.sym  <RELROOT>/rel2/ppcnt/wccppc.sym
#    <CPCMD> ntppc.ppc/wccdppce.dll <RELROOT>/rel2/ppcnt/wccdppc.dll
#    <CPCMD> ntppc.ppc/wccdppce.sym <RELROOT>/rel2/ppcnt/wccdppc.sym
#    <CPCMD> ntppc.ppc/wccppc01.int <RELROOT>/rel2/ppcnt/wccppc01.int

  [ IFDEF (cpu_mps) <2*> ]          # MIPS target from 386 hosts
#    <CPCMD> nt386.mps/wccmpse.exe   <RELROOT>/rel2/binnt/wccmps.exe
#    <CPCMD> nt386.mps/wccmpse.sym   <RELROOT>/rel2/binnt/wccmps.sym
#    <CPCMD> nt386.mps/wccdmpse.dll  <RELROOT>/rel2/binnt/wccdmps.dll
#    <CPCMD> nt386.mps/wccdmpse.sym  <RELROOT>/rel2/binnt/wccdmps.sym
#    <CPCMD> nt386.mps/wccmps01.int  <RELROOT>/rel2/binnt/wccmps01.int
    <CPCMD> os2386.mps/wccmpse.exe  <RELROOT>/rel2/binp/wccmps.exe
    <CPCMD> os2386.mps/wccmpse.sym  <RELROOT>/rel2/binp/wccmps.sym
    <CPCMD> os2386.mps/wccdmpse.dll <RELROOT>/rel2/binp/dll/wccdmps.dll
    <CPCMD> os2386.mps/wccdmpse.sym <RELROOT>/rel2/binp/dll/wccdmps.sym

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> qnx386.i86/wcci86c.qnx  <RELROOT>/rel2/qnx/binq/wcc.
    <CPCMD> qnx386.i86/wcci86c.sym  <RELROOT>/rel2/qnx/sym/wcc.sym
    <CPCMD> qnx386.386/wcc386c.qnx  <RELROOT>/rel2/qnx/binq/wcc386.
    <CPCMD> qnx386.386/wcc386c.sym  <RELROOT>/rel2/qnx/sym/wcc.sym

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>

