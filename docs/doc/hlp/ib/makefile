
!ifndef hbook
hbook=tools
!endif

.EXTENSIONS: .ib .ihp

deftarg : .symbolic $(hbook).ihp
    if exist $(hbook).ihp   erase $(hbook).ihp > nul
    if exist $(hbook).h     erase $(hbook).h   > nul
    if exist $(%TEMP)\$(hbook).dos erase $(%TEMP)\$(hbook).dos > nul
    copy $(hbook).whp $(%TEMP)\$(hbook).dos
    if exist $(hbook).whp   erase $(hbook).whp > nul
    if exist $(hbook).ib    erase $(hbook).ib > nul

!include ..\makefile

$(hbook).ihp : $(hbook).ib
    $(%HCDOS) -h 25 -w 80 $(hbook).ib $(hbook).ihp | tee /a $(hbook).log
    if exist $(hbook).ihp copy $(hbook).ihp $(%HLPDIR)\ib > nul
    if exist $(hbook).h   copy $(hbook).h   $(%HLPDIR)\ib > nul


$(hbook).ib : $(hbook).whp
    $(%WHPCVT) -ib -i -ix -h -hn -t -tc -e -lk -br -kb -kw -up -dt "Table of Contents" -ds "$(book_title)" $* | tee $(hbook).log
#   $(%WHPCVT) -ib -i -ix -h -hn -t -tc -e -lk -br -kb -kw -up -dt "Table of Contents" -ds "$(book_title)" $*

clean : .symbolic
    if exist $(hbook).whp   erase $(hbook).whp > nul
    if exist $(hbook).ib    erase $(hbook).ib > nul
    if exist $(hbook).ihp   erase $(hbook).ihp > nul
    if exist $(hbook).h     erase $(hbook).h > nul
#   erase $(WHELP)\whelp.cop
#   erase $(WHELP)\whelpdrv.cop
