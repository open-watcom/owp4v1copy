#pmake: all build trap lcl os_dos dsx cpu_386 x86
host_CPU = i86
host_OS = dos

os = dos
srv = rsi
cpu = x86
which = TRAP

extra_c_flags = -dDOS4G -dDOSXTRAP

extra_objs = $(dosx_trap_objs)

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif
