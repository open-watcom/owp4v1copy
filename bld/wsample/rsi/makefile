#pmake: all build os_dos cpu_386
host_CPU = i86

!include cproj.mif

.NOCHECK

.EXTENSIONS
.EXTENSIONS : .exe .obj .nbj .c .asm

.BEFORE
        set include=$(rsilib);$(%include)

model=l

xname       = wsamprsi

nxname      = nsamprsi

lf = tmp.lnk

cflags = -zq-m$(model)-zdp-d1-osail-s-w3-dREPORT_TYPE= -dFAR_PTR=__far -fi=watcom.h -i"$(plsdir);$(dig_dir)/h"

first = dos16m

objs  = $(lang_root)/lib286/dos/$(first).obj segs_16m.obj dbglib.obj &
        dbgliba.obj rmhdlr.obj sbrk.obj sample.obj samprsi.obj sampdata.obj &
        io.obj sysio.obj sampexe.obj settime.obj magicstr.obj wmsg.obj sysinit.obj

nbjs  = $(lang_root)/lib286/dos/$(first).nbj segs_16m.nbj intrm.nbj &
        dbgliba.nbj rmhdlr.nbj sbrk.nbj sample.nbj samprsi.nbj sampdata.nbj &
        io.nbj sysio.nbj sampexe.nbj settime.nbj magicstr.nbj wmsg.nbj sysinit.nbj


all: at nec .symbolic
    @%null

at: ../bin/$(xname).exe .symbolic
    @%null

nec: ../bin/$(nxname).exe .symbolic
    @%null

../bin/$(xname).exe : $(xname).exp $(xname).res
    $(rsidir)splice $^@ $(xname).exp -ALT $(rsidir)wstubq.exe
    wstrip -q -a -r $^@ . $(xname).res

$(xname).exp : $(objs) $(__MAKEFILES__)
    @%create $(lf)
    @%append $(lf) exp(wsamprsi) map(wsamprsi) log(wsamprsi.log)
    @%append $(lf) -nod -DPMI -map -acg -farcall -quiet -auto -rtnerr 2
    @for %i in ($(objs)) do @%append $(lf) %i
    @%append $(lf) $(rsilib)/d16libc.lib $(lang_root)/lib286/dos/clib$(model).lib
    @%append $(lf) $(wres_dir)/lib/wres$(model)p.lib
    $(rsidir)glu @$(lf)
    @rm -f *.err

../bin/$(nxname).exe : $(nxname).exp $(nxname).res
    $(rsidir)splice $^@ $(nxname).exp -ALT $(rsidir)wstubq.exe
    wstrip -q -a -r $^@ . $(nxname).res

nsamprsi.exp : $(nobjs) $(__MAKEFILES__)
    @%create $(lf)
    @%append $(lf) exp(nsamprsi) map(nsamprsi) log(nsamprsi.log)
    @%append $(lf) -nod -DPMI -map -acg -farcall -auto -rtnerr 2
    @for %i in ($(objs)) do @%append $(lf) %i
    @%append $(lf) $(rsilib)/d16libc.lib $(lang_root)/lib286/dos/clib$(model).lib
    @%append $(lf) $(wres_dir)/lib/wres$(model)p.lib
    $(rsidir)glu @$(lf)
    @rm -f *.err

.c: .;../c;$(rsilib)

.c.obj :
#    $(comp286) $[* $(cflags)
    $(cc) $[* $(cflags)

.c.nbj :
#    $(comp286) $[* $(cflags)-d_NEC_PC-fo=.nbj
    $(cc) $[* $(cflags) -d_NEC_PC -fo=.nbj

.asm: .;../a;$(rsilib)

.asm.obj :
    masm -mx -s -Dlarge $[@;

.asm.nbj :
    masm -mx -s -Dlarge $[@.nbj;

$(xname).res : ../h/wsample.rc ../h/wmsg.h
    $(rc_aui) -zk0 -dRSI -fo=$^@ $[@
$(nxname).res : ../h/wsample.rc ../h/wmsg.h
    $(rc_aui) -zk0 -dRSI -dNEC -fo=$^@ $[@

clean : .symbolic
    @rm -f *.obj *.res *.lnk *.map *.exp *.log *.err
    @rm -f ../bin/$(xname).exe ../bin/$(nxname).exe

