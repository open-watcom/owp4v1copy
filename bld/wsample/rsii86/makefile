#pmake: all build os_dos cpu_386
host_CPU = i86
host_OS = dos

.NOCHECK

!ifeq machine NEC
xname = nsamprsi
!else
xname = wsamprsi
!endif

masmdir = $(tools_root)
rsidir = $(tools_root)

#until wasm gets fixed up

assemble = $(masmdir)\masm -mx -s -I$(rsilib) -Dlarge $[@; # masm 5
#assemble = ml -nologo -c -Zm -Cx -I$(rsilib) -Dlarge $[@ # masm 6

!include cproj.mif
!include defrule.mif

objs =  $(lang_root)/lib286/dos/dos16m.obj &
        dbglib.obj &
        segs_16m.obj &
        dbgliba.obj &
        rmhdlr.obj &
        sbrk.obj &
        sample.obj &
        samprsi.obj &
        sampdata.obj &
        io.obj &
        sysio.obj &
        sampexe.obj &
        settime.obj &
        magicstr.obj &
        wmsg.obj &
        sysinit.obj

extra_c_flags = -s -dREPORT_TYPE= -dFAR_PTR=__far -fi=watcom.h -zdp
!ifeq machine NEC
extra_c_flags += -d_NEC_PC
extra_rc_flags = -dNEC
!endif

extra_a_flags = -Dlarge

!ifeq machine NEC
inc_dirs = ../rsii86;$(rsilib);$(dig_dir)/h
!else
inc_dirs = $(rsilib);$(dig_dir)/h
!endif

!ifeq machine NEC
.c: ../rsii86;../c;$(rsilib)
.asm: ../rsii86;../a;$(rsilib)
!else
.c: .;../c;$(rsilib)
.asm: .;../a;$(rsilib)
!endif

lf = tmp.lnk

../bin/$(xname).exe : $(xname).exp $(xname).res
#    $(rsidir)\splice $^@ $(xname).exp -ALT $(rsidir)\wstubq.exe
    $(rsidir)\splice $^@ $(xname).exp -ALT ..\..\wstub\wstubq.exe
    wstrip -q -a -r $^@ . $(xname).res
    %null

$(xname).exp : $(objs) $(__MAKEFILES__)
    @%create $(lf)
    @%append $(lf) exp($(xname)) map($(xname)) log($(xname).err)
    @%append $(lf) -nod -DPMI -map -acg -farcall -quiet -auto -rtnerr 2
    @for %i in ($(objs)) do @%append $(lf) %i
    @%append $(lf) $(rsilib)/d16libc.lib
    @%append $(lf) $(lang_root)/lib286/dos/clibl.lib
    @%append $(lf) $(wres_dir)/lib/wreslp.lib
    -$(rsidir)\glu @$(lf)

$(xname).res : ../h/wsample.rc ../h/wmsg.h
    $(rc_aui) $(extra_rc_flags) -dRSI -fo=$^@ $[@

clean : .symbolic
    rm -f *.?bj *.res *.lnk *.map *.exp *.log *.err
    rm -f ../bin/$(xname).exe

!ifdef assemble
segs_16m.obj : segs_16m.asm
    $(assemble)

dbgliba.obj : dbgliba.asm
    $(assemble)

rmhdlr.obj : rmhdlr.asm
    $(assemble)
!endif
