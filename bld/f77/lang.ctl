# WFC Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================

# intel compilers and link utilities
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <DEVDIR>/f77/wfc/dos386.i86/wfc.exe          <RELROOT>/rel2/binw/wfc.exe
    <CPCMD> <DEVDIR>/f77/wfc/dos386.i86/wfc.sym          <RELROOT>/rel2/binw/wfc.sym
    <CPCMD> <DEVDIR>/f77/wfc/dos386.386/wfc386.exe       <RELROOT>/rel2/binw/wfc386.exe
    <CPCMD> <DEVDIR>/f77/wfc/dos386.386/wfc386.sym       <RELROOT>/rel2/binw/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.i86/wfl.exe          <RELROOT>/rel2/binw/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.i86/wfl.sym          <RELROOT>/rel2/binw/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.386/wfl386.exe       <RELROOT>/rel2/binw/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.386/wfl386.sym       <RELROOT>/rel2/binw/wfl386.sym

  [ IFDEF (os_win "") <2*> ]

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <DEVDIR>/f77/wfc/os2386.i86/wfc.exe          <RELROOT>/rel2/binp/wfc.exe
    <CPCMD> <DEVDIR>/f77/wfc/os2386.i86/wfc.sym          <RELROOT>/rel2/binp/wfc.sym
    <CPCMD> <DEVDIR>/f77/wfc/os2386.386/wfc386.exe       <RELROOT>/rel2/binp/wfc386.exe
    <CPCMD> <DEVDIR>/f77/wfc/os2386.386/wfc386.sym       <RELROOT>/rel2/binp/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/os2386.i86/wfl.exe          <RELROOT>/rel2/binp/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/os2386.i86/wfl.sym          <RELROOT>/rel2/binp/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/os2386.386/wfl386.exe       <RELROOT>/rel2/binp/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/os2386.386/wfl386.sym       <RELROOT>/rel2/binp/wfl386.sym
 # I wonder if these builds should be enabled to prevent bit rot. They seem to build fine. Mat Nieuwenhoven
#    <CPCMD> <DEVDIR>/f77/wfc/os2386.axp/wfcaxp.exe       <RELROOT>/rel2/binp/wfcaxp.exe
#    <CPCMD> <DEVDIR>/f77/wfl/os2386.axp/wflaxp.exe       <RELROOT>/rel2/binp/wflaxp.exe

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/f77/wfc/nt386.i86/wfc.exe           <RELROOT>/rel2/binnt/wfc.exe
    <CPCMD> <DEVDIR>/f77/wfc/nt386.i86/wfc.sym           <RELROOT>/rel2/binnt/wfc.sym
    <CPCMD> <DEVDIR>/f77/wfc/nt386.386/wfc386.exe        <RELROOT>/rel2/binnt/wfc386.exe
    <CPCMD> <DEVDIR>/f77/wfc/nt386.386/wfc386.sym        <RELROOT>/rel2/binnt/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/nt386.i86/wfl.exe           <RELROOT>/rel2/binnt/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/nt386.i86/wfl.sym           <RELROOT>/rel2/binnt/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/nt386.386/wfl386.exe        <RELROOT>/rel2/binnt/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/nt386.386/wfl386.sym        <RELROOT>/rel2/binnt/wfl386.sym
 # I wonder if these builds should be enabled to prevent bit rot. They seem to build fine. Mat Nieuwenhoven
#    <CPCMD> <DEVDIR>/f77/wfc/nt386.axp/wfcaxp.exe        <RELROOT>/rel2/binnt/wfcaxp.exe
#    <CPCMD> <DEVDIR>/f77/wfl/nt386.axp/wflaxp.exe        <RELROOT>/rel2/binnt/wflaxp.exe

  [ IFDEF (os_nov "") <2*> ]

  [ IFDEF (os_linux "") <2*> ]
#    <CPCMD> <DEVDIR>/f77/wfc/linux386.i86/wfc.exe        <RELROOT>/rel2/binl/wfc
#    <CPCMD> <DEVDIR>/f77/wfc/linux386.i86/wfc.sym        <RELROOT>/rel2/binl/wfc.sym
#    <CPCMD> <DEVDIR>/f77/wfc/linux386.386/wfc386.exe     <RELROOT>/rel2/binl/wfc386
#    <CPCMD> <DEVDIR>/f77/wfc/linux386.386/wfc386.sym     <RELROOT>/rel2/binl/wfc386.sym
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.i86/wfl.exe        <RELROOT>/rel2/binl/wfl
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.i86/wfl.sym        <RELROOT>/rel2/binl/wfl.sym
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.386/wfl386.exe     <RELROOT>/rel2/binl/wfl386
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.386/wfl386.sym     <RELROOT>/rel2/binl/wfl386.sym

# axp compilers and link utilities (NT)
  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <DEVDIR>/f77/wfc/ntaxp.axp/wfcaxp.exe        <RELROOT>/rel2/axpnt/wfcaxp.exe
    <CPCMD> <DEVDIR>/f77/wfl/ntaxp.axp/wflaxp.exe        <RELROOT>/rel2/axpnt/wflaxp.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>

