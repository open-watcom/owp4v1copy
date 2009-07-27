#pmake: serv remote os_linux cpu_mips tcp mips monolithic

bootstrap = 1

host_os  = linux
host_cpu = mips

os    = linux
srv   = tcp
cpu   = mips
which = SERVER
type  = remote_link

extra_srcs = $(trap_dir)/lcl/linux/c;$(trap_dir)/lcl/c
inc_dirs   = -I"$(trap_dir)/lcl/linux/h"

extra_cflags = -DBUILTIN_TRAP_FILE

extra_objs = peloader.obj &
             linuxstrt.obj &
             linuxacc.obj &
             linuxmap.obj &
             linuxcomm.obj &
             linux$(cpu).obj &
             linuxfile.obj &
             linuxutl.obj &
             supp.obj &
             core.obj

!define substitute_linker

!include $(trap_dir)/master.mif

$(target) : $(libs) $(objs) $(__MAKEFILES__)
    $(cl) $(clflags) $(objs) $(cl_libs)
