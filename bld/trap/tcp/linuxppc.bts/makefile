#pmake: serv remote os_linux cpu_ppc tcp ppc monolithic

bootstrap = 1

host_os  = linux
host_cpu = ppc

os    = linux
srv   = tcp
cpu   = ppc
which = SERVER
type  = remote_link

extra_srcs = $(trap_dir)/lcl/linux/c;$(trap_dir)/lcl/c
inc_dirs   = -I"$(trap_dir)/lcl/linux/h"

extra_cflags = -DBUILTIN_TRAP_FILE

extra_objs = &
    lnxstrt.obj &
    lnxacc.obj &
    lnxmap.obj &
    lnxcomm.obj &
    lnx$(cpu).obj &
    lnxfile.obj &
    lnxutl.obj &
    supp.obj &
    core.obj

!define substitute_linker

!include $(trap_dir)/master.mif

$(target) : $(libs) $(objs) $(__MAKEFILES__)
    $(cl) $(clflags) $(objs) $(cl_libs)
