# DIP Builder Control file
# ========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
#       OS2
#        <CPCMD> <devdir>/dip/dwarf/os2i86/dwarf.dll         <relroot>/rel2/binp/dll/
#        <CPCMD> <devdir>/dip/dwarf/os2i86/dwarf.sym         <relroot>/rel2/binp/dll/
#        <CPCMD> <devdir>/dip/codeview/os2i86/codeview.dll   <relroot>/rel2/binp/dll/
#        <CPCMD> <devdir>/dip/codeview/os2i86/codeview.sym   <relroot>/rel2/binp/dll/
#        <CPCMD> <devdir>/dip/export/os2i86/export.dll       <relroot>/rel2/binp/dll/
#        <CPCMD> <devdir>/dip/export/os2i86/export.sym       <relroot>/rel2/binp/dll/
#        <CPCMD> <devdir>/dip/watcom/os2i86/watcom.dll       <relroot>/rel2/binp/dll/
#        <CPCMD> <devdir>/dip/watcom/os2i86/watcom.sym       <relroot>/rel2/binp/dll/
        <CPCMD> <devdir>/dip/dwarf/os2386/dwarf.d32         <relroot>/rel2/binp/
        <CPCMD> <devdir>/dip/codeview/os2386/codeview.d32   <relroot>/rel2/binp/
        <CPCMD> <devdir>/dip/export/os2386/export.d32       <relroot>/rel2/binp/
        <CPCMD> <devdir>/dip/watcom/os2386/watcom.d32       <relroot>/rel2/binp/
#       NT
        <CPCMD> <devdir>/dip/dwarf/nt386/dwarf.dll          <relroot>/rel2/binnt/
#        <CPCMD> <devdir>/dip/dwarf/nt386/dwarf.sym          <relroot>/rel2/binnt/
        <CPCMD> <devdir>/dip/codeview/nt386/codeview.dll    <relroot>/rel2/binnt/
#        <CPCMD> <devdir>/dip/codeview/nt386/codeview.sym    <relroot>/rel2/binnt/
        <CPCMD> <devdir>/dip/export/nt386/export.dll        <relroot>/rel2/binnt/
#        <CPCMD> <devdir>/dip/export/nt386/export.sym        <relroot>/rel2/binnt/
        <CPCMD> <devdir>/dip/watcom/nt386/watcom.dll        <relroot>/rel2/binnt/
#        <CPCMD> <devdir>/dip/watcom/nt386/watcom.sym        <relroot>/rel2/binnt/
#       WINDOWS
        <CPCMD> <devdir>/dip/dwarf/wini86/dwarf.dll         <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/dwarf/wini86/dwarf.sym         <relroot>/rel2/binw/
        <CPCMD> <devdir>/dip/codeview/wini86/codeview.dll   <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/codeview/wini86/codeview.sym   <relroot>/rel2/binw/
        <CPCMD> <devdir>/dip/export/wini86/export.dll       <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/export/wini86/export.sym       <relroot>/rel2/binw/
        <CPCMD> <devdir>/dip/watcom/wini86/watcom.dll       <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/watcom/wini86/watcom.sym       <relroot>/rel2/binw/
#       DOS
        <CPCMD> <devdir>/dip/dwarf/dos386/dwarf.dip         <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/dwarf/dos386/dwarf.sym         <relroot>/rel2/binw/dwarf.dsy
        <CPCMD> <devdir>/dip/codeview/dos386/codeview.dip   <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/codeview/dos386/codeview.sym   <relroot>/rel2/binw/codeview.dsy
        <CPCMD> <devdir>/dip/export/dos386/export.dip       <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/export/dos386/export.sym       <relroot>/rel2/binw/export.dsy
        <CPCMD> <devdir>/dip/watcom/dos386/watcom.dip       <relroot>/rel2/binw/
#        <CPCMD> <devdir>/dip/watcom/dos386/watcom.sym       <relroot>/rel2/binw/watcom.dsy
#       QNX
#        <CPCMD> <devdir>/dip/dwarf/qnx386/dwarf.dip         <relroot>/rel2/qnx/watcom/wd/
#        <CPCMD> <devdir>/dip/dwarf/qnx386/dwarf.sym         <relroot>/rel2/qnx/sym/
#        <CPCMD> <devdir>/dip/codeview/qnx386/codeview.dip   <relroot>/rel2/qnx/watcom/wd/
#        <CPCMD> <devdir>/dip/codeview/qnx386/codeview.sym   <relroot>/rel2/qnx/sym/
#        <CPCMD> <devdir>/dip/export/qnx386/export.dip       <relroot>/rel2/qnx/watcom/wd/
#        <CPCMD> <devdir>/dip/export/qnx386/export.sym       <relroot>/rel2/qnx/sym/
#        <CPCMD> <devdir>/dip/watcom/qnx386/watcom.dip       <relroot>/rel2/qnx/watcom/wd/
#        <CPCMD> <devdir>/dip/watcom/qnx386/watcom.sym       <relroot>/rel2/qnx/sym/
#       ALPHA
#        <CPCMD> <devdir>/dip/dwarf/ntaxp/dwarf.dll          <relroot>/rel2/axpnt/
#        <CPCMD> <devdir>/dip/dwarf/ntaxp/dwarf.sym          <relroot>/rel2/axpnt/
#        <CPCMD> <devdir>/dip/codeview/ntaxp/codeview.dll    <relroot>/rel2/axpnt/
#        <CPCMD> <devdir>/dip/codeview/ntaxp/codeview.sym    <relroot>/rel2/axpnt/
#        <CPCMD> <devdir>/dip/export/ntaxp/export.dll        <relroot>/rel2/axpnt/
#        <CPCMD> <devdir>/dip/export/ntaxp/export.sym        <relroot>/rel2/axpnt/
#        <CPCMD> <devdir>/dip/watcom/ntaxp/watcom.dll        <relroot>/rel2/axpnt/
#        <CPCMD> <devdir>/dip/watcom/ntaxp/watcom.sym        <relroot>/rel2/axpnt/
#       LINUX
        <CPCMD> <devdir>/dip/dwarf/linux386/dwarf.dip       <relroot>/rel2/binl/
#        <CPCMD> <devdir>/dip/dwarf/linux386/dwarf.sym       <relroot>/rel2/binl/
        <CPCMD> <devdir>/dip/watcom/linux386/watcom.dip     <relroot>/rel2/binl/
#        <CPCMD> <devdir>/dip/watcom/linux386/watcom.sym     <relroot>/rel2/binl/
#        <CPCMD> <devdir>/dip/codeview/linux386/codeview.dip <relroot>/rel2/binl/
#        <CPCMD> <devdir>/dip/codeview/linux386/codeview.sym <relroot>/rel2/binl/
#        <CPCMD> <devdir>/dip/export/linux386/export.dip     <relroot>/rel2/binl/
#        <CPCMD> <devdir>/dip/export/linux386/export.sym     <relroot>/rel2/binl/


[ BLOCK <1> clean ]
#==================
    pmake -d all <2> <3> <4> <5> <6> <7> <8> <9> -h clean
