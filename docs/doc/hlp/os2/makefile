!ifndef hbook
hbook=tools
!endif

.EXTENSIONS: .ipf .hlp .inf

deftarg : .symbolic $(hbook).inf $(hbook).hlp
    if exist $(hbook).hlp   erase $(hbook).hlp > nul
    if exist $(hbook).inf   erase $(hbook).inf > nul
    if exist $(hbook).h     erase $(hbook).h   > nul
    copy $(hbook).whp $(%TEMP)\$(hbook).os2
#   rem if exist $(hbook).whp   erase $(hbook).whp > nul
#   rem if exist $(hbook).ipf   erase $(hbook).ipf > nul


!ifeq hbook wccerrs
whp_opts =
!else ifeq hbook wpperrs
whp_opts =
!else ifeq hbook wfcerrs
whp_opts =
!else ifeq hbook lnkerrs
whp_opts =
!else
whp_opts = -h
!endif

gml_opts = set dotarget os2
!include ..\makefile

$(hbook).inf : $(hbook).ipf
    set ipfcartwork=$(book_incs)
    ipfc $(hbook).ipf /inf | tee /a $(hbook).log
    if exist $(hbook).inf copy $(hbook).inf $(%HLPDIR)\os2 > nul
    if exist $(hbook).h   copy $(hbook).h   $(%HLPDIR)\os2 > nul

!ifdef book_os2_help
$(hbook).hlp : $(hbook).ipf
    set ipfcartwork=$(book_incs)
    ipfc $(hbook).ipf
    if exist $(hbook).hlp copy $(hbook).hlp $(%HLPDIR)\os2 > nul
!else
$(hbook).hlp : .symbolic
!endif

ipftarg : .symbolic $(hbook).ipf

$(hbook).ipf : $(hbook).whp
!ifeq hbook wccerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook wpperrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook wfcerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook lnkerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!endif
    whpcvt -ipf $(whp_opts) -i -hn -tl "$(book_title)" $* | tee /a $(hbook).log
    @%create $^*.tbl

clean : .symbolic
        if exist $(hbook).whp erase $(hbook).whp
        if exist $(hbook).ipf erase $(hbook).ipf
        if exist $(hbook).hlp erase $(hbook).hlp
        if exist $(hbook).inf erase $(hbook).inf
        if exist $(hbook).h   erase $(hbook).h
#       if exist $(WHELP)\whelp.cop    erase $(WHELP)\whelp.cop
#       if exist $(WHELP)\whelpdrv.cop erase $(WHELP)\whelpdrv.cop
