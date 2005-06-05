# Wlink Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <PROJDIR>/wlsystem.lnk                      <RELROOT>/rel2/binw/wlsystem.lnk

  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <PROJDIR>/dos386/wl.exe                     <RELROOT>/rel2/binw/wlink.exe
    <CPCMD> <PROJDIR>/dos386/wl.sym                     <RELROOT>/rel2/binw/wlink.sym
    <CPCMD> <PROJDIR>/ms2wlink/dosi86/ms2wlink.exe      <RELROOT>/rel2/binw/ms2wlink.exe
    <CPCMD> <PROJDIR>/fcenable/dosi86/fcenable.exe      <RELROOT>/rel2/binw/fcenable.exe
    <CPCMD> <PROJDIR>/exe2bin/dosi86/exe2bin.exe        <RELROOT>/rel2/binw/exe2bin.exe
    <CPCMD> <PROJDIR>/wlbin.lnk                         <RELROOT>/rel2/binw/wlink.lnk
    <CPCMD> <PROJDIR>/ovlldr/wovl.lib                   <RELROOT>/rel2/lib286/dos/wovl.lib
#    <CPCMD> <PROJDIR>/ovlldr/wmovl.lib                  <RELROOT>/rel2/lib286/dos/wmovl.lib

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <PROJDIR>/os2386.dll/wlstub.exe             <RELROOT>/rel2/binp/wlink.exe
    <CPCMD> <PROJDIR>/os2386.dll/wl.dll                 <RELROOT>/rel2/binp/dll/wlink.dll
    <CPCMD> <PROJDIR>/os2386.dll/wl.sym                 <RELROOT>/rel2/binp/dll/wlink.sym
    <CPCMD> <PROJDIR>/ms2wlink/os2i86/ms2wlink.exe      <RELROOT>/rel2/binp/ms2wlink.exe
    <CPCMD> <PROJDIR>/fcenable/os2i86/fcenable.exe      <RELROOT>/rel2/binp/fcenable.exe
    <CPCMD> <PROJDIR>/exe2bin/os2386/exe2bin.exe        <RELROOT>/rel2/binp/exe2bin.exe
    <CPCMD> <PROJDIR>/wlbinp.lnk                        <RELROOT>/rel2/binp/wlink.lnk

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <PROJDIR>/nt386.dll/wlstub.exe              <RELROOT>/rel2/binnt/wlink.exe
    <CPCMD> <PROJDIR>/nt386.dll/wl.dll                  <RELROOT>/rel2/binnt/wlink.dll
    <CPCMD> <PROJDIR>/nt386.dll/wl.sym                  <RELROOT>/rel2/binnt/wlink.sym
    <CPCMD> <PROJDIR>/nt386.rtd/wl.dll                  <RELROOT>/rel2/binnt/rtdll/wlink.dll
    <CPCMD> <PROJDIR>/ms2wlink/nt386/ms2wlink.exe       <RELROOT>/rel2/binnt/ms2wlink.exe
    <CPCMD> <PROJDIR>/fcenable/nt386/fcenable.exe       <RELROOT>/rel2/binnt/fcenable.exe
    <CPCMD> <PROJDIR>/exe2bin/nt386/exe2bin.exe         <RELROOT>/rel2/binnt/exe2bin.exe
    <CPCMD> <PROJDIR>/wlbinnt.lnk                       <RELROOT>/rel2/binnt/wlink.lnk

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <PROJDIR>/linux386/wl.exe                   <RELROOT>/rel2/binl/wlink
    <CPCMD> <PROJDIR>/linux386/wl.sym                   <RELROOT>/rel2/binl/wlink.sym
    <CPCMD> <PROJDIR>/wlbinl.lnk                        <RELROOT>/rel2/binl/wlink.lnk

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <PROJDIR>/ntaxp.dll/wlstub.exe              <RELROOT>/rel2/axpnt/wlink.exe
    <CPCMD> <PROJDIR>/ntaxp.dll/wl.dll                  <RELROOT>/rel2/axpnt/wlink.dll
    <CPCMD> <PROJDIR>/wlaxpnt.lnk                       <RELROOT>/rel2/axpnt/wlink.lnk

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> <PROJDIR>/qnx386/wl.exe                     <RELROOT>/rel2/qnx/binq/wlink
    <CPCMD> <PROJDIR>/qnx386/wl.sym                     <RELROOT>/rel2/qnx/sym/wlink.sym
    cat <DEVDIR>\wl\wlsystem.lnk <DEVDIR>\wl\wlqnx.lnk > <RELROOT>\rel2\qnx\etc\wlink.lnk

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
