#pmake: all build serv remote os_dos cpu_i86 cpu_386 tcp x86
host_CPU = i86
host_OS = dos

memory_model_i86 = l

os = dos
srv = tcp
cpu = x86
which = SERVER

extra_objs = $(remote_objs)
inc_dirs = $(%owroot)/contrib/wattcp/inc
extra_libs = $(%owroot)/contrib/wattcp/lib/wattcpwl.lib

!include $(trap_dir)/master.mif

