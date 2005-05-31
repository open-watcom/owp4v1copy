# VI Builder Control file
# =======================
set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h bind=1

[ BLOCK <1> rel2 cprel2 ]
#========================
# Common stuff

    <CPCMD> <PROJDIR>/dat/v*.cmd    <RELROOT>/rel2/binp/
    <CPCMD> <PROJDIR>/dat/v*.bat    <RELROOT>/rel2/binw/
    <CPCMD> <PROJDIR>/dat/*.vi      <RELROOT>/rel2/eddat/
    <CPCMD> <PROJDIR>/dat/*.cfg     <RELROOT>/rel2/eddat/
    <CPCMD> <PROJDIR>/dat/*.dat     <RELROOT>/rel2/eddat/
    <CPCMD> <PROJDIR>/dat/*.ini     <RELROOT>/rel2/eddat/
    <CPCMD> <PROJDIR>/dat/bindvi.*  <RELROOT>/rel2/eddat/
    <CPCMD> <PROJDIR>/doc/*.hlp  <RELROOT>/rel2/eddat/

  [ IFDEF (os_dos "") <2*> ]
#    <CPCMD> <PROJDIR>/obj286/vi.exe            <RELROOT>/rel2/binw/vi286.exe
    <CPCMD> <PROJDIR>/dos386/vi.exe             <RELROOT>/rel2/binw/vi.exe
    <CPCMD> <PROJDIR>/ctags/dos386/ctags.exe    <RELROOT>/rel2/binw/ctags.exe
    <CPCMD> <PROJDIR>/bind/dosi86/edbind.exe    <RELROOT>/rel2/binw/edbind.exe

  [ IFDEF (os_win "") <2*> ]
    <CPCMD> <PROJDIR>/wini86/vi.exe             <RELROOT>/rel2/binw/viw.exe
    <CPCMD> <PROJDIR>/win386/vi.exe             <RELROOT>/rel2/binw/viwp.exe

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> <PROJDIR>/nt386/vi.exe              <RELROOT>/rel2/binnt/vi.exe
    <CPCMD> <PROJDIR>/nt386.win/vi.exe          <RELROOT>/rel2/binnt/viw.exe
    <CPCMD> <PROJDIR>/ctags/nt386/ctags.exe     <RELROOT>/rel2/binnt/ctags.exe
    <CPCMD> <PROJDIR>/bind/nt386/edbind.exe     <RELROOT>/rel2/binnt/edbind.exe

  [ IFDEF (cpu_axp) <2*> ]
    <CPCMD> <PROJDIR>/ntaxp/vi.exe              <RELROOT>/rel2/axpnt/vi.exe
    <CPCMD> <PROJDIR>/ntaxp.win/viaxpw.exe      <RELROOT>/rel2/axpnt/viw.exe
    <CPCMD> <PROJDIR>/ctags/ntaxp/ctags.exe     <RELROOT>/rel2/axpnt/ctags.exe

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> <PROJDIR>/os2386/vi.exe             <RELROOT>/rel2/binp/vi.exe
    <CPCMD> <PROJDIR>/ctags/os2386/ctags.exe    <RELROOT>/rel2/binp/ctags.exe
    <CPCMD> <PROJDIR>/bind/os2386/edbind.exe    <RELROOT>/rel2/binp/edbind.exe

  [ IFDEF (os_qnx) <2*> ]
    <CPCMD> <PROJDIR>/qnxi86/vi.exe             <RELROOT>/rel2/qnx/binq/vi

  [ IFDEF (os_linux "") <2*> ]
    <CPCMD> <PROJDIR>/linux386/vi.exe           <RELROOT>/rel2/binl/vi
    <CPCMD> <PROJDIR>/linux386/vi.sym           <RELROOT>/rel2/binl/vi.sym
    <CPCMD> <PROJDIR>/ctags/linux386/ctags.exe  <RELROOT>/rel2/binl/ctags
    <CPCMD> <PROJDIR>/bind/linux386/edbind.exe  <RELROOT>/rel2/binl/edbind

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
