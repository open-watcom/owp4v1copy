#pmake: all build os_dos cpu_386
host_CPU = 386
host_OS = dos

.NOCHECK

!ifeq machine NEC
xname = nsamppls
!else
xname = wsamppls
!endif

assembly = masm $[@ -i$(misc_h); #masm 5
#assembly = ml /c /Zm /Cx /Cp $[@ #masm 6

!include cproj.mif
!include defrule.mif

objects = sample.obj samppls.obj sampdata.obj io.obj sysio.obj &
          timer.obj sampexe.obj settime.obj wmsg.obj 

!ifeq machine NEC
!else
objects += sysinit.obj
!endif

extra_c_flags = -dREPORT_TYPE= -d_PLS -dFAR_PTR= -fi=watcom.h

!ifeq machine NEC
extra_c_flags += -d_NEC_PC
extra_rc_flags = -dNEC
!endif

!ifeq machine NEC
inc_dirs = ../pls386;$(plslib);$(dig_dir)/h
!else
inc_dirs = $(plslib);$(dig_dir)/h
!endif

!ifeq machine NEC
.c: ../pls386;../c
.asm: ../pls386;../a
!else
.c: .;../c
.asm: .;../a
!endif

lnk = tmp.lnk

../bin/$(xname).exp : $(objects) $(xname).res $(__MAKEFILES__)
    @%write $(lnk) format phar ext
    @%append $(lnk) op map
    @%append $(lnk) name ../bin/$(xname)
    @%append $(lnk) libpath $(lang_root)/lib386/dos
    @%append $(lnk) libpath $(lang_root)/lib386
    @%append $(lnk) libpath $(plsdir)
    @%append $(lnk) libpath $(wres_dir)/lib
    @%append $(lnk) lib clib3r,wdos32,wdosx32,wresf
    @%append $(lnk) option maxdata=0
    @%append $(lnk) runtime maxreal=1023K
    @%append $(lnk) runtime maxibuf=4K
    @%append $(lnk) runtime flag=2
    @%append $(lnk) file $(plsdir)/dilseg
    @%append $(lnk) file $(plsdir)/dilglue
    @for %i in ($(objects)) do @%append $(lnk) file %i
    $(linker) $(%debug) @$(lnk)
    wstrip -q -a -r $^@ . $(xname).res

$(xname).res : ../h/wsample.rc ../h/wmsg.h
    $(rc_aui) $(extra_rc_flags) -dPLS -fo=$^@ $[@

clean : .symbolic
    rm -f *.?bj *.res *.lnk *.map *.exp *.log *.err
    rm -f ../bin/$(xname).exp

.asm.obj :
    $(assembly)
