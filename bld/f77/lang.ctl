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

# intel compilers and link utilities
  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.i86/wfc.exe          <RELROOT>/rel2/binw/wfc.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.i86/wfc.sym          <RELROOT>/rel2/binw/wfc.sym
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.386/wfc386.exe       <RELROOT>/rel2/binw/wfc386.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/dos386.386/wfc386.sym       <RELROOT>/rel2/binw/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.i86/wfl.exe              <RELROOT>/rel2/binw/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.i86/wfl.sym              <RELROOT>/rel2/binw/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.386/wfl386.exe           <RELROOT>/rel2/binw/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/dosi86.386/wfl386.sym           <RELROOT>/rel2/binw/wfl386.sym
   # 16-bit DOS Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.086/ml/flibl.lib      <RELROOT>/rel2/lib286/dos/
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.086/mm/flibm.lib      <RELROOT>/rel2/lib286/dos/
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.087/ml/flib7l.lib     <RELROOT>/rel2/lib286/dos/
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.087/mm/flib7m.lib     <RELROOT>/rel2/lib286/dos/
   # 32-bit DOS Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.386/ms_r/flib.lib     <RELROOT>/rel2/lib386/dos/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.386/ms_s/flibs.lib    <RELROOT>/rel2/lib386/dos/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.387/ms_r/flib7.lib    <RELROOT>/rel2/lib386/dos/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/msdos.387/ms_s/flib7s.lib   <RELROOT>/rel2/lib386/dos/flib7s.lib

  [ IFDEF (os_win "") <2*> ]
   # 16-bit Windows Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.086/ml/flibl.lib    <RELROOT>/rel2/lib286/win/flibl.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.086/mm/flibm.lib    <RELROOT>/rel2/lib286/win/flibm.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.087/ml/flib7l.lib   <RELROOT>/rel2/lib286/win/flib7l.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.087/mm/flib7m.lib   <RELROOT>/rel2/lib286/win/flib7m.lib
   # 32-bit Windows Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.386/mf_r/flib.lib   <RELROOT>/rel2/lib386/win/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.386/mf_s/flibs.lib  <RELROOT>/rel2/lib386/win/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.387/mf_r/flib7.lib  <RELROOT>/rel2/lib386/win/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/windows.387/mf_s/flib7s.lib <RELROOT>/rel2/lib386/win/flib7s.lib

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.i86/wfc.exe          <RELROOT>/rel2/binp/wfc.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.i86/wfc.sym          <RELROOT>/rel2/binp/wfc.sym
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.386/wfc386.exe       <RELROOT>/rel2/binp/wfc386.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.386/wfc386.sym       <RELROOT>/rel2/binp/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/os2386.i86/wfl.exe              <RELROOT>/rel2/binp/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/os2386.i86/wfl.sym              <RELROOT>/rel2/binp/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/os2386.386/wfl386.exe           <RELROOT>/rel2/binp/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/os2386.386/wfl386.sym           <RELROOT>/rel2/binp/wfl386.sym
 # I wonder if these builds should be enabled to prevent bit rot. They seem to build fine. Mat Nieuwenhoven
#    <CPCMD> <DEVDIR>/f77/cg86/cp/os2386.axp/wfcaxp.exe       <RELROOT>/rel2/binp/wfcaxp.exe
#    <CPCMD> <DEVDIR>/f77/wfl/os2386.axp/wflaxp.exe           <RELROOT>/rel2/binp/wflaxp.exe
   # 16-bit OS/2 Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.286/ml/flibl.lib        <RELROOT>/rel2/lib286/os2/flibl.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.286/mm/flibm.lib        <RELROOT>/rel2/lib286/os2/flibm.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.287/ml/flib7l.lib       <RELROOT>/rel2/lib286/os2/flib7l.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.287/mm/flib7m.lib       <RELROOT>/rel2/lib286/os2/flib7m.lib
   # 32-bit OS/2 Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.386/mf_r/flib.lib       <RELROOT>/rel2/lib386/os2/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.386/mf_s/flibs.lib      <RELROOT>/rel2/lib386/os2/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.387/mf_r/flib7.lib      <RELROOT>/rel2/lib386/os2/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/os2.387/mf_s/flib7s.lib     <RELROOT>/rel2/lib386/os2/flib7s.lib

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.i86/wfc.exe           <RELROOT>/rel2/binnt/wfc.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.i86/wfc.sym           <RELROOT>/rel2/binnt/wfc.sym
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.386/wfc386.exe        <RELROOT>/rel2/binnt/wfc386.exe
    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.386/wfc386.sym        <RELROOT>/rel2/binnt/wfc386.sym
    <CPCMD> <DEVDIR>/f77/wfl/nt386.i86/wfl.exe               <RELROOT>/rel2/binnt/wfl.exe
    <CPCMD> <DEVDIR>/f77/wfl/nt386.i86/wfl.sym               <RELROOT>/rel2/binnt/wfl.sym
    <CPCMD> <DEVDIR>/f77/wfl/nt386.386/wfl386.exe            <RELROOT>/rel2/binnt/wfl386.exe
    <CPCMD> <DEVDIR>/f77/wfl/nt386.386/wfl386.sym            <RELROOT>/rel2/binnt/wfl386.sym
 # I wonder if these builds should be enabled to prevent bit rot. They seem to build fine. Mat Nieuwenhoven
#    <CPCMD> <DEVDIR>/f77/cg86/cp/nt386.axp/wfcaxp.exe        <RELROOT>/rel2/binnt/wfcaxp.exe
#    <CPCMD> <DEVDIR>/f77/wfl/nt386.axp/wflaxp.exe            <RELROOT>/rel2/binnt/wflaxp.exe
   # NT Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/winnt.386/mf_r/flib.lib     <RELROOT>/rel2/lib386/nt/flib.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/winnt.386/mf_s/flibs.lib    <RELROOT>/rel2/lib386/nt/flibs.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/winnt.387/mf_r/flib7.lib    <RELROOT>/rel2/lib386/nt/flib7.lib
    <CPCMD> <DEVDIR>/f77/cg86/rt/winnt.387/mf_s/flib7s.lib   <RELROOT>/rel2/lib386/nt/flib7s.lib

  [ IFDEF (os_nov "") <2*> ]
   # 32-bit NETWARE Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/netware.386/ms_s/flibs.lib  <RELROOT>/rel2/lib386/netware/
    <CPCMD> <DEVDIR>/f77/cg86/rt/netware.387/ms_s/flib7s.lib <RELROOT>/rel2/lib386/netware/

  [ IFDEF (os_linux "") <2*> ]
#    <CPCMD> <DEVDIR>/f77/cg86/cp/linux386.i86/wfc.exe        <RELROOT>/rel2/binl/wfc
#    <CPCMD> <DEVDIR>/f77/cg86/cp/linux386.i86/wfc.sym        <RELROOT>/rel2/binl/wfc.sym
#    <CPCMD> <DEVDIR>/f77/cg86/cp/linux386.386/wfc386.exe     <RELROOT>/rel2/binl/wfc386
#    <CPCMD> <DEVDIR>/f77/cg86/cp/linux386.386/wfc386.sym     <RELROOT>/rel2/binl/wfc386.sym
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.i86/wfl.exe            <RELROOT>/rel2/binl/wfl
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.i86/wfl.sym            <RELROOT>/rel2/binl/wfl.sym
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.386/wfl386.exe         <RELROOT>/rel2/binl/wfl386
#    <CPCMD> <DEVDIR>/f77/wfl/linux386.386/wfl386.sym         <RELROOT>/rel2/binl/wfl386.sym
   # LINUX Libraries
#    <CPCMD> <DEVDIR>/f77/cg86/rt/linux.386/mf_r/flib.lib     <RELROOT>/rel2/lib386/linux/flib.lib
#    <CPCMD> <DEVDIR>/f77/cg86/rt/linux.386/mf_s/flibs.lib    <RELROOT>/rel2/lib386/linux/flibs.lib
#    <CPCMD> <DEVDIR>/f77/cg86/rt/linux.387/mf_r/flib7.lib    <RELROOT>/rel2/lib386/linux/flib7.lib
#    <CPCMD> <DEVDIR>/f77/cg86/rt/linux.387/mf_s/flib7s.lib   <RELROOT>/rel2/lib386/linux/flib7s.lib

# axp compilers and link utilities (NT)
  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <DEVDIR>/f77/cg86/cp/ntaxp.axp/wfcaxp.exe        <RELROOT>/rel2/axpnt/wfcaxp.exe
    <CPCMD> <DEVDIR>/f77/wfl/ntaxp.axp/wflaxp.exe            <RELROOT>/rel2/axpnt/wflaxp.exe
   # NT Libraries
    <CPCMD> <DEVDIR>/f77/cg86/rt/winnt.axp/_s/flib.lib       <RELROOT>/rel2/libaxp/nt/

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cd <PROJDIR>

