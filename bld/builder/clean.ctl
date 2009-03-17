# Build tools cleaning Control File
# =================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> clean ]
#==================
    rm -f <OWBINDIR>/langdat
    rm -f <OWBINDIR>/mksetup
    rm -f <OWBINDIR>/cdsay
    rm -f <OWBINDIR>/sweep
    rm -f <OWBINDIR>/wsplice
    rm -f <OWBINDIR>/comstrip
    rm -f <OWBINDIR>/wgrep
    rm -f <OWBINDIR>/wtouch
    rm -f <OWBINDIR>/aliasgen
    rm -f <OWBINDIR>/wmake
    rm -f <OWBINDIR>/langdat.exe
    rm -f <OWBINDIR>/mksetup.exe
    rm -f <OWBINDIR>/cdsay.exe
    rm -f <OWBINDIR>/sweep.exe
    rm -f <OWBINDIR>/wsplice.exe
    rm -f <OWBINDIR>/diff.exe
    rm -f <OWBINDIR>/comstrip.exe
    rm -f <OWBINDIR>/wgrep.exe
    rm -f <OWBINDIR>/wtouch.exe
    rm -f <OWBINDIR>/aliasgen.exe
    rm -f <OWBINDIR>/wmake.exe
    rm -f <OWBINDIR>/rm.exe
    rm -f <OWBINDIR>/awk.exe
    rm -f <OWBINDIR>/sed.exe

[ BLOCK . . ]
#============

