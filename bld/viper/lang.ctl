# VIPER Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> viper.doc            <RELROOT>/rel2/ide.doc
    <CPCMD> viper/ide.cfg        <RELROOT>/rel2/binw/ide.cfg
    <CPCMD> viper/idedos.cfg     <RELROOT>/rel2/binw/idedos.cfg
    <CPCMD> viper/idedos32.cfg   <RELROOT>/rel2/binw/idedos32.cfg
    <CPCMD> viper/idewin.cfg     <RELROOT>/rel2/binw/idewin.cfg
    <CPCMD> viper/ideos2.cfg     <RELROOT>/rel2/binw/ideos2.cfg
    <CPCMD> viper/ideos232.cfg   <RELROOT>/rel2/binw/ideos232.cfg
    <CPCMD> viper/idew32.cfg     <RELROOT>/rel2/binw/idew32.cfg
    <CPCMD> viper/idew386.cfg    <RELROOT>/rel2/binw/idew386.cfg
#    <CPCMD> viper/ideads.cfg     <RELROOT>/rel2/binw/ideads.cfg
    <CPCMD> viper/idenlm.cfg     <RELROOT>/rel2/binw/idenlm.cfg
#    <CPCMD> viper/idemfc16.cfg   <RELROOT>/rel2/binw/idemfc16.cfg
#    <CPCMD> viper/idemfc32.cfg   <RELROOT>/rel2/binw/idemfc32.cfg
#    <CPCMD> viper/idemfca.cfg    <RELROOT>/rel2/binw/idemfca.cfg
    <CPCMD> viper/ideaxp.cfg     <RELROOT>/rel2/binw/ideaxp.cfg

  [ IFDEF (os_win "") <2*> ]
    <CPCMD> viper/wini86/ide.exe <RELROOT>/rel2/binw/ide.exe
    <CPCMD> viper/win/idex.cfg   <RELROOT>/rel2/binw/idex.cfg
    <CPCMD> viper/win/wsrv.pif   <RELROOT>/rel2/binw/wsrv.pif
    <CPCMD> viper/win/wd.pif     <RELROOT>/rel2/binw/wd.pif
    <CPCMD> vpdll/wini86/vpdll.dll <RELROOT>/rel2/binw/vpdll.dll

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> viper/os2386/ide.exe <RELROOT>/rel2/binp/ide.exe
    <CPCMD> viper/os2/idex.cfg   <RELROOT>/rel2/binp/idex.cfg

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> viper/nt386/ide.exe  <RELROOT>/rel2/binnt/ide.exe
    <CPCMD> viper/nt/idex.cfg    <RELROOT>/rel2/binnt/idex.cfg
    <CPCMD> vpdll/nt386/vpdll.dll  <RELROOT>/rel2/binnt/vpdll.dll

  [ IFDEF (cpu_axp) <2*> ] 
    <CPCMD> viper/ntaxp/ide.exe  <RELROOT>/rel2/axpnt/ide.exe
    <CPCMD> viper/axp/idex.cfg   <RELROOT>/rel2/axpnt/idex.cfg

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
