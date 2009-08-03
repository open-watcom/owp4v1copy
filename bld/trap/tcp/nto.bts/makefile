#pmake: serv remote os_nto cpu_x86 tcp x86 monolithic

bootstrap = 1

host_os  = nto
host_cpu = 386

os    = nto
srv   = tcp
cpu   = x86
which = SERVER
type  = remote_link

builtin_trap = 1

extra_srcs = $(trap_dir)/lcl/nto/c;$(trap_dir)/lcl/c;$(clib_dir)/environ/c
inc_dirs   = -I"$(trap_dir)/lcl/nto/h" -I"$(trap_dir)/lcl/h" -I"$(lib_misc_dir)/h"

extra_objs = gtcmdunx.obj &
             x86cpu.obj &
             ntostrt.obj &
             ntoacc.obj &
			 ntomap.obj &
			 ntoutl.obj &
             ntocomm.obj &
             ntofile.obj &
             supp.obj &
             core.obj

!define substitute_linker

!include $(trap_dir)/master.mif

$(target) : $(libs) $(objs) $(__MAKEFILES__)
    $(cl) $(clflags) $(objs) -lsocket #$(cl_libs)
