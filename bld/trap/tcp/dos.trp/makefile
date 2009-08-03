#pmake: trap remote os_dos cpu_i86 cpu_386 tcp x86
host_CPU = i86
host_OS = dos

memory_model_i86 = l

# NOTE: This is not working at present...

os = dos
srv = tcp
cpu = x86
which = TRAP
type  = remote_link

inc_dirs = -I"$(%OWROOT)/contrib/wattcp/inc"
extra_libs = $(%OWROOT)/contrib/wattcp/lib/wattcpwl.lib

!include $(trap_dir)/master.mif
