#pmake: all serv remote os_qnx cpu_i86 cpu_386 tcp x86
host_CPU = 386

os = qnx
srv = tcp
cpu = x86

extra_objs = $(remote_objs)
extra_libs = $(watcom_dir)/lib/qnx/socket3r.lib
extra_incs = $(watcom_dir)\qnx_h

!include $(trap_dir)/serv.mif
