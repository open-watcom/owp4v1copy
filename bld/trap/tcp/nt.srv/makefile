#pmake: all build serv remote os_nt cpu_i86 cpu_386 tcp x86
host_CPU = 386
host_OS = nt

gui = 1
os = nt
srv = tcp
cpu = x86
which = SERVER

extra_objs = $(remote_objs)
extra_libs = $(lang_root)/lib386/nt/wsock32.lib
# extra_libs = $(watcom_dir)/lib/tcpipdll.lib
# extra_incs = $(watcom_dir)/os220_h/tcpip

!include $(trap_dir)/master.mif
