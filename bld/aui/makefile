inc_dirs_lang_qnx = $(lang_root)/qh
!ifdef inc_dirs_lang_$(host_OS)
inc_dirs_lang   = $(inc_dirs_lang_$(host_OS))
!else
inc_dirs_lang   = $(lang_root)/h
!endif

.before:
        @set INCPATHS=-i"$(auidir)/h;$(guidir)/h;../h;$(posix_dir)/h;$(inc_dirs_lang)"

.extensions:
.extensions: .exe .lib .obj .i .c .h .res .dlg .rc
.c: $(auidir)/c;.
.h: $(auidir)/h;.

!ifdef on_build_machine
copts = -zq -oxt -d1
!else
!ifndef nodebug
!ifdef codeview
copts = -hc -d1 -zq
!else
copts = -zq -d2
!endif
!else
copts = -zq -oxt -d1
!endif
!endif
!ifndef axp
copts += -fpc
!endif
copts += @INCPATHS
!ifdef __LINUX__
copts += -fo=.obj
!endif

!ifndef librarian
librarian = wlib
!endif

.c.obj:
        $(compile) $(copts) # precompiled headers are buggy! /fh

.c.i:
        $(compile) $(copts) -fh -plc -fo

!ifdef pm
dlgs = dlgrx.dlg dlgsrch.dlg dlgsrcha.dlg &
       japrx.dlg japsrch.dlg japsrcha.dlg
auirc = $(auidir)/pmf/aui.rc
!else
auirc = $(auidir)/h/aui.rc
!endif

all : .symbolic $(auidir)/h/auistr.h aui.lib $(dlgs)
        @%null

eraseall.obj : $(auidir)/compile.gbl
        rm -f *.obj
        wtouch eraseall.obj

regexp.obj : $(posix_dir)/misc/regexp.c ../h/wndregx.h
        $(compile) $(copts) -fi=wndregx.h

objs =  guimain.obj &
        wndgadgt.obj &
        wndrect.obj &
        wndtbatr.obj &
        wndbgch.obj &
        wndstyle.obj &
        wndhist.obj &
        wndhelp.obj &
        wndstat.obj &
        wndchoos.obj &
        wndcreat.obj &
        wndcrsr.obj &
        wndcurr.obj &
        wndid.obj &
        wndmem.obj &
        wndmeth.obj &
        wndutil.obj &
        wndchar.obj &
        wndpop.obj &
        regexp.obj &
        wndscrol.obj &
        wndsel.obj &
        wndsrch.obj &
        wndfdlg.obj &
        wnddlg.obj &
        dlgsrch.obj &
        dlgpick.obj &
        dlgnew.obj &
        wndspawn.obj &
        wndalloc.obj &
        wndshrnk.obj &
        wndtool.obj &
        wnddoin.obj &
        wndresiz.obj &
        wndsysin.obj &
        wndclean.obj &
        wndmacro.obj &
        wndring.obj &
        wndefra.obj &
        wndfonhk.obj &
        wndshthk.obj &
        wndqryhk.obj &
        wndendhk.obj &
        wndrszhk.obj &
        wndsfra.obj &
        wndfont.obj &
        wndldstr.obj &
        wndfront.obj &
        wndpaint.obj &
        wnddirt.obj &
        wnddfclr.obj &
        wndcolor.obj &
        wndmouse.obj &
        wndrxerr.obj &
        wndfref.obj &
        wndmsg.obj &
        wndmnsz.obj &
        wndimain.obj

aui.lib : eraseall.obj $(objs) $(__makefiles__) $(auirc)
        rm -f tmp.lbc
        @for %i in ($(objs)) do @%append tmp.lbc +%i
        $(librarian) -q -b -n aui @tmp.lbc

.rc: ../h
!ifdef pm
$(auirc) : $(auidir)/h/aui.rc
    cp $[@ $^@

dlgrx.dlg : ../h/dlgrx.dlg
    parsedlg -font $[@ dlgrx.dlg

dlgsrch.dlg : ../h/dlgsrch.dlg
    parsedlg -font $[@ dlgsrch.dlg

dlgsrcha.dlg : ../h/dlgsrcha.dlg
    parsedlg -font $[@ dlgsrcha.dlg

japrx.dlg : ../h/japrx.dlg
    parsedlg -font $[@ japrx.dlg

japsrch.dlg : ../h/japsrch.dlg
    parsedlg -font $[@ japsrch.dlg

japsrcha.dlg : ../h/japsrcha.dlg
    parsedlg -font $[@ japsrcha.dlg
!else
.dlg : ../h
!endif

dlgfiles = dlgrx.h dlgsrch.h dlgrx.dlg dlgsrch.dlg dlgsrcha.dlg $(auidir)/h/auistr.h auistr.rc auigui.rc

./mkstr.exe: $(auidir)/c/mkstr.c
        wcl386 -zq $]@ -fe=$^.

$(auidir)/h/auistr.h : ./mkstr.exe $(auidir)/h/aui.msg
        $[@ $^@

clean : .SYMBOLIC
        rm -f *.obj *.pch *.lib *.rc *.lbc *.dlg mkstr.*
        rm -f $(auidir)/h/auistr.h

