#pmake: all build os_dos cpu_386
host_CPU = i86
host_OS = dos

.NOCHECK

!ifeq machine NEC
xname = nsamprsi
!else
xname = wsamprsi
!endif

assembly = masm -mx -s -Dlarge $[@; #masm 5
#assembly = ml /c /Zm /Cx /Dlarge $[@ #masm 6

!include cproj.mif
!include defrule.mif

rsidir=$(toolsdir)

objs = $(lang_root)/lib286/dos/dos16m.obj dbglib.obj &
        segs_16m.obj dbgliba.obj rmhdlr.obj sbrk.obj sample.obj &
        samprsi.obj sampdata.obj io.obj sysio.obj sampexe.obj settime.obj &
        magicstr.obj wmsg.obj sysinit.obj

extra_c_flags = -zdp -osail -s -dREPORT_TYPE= -dFAR_PTR=__far -fi=watcom.h
!ifeq machine NEC
extra_c_flags += -d_NEC_PC
extra_rc_flags = -dNEC
!endif
extra_c_flags_dbglib = -wcd=1177

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
    $(rsidir)splice $^@ $(xname).exp -ALT $(rsidir)wstubq.exe
    wstrip -q -a -r $^@ . $(xname).res

$(xname).exp : $(objs) $(__MAKEFILES__)
    @%create $(lf)
    @%append $(lf) exp($(xname)) map($(xname)) log($(xname).log)
    @%append $(lf) -nod -DPMI -map -acg -farcall -auto -rtnerr 2
    @for %i in ($(objs)) do @%append $(lf) %i
    @%append $(lf) $(rsilib)/d16libc.lib
    @%append $(lf) $(lang_root)/lib286/dos/clibl.lib
    @%append $(lf) $(wres_dir)/lib/wreslp.lib
    -$(rsidir)glu @$(lf)

$(xname).res : ../h/wsample.rc ../h/wmsg.h
    $(rc_aui) $(extra_rc_flags) -dRSI -fo=$^@ $[@

clean : .symbolic
    rm -f *.?bj *.res *.lnk *.map *.exp *.log *.err
    rm -f ../bin/$(xname).exe

.asm.obj :
    @set include=$(rsilib)
    $(assembly)
