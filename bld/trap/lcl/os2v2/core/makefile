#pmake: lcl os_os2 cpu_386 trap x86

host_os  = os2
host_cpu = 386

#
# Create the 32-bit OS/2 core.dll
#

os      = os2v2
srv     = lcl
cpu     = x86
name    = core
which   = TRAP

extra_cflags = -DLINUXCORE

extra_srcs = $(trap_dir)/lcl/linux/c

inc_dirs = -I"$(orl_dir)/h"

extra_objs =          &
        linuxcore.obj &
	coremisc.obj  &
#        accmisc.obj   &
        x86cpu.obj    &
        supp.obj      &
        core.obj

extra_libs = $(orl_lib) $(os2386)

!include $(orl_dir)/client.mif
!include $(trap_dir)/lcl/lcl.mif
!include $(trap_dir)/master.mif
