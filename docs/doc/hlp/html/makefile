
!ifndef hbook
hbook=tools
!endif

.EXTENSIONS: .htm

all: .symbolic $(hbook).htm
    @%null

gml_opts = set dotarget html

!ifdef on_build_machine
whp_ver = whpcvt
!else
whp_ver = whpcvto2
!endif

!ifeq hbook wccerrs
whp_opts =
!else ifeq hbook wpperrs
whp_opts =
!else ifeq hbook wfcerrs
whp_opts =
!else
whp_opts = -h
!endif

!include ..\makefile

$(hbook).htm : $(hbook).whp
!ifeq hbook wccerrs
    copy sysusr02.gml $*.h
!else ifeq hbook wfcerrs
    copy sysusr02.gml $*.h
!endif
    $(WHPCVT)\$(whp_ver) -html $(whp_opts) -i -hn -tl "$(book_title)" $* | tee -a $(hbook).log
    @%create $^*.tbl

clean : .symbolic
    erase $(hbook).whp
    erase $(hbook).htm
    erase $(hbook).h
#   erase $(WHELP)\whelp.cop
#   erase $(WHELP)\whelpdrv.cop
