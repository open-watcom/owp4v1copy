# PLUS Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d buildwpp <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================

#
# 386 NT hosted compilers
#
  [ IFDEF (os_nt "") <2*> ]
#  i86 target
    <CPCMD> <PROJDIR>/nt386.i86/wcppi86.exe   <RELROOT>/rel2/binnt/wpp.exe
    <CPCMD> <PROJDIR>/nt386.i86/wppdi86.dll   <RELROOT>/rel2/binnt/wppdi86.dll
    <CPCMD> <PROJDIR>/nt386.i86/wppdi86.sym   <RELROOT>/rel2/binnt/wppdi86.sym
    <CPCMD> <PROJDIR>/nt386.i86/wppi8601.int  <RELROOT>/rel2/binnt/wppi8601.int
#  386 target
    <CPCMD> <PROJDIR>/nt386.386/wcpp386.exe   <RELROOT>/rel2/binnt/wpp386.exe
    <CPCMD> <PROJDIR>/nt386.386/wppd386.dll   <RELROOT>/rel2/binnt/wppd386.dll
    <CPCMD> <PROJDIR>/nt386.386/wppd386.sym   <RELROOT>/rel2/binnt/wppd386.sym
    <CPCMD> <PROJDIR>/nt386.386/wpp38601.int  <RELROOT>/rel2/binnt/wpp38601.int
#  AXP target
    <CPCMD> <PROJDIR>/nt386.axp/wcppaxp.exe   <RELROOT>/rel2/binnt/wppaxp.exe
    <CPCMD> <PROJDIR>/nt386.axp/wppdaxp.dll   <RELROOT>/rel2/binnt/wppdaxp.dll
    <CPCMD> <PROJDIR>/nt386.axp/wppdaxp.sym   <RELROOT>/rel2/binnt/wppdaxp.sym
    <CPCMD> <PROJDIR>/nt386.axp/wppaxp01.int  <RELROOT>/rel2/binnt/wppaxp01.int

#  Optima 386 target (with -br switch)
    <CPCMD> <PROJDIR>/nt386dll.386/wcpp386.exe   <RELROOT>/rel2/binnt/rtdll/wpp386.exe
    <CPCMD> <PROJDIR>/nt386dll.386/wppd386.dll   <RELROOT>/rel2/binnt/rtdll/wppd386.dll
    <CPCMD> <PROJDIR>/nt386dll.386/wppd386.sym   <RELROOT>/rel2/binnt/rtdll/wppd386.sym
    <CPCMD> <PROJDIR>/nt386dll.386/wpp38601.int  <RELROOT>/rel2/binnt/rtdll/wpp38601.int

#
# 386 OS/2 hosted compilers
#
  [ IFDEF (os_os2 "") <2*> ]
#  i86 target
    <CPCMD> <PROJDIR>/os2386.i86/wcppi86.exe   <RELROOT>/rel2/binp/wpp.exe
    <CPCMD> <PROJDIR>/os2386.i86/wppdi86.dll   <RELROOT>/rel2/binp/dll/wppdi86.dll
    <CPCMD> <PROJDIR>/os2386.i86/wppdi86.sym   <RELROOT>/rel2/binp/dll/wppdi86.sym
    <CPCMD> <PROJDIR>/os2386.i86/wppi8601.int  <RELROOT>/rel2/binp/dll/wppi8601.int
#  386 target
    <CPCMD> <PROJDIR>/os2386.386/wcpp386.exe   <RELROOT>/rel2/binp/wpp386.exe
    <CPCMD> <PROJDIR>/os2386.386/wppd386.dll   <RELROOT>/rel2/binp/dll/wppd386.dll
    <CPCMD> <PROJDIR>/os2386.386/wppd386.sym   <RELROOT>/rel2/binp/dll/wppd386.sym
    <CPCMD> <PROJDIR>/os2386.386/wpp38601.int  <RELROOT>/rel2/binp/dll/wpp38601.int
#  386 target
    <CPCMD> <PROJDIR>/os2386.axp/wcppaxp.exe   <RELROOT>/rel2/binp/wppaxp.exe
    <CPCMD> <PROJDIR>/os2386.axp/wppdaxp.dll   <RELROOT>/rel2/binp/dll/wppdaxp.dll
    <CPCMD> <PROJDIR>/os2386.axp/wppdaxp.sym   <RELROOT>/rel2/binp/dll/wppdaxp.sym
    <CPCMD> <PROJDIR>/os2386.axp/wppaxp01.int  <RELROOT>/rel2/binp/dll/wppaxp01.int
  [ ENDIF ]
#
# 386 OSI hosted compilers
#
#  i86 target
#    <CPCMD> <PROJDIR>/osi386.i86/wcppi86.exe   <RELROOT>/rel2/binw/wpp.exe
#    <CPCMD> <PROJDIR>/osi386.i86/wcppi86.sym   <RELROOT>/rel2/binw/wpp.sym
#    <CPCMD> <PROJDIR>/osi386.i86/wppi8601.int  <RELROOT>/rel2/binw/wppi8601.int
#  386 target
#    <CPCMD> <PROJDIR>/osi386.386/wcpp386.exe   <RELROOT>/rel2/binw/wpp386.exe
#    <CPCMD> <PROJDIR>/osi386.386/wcpp386.sym   <RELROOT>/rel2/binw/wpp386.sym
#    <CPCMD> <PROJDIR>/osi386.386/wpp38601.int  <RELROOT>/rel2/binw/wpp38601.int

#
# 386 DOS hosted compilers
#
  [ IFDEF (os_dos "") <2*> ]
#  i86 target
    <CPCMD> <PROJDIR>/dos386.i86/wcppi86.exe   <RELROOT>/rel2/binw/wpp.exe
    <CPCMD> <PROJDIR>/dos386.i86/wcppi86.sym   <RELROOT>/rel2/binw/wpp.sym
    <CPCMD> <PROJDIR>/dos386.i86/wppi8601.int  <RELROOT>/rel2/binw/wppi8601.int
#  386 target
    <CPCMD> <PROJDIR>/dos386.386/wcpp386.exe   <RELROOT>/rel2/binw/wpp386.exe
    <CPCMD> <PROJDIR>/dos386.386/wcpp386.sym   <RELROOT>/rel2/binw/wpp386.sym
    <CPCMD> <PROJDIR>/dos386.386/wpp38601.int  <RELROOT>/rel2/binw/wpp38601.int
#
# 386 QNX hosted compilers
#
  [ IFDEF (os_qnx) <2*> ]
#  i86 target
    <CPCMD> <PROJDIR>/qnx386.i86/wcppi86.exe   <RELROOT>/rel2/qnx/binq/wpp.
    <CPCMD> <PROJDIR>/qnx386.i86/wcppi86.sym   <RELROOT>/rel2/qnx/sym/wpp.sym
    <CPCMD> <PROJDIR>/qnx386.i86/wppi8601.int  <RELROOT>/rel2/qnx/sym/wppi8601.int
#  386 target
    <CPCMD> <PROJDIR>/qnx386.386/wcpp386.exe   <RELROOT>/rel2/qnx/binq/wpp386.
    <CPCMD> <PROJDIR>/qnx386.386/wcpp386.sym   <RELROOT>/rel2/qnx/sym/wpp386.sym
    <CPCMD> <PROJDIR>/qnx386.386/wpp38601.int  <RELROOT>/rel2/qnx/sym/wpp38601.int

#
# 386 Linux hosted compilers
#
  [ IFDEF (os_linux "") <2*> ]
#  i86 target
    <CPCMD> <PROJDIR>/linux386.i86/wcppi86.exe   <RELROOT>/rel2/binl/wpp
    <CPCMD> <PROJDIR>/linux386.i86/wcppi86.sym   <RELROOT>/rel2/binl/wpp.sym
    <CPCMD> <PROJDIR>/linux386.i86/wppi8601.int  <RELROOT>/rel2/binl/wppi8601.int
#  386 target
    <CPCMD> <PROJDIR>/linux386.386/wcpp386.exe   <RELROOT>/rel2/binl/wpp386
    <CPCMD> <PROJDIR>/linux386.386/wcpp386.sym   <RELROOT>/rel2/binl/wpp386.sym
    <CPCMD> <PROJDIR>/linux386.386/wpp38601.int  <RELROOT>/rel2/binl/wpp38601.int

#
# AXP NT hosted compilers
#
  [ IFDEF (cpu_axp) <2*> ]
#  AXP target
    <CPCMD> <PROJDIR>/ntaxp.axp/wcppaxp.exe   <RELROOT>/rel2/axpnt/wppaxp.exe
    <CPCMD> <PROJDIR>/ntaxp.axp/wcppaxp.sym   <RELROOT>/rel2/axpnt/wppaxp.sym
#  386 target
    <CPCMD> <PROJDIR>/ntaxp.386/wcpp386.exe   <RELROOT>/rel2/axpnt/wpp386.exe
    <CPCMD> <PROJDIR>/ntaxp.386/wcpp386.sym   <RELROOT>/rel2/axpnt/wpp386.sym

[ BLOCK <1> clean ]
#==================
    pmake -d buildwpp <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
