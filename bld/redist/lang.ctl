# Redistributable binary files control file
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <devdir>/redist/causeway/* <relroot>/rel2/binw/
    <CPCMD> <devdir>/redist/dos4gw/* <relroot>/rel2/binw/
    <CPCMD> <devdir>/redist/dos32a/* <relroot>/rel2/binw/
    <CPCMD> <devdir>/redist/pmodew/* <relroot>/rel2/binw/

