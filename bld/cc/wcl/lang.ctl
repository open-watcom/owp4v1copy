# wcl Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d buildwcl <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 acprel2 ]
#================================
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <PROJDIR>/dosi86.i86/wcl.exe        <RELROOT>/rel2/binw/wcl.exe
    <CPCMD> <PROJDIR>/dosi86.386/wcl386.exe     <RELROOT>/rel2/binw/wcl386.exe
    <CPCMD> <PROJDIR>/dosi86.axp/wclaxp.exe     <RELROOT>/rel2/binw/wclaxp.exe
    <CPCMD> <PROJDIR>/dosi86.ppc/wclppc.exe     <RELROOT>/rel2/binw/wclppc.exe

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <PROJDIR>/nt386/owcc.exe            <RELROOT>/rel2/binnt/owcc.exe
    <CPCMD> <PROJDIR>/nt386.i86/wcl.exe         <RELROOT>/rel2/binnt/wcl.exe
    <CPCMD> <PROJDIR>/nt386.386/wcl386.exe      <RELROOT>/rel2/binnt/wcl386.exe
    <CPCMD> <PROJDIR>/nt386.axp/wclaxp.exe      <RELROOT>/rel2/binnt/wclaxp.exe
    <CPCMD> <PROJDIR>/nt386.ppc/wclppc.exe      <RELROOT>/rel2/binnt/wclppc.exe
    <CPCMD> <PROJDIR>/nt386.mps/wclmps.exe      <RELROOT>/rel2/binnt/wclmps.exe

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <PROJDIR>/os2386/owcc.exe           <RELROOT>/rel2/binp/owcc.exe
    <CPCMD> <PROJDIR>/os2386.i86/wcl.exe        <RELROOT>/rel2/binp/wcl.exe
    <CPCMD> <PROJDIR>/os2386.386/wcl386.exe     <RELROOT>/rel2/binp/wcl386.exe
    <CPCMD> <PROJDIR>/os2386.axp/wclaxp.exe     <RELROOT>/rel2/binp/wclaxp.exe
    <CPCMD> <PROJDIR>/os2386.ppc/wclppc.exe     <RELROOT>/rel2/binp/wclppc.exe
    <CPCMD> <PROJDIR>/os2386.mps/wclmps.exe     <RELROOT>/rel2/binp/wclmps.exe

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <PROJDIR>/linux386/owcc.elf         <RELROOT>/rel2/binl/owcc
    <CPCMD> <PROJDIR>/linux386.i86/wcl.exe      <RELROOT>/rel2/binl/wcl
    <CPCMD> <PROJDIR>/linux386.i86/wcl.sym      <RELROOT>/rel2/binl/wcl.sym
    <CPCMD> <PROJDIR>/linux386.386/wcl386.exe   <RELROOT>/rel2/binl/wcl386
    <CPCMD> <PROJDIR>/linux386.386/wcl386.sym   <RELROOT>/rel2/binl/wcl386.sym
    <CPCMD> <PROJDIR>/linux386.axp/wclaxp.exe   <RELROOT>/rel2/binl/wclaxp
    <CPCMD> <PROJDIR>/linux386.mps/wclmps.exe   <RELROOT>/rel2/binl/wclmps

  [ IFDEF (cpu_axp) <2*> ]
#    <CPCMD> <PROJDIR>/ntaxp.i86/wcl.exe         <RELROOT>/rel2/axpnt/wcl.exe
#    <CPCMD> <PROJDIR>/ntaxp.386/wcl386.exe      <RELROOT>/rel2/axpnt/wcl386.exe
#    <CPCMD> <PROJDIR>/ntaxp.axp/wclaxp.exe      <RELROOT>/rel2/axpnt/wclaxp.exe

[ BLOCK <1> clean ]
#==================
    pmake -d buildwcl <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>

