# Languages -- ctl file for FULL build -- the equivalent of BUILD REL2
# --------------------------------------------------------------------

[ INCLUDE <LANG_BLD>\master.ctl ]
[ LOG <devdir>\<LOGFNAME>.<LOGEXT> ]

set FULLBUILD=1

cdsay .

echo Languages Build <1>

#       H must be done first to create up-to-date header files
[ INCLUDE <devdir>\hdr\lang.ctl ]
#       This used to be part of HDR
#[ INCLUDE <devdir>\nwsdk\lang.ctl ]
#       BW32LDR must be done before EXE's that use the loader
[ INCLUDE <devdir>\w32loadr\lang.ctl ]
#        RC builds resource file libraries for everybody
#[ INCLUDE <devdir>\sdk\rc\lang.ctl ]
#        BWOMP must be done before BWASM
[ INCLUDE <devdir>\womp\lang.ctl ]
#        BWASM must be done early so that inline assembler users are up-to-date
[ INCLUDE <devdir>\wasm\lang.ctl ]
#        BDWARF must be done early so that DWARF library users are up-to-date
[ INCLUDE <devdir>\dwarf\lang.ctl ]
#        OWL/ORL must be done early so that clients are up-to-date
[ INCLUDE <devdir>\owl\lang.ctl ]
[ INCLUDE <devdir>\orl\lang.ctl ]
#        BAS must be done after BOWL but before BCFE
[ INCLUDE <devdir>\as\lang.ctl ]
#        BRCSDLL must be before BVI and BVIPER
[ INCLUDE <devdir>\rcsdll\lang.ctl ]
[ INCLUDE <devdir>\vi\lang.ctl ]
[ INCLUDE <devdir>\wdisasm\lang.ctl ]
[ INCLUDE <devdir>\ndisasm\lang.ctl ]
#        BWPI must be done before BSDK and hui, dui and lui
[ INCLUDE <devdir>\wpi\lang.ctl ]
#        BSDK must be done after BDISASM
[ INCLUDE <devdir>\sdk\lang.ctl ]
[ INCLUDE <devdir>\wstub\lang.ctl ]
[ INCLUDE <devdir>\bdiff\lang.ctl ]
[ INCLUDE <devdir>\ui\lang.ctl ]
[ INCLUDE <devdir>\gui\lang.ctl ]
[ INCLUDE <devdir>\aui\lang.ctl ]
[ INCLUDE <devdir>\misc\lang.ctl ]
[ INCLUDE <devdir>\techinfo\lang.ctl ]
[ INCLUDE <devdir>\wclass\lang.ctl ]
[ INCLUDE <devdir>\help\lang.ctl ]
[ INCLUDE <devdir>\cmdedit\lang.ctl ]
[ INCLUDE <devdir>\nwlib\lang.ctl ]
[ INCLUDE <devdir>\cfloat\lang.ctl ]
[ INCLUDE <devdir>\cg\lang.ctl ]
#        c front ends must be built after code generator
[ INCLUDE <devdir>\cc\lang.ctl ]
[ INCLUDE <devdir>\fe_misc\lang.ctl ]
[ INCLUDE <devdir>\brinfo\lang.ctl ]
[ INCLUDE <devdir>\plusplus\lang.ctl ]
[ INCLUDE <devdir>\cc\wcl\lang.ctl ]
[ INCLUDE <devdir>\trap\lang.ctl ]
[ INCLUDE <devdir>\dip\lang.ctl ]
[ INCLUDE <devdir>\mad\lang.ctl ]
[ INCLUDE <devdir>\wv\lang.ctl ]
[ INCLUDE <devdir>\wsample\lang.ctl ]
[ INCLUDE <devdir>\wprof\lang.ctl ]
[ INCLUDE <devdir>\wmake\lang.ctl ]
[ INCLUDE <devdir>\wtouch\lang.ctl ]
[ INCLUDE <devdir>\wstrip\lang.ctl ]
[ INCLUDE <devdir>\wl\lang.ctl ]
[ INCLUDE <devdir>\editdll\lang.ctl ]
[ INCLUDE <devdir>\browser\lang.ctl ]
[ INCLUDE <devdir>\idebatch\lang.ctl ]
[ INCLUDE <devdir>\viper\lang.ctl ]
[ INCLUDE <devdir>\mstools\lang.ctl ]
#        emu libraries must be made before C libraries
[ INCLUDE <devdir>\emu\lang.ctl ]
[ INCLUDE <devdir>\emu86\lang.ctl ]
#        C libraries must be made after linker
[ INCLUDE <devdir>\lib_misc\lang.ctl ]
[ INCLUDE <devdir>\clib\lang.ctl ]
[ INCLUDE <devdir>\mathlib\lang.ctl ]
[ INCLUDE <devdir>\win386\lang.ctl ]
#        build C++ libraries after C library
[ INCLUDE <devdir>\plusplus\cpplib\lang.ctl ]
#        src must be made after the C library
[ INCLUDE <devdir>\src\lang.ctl ]
#        graphics library must be made after C library
[ INCLUDE <devdir>\graphlib\lang.ctl ]                          THORSTEN has to make this work
#        rtdll must be made after all other libraries
[ INCLUDE <devdir>\rtdll\lang.ctl ]
#        FORTRAN 77 compilers must be built after code generators
#        flib & flib286 get built with f77
[ INCLUDE <devdir>\f77\lang.ctl ]
[ INCLUDE <devdir>\f77\samples\lang.ctl ]
[ INCLUDE <devdir>\viprdemo\lang.ctl ]
[ INCLUDE <devdir>\online\lang.ctl ]

[ INCLUDE <devdir>\diff\lang.ctl ]
[ INCLUDE <devdir>\posix\lang.ctl ]
[ INCLUDE <devdir>\cpp\lang.ctl ]
[ INCLUDE <devdir>\pgchart\lang.ctl ]
[ INCLUDE <devdir>\hlp\lang.ctl ]
[ INCLUDE <devdir>\dig\lang.ctl ]
[ INCLUDE <devdir>\uiforms\lang.ctl ]
[ INCLUDE <devdir>\watcom\lang.ctl ]
[ INCLUDE <devdir>\pbide\lang.ctl ]
[ INCLUDE <devdir>\trmem\lang.ctl ]
[ INCLUDE <devdir>\fmedit\lang.ctl ]
#[ INCLUDE <devdir>\version\lang.ctl ]
#[ INCLUDE <devdir>\wpack\lang.ctl ]
[ INCLUDE <devdir>\cvpack\lang.ctl ]
[ INCLUDE <devdir>\build\lang.ctl ]
[ INCLUDE <devdir>\wsplice\lang.ctl ]
[ INCLUDE <devdir>\pmake\lang.ctl ]
#[ INCLUDE <devdir>\wic\lang.ctl ]      SH: Removed as not needed for 11.0c


# deal with the project which contains this file last --- the BAT directory
# BATDIR Builder Control file
# ===========================
[ INCLUDE <LANG_BLD>\wproj.ctl ]
