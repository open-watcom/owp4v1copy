# Documentation Builder Control file
# ==================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

set OWDOCUMENTATION=0

[ BLOCK .<DOC_BUILD> . .1 ]
    set OWDOCUMENTATION=1

[ BLOCK .<FULLBUILD> . ]
    set OWDOCUMENTATION=1

[ BLOCK <OWDOCUMENTATION> 1 ]
    [ INCLUDE build.ctl ]
