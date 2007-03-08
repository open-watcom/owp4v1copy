# Prerequisite Tool Build Control File
# ====================================

set PROJDIR=<CWD>
set PROJNAME=wpack

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

set BLD_OPT=prebuild=1
set ODIR=<PREOBJDIR>

[ BLOCK <OWLINUXBUILD> bootstrap ]
#=================================
set BLD_OPT=bootstrp=1
set ODIR=<OBJDIR>

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<ODIR>
    rm -f -r <PROJDIR>/<ODIR>
    rm -f <OWBINDIR>/<PROJNAME>
    rm -f <OWBINDIR>/<PROJNAME>.exe

[ BLOCK <1> build rel2 ]
#=======================
    mkdir <PROJDIR>/<PREOBJDIR>
    cdsay <PROJDIR>/<PREOBJDIR>
    wmake -h -f ../<BUILD_PLATFORM>/makefile <BLD_OPT>
    <CPCMD> <PROJNAME>.exe <OWBINDIR>/<PROJNAME><CMDEXT>
    
[ BLOCK . . ]
#============
set BLD_OPT=
set ODIR=

cdsay <PROJDIR>
