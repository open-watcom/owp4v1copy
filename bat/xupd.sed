#
# List of RE's to identify files that should NOT be copied to R:\[D]LANG
#
/\\tmp\./d
/\\win32s\\/d
/\\fmr\\/d
/\\japan\\/d
/\\mfc\\/d
/\\toolkt2x\\/d
/\\som\\/d
# /\\h\\os2\\/d
# /src\\win\\/d
# /src\\inc\\/d
# /src\\cpplib\\/d
/__wprj__/d
/clibshar/d
/rel.\\qnx/d
/rel.\\ph/d
/gomtlib/d
/ngraph.h/d
/calendar.jc/d
/wlinkjp.exe/d
/wcc386r.exe/d
/wlinkj.exe/d
/\\clibexam/d
/\\goodies/d
/\\samples/d
/\\sdk\\/d
/\.jxt/d
#/[a-z1-9]\.sym/d       # we want .sym files updated if they are produced
/\.dsy/d
#/\.hlp/d
#/\.hdx/d
#/\.inf/d
#/\.ihp/d
#/\.cnt/d
#
# These next two lines MUST be last
#
s/^.*rel[1-2]\\//
s/^.*$/copy & %1\\&/
