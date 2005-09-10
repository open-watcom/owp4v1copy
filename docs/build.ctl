# Documentation Builder Control file
# ==================================

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
   
[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DOC_ROOT>/dos/*.ihp    <RELROOT>/rel2/binw/
    <CPCMD> <DOC_ROOT>/win/*.hlp    <RELROOT>/rel2/binw/
    <CPCMD> <DOC_ROOT>/os2/*.inf    <RELROOT>/rel2/binp/help/
    <CPCMD> <DOC_ROOT>/os2/*.hlp    <RELROOT>/rel2/binp/help/
    <CPCMD> <DOC_ROOT>/nt/*.hlp     <RELROOT>/rel2/binnt/
    <CPCMD> <DOC_ROOT>/nt/*.cnt     <RELROOT>/rel2/binnt/
    <CPCMD> <DOC_ROOT>/readme.txt   <RELROOT>/rel2/readme.txt
    <CPCMD> <DOC_ROOT>/freadme.txt  <RELROOT>/rel2/freadme.txt
    <CPCMD> <DOC_ROOT>/areadme.txt  <RELROOT>/rel2/areadme.txt
   
[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
