#pmake: build remote support nmp os_os2 cpu_i86 cpu_386

host_OS  = os2
host_CPU = i86

.ERASE
.CONTINUE

!include cproj.mif
!include deftarg.mif
!include defrule.mif

!ifndef bin_root
!ifdef bin_root_wv
bin_root = $(bin_root_wv)
!endif
!endif

objs = nmpbind.obj nmpos2.obj

extra_c_flags = -bm -zu
extra_l_flags = op stack=8k

!ifdef on_build_machine
default_targ    = w
!else
default_targ    = d
!endif

first : $(default_targ) .SYMBOLIC
        @%null

w : $(bin_root)/binp/nmpbind.exe .SYMBOLIC
        @%null

d : $(bin_root)/binp/dnmpbind.exe .SYMBOLIC
        @%null

$(bin_root)/binp/nmpbind.exe : _err.gh _jerr.gh $(objs)
    $(linker) $(lflags) file {$objs} name $^@ 

$(bin_root)/binp/dnmpbind.exe : _err.gh _jerr.gh $(objs)
    $(linker) $(lflags) file {$objs} name $^@

.c: ../c

inc_dirs = $(trap_dir)/h

_err.gh : $(trap_dir)/c/mkstr.c $(trap_dir)/h/trap.msg
        $(bld_cl) $[@ -i"$(trap_dir)/h"
        mkstr $^@
        @rm -f mkstr.*

_jerr.gh : $(trap_dir)/c/mkstr.c $(trap_dir)/h/trap.msg
        $(bld_cl) $[@ -dJAPANESE_MESSAGES -i"$(trap_dir)/h"
        mkstr $^@
        @rm -f mkstr.*
