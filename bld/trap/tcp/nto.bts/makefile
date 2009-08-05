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

!define substitute_linker

!include $(trap_dir)/lcl/nto/trap.mif
!include $(trap_dir)/master.mif

$(target) : $(libs) $(objs) $(__MAKEFILES__)
    $(cl) $(clflags) $(objs) -lsocket #$(cl_libs)
