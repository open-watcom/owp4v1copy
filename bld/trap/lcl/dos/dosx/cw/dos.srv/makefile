#pmake: build serv lcl os_dos dsx cpu_386 x86
host_cpu = 386

os  = cw
srv = cw
cpu = x86
name = cwhelp

extra_cflags = -dDOS4G -dDOSXHELP
extra_incs = $(trap_dir)/lcl/dos/h;../h
extra_srcs = $(trap_dir)/lcl/dos/c
extra_aflags_cwhelp = -i"../h"

!define substitute_linker

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/serv.mif

objs =  cwhelp.obj &
        c2asm.obj

$(target) $(dtarget) : eraseall.obj $(objs) $(__MAKEFILES__)
    $(linker) system causeway name $^@ op map, stub=cwdstub.exe f {$(objs)} l ../lib/system
