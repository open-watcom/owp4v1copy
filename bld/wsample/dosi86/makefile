#pmake: all build os_dos cpu_i86
host_OS = dos
host_CPU = i86

!include cproj.mif

.erase

#linker = *wlink

objects = sample.obj sampdata.obj realmode.obj rmhooks.obj sampdos.obj &
          io.obj sysio.obj timasm.obj interc.obj sampexe.obj settime.obj &
          wmsg.obj

cflags = -zq-ms-d1-oaxt-w3-dREPORT_TYPE=__interrupt -dFAR_PTR=__far -fi=watcom.h -i"$(dig_dir)/h"
cflags2 = -zu $(cflags)

xname   =   wsample

../bin/$(xname).exe : $(objects) makefile $(xname).res
    @%write tmp.lnk $(%debug) option map, quiet system dos
    @%append tmp.lnk libpath $(wres_dir)/lib
    @%append tmp.lnk lib wress
    @%append tmp.lnk name $^@
    @for %i in ($objects) do @%append tmp.lnk file %i
    $(linker) @tmp.lnk
    wstrip -q -a -r $^@ . $(xname).res

.c: .;../c
.c.obj :
#    $(comp286) $[* $cflags2
    $(cc) $[* $cflags2

.asm: .
.asm.obj :
#    $(asm286) $[*/ZD;
#    *wasm -q $[*
    $(as) $[*

$(xname).res : ../h/wsample.rc ../h/wmsg.h
    $(rc_aui) -zk0 -dDOS -fo=$^. $[@

wmsg.obj: ../c/wmsg.c ../h/wmsg.h
#    $(comp286) $[* $cflags
    $(cc) $[* $cflags

clean : .symbolic
    @rm -f *.obj *.res *.lnk *.map
    @rm -f ../bin/$(xname).exe
