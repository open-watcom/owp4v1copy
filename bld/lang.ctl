# --------------------------------------------------------------------

[ INCLUDE <LANG_BLD>/master.ctl ]
[ LOG <devdir>/<LOGFNAME>.<LOGEXT> ]

set FULLBUILD=1

cdsay .

echo Languages Build <1>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#================================
[ INCLUDE <devdir>/langlnx.ctl ]

[ BLOCK . . ]
#
#        First of all build prerequisite utilities used in the build
#
[ INCLUDE <devdir>/builder/lang.ctl ]
[ INCLUDE <devdir>/pmake/lang.ctl ]
[ INCLUDE <devdir>/yacc/lang.ctl ]
[ INCLUDE <devdir>/re2c/lang.ctl ]
[ INCLUDE <devdir>/whpcvt/lang.ctl ]
[ INCLUDE <devdir>/helpcomp/lang.ctl ]
[ INCLUDE <devdir>/ssl/lang.ctl ]
[ INCLUDE <devdir>/posixbld/lang.ctl ]
#
#        Prebuild newest WASM version for building process
[ INCLUDE <devdir>/wasmbld/lang.ctl ]
#
#        Next build online docs. This will create required header files
#        as a by-product.
#
[ INCLUDE <doc_root>/lang.ctl ]
#
#        Before building anything for real, create up-to-date header files
#
[ INCLUDE <devdir>/hdr/lang.ctl ]
#        Same for OS/2 and Win32 headers/import libs
[ INCLUDE <devdir>/os2api/lang.ctl ]
[ INCLUDE <devdir>/w32api/lang.ctl ]
#        Same for Win16 headers/import libs
[ INCLUDE <devdir>/w16api/lang.ctl ]
#
#        Next step, build libraries used by various projects
#
#        RC builds resource file libraries for everybody
[ INCLUDE <devdir>/fmedit/lang.ctl ]
#        BWPI needs to be done before BSDK
[ INCLUDE <devdir>/wpi/lang.ctl ]
#        BSDK must be done after BDISASM ?
[ INCLUDE <devdir>/sdk/lang.ctl ]
#        Librarys for different Programs as next
[ INCLUDE <devdir>/aui/lang.ctl ]
#        BWPI must be done before BSDK and hui, dui and lui
[ INCLUDE <devdir>/gui/lang.ctl ]
[ INCLUDE <devdir>/wclass/lang.ctl ]
[ INCLUDE <devdir>/ui/lang.ctl ]
#        OWL/ORL must be done early so that clients are up-to-date
[ INCLUDE <devdir>/owl/lang.ctl ]
#        BAS must be done after BOWL but before BCFE
[ INCLUDE <devdir>/as/lang.ctl ]
[ INCLUDE <devdir>/orl/lang.ctl ]
#        BDWARF must be done early so that DWARF library users are up-to-date
[ INCLUDE <devdir>/dwarf/lang.ctl ]
#
#        Now build Open Watcom libraries
#
#        emu libraries must be made before C libraries
[ INCLUDE <devdir>/emu/lang.ctl ]
[ INCLUDE <devdir>/emu86/lang.ctl ]
#        C libraries must be made after linker
[ INCLUDE <devdir>/lib_misc/lang.ctl ]
[ INCLUDE <devdir>/clib/lang.ctl ]
[ INCLUDE <devdir>/mathlib/lang.ctl ]
[ INCLUDE <devdir>/cfloat/lang.ctl ]
[ INCLUDE <devdir>/win386/lang.ctl ]
#        src must be made after the C library
[ INCLUDE <devdir>/src/lang.ctl ]
#        graphics library must be made after C library
[ INCLUDE <devdir>/graphlib/lang.ctl ]
[ INCLUDE <devdir>/nwlib/lang.ctl ]
#        BRCSDLL must be before BVI and BVIPER
[ INCLUDE <devdir>/rcsdll/lang.ctl ]
#
#       Then build the compilers
#
#       BW32LDR must be done before EXE's that use the loader
[ INCLUDE <devdir>/w32loadr/lang.ctl ]
#
#        Starting with the code generators
#
#        BWOMP must be done before BWASM and F77, now it is not necessary
#[ INCLUDE <devdir>/womp/lang.ctl ]
#        BWASM must be done early so that inline assembler users are up-to-date
#        now it is not necessary, it can be anywhere
[ INCLUDE <devdir>/wasm/lang.ctl ]
[ INCLUDE <devdir>/cg/lang.ctl ]
#        c front ends must be built after code generator
[ INCLUDE <devdir>/cc/lang.ctl ]
[ INCLUDE <devdir>/cc/wcl/lang.ctl ]
[ INCLUDE <devdir>/plusplus/lang.ctl ]
[ INCLUDE <devdir>/fe_misc/lang.ctl ]
#        FORTRAN 77 compilers must be built after code generators
#        flib & flib286 get built with f77
[ INCLUDE <devdir>/f77/lang.ctl ]
[ INCLUDE <devdir>/f77/samples/lang.ctl ]
[ INCLUDE <devdir>/wl/lang.ctl ]
#
#        Now we can build the C++ libraries - must be done after C library
#        as well as after the C++ compilers
#
[ INCLUDE <devdir>/plusplus/cpplib/lang.ctl ]
#        rtdll must be made after all other libraries
[ INCLUDE <devdir>/rtdll/lang.ctl ]
#
#        Now let's build the utilities and other stuff
#
[ INCLUDE <devdir>/vi/lang.ctl ]
[ INCLUDE <devdir>/wdisasm/lang.ctl ]
[ INCLUDE <devdir>/ndisasm/lang.ctl ]
[ INCLUDE <devdir>/wstub/lang.ctl ]
[ INCLUDE <devdir>/bdiff/lang.ctl ]
[ INCLUDE <devdir>/misc/lang.ctl ]
[ INCLUDE <devdir>/techinfo/lang.ctl ]
[ INCLUDE <devdir>/help/lang.ctl ]
[ INCLUDE <devdir>/cmdedit/lang.ctl ]
[ INCLUDE <devdir>/brinfo/lang.ctl ]
[ INCLUDE <devdir>/trap/lang.ctl ]
[ INCLUDE <devdir>/dip/lang.ctl ]
[ INCLUDE <devdir>/mad/lang.ctl ]
[ INCLUDE <devdir>/wv/lang.ctl ]
[ INCLUDE <devdir>/wsample/lang.ctl ]
[ INCLUDE <devdir>/wprof/lang.ctl ]
[ INCLUDE <devdir>/wmake/lang.ctl ]
[ INCLUDE <devdir>/wtouch/lang.ctl ]
[ INCLUDE <devdir>/wstrip/lang.ctl ]
[ INCLUDE <devdir>/editdll/lang.ctl ]
[ INCLUDE <devdir>/browser/lang.ctl ]
[ INCLUDE <devdir>/idebatch/lang.ctl ]
[ INCLUDE <devdir>/viper/lang.ctl ]
[ INCLUDE <devdir>/viper/bviper/lang.ctl ]
[ INCLUDE <devdir>/mstools/lang.ctl ]
[ INCLUDE <devdir>/viprdemo/lang.ctl ]
[ INCLUDE <devdir>/online/lang.ctl ]
[ INCLUDE <devdir>/diff/lang.ctl ]
[ INCLUDE <devdir>/posix/lang.ctl ]
[ INCLUDE <devdir>/cpp/lang.ctl ]
[ INCLUDE <devdir>/pgchart/lang.ctl ]
[ INCLUDE <devdir>/hlp/lang.ctl ]
[ INCLUDE <devdir>/dig/lang.ctl ]
[ INCLUDE <devdir>/uiforms/lang.ctl ]
[ INCLUDE <devdir>/watcom/lang.ctl ]
[ INCLUDE <devdir>/pbide/lang.ctl ]
[ INCLUDE <devdir>/trmem/lang.ctl ]
#[ INCLUDE <devdir>/version/lang.ctl ]
[ INCLUDE <devdir>/dmpobj/lang.ctl ]
[ INCLUDE <devdir>/wpack/lang.ctl ]
[ INCLUDE <devdir>/cvpack/lang.ctl ]
#[ INCLUDE <devdir>/wic/lang.ctl ]      SH: Removed as not needed for 11.0c
[ INCLUDE <devdir>/redist/lang.ctl ]

# deal with the project which contains this file last --- the BAT directory
# BATDIR Builder Control file
# ===========================
[ INCLUDE <LANG_BLD>/wproj.ctl ]
