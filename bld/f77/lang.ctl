# WFC Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
#    <CPCMD> <DEVDIR>/f77/cg86/cp/osi386.i86/wfc.exe    <RELROOT>/rel2/binw/wfc.exe
#    <CPCMD> <DEVDIR>/f77/cg86/cp/osi386.386/wfc386.exe <RELROOT>/rel2/binw/wfc386.exe

# intel compilers and link utilities
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.i86/wfc.exe     <RELROOT>/rel2/binw/wfc.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.i86/wfc.sym     <RELROOT>/rel2/binw/wfc.sym
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.386/wfc386.exe  <RELROOT>/rel2/binw/wfc386.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.386/wfc386.sym  <RELROOT>/rel2/binw/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.i86/wfl.exe         <RELROOT>/rel2/binw/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.i86/wfl.sym         <RELROOT>/rel2/binw/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.386/wfl386.exe      <RELROOT>/rel2/binw/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.386/wfl386.sym      <RELROOT>/rel2/binw/wfl386.sym
   # 16-bit DOS Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/bcbd/flibl.lib         <RELROOT>/rel2/lib286/dos/flibl.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/bcbd7/flib7l.lib       <RELROOT>/rel2/lib286/dos/flib7l.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/bcsd/flibm.lib         <RELROOT>/rel2/lib286/dos/flibm.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/bcsd7/flib7m.lib       <RELROOT>/rel2/lib286/dos/flib7m.lib
   # 32-bit DOS Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/386/flib.lib           <RELROOT>/rel2/lib386/dos/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/387/flib7.lib          <RELROOT>/rel2/lib386/dos/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/386s/flibs.lib         <RELROOT>/rel2/lib386/dos/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/387s/flib7s.lib        <RELROOT>/rel2/lib386/dos/flib7s.lib

  [ IFDEF (os_win "") <2*> ]
   # 16-bit Windows Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/wbcbd/flibl.lib        <RELROOT>/rel2/lib286/win/flibl.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/wbcbd7/flib7l.lib      <RELROOT>/rel2/lib286/win/flib7l.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/wbcsd/flibm.lib        <RELROOT>/rel2/lib286/win/flibm.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/wbcsd7/flib7m.lib      <RELROOT>/rel2/lib286/win/flib7m.lib
   # 32-bit Windows Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/w386/flib.lib          <RELROOT>/rel2/lib386/win/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/w387/flib7.lib         <RELROOT>/rel2/lib386/win/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/w386s/flibs.lib        <RELROOT>/rel2/lib386/win/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/w387s/flib7s.lib       <RELROOT>/rel2/lib386/win/flib7s.lib

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.i86/wfc.exe     <RELROOT>/rel2/binp/wfc.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.i86/wfc.sym     <RELROOT>/rel2/binp/wfc.sym
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.386/wfc386.exe  <RELROOT>/rel2/binp/wfc386.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.386/wfc386.sym  <RELROOT>/rel2/binp/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/os2386.i86/wfl.exe         <RELROOT>/rel2/binp/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/os2386.i86/wfl.sym         <RELROOT>/rel2/binp/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/os2386.386/wfl386.exe      <RELROOT>/rel2/binp/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/os2386.386/wfl386.sym      <RELROOT>/rel2/binp/wfl386.sym
 # I wonder if these builds should be enabled to prevent bit rot. They seem to build fine. Mat Nieuwenhoven
#    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.axp/wfcaxp.exe  <RELROOT>/rel2/binp/wfcaxp.exe
#    <CPCMD> <DEVDIR>/f77/wfl/os2386.axp/wflaxp.exe      <RELROOT>/rel2/binp/wflaxp.exe
   # 16-bit OS/2 Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/2bcbd/flibl.lib        <RELROOT>/rel2/lib286/os2/flibl.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/2bcbd7/flib7l.lib      <RELROOT>/rel2/lib286/os2/flib7l.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/2bcsd/flibm.lib        <RELROOT>/rel2/lib286/os2/flibm.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/2bcsd7/flib7m.lib      <RELROOT>/rel2/lib286/os2/flib7m.lib
   # 32-bit OS/2 Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/23r/flib.lib           <RELROOT>/rel2/lib386/os2/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/23r7/flib7.lib         <RELROOT>/rel2/lib386/os2/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/23s/flibs.lib          <RELROOT>/rel2/lib386/os2/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/23s7/flib7s.lib        <RELROOT>/rel2/lib386/os2/flib7s.lib

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.i86/wfc.exe      <RELROOT>/rel2/binnt/wfc.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.i86/wfc.sym      <RELROOT>/rel2/binnt/wfc.sym
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.386/wfc386.exe   <RELROOT>/rel2/binnt/wfc386.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.386/wfc386.sym   <RELROOT>/rel2/binnt/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/nt386.i86/wfl.exe          <RELROOT>/rel2/binnt/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/nt386.i86/wfl.sym          <RELROOT>/rel2/binnt/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/nt386.386/wfl386.exe       <RELROOT>/rel2/binnt/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/nt386.386/wfl386.sym       <RELROOT>/rel2/binnt/wfl386.sym
 # I wonder if these builds should be enabled to prevent bit rot. They seem to build fine. Mat Nieuwenhoven
#    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.axp/wfcaxp.exe   <RELROOT>/rel2/binnt/wfcaxp.exe
#    <CPCMD> <DEVDIR>/f77/wfl/nt386.axp/wflaxp.exe       <RELROOT>/rel2/binnt/wflaxp.exe
   # NT Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/ntr/flib.lib           <RELROOT>/rel2/lib386/nt/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/ntr7/flib7.lib         <RELROOT>/rel2/lib386/nt/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/nts/flibs.lib          <RELROOT>/rel2/lib386/nt/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/nts7/flib7s.lib        <RELROOT>/rel2/lib386/nt/flib7s.lib

  [ IFDEF (os_nov "") <2*> ]
   # 32-bit NETWARE Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/net386s/flibs.lib      <RELROOT>/rel2/lib386/netware/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/net387s/flib7s.lib     <RELROOT>/rel2/lib386/netware/flib7s.lib

# axp compilers and link utilities (NT)
  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/ntaxp.axp/wfcaxp.exe   <RELROOT>/rel2/axpnt/wfcaxp.exe
    <CPCMD> <DEVDIR>/f77/wfl/ntaxp.axp/wflaxp.exe       <RELROOT>/rel2/axpnt/wflaxp.exe

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>

