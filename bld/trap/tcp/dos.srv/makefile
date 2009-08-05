#pmake: build serv remote os_dos cpu_i86 cpu_386 tcp x86

host_os  = dos
host_cpu = i86

memory_model_i86 = l

os  = dos
srv = tcp
cpu = x86
which = SERVER
type  = remote_link

inc_dirs   = -I"$(contrib_dir)/wattcp/inc"
extra_libs = $(contrib_dir)/wattcp/lib/wattcpwl.lib

!include $(trap_dir)/master.mif

