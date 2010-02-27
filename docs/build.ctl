# Documentation Builder Control file
# ==================================

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 ]
#=================
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DOC_ROOT>/dos/*.ihp        <RELROOT>/binw/
    <CPCMD> <DOC_ROOT>/win/*.hlp        <RELROOT>/binw/
    <CPCMD> <DOC_ROOT>/os2/*.inf        <RELROOT>/binp/help/
    <CPCMD> <DOC_ROOT>/os2/*.hlp        <RELROOT>/binp/help/
    <CPCMD> <DOC_ROOT>/nt/*.hlp         <RELROOT>/binnt/
    <CPCMD> <DOC_ROOT>/nt/*.cnt         <RELROOT>/binnt/
    <CPCMD> <DOC_ROOT>/htmlhelp/*.chi   <RELROOT>/binnt/help
    <CPCMD> <DOC_ROOT>/htmlhelp/*.chm   <RELROOT>/binnt/help
    <CPCMD> <DOC_ROOT>/pdf/*.pdf        <RELROOT>/docs/
    <CPCMD> <DOC_ROOT>/readme.txt       <RELROOT>/readme.txt
    <CPCMD> <DOC_ROOT>/freadme.txt      <RELROOT>/freadme.txt
    <CPCMD> <DOC_ROOT>/areadme.txt      <RELROOT>/areadme.txt
    <CPCMD> <DOC_ROOT>/instlic.txt      <RELROOT>/instlic.txt
    cdsay .

[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
