# Builder control file for Linux. Using a separate file
# because of bootstrapping issues and the fact that only
# a small subset can be built
set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]

[ INCLUDE <devdir>/clib/prereq.ctl ]
[ INCLUDE <devdir>/builder/lang.ctl ]
[ INCLUDE <devdir>/pmake/lang.ctl ]
[ INCLUDE <devdir>/yacc/lang.ctl ]
[ INCLUDE <devdir>/sdk/rc/prereq.ctl ]
[ INCLUDE <devdir>/orl/prereq.ctl ]
[ INCLUDE <devdir>/wstrip/prereq.ctl ]
[ INCLUDE <devdir>/wmake/prereq.ctl ]

[ INCLUDE <devdir>/nwlib/prereq.ctl ]
[ INCLUDE <devdir>/cfloat/prereq.ctl ]
[ INCLUDE <devdir>/owl/prereq.ctl ]
[ INCLUDE <devdir>/dwarf/prereq.ctl ]
[ INCLUDE <devdir>/wasm/prereq.ctl ]
[ INCLUDE <devdir>/cg/prereq.ctl ]
[ INCLUDE <devdir>/cc/prereq.ctl ]
[ INCLUDE <devdir>/wl/prereq.ctl ]

[ INCLUDE <devdir>/hdr/lang.ctl ]
[ INCLUDE <devdir>/clib/lang.ctl ]
[ INCLUDE <devdir>/mathlib/lang.ctl ]
[ INCLUDE <devdir>/cfloat/lang.ctl ]
[ INCLUDE <devdir>/cc/wcl/lang.ctl ]
[ INCLUDE <devdir>/sdk/rc/lang.ctl ]
[ INCLUDE <devdir>/orl/lang.ctl ]
[ INCLUDE <devdir>/owl/lang.ctl ]
[ INCLUDE <devdir>/dwarf/lang.ctl ]
[ INCLUDE <devdir>/cg/lang.ctl ]
[ INCLUDE <devdir>/cc/lang.ctl ]
[ INCLUDE <devdir>/wl/lang.ctl ]
[ INCLUDE <devdir>/wmake/lang.ctl ]
[ INCLUDE <devdir>/wasm/lang.ctl ]
[ INCLUDE <devdir>/ndisasm/lang.ctl ]

[ BLOCK <1> clean ]
    rm -rf <devdir>/clib/<OBJDIR>
    rm -rf <devdir>/builder/<OBJDIR>
    rm -rf <devdir>/pmake/<OBJDIR>
    rm -rf <devdir>/yacc/<OBJDIR>
    rm -rf <devdir>/sdk/rc/wres/<OBJDIR>
    rm -rf <devdir>/sdk/rc/rc/<OBJDIR>
    rm -rf <devdir>/orl/<OBJDIR>
    rm -rf <devdir>/wstrip/<OBJDIR>
    rm -rf <devdir>/wmake/<OBJDIR>

    rm -rf <devdir>/nwlib/<OBJDIR>
    rm -rf <devdir>/cfloat/<OBJDIR>
    rm -rf <devdir>/owl/<OBJDIR>
    rm -rf <devdir>/dwarf/dw/<OBJDIR>
    rm -rf <devdir>/cg/intel/386/<OBJDIR>
    rm -rf <devdir>/cc/<OBJDIR>
    rm -rf <devdir>/wasm/<OBJDIR>
    rm -rf <devdir>/wl/<OBJDIR>
