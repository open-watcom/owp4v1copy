#pmake: all build os_nec cpu_i86
host_OS = dos
host_CPU = i86

!include cproj.mif

.erase

objects = sample.obj sampdata.obj realmode.obj rmhooks.obj sampdos.obj &
          io.obj sysio.obj timasm.obj interc.obj sampexe.obj settime.obj &
          wmsg.obj #sysinit.obj

cflags = -zq-ms-d1-oaxt-w3-dREPORT_TYPE=__interrupt -dFAR_PTR=__far -fi=watcom.h -i"$(dig_dir)/h"
cflags2 = -zu-d_NEC_PC $(cflags)

extra_l_flags = op map

xname       = wsampnec

../bin/$(xname).exe : $(objects) makefile $(xname).res
    @%create tmp.lnk
#    @%write tmp.lnk $(%debug) option map, quiet system dos
    @%append tmp.lnk libpath $(wres_dir)/lib
    @%append tmp.lnk lib wress
    @%append tmp.lnk name $^@
    @for %i in ($objects) do @%append tmp.lnk file %i

#!include ../setsize.mif
    $(linker) $(lflags) @tmp.lnk
    wstrip -q -a -r $^@ . $(xname).res

#!include ../objsize.mif

.c: ../dos;../c
.c.obj :
#    $(comp286) $[* $cflags2
    $(cc) $[* $cflags2

.asm: ../dos
.asm.obj :
#    $(asm286) $[* -ZD;
    $(as) $[*

$(xname).res : ../h/wsample.rc ../h/wmsg.h
    $(rc_aui) -zk0 -dDOS -dNEC -fo=$^. $[@

wmsg.obj: ../c/wmsg.c ../h/wmsg.h
#    $(comp286) $[* $cflags
    $(cc) $[* $cflags

clean : .symbolic
    @rm -f *.obj *.res *.lnk *.map
    @rm -f ../bin/$(xname).exe

