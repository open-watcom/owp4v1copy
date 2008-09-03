#pmake: build serv lcl os_dos dsx cpu_386 x86

host_os  = dos
host_cpu = 386

os  = cw
srv = cw
cpu = x86
name = cwhelp
which = SERVER

extra_c_flags = -DCAUSEWAY -DDOSXHELP
extra_a_flags_cwhelp = -fpi87

inc_dirs = -I"$(cw_dir)/inc"

!define substitute_linker

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif

objs =  cwhelp.obj c2asm.obj

$(target) : $(objs) $(__MAKEFILES__)
    $(linker) system causeway name $^@ op map, stub=$(contrib_dir)/extender/causeway/bin/cwdstub.exe f {$(objs)} l ../lib/system
