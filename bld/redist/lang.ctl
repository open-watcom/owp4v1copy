# Redistributable binary files control file
# =========================================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/redist/causeway/* <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/redist/dos4gw/* <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/redist/dos32a/* <RELROOT>/rel2/binw/
    <CPCMD> <DEVDIR>/redist/pmodew/* <RELROOT>/rel2/binw/

