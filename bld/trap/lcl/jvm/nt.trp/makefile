#pmake: trap lcl os_dos dsx cpu_386 x86
host_CPU = 386

os = nt
cpu = x86
srv = jvm

extra_objs = $(jvmx_trap_objs) jvminfo.obj acclcl.obj
extra_cflags = -dJVM -dJVMXTRAP -fh

!include $(trap_dir)/lcl/jvm/jvm.mif
!include $(trap_dir)/trap.mif
