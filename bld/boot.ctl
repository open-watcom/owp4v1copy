# Builder control file for "bootstrap" style build. Starts out
# with GNU make but later switches to wmake. Intended to be OS
# and compiler agnostic as much as possible. Usable on platforms
# not supported by Open Watcom compilers.
#
# NB: Environment variables BOOTSTRAP_OS and BOOTSTRAP_CPU must
# be set. Currently valid combinations are:
#
#  OS        CPU   comment
#-----------------------------------------
#  linux     386   Linux on IA-32
#  linux     ppc   Linux on 32-bit PowerPC
# 
set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]

set OWLINUXBUILD=bootstrap
# compiled using GNU make + gcc + ar
[ INCLUDE <DEVDIR>/clib/boot.ctl ]
[ INCLUDE <DEVDIR>/builder/lang.ctl ]
[ INCLUDE <DEVDIR>/pmake/lang.ctl ]
[ INCLUDE <DEVDIR>/yacc/lang.ctl ]
[ INCLUDE <DEVDIR>/sdk/rc/boot.ctl ]
[ INCLUDE <DEVDIR>/orl/boot.ctl ]
[ INCLUDE <DEVDIR>/wstrip/boot.ctl ]
[ INCLUDE <DEVDIR>/wmake/boot.ctl ]

# compiled using wmake + gcc
[ INCLUDE <DEVDIR>/nwlib/boot.ctl ]

# compiled using wmake + gcc + wlib
[ INCLUDE <DEVDIR>/ncurses/boot.ctl ]
[ INCLUDE <DEVDIR>/ui/boot.ctl ]
[ INCLUDE <DEVDIR>/vi/boot.ctl ]
[ INCLUDE <DEVDIR>/help/boot.ctl ]
[ INCLUDE <DEVDIR>/wsample/boot.ctl ]
[ INCLUDE <DEVDIR>/aui/boot.ctl ]
[ INCLUDE <DEVDIR>/gui/boot.ctl ]
[ INCLUDE <DEVDIR>/wprof/boot.ctl ]
