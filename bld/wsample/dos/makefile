#pmake: all build os_dos cpu_i86
.erase

linker = *wlink

objects = sample.obj sampdata.obj realmode.obj rmhooks.obj sampdos.obj &
          io.obj sysio.obj timasm.obj interc.obj sampexe.obj settime.obj &
          wmsg.obj

cflags = /zq/ms/d1/oaxt/w3/dREPORT_TYPE=__interrupt /dFAR_PTR=__far /fiwatcom.h /i$(dig_dir)\h
cflags2 = /zu $(cflags)

xname   =   wsample

..\bin\$(xname).exe : $(objects) makefile $(xname).res
    @%write tmp.lnk $(%debug) option map, quiet system dos
    @%append tmp.lnk libpath $(wres_dir)\lib
    @%append tmp.lnk lib wress
    @%append tmp.lnk name ..\bin\$(xname)
    @for %i in ($objects) do @%append tmp.lnk file %i
    wlink @tmp.lnk
    wstrip /q /a /r ..\bin\$(xname).exe . $(xname).res

.c: .;..\c
.c.obj :
    $(comp286) $[* $cflags2

.asm: .
.asm.obj :
#    $(asm286) $[*/ZD;
    wasm $[*

$(xname).res : ..\h\wsample.rc ..\h\wmsg.h
        wrc /bt=windows /zk0 /r /dDOS /fo=$^. ..\h\wsample.rc

wmsg.obj: ..\c\wmsg.c ..\h\wmsg.h
    $(comp286) $[* $cflags

clean : .symbolic
    @rm -f *.obj *.res *.lnk *.map
    @rm -f ..\bin\$(xname).exe
