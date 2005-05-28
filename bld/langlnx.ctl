# Builder control file for Linux. Using a separate file
# because of bootstrapping issues and the fact that only
# a small subset can be built
set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]

set OWLINUXBUILD=bootstrap
# compiled using GNU make + gcc + ar
[ INCLUDE <DEVDIR>/clib/prereq.ctl ]
[ INCLUDE <DEVDIR>/builder/lang.ctl ]
[ INCLUDE <DEVDIR>/pmake/lang.ctl ]
[ INCLUDE <DEVDIR>/yacc/prereq.ctl ]
[ INCLUDE <DEVDIR>/sdk/rc/prereq.ctl ]
[ INCLUDE <DEVDIR>/orl/prereq.ctl ]
[ INCLUDE <DEVDIR>/wstrip/prereq.ctl ]
[ INCLUDE <DEVDIR>/wmake/prereq.ctl ]

# compiled using wmake + gcc + wlib
[ INCLUDE <DEVDIR>/nwlib/prereq.ctl ]
[ INCLUDE <DEVDIR>/cfloat/prereq.ctl ]
[ INCLUDE <DEVDIR>/owl/prereq.ctl ]
[ INCLUDE <DEVDIR>/dwarf/prereq.ctl ]
[ INCLUDE <DEVDIR>/wl/prereq.ctl ]
[ INCLUDE <DEVDIR>/wasm/prereq.ctl ]
[ INCLUDE <DEVDIR>/cg/prereq.ctl ]
[ INCLUDE <DEVDIR>/cc/prereq.ctl ]

# compiled using watcom utilities
[ INCLUDE <DEVDIR>/hdr/lang.ctl ]
[ INCLUDE <DEVDIR>/clib/lang.ctl ]
[ INCLUDE <DEVDIR>/mathlib/lang.ctl ]
[ INCLUDE <DEVDIR>/emu/lang.ctl ]
[ INCLUDE <DEVDIR>/cc/wcl/prereq.ctl ]
[ INCLUDE <DEVDIR>/cfloat/lang.ctl ]
[ INCLUDE <DEVDIR>/owl/lang.ctl ]
[ INCLUDE <DEVDIR>/dwarf/lang.ctl ]
[ INCLUDE <DEVDIR>/cg/lang.ctl ]
[ INCLUDE <DEVDIR>/cc/lang.ctl ]
[ INCLUDE <DEVDIR>/plusplus/lang.ctl ]
[ INCLUDE <DEVDIR>/plusplus/cpplib/lang.ctl ]
[ INCLUDE <DEVDIR>/cc/wcl/lang.ctl ]

[ BLOCK <1> build cprel2 ]
    <CPCMD> <DEVDIR>/cc/wcl/linux386.386/wcl386.exe <DEVDIR>/build/binl/wcl386
    <CPCMD> <DEVDIR>/cc/wcl/linux386.i86/wcl.exe <DEVDIR>/build/binl/wcl
    <CPCMD> <DEVDIR>/cc/linux386.386/wcc386c.elf <DEVDIR>/build/binl/wcc386
    <CPCMD> <DEVDIR>/cc/linux386.i86/wcci86c.elf <DEVDIR>/build/binl/wcc
    <CPCMD> <DEVDIR>/plusplus/linux386.386/wcpp386.elf <DEVDIR>/build/binl/wpp386
    <CPCMD> <DEVDIR>/plusplus/linux386.i86/wcppi86.elf <DEVDIR>/build/binl/wpp

[ BLOCK . . ]
set OWLINUXBUILD=normal

# all is ready to do everything else now

[ BLOCK <1> clean ]
    rm -rf <DEVDIR>/clib/<OBJDIR>
    rm -rf <DEVDIR>/builder/<OBJDIR>
    rm -rf <DEVDIR>/pmake/<OBJDIR>
    rm -rf <DEVDIR>/yacc/<OBJDIR>
    rm -rf <DEVDIR>/sdk/rc/wres/<OBJDIR>
    rm -rf <DEVDIR>/sdk/rc/rc/<OBJDIR>
    rm -rf <DEVDIR>/orl/<OBJDIR>
    rm -rf <DEVDIR>/wstrip/<OBJDIR>
    rm -rf <DEVDIR>/wmake/<OBJDIR>

    rm -rf <DEVDIR>/nwlib/<OBJDIR>
    rm -rf <DEVDIR>/cfloat/<OBJDIR>
    rm -rf <DEVDIR>/owl/<OBJDIR>
    rm -rf <DEVDIR>/dwarf/dw/<OBJDIR>
    rm -rf <DEVDIR>/cg/intel/386/<OBJDIR>
    rm -rf <DEVDIR>/cc/<OBJDIR>
    rm -rf <DEVDIR>/wasm/<OBJDIR>
    rm -rf <DEVDIR>/wl/<OBJDIR>
