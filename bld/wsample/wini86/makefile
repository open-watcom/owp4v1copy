#pmake: build os_win cpu_i86
host_OS = win
host_CPU = i86

proj_name = wsample

!ifndef wsample_autodepends
wsample_autodepends = .AUTODEPEND
!endif

memory_model_i86 = c

!include cproj.mif
#!include deftarg.mif
!include defrule.mif

#.EXTENSIONS:
#.EXTENSIONS: .exe
#.EXTENSIONS: .obj
#.EXTENSIONS: .asm .c .h

!ifdef __LINUX__
description = \'Open Watcom Sampler\'
!else
description = 'Open Watcom Sampler'
!endif

objects = winmain.obj sample.obj sampdata.obj sampwin.obj io.obj &
          wmsg.obj sysio.obj sampexe.obj winopts.obj &
          fault.obj notify.obj libload.obj &
          globals.obj getidata.obj &
          segmem2.obj segmem.obj mythelp.obj inth.obj intdata.obj &
          dbg386.obj ismod32.obj

LNK = tmp.lnk

#.before
#        @set wcc=-i"$(sdkdir)/misc;$(dig_dir)/h" -dFAR_PTR=__far -dREPORT_TYPE= -fi=watcom.h

inc_dirs = $(sdkdir)/misc;$(dig_dir)/h

extra_c_flags = -dFAR_PTR=__far -dREPORT_TYPE= -fi=watcom.h -2 -zu -zw -orlt -s

xname=wsamplew

../bin/$(xname).exe : $(objects) $(xname).res makefile
    @%write $(lnk) option map,quiet
!ifndef on_build_machine
    @%append $(lnk) debug all op symfile
!endif
    @for %i in ($objects) do @%append $(lnk) file %i
#    @%append $(lnk) sys windows
    @%append $(lnk) name $^@
    @%append $(lnk) option heap=10k, stack=10k
    @%append $(lnk) option description $(description)
    @%append $(lnk) segment class 'CODE' preload fixed
    @%append $(lnk) segment class 'DATA' preload fixed
    @%append $(lnk) lib commdlg,toolhelp
    $(linker) $(lflags) @$(lnk)
    $(rc) $(rc_flags) $(xname).res $^@

$(xname).res : $(xname).ico $(xname).rc wsampres.rc ../h/wsample.rc ../h/wmsg.h
    $(rc) $(rc_flags) -r -zk0 -dWIN -30 -i"$(lang_root)/h/win" $(xname).rc

.c: .;../c;$(sdkdir)/misc
.asm: $(sdkdir)/misc

.obj : obj

#.c.obj :
#    $(comp286) $(cflags) $[@ -fo=.obj
#    $(cc) $(cflags) $[@ -fo=.obj

.asm.obj :
#    wasm -q -fo=$[&.obj $[*
    $(as) -fo=$[&.obj $[*

clean : .symbolic
    @rm -f *.obj *.res *.lnk *.map *.err
    @rm -f ../bin/$(xname).exe
