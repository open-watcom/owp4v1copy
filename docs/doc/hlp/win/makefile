
!ifndef hbook
hbook=tools
!endif

.EXTENSIONS: .hlp .h95 .rtf .cnt .hpj .p95

deftarg : .symbolic $(hbook).h95 $(hbook).hlp
    if exist $(hbook).hlp   erase $(hbook).hlp > nul
    if exist $(hbook).h95   erase $(hbook).h95 > nul
    if exist $(hbook).cnt   erase $(hbook).cnt > nul
    if exist $(hbook).h     erase $(hbook).h   > nul
    if exist $(%TEMP)\$(hbook).win erase $(%TEMP)\$(hbook).win > nul
    copy $(hbook).whp $(%TEMP)\$(hbook).win
    if exist $(hbook).whp   erase $(hbook).whp > nul
    if exist $(hbook).rtf   erase $(hbook).rtf > nul

w95targ : .symbolic $(hbook).h95
    if exist $(hbook).h95   erase $(hbook).h95 > nul
    if exist $(hbook).cnt   erase $(hbook).cnt > nul
    if exist $(hbook).h     erase $(hbook).h   > nul
    if exist $(%TEMP)\$(hbook).win erase $(%TEMP)\$(hbook).win > nul
    copy $(hbook).whp $(%TEMP)\$(hbook).win
    if exist $(hbook).whp   erase $(hbook).whp > nul
    if exist $(hbook).rtf   erase $(hbook).rtf > nul

!ifeq hbook wccerrs
whp_opts = -hn
!else ifeq hbook wpperrs
whp_opts = -hn
!else ifeq hbook wfcerrs
whp_opts = -hn
!else ifeq hbook wjcerrs
whp_opts = -hn
!else ifeq hbook wlnkerrs
whp_opts = -hn
!else ifeq hbook wrcerrs
whp_opts = -hn
!else
whp_opts = -h
!endif

!include ..\makefile

!ifeq hbook cmix
$(hbook).h95 : .symbolic

!else ifeq hbook fmix
$(hbook).h95 : .symbolic

!else
$(hbook).h95 : $(hbook).p95 $(hbook).cnt $(hbook).rtf
    if exist $(hbook).hlp erase $(hbook).hlp
    $(%WIN95HC) -xn $(hbook).p95
    if exist $(hbook).hlp copy $(hbook).hlp $(%HLPDIR)\win95 > nul
    if exist $(hbook).hlp ren  $(hbook).hlp $(hbook).h95     > nul
    if exist $(hbook).cnt copy $(hbook).cnt $(%HLPDIR)\win95 > nul

!endif

$(hbook).hlp : $(hbook).hpj $(hbook).rtf
!ifeq book_winhc hc31
    $(%WIN31HC) $(hbook).hpj
!else
    $(%WAT31HC) $(hbook).hpj
!endif
    if exist $(hbook).hlp copy $(hbook).hlp $(%HLPDIR)\win > nul
    if exist $(hbook).h   copy $(hbook).h   $(%HLPDIR)\win > nul


$(hbook).p95 : makefile
    @%create $^@
    @%append $^@ ; This file is maintained by WMAKE. Do not modify this file directly.
    @%append $^@
    @%append $^@ [OPTIONS]
    @%append $^@ HCW=0
    @%append $^@ COMPRESS=60 Hall Zeck
    @%append $^@ LCID=0x409 0x0 0x0 ;English (United States)
    @%append $^@ REPORT=Yes
    @%append $^@ CONTENTS=table_of_contents
    @%append $^@ TITLE=$(book_title) Help
    @%append $^@ COPYRIGHT=Copyright © 1996 Sybase, Inc. and its subsidiaries. All rights reserved. %date
    @%append $^@ BMROOT=$(book_incs)
    @%append $^@ ROOT=.
    @%append $^@ HLP=.\$(hbook).hlp
    @%append $^@ ERRORLOG=.\$(hbook).err
    @%append $^@
    @%append $^@ [FILES]
    @%append $^@ $(hbook).rtf
    @%append $^@
    @%append $^@ [MAP]
    @%append $^@ $#include <$(hbook).hh>
    @%append $^@
    @%append $^@ [WINDOWS]
    @%append $^@ main="$(book_title) Help",,28932
    @%append $^@
    @%append $^@ [CONFIG]
    @%append $^@ BrowseButtons()
    @%append $^@ CreateButton( "btn_index", "&Index", "JumpId( `$(hbook).hlp', `index_of_topics' )" )
    @%append $^@ CreateButton( "btn_up", "&Up", "Contents()" )

$(hbook).hpj : makefile
    @%create $^@
    @%append $^@ ; This file is maintained by WMAKE. Do not modify this file directly.
    @%append $^@
    @%append $^@ [Options]
    @%append $^@ COMPRESS=TRUE
    @%append $^@ WARNING=3
    @%append $^@ CONTENTS=table_of_contents
    @%append $^@ Title=$(book_title) Help
    @%append $^@ BMROOT=$(book_incs)
    @%append $^@ OLDKEYPHRASE=YES
    @%append $^@ ROOT=.
    @%append $^@ [Config]
    @%append $^@ BrowseButtons()
    @%append $^@ CreateButton( "btn_index", "&Index", "JumpId( `$(hbook).hlp', `index_of_topics' )" )
    @%append $^@ CreateButton( "btn_up", "&Up", "Contents()" )
    @%append $^@ [Files]
    @%append $^@ $(hbook).rtf
    @%append $^@ [MAP]
    @%append $^@ $#include <$(hbook).hh>
    @%append $^@ ; end of HPJ file

$(hbook).cnt : makefile $(hbook).whp
!ifeq hbook cbooks
    copy $(%doc_root)\doc\gs\cbooks.cnt
!else ifeq hbook fbooks
    copy $(%doc_root)\doc\gs\fbooks.cnt
!else ifeq hbook ebooks
    copy $(%doc_root)\doc\gs\ebooks.cnt
!else
    @%create $^@
    @%append $^@ :Base $(hbook).hlp>main
    @%append $^@ :Title $(book_title)
    @%append $^@ :Index $(book_title) Help=$(hbook).hlp
    @%append $^@
    @%append $^@ 1 $(book_title)
    copy $^@+sysusr03.gml $^@ > nul
    del  sysusr03.gml
!endif
    !vi -d -q "-k:1,$$s/=$$/=ZZZ/\n:g/=ZZZ/j\n:1,$$s/=ZZZ /=/\n:wq\n" $^@
!ifeq multi_chap false
    !vi -d -q "-k:/^1/d\n:1,$$s/^2/1/\n:1,$$s/^3/2/\n:1,$$s/^4/3/\n:1,$$s/^5/4/\n:1,$$s/^6/5/\n:wq\n" $^@
!endif
    echo Done with vi scripts

$(hbook).rtf : $(hbook).whp
!ifeq hbook wccerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook wpperrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook wfcerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook wjcerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook wlnkerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!else ifeq hbook wrcerrs
    copy sysusr02.gml $*.h
    del  sysusr02.gml
!endif
    whpcvt -rtf $(whp_opts) -i -hh -k -b -up -t -e -lk $*
    if exist $(hbook).rtf copy $(hbook).rtf $(%HLPDIR)\win > nul

clean : .symbolic
    if exist $(hbook).whp erase $(hbook).whp
    if exist $(hbook).rtf erase $(hbook).rtf
    if exist $(hbook).hlp erase $(hbook).hlp
    if exist $(hbook).hpj erase $(hbook).hpj
    if exist $(hbook).h95 erase $(hbook).h95
    if exist $(hbook).p95 erase $(hbook).p95
    if exist $(hbook).h erase $(hbook).h
#   erase $(WHELP)\whelp.cop
#   erase $(WHELP)\whelpdrv.cop
