#pmake: all build trap lcl os_dos dsx cpu_386 x86
host_cpu = i86

os = dos
srv = cw
cpu = x86

extra_objs = $(dosx_trap_objs)
extra_cflags = -dCAUSEWAY -dDOSXTRAP
extra_incs = $(trap_dir)/lcl/dos/h
extra_srcs = $(trap_dir)/lcl/dos/c;$(trap_dir)/lcl/dos/asm

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/trap.mif
