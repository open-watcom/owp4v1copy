# setupgui Builder Control file
# =============================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>\master.ctl ]
[ INCLUDE <LANG_BLD>\wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .
    
[ BLOCK <1> build rel2 ]
    cdsay <PROJDIR>\o.4gw
    wmake setup.exe
    cdsay <PROJDIR>\o.nec
    wmake setup.exe
    cdsay <PROJDIR>\o.os2
    wmake csetup.exe f77setup.exe jcsetup.exe jfsetup.exe
    cdsay <PROJDIR>\o.nt
    wmake csetup.exe f77setup.exe jcsetup.exe jfsetup.exe
    cdsay <PROJDIR>\o.win
    wmake csetup.exe f77setup.exe jcsetup.exe jfsetup.exe
    cdsay <PROJDIR>\mkdisk
    wmake
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
# C/C++
    <CPCMD> <PROJDIR>\o.os2\csetup.exe <distroot>\wc\csetup\install.exe
    <CPCMD> <PROJDIR>\o.nt\csetup.exe <distroot>\wc\csetup\setup32.exe
    <CPCMD> <PROJDIR>\o.win\csetup.exe <distroot>\wc\csetup\setup.exe
    <CPCMD> <PROJDIR>\o.4gw\setup.exe <distroot>\wc\csetup\dossetup.exe
    <CPCMD> <PROJDIR>\o.nec\setup.exe <distroot>\wc\csetup\98setup.exe
    <CPCMD> <PROJDIR>\o.os2\setup.sym <distroot>\wc\csetup\install.sym
    <CPCMD> <PROJDIR>\o.nt\setup.sym <distroot>\wc\csetup\setup32.sym
    <CPCMD> <PROJDIR>\o.win\setup.sym <distroot>\wc\csetup\setup.sym
    <CPCMD> <PROJDIR>\o.4gw\setup.sym <distroot>\wc\csetup\dossetup.sym
    <CPCMD> <PROJDIR>\o.nec\setup.sym <distroot>\wc\csetup\98setup.sym
#fortran  
    <CPCMD> <PROJDIR>\o.os2\f77setup.exe <distroot>\wc\f77setup\install.exe
    <CPCMD> <PROJDIR>\o.nt\f77setup.exe <distroot>\wc\f77setup\setup32.exe
    <CPCMD> <PROJDIR>\o.win\f77setup.exe <distroot>\wc\f77setup\setup.exe
    <CPCMD> <PROJDIR>\o.4gw\setup.exe <distroot>\wc\f77setup\dossetup.exe
    <CPCMD> <PROJDIR>\o.nec\setup.exe <distroot>\wc\f77setup\98setup.exe
    <CPCMD> <PROJDIR>\o.os2\setup.sym <distroot>\wc\f77setup\install.sym
    <CPCMD> <PROJDIR>\o.nt\setup.sym <distroot>\wc\f77setup\setup32.sym
    <CPCMD> <PROJDIR>\o.win\setup.sym <distroot>\wc\f77setup\setup.sym
    <CPCMD> <PROJDIR>\o.4gw\setup.sym <distroot>\wc\f77setup\dossetup.sym
    <CPCMD> <PROJDIR>\o.nec\setup.sym <distroot>\wc\f77setup\98setup.sym
    <CPCMD> <PROJDIR>\mkdisk\*.exe <distroot>\supp\
#japanese c/c++
    <CPCMD> <PROJDIR>\o.os2\jcsetup.exe <distroot>\wc\jcsetup\install.exe
    <CPCMD> <PROJDIR>\o.nt\jcsetup.exe <distroot>\wc\jcsetup\setup32.exe
    <CPCMD> <PROJDIR>\o.win\jcsetup.exe <distroot>\wc\jcsetup\setup.exe
    <CPCMD> <PROJDIR>\o.4gw\setup.exe <distroot>\wc\jcsetup\dossetup.exe
    <CPCMD> <PROJDIR>\o.nec\setup.exe <distroot>\wc\jcsetup\98setup.exe
    <CPCMD> <PROJDIR>\o.os2\setup.sym <distroot>\wc\jcsetup\install.sym
    <CPCMD> <PROJDIR>\o.nt\setup.sym <distroot>\wc\jcsetup\setup32.sym
    <CPCMD> <PROJDIR>\o.win\setup.sym <distroot>\wc\jcsetup\setup.sym
    <CPCMD> <PROJDIR>\o.4gw\setup.sym <distroot>\wc\jcsetup\dossetup.sym
    <CPCMD> <PROJDIR>\o.nec\setup.sym <distroot>\wc\jcsetup\98setup.sym
#japanese fortran 
    <CPCMD> <PROJDIR>\o.os2\jfsetup.exe <distroot>\wc\jf77setup\install.exe
    <CPCMD> <PROJDIR>\o.nt\jfsetup.exe <distroot>\wc\jf77setup\setup32.exe
    <CPCMD> <PROJDIR>\o.win\jfsetup.exe <distroot>\wc\jf77setup\setup.exe
    <CPCMD> <PROJDIR>\o.4gw\setup.exe <distroot>\wc\jf77setup\dossetup.exe
    <CPCMD> <PROJDIR>\o.nec\setup.exe <distroot>\wc\jf77setup\98setup.exe
    <CPCMD> <PROJDIR>\o.os2\setup.sym <distroot>\wc\jf77setup\install.sym
    <CPCMD> <PROJDIR>\o.nt\setup.sym <distroot>\wc\jf77setup\setup32.sym
    <CPCMD> <PROJDIR>\o.win\setup.sym <distroot>\wc\jf77setup\setup.sym
    <CPCMD> <PROJDIR>\o.4gw\setup.sym <distroot>\wc\jf77setup\dossetup.sym
    <CPCMD> <PROJDIR>\o.nec\setup.sym <distroot>\wc\jf77setup\98setup.sym
    <CPCMD> <PROJDIR>\mkdisk\*.exe <distroot>\supp\
	
[ BLOCK <1> clean ]
#==================
    sweep killobjs
