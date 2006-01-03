# yacc Builder Control file
# =========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>

[ BLOCK <1> boot ]
#=================
    echo Building the yacc bootstrap
    <MAKE> -f gnumake
    <CPCMD> bootstrp/yacc <DEVDIR>/build/bin/byacc
    <CPCMD> bootstrp/yacc <DEVDIR>/build/binl/byacc
    cdsay <PROJDIR>
