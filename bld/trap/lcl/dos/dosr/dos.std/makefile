#pmake: all build lcl os_dos cpu_i86 trap x86
host_cpu = i86

os      = dos
srv     = lcl
cpu     = x86

std_objs = &
        $(dos_objs) &
        squish87.obj &
        dosovl.obj &
        dosacc.obj &
        dbgtrap.obj &
        npxtype.obj &
        x86cpu.obj

extra_objs  = $(std_objs) realtrap.obj
extra_dobjs = $(std_objs) dumtrap.obj
extra_incs = $(trap_dir)/lcl/dos/h

extra_aflags_npxtype = -fpi87

!include $(trap_dir)/lcl/dos/dos.mif
!include $(trap_dir)/trap.mif

realtrap.obj : ../asm/trap.asm

dumtrap.obj : ../asm/trap.asm
