.EXTENSIONS:
.EXTENSIONS: .pp .exe .lib .obj .res .asm .c .h .msg

!ifndef host_CPU
!ifeqi model f
host_CPU = 386
!else
host_CPU = i86
!endif
!endif

!ifndef host_OS
host_OS=$(os)
!endif

compiler_i86=$(comp286)
compiler_386=$(comp286)
compiler_axp=wccaxp

!ifndef compiler
compiler=compiler_$(host_CPU)
!endif
source_dir = $(guidir)/c
os_source_dir =  $(guidir)/$(type)/c;$(sys_dep_dir)
gui_msg_file = ../../h/_guimsgs.h

inc_dirs_lang_qnx = $(lang_root)/qh

!ifdef inc_dirs_lang_$(host_OS)
inc_dirs_lang   = $(inc_dirs_lang_$(host_OS))
!else
inc_dirs_lang   = $(lang_root)/h
!endif

.before
    set $(build_target)_include=$(sysinclude);$(sys_dep_dir);$(watcom_dir)/h;$(guidir)/h;$(guidir)/$(type)/h;$(trmem_dir);$(help_dir)/h;$(inc_dirs_lang);$(plat_inc)

nodebugflags = $(xflags)-zq-w3 -bt=$(build_target)
!ifndef axp
nodebugflags += $(deffp)
!endif
!ifdef model
nodebugflags += -m$model
!endif

!ifdef on_build_machine
cflags = $(nodebugflags) -oxt-d1
track =
!else
!ifdef nodebug
cflags = $(nodebugflags) -oxt-d1
!else
!ifdef codeview
cflags += $(nodebugflags) -d1 -hc
!else
cflags = $(nodebugflags) -d2    # need a space: -bt=foo-d2 eats the -d2. Ugh!
!endif
!endif
!endif
!ifdef __LINUX__
cflags += -fo=.obj
!endif

!ifdef wpi_lib
wpi_library=$(wpi_dir)/lib/$(wpi_lib).lib
!endif

librarian_i86=wlib
librarian_386=wlib
librarian_axp=wlib

!ifeq host_CPU axp
cflags += -ai -aq
!endif

compile  = $($(compiler)) $(cflags) $[*

!ifndef track
track=/dTRMEM
!ifndef axp
track += -of+
!endif
!endif

wv_objs = &
       guicreat.obj   &
       guihint.obj    &
       guihook.obj    &
       guimdi.obj     &
       guimdime.obj   &
       guiimdi.obj    &
       guiimmdi.obj   &
       guihflt.obj    &
       guihot.obj     &
       guidraw.obj    &
       guiutil.obj    &
       guipixel.obj   &
       guipick.obj    &
       guiscale.obj   &
       guirscal.obj   &
       guigcolr.obj   &
       guimkey.obj    &
       guizlist.obj   &
       guiextra.obj   &
       guimin.obj     &
       guisetup.obj   &
       guidlg.obj     &
       guindlg.obj    &
       guistyle.obj   &
       guiwnclr.obj   &
       guistr.obj     &
       guiisgui.obj   &
       guideath.obj   &
       guidead.obj    &
       guihtool.obj   &
       guiclrst.obj   &
       guiev.obj      &
       guisdef.obj    &
       guiextnm.obj   &
       guifcrck.obj   &


mem_objs = &
       guimem.obj     &

!ifndef on_build_machine
mem_objs += &
       trmemcvr.obj   &
       trmem.obj
!endif

objs = $(wv_objs) $(mem_objs)

tail_i86 = $(model)
tail_386 = $(model)
tail_axp = axp

../gui$(host_OS)$(tail_$(host_CPU)).lib : eraseall.obj $(gui_msg_file) $(objs) $(extra_objs) $(sysobjs) $(__makefiles__) $(wpi_library) $(ctl3d_lib)
        @%create tmp.lbc
        @for %i in ($(objs)) do @%append tmp.lbc +%i
        @for %i in ($(sysobjs)) do @%append tmp.lbc +%i
!ifdef wpi_lib
        @%append tmp.lbc +$(wpi_dir)/lib/$(wpi_lib).lib
        @%append tmp.lbc +wpimem
!endif
!ifdef ctl3d_lib
        @%append tmp.lbc +$(ctl3d_lib)
!endif
        $(librarian_$(host_CPU)) $(wlib_opt) -n -b -q $^@ @tmp.lbc
        @rm -f tmp.lbc

eraseall.obj : $(guidir)/$(type)/compile.gbl
        if exist *.obj erase *.obj
        wtouch eraseall.obj


.CONTINUE
.ERASE


.c:     $(source_dir);$(os_source_dir);$(os_extra_dir);$(trmem_dir);$(help_dir)/c
.h:     $(guidir)/h;$(guidir)/$(type)/h;$(wpi_dir)/h
.msg:   $(guidir)/h

.c.obj:
!ifdef on_build_machine
        $(compile)
!else
        $(compile) -fh
!endif

.c.pp:
        $(compile) -plc -fo=$^&.pp

guisetup.obj    : guisetup.c guiwind.h
guimin.obj      : guimin.c guimin.h
guistr.obj      : guistr.c guistr.h gui.msg


!ifeq model f
wasmopt=-3
!endif
!ifeq model s
wasmopt=-3
!endif

guimem.obj :    $(guidir)/c/guimem.c
           $($(compiler)) $[@ $(cflags) $(track)

trmemcvr.obj :  $(trmem_dir)/trmemcvr.c
           $($(compiler)) $[@ $(cflags) $(track)

trmem.obj :     $(trmem_dir)/trmem.c
           $($(compiler)) $[@ $(cflags) $(track)

./mkstr.exe: gui.msg $(guidir)/c/mkstr.c
        $(bld_cl) -i"$(guidir)/h" -fe=$^@ $]@ -lr

$(gui_msg_file): gui.msg ./mkstr.exe
        $]@ $(gui_msg_file) $(gui_msg_id_modifier)

clean : .SYMBOLIC
        @rm -f *.?bj *.res *.err *.sym *.map *.lnk *.pp

