compile = $(comp386) $[@ -ox-4-d1-w4-zq-i$(watcom_h)

.c: ../c
.c.obj :
        $(compile)

objs = &
cmdall.obj &
cmddos.obj &
cmdline.obj &
cmdnov.obj &
cmdos2.obj &
cmdphar.obj &
cmdqnx.obj &
cmdutils.obj &
demangle.obj &
msg.obj &
ntio.obj &
linkutil.obj &
wlnkmsg.obj &
global.obj &
trmem.obj &
dmem.obj &
command.obj

command.exe : $(objs) wlink.res
        @%create tmp.lnk
        @for %i in ($(objs)) do @%append tmp.lnk file %i
        $(linker) debug all sys pharlap option undefsok,map libpath $(wres_dir)/lib lib wresf name command @tmp.lnk
!ifndef new_pharlap
        $(pharlap)bind386 $(pharlap)run386b.exe command -exe $^@
!else
        copy/b $(pharlap)stub386.exe+command.exp $^@
!endif
        wstrip -q -a -r command.exe . wlink.res

trmem.obj : $(%DEVDIR)/trmem/trmem.c
        $(compile) -i="$(%DEVDIR)/trmem"

dmem.obj : mem.c
        $(compile) -dTRACKER -fo=dmem.obj -of+ -i="$(%DEVDIR)/trmem"

msg.obj : msg.c ../h/msg.h ../h/wlnkmsg.h
        $(compile) -i="$(%DEVDIR)/plusplus/supp"

demangle.obj : $(%DEVDIR)/plusplus/supp/demangle.c
        $(compile) -i="$(%DEVDIR)/plusplus/supp"

wlink.res : ../h/wlink.rc ../h/wlnkmsg.h
#       wrc -r -fo=$^@ ../h/wlink.rc
        wccp -pl -zw -dRC_INVOKED -fo ../h/wlink.rc
        wrc -r -zn -fo=$^@ wlink.i
        del wlink.i
