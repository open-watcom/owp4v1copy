# RC Builder Control file
# =======================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    cdsay <PROJDIR>/wres
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/mkcdpg
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/exedmp
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/rc
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/restest
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cdsay <PROJDIR>/wresdmp
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
  <CPCMD> <PROJDIR>/rc/*.uni                    <RELROOT>/rel2/binw/
  <CPCMD> <PROJDIR>/rc/*.uni                    <RELROOT>/rel2/binl/

  [ IFDEF (os_osi) <2*> ]
#    <CPCMD> <PROJDIR>/rc/osi386/wrce.exe        <RELROOT>/rel2/binw/wrc.exe

  [ IFDEF (os_dos "") <2*> ]
    <CPCMD> <PROJDIR>/rc/dos386/wrce.exe        <RELROOT>/rel2/binw/wrc.exe

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <PROJDIR>/rc/os2386/wrce.exe        <RELROOT>/rel2/binp/wrc.exe
    <CPCMD> <PROJDIR>/rc/os2386.dll/wrce.dll    <RELROOT>/rel2/binp/dll/wrc.dll
    <CPCMD> <PROJDIR>/exedmp/os2386/exedmp.exe  <RELROOT>/rel2/binp/exedmp.exe

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <PROJDIR>/rc/nt386/wrce.exe         <RELROOT>/rel2/binnt/wrc.exe
    <CPCMD> <PROJDIR>/rc/nt386.dll/wrce.dll     <RELROOT>/rel2/binnt/wrc.dll
    <CPCMD> <PROJDIR>/rc/nt386rt.dll/wrce.dll   <RELROOT>/rel2/binnt/rtdll/wrc.dll
    <CPCMD> <PROJDIR>/mkcdpg/nt386/mkcdpg.exe   <RELROOT>/rel2/binnt/mkcdpg.exe
    <CPCMD> <PROJDIR>/exedmp/nt386/exedmp.exe   <RELROOT>/rel2/binnt/exedmp.exe

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <PROJDIR>/rc/ntaxp/wrce.exe         <RELROOT>/rel2/axpnt/wrc.exe
    <CPCMD> <PROJDIR>/mkcdpg/ntaxp/mkcdpg.exe   <RELROOT>/rel2/axpnt/mkcdpg.exe

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <PROJDIR>/rc/linux386/wrce.exe      <RELROOT>/rel2/binl/wrc
    <CPCMD> <PROJDIR>/rc/linux386/wrce.sym      <RELROOT>/rel2/binl/wrc.sym

[ BLOCK <1> clean ]
#==================
    cdsay <PROJDIR>/wres
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>/mkcdpg
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>/exedmp
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>/rc
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>/restest
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
    cdsay <PROJDIR>/wresdmp
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
