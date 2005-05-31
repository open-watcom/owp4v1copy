# plib Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

set destdir=<RELROOT>/rel2

#
# Make sure we have the reference compilers
#
[ BLOCK <1> build rel2 ]
#=======================
    [ INCLUDE prereq.ctl ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 ]
#=================
    set destdir=<RELROOT>/rel2

[ BLOCK <1> nlang ]
#==================
    set destdir=<DWATCOM>
    set CPCMD=copy

[ BLOCK <1> rel2 cprel2 acprel2 cpu_i86 nlang ]
#==============================================
  [ IFDEF (os_osi "") <2*> ]
    <CPCMD> generic.086/ms/plibs.lib             <destdir>/lib286/
    <CPCMD> generic.086/ms/plbxs.lib             <destdir>/lib286/
    <CPCMD> generic.086/mc/plibc.lib             <destdir>/lib286/
    <CPCMD> generic.086/mc/plbxc.lib             <destdir>/lib286/
    <CPCMD> generic.086/mm/plibm.lib             <destdir>/lib286/
    <CPCMD> generic.086/mm/plbxm.lib             <destdir>/lib286/
    <CPCMD> generic.086/ml/plibl.lib             <destdir>/lib286/
    <CPCMD> generic.086/ml/plbxl.lib             <destdir>/lib286/
    <CPCMD> generic.086/mh/plibh.lib             <destdir>/lib286/
    <CPCMD> generic.086/mh/plbxh.lib             <destdir>/lib286/
    <CPCMD> complex/generic.086/ms/cplxs.lib     <destdir>/lib286/
    <CPCMD> complex/generic.086/mc/cplxc.lib     <destdir>/lib286/
    <CPCMD> complex/generic.086/mm/cplxm.lib     <destdir>/lib286/
    <CPCMD> complex/generic.086/ml/cplxl.lib     <destdir>/lib286/
    <CPCMD> complex/generic.086/mh/cplxh.lib     <destdir>/lib286/
    <CPCMD> complex/generic.087/ms/cplx7s.lib    <destdir>/lib286/
    <CPCMD> complex/generic.087/mc/cplx7c.lib    <destdir>/lib286/
    <CPCMD> complex/generic.087/mm/cplx7m.lib    <destdir>/lib286/
    <CPCMD> complex/generic.087/ml/cplx7l.lib    <destdir>/lib286/
    <CPCMD> complex/generic.087/mh/cplx7h.lib    <destdir>/lib286/

  [ IFDEF (os_win "") <2*> ]
    <CPCMD> windows.086/ms/plibs.lib             <destdir>/lib286/win/
    <CPCMD> windows.086/ms/plbxs.lib             <destdir>/lib286/win/
    <CPCMD> windows.086/mc/plibc.lib             <destdir>/lib286/win/
    <CPCMD> windows.086/mc/plbxc.lib             <destdir>/lib286/win/
    <CPCMD> windows.086/mm/plibm.lib             <destdir>/lib286/win/
    <CPCMD> windows.086/mm/plbxm.lib             <destdir>/lib286/win/
    <CPCMD> windows.086/ml/plibl.lib             <destdir>/lib286/win/
    <CPCMD> windows.086/ml/plbxl.lib             <destdir>/lib286/win/
    <CPCMD> complex/windows.086/ms/cplxs.lib     <destdir>/lib286/win/
    <CPCMD> complex/windows.086/mc/cplxc.lib     <destdir>/lib286/win/
    <CPCMD> complex/windows.086/mm/cplxm.lib     <destdir>/lib286/win/
    <CPCMD> complex/windows.086/ml/cplxl.lib     <destdir>/lib286/win/
    <CPCMD> complex/windows.087/ms/cplx7s.lib    <destdir>/lib286/win/
    <CPCMD> complex/windows.087/mc/cplx7c.lib    <destdir>/lib286/win/
    <CPCMD> complex/windows.087/mm/cplx7m.lib    <destdir>/lib286/win/
    <CPCMD> complex/windows.087/ml/cplx7l.lib    <destdir>/lib286/win/

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> os2.286/ml_mt/plibmtl.lib            <destdir>/lib286/os2/
    <CPCMD> os2.286/ml_mt/plbxmtl.lib            <destdir>/lib286/os2/


[ BLOCK <1> rel2 cprel2 acprel2 cpu_386 ]
#========================================
  [ IFDEF (os_osi os_dos os_linux os_nov "") <2*> ]
    <CPCMD> generic.386/mf_r/plib3r.lib          <destdir>/lib386/
    <CPCMD> generic.386/mf_r/plbx3r.lib          <destdir>/lib386/
    <CPCMD> generic.386/mf_s/plib3s.lib          <destdir>/lib386/
    <CPCMD> generic.386/mf_s/plbx3s.lib          <destdir>/lib386/
    <CPCMD> complex/generic.386/mf_r/cplx3r.lib  <destdir>/lib386/
    <CPCMD> complex/generic.386/mf_s/cplx3s.lib  <destdir>/lib386/

  [ IFDEF (os_osi "") <2*> ]
    <CPCMD> generic.386/mf_rmt/plibmt3r.lib      <destdir>/lib386/
    <CPCMD> generic.386/mf_rmt/plbxmt3r.lib      <destdir>/lib386/
    <CPCMD> generic.386/mf_smt/plibmt3s.lib      <destdir>/lib386/
    <CPCMD> generic.386/mf_smt/plbxmt3s.lib      <destdir>/lib386/
    <CPCMD> complex/generic.387/mf_r/cplx73r.lib <destdir>/lib386/
    <CPCMD> complex/generic.387/mf_s/cplx73s.lib <destdir>/lib386/

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> winnt.386/mf_r/plbx3r.lib            <destdir>/lib386/nt/
    <CPCMD> winnt.386/mf_r/plib3r.lib            <destdir>/lib386/nt/
    <CPCMD> winnt.386/mf_s/plbx3s.lib            <destdir>/lib386/nt/
    <CPCMD> winnt.386/mf_s/plib3s.lib            <destdir>/lib386/nt/
    <CPCMD> winnt.386/mf_rmt/plbxmt3r.lib        <destdir>/lib386/nt/
    <CPCMD> winnt.386/mf_rmt/plibmt3r.lib        <destdir>/lib386/nt/
    <CPCMD> winnt.386/mf_smt/plbxmt3s.lib        <destdir>/lib386/nt/
    <CPCMD> winnt.386/mf_smt/plibmt3s.lib        <destdir>/lib386/nt/

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> os2.386/mf_r/plbx3r.lib              <destdir>/lib386/os2/
    <CPCMD> os2.386/mf_r/plib3r.lib              <destdir>/lib386/os2/
    <CPCMD> os2.386/mf_s/plbx3s.lib              <destdir>/lib386/os2/
    <CPCMD> os2.386/mf_s/plib3s.lib              <destdir>/lib386/os2/
    <CPCMD> os2.386/mf_rmt/plbxmt3r.lib          <destdir>/lib386/os2/
    <CPCMD> os2.386/mf_rmt/plibmt3r.lib          <destdir>/lib386/os2/
    <CPCMD> os2.386/mf_smt/plbxmt3s.lib          <destdir>/lib386/os2/
    <CPCMD> os2.386/mf_smt/plibmt3s.lib          <destdir>/lib386/os2/

  [ IFDEF (os_nov "") <2*> ]
    <CPCMD> complex/netware.386/ms_s/cplx3s.lib  <destdir>/lib386/netware/
    <CPCMD> complex/netware.387/ms_s/cplx73s.lib <destdir>/lib386/netware/

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> q3rmt/plbxmt3r.lib                   <destdir>/lib386/qnx/
    <CPCMD> q3rmt/plibmt3r.lib                   <destdir>/lib386/qnx/
    <CPCMD> q3smt/plbxmt3s.lib                   <destdir>/lib386/qnx/
    <CPCMD> q3smt/plibmt3s.lib                   <destdir>/lib386/qnx/

[ BLOCK <1> rel2 cprel2 acprel2 cpu_axp ]
#========================================
  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> winnt.axp/_s/plib.lib                <destdir>/libaxp/nt/plib.lib
    <CPCMD> winnt.axp/_s/plbx.lib                <destdir>/libaxp/nt/plbx.lib
    <CPCMD> winnt.axp/_smt/plibmt.lib            <destdir>/libaxp/nt/plibmt.lib
    <CPCMD> winnt.axp/_smt/plbxmt.lib            <destdir>/libaxp/nt/plbxmt.lib
    <CPCMD> complex/winnt.axp/_s/cplx.lib        <destdir>/libaxp/cplx.lib

[ BLOCK <1> clean ]
#==================
    [ INCLUDE clean.ctl ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
