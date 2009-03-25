#pmake: build serv lcl os_dos dsx cpu_386 x86

host_os  = dos
host_cpu = 386

os  = cw
srv = cw
cpu = x86
name = cwhelp
which = SERVER

extra_c_flags = -DCAUSEWAY -ms

extra_a_flags = -DCAUSEWAY -ms
extra_a_flags_cwhelp = -fpi87

inc_dirs = -I"$(cw_dir)/inc"

extra_libs = ../lib/system.lib

extra_objs = &
    cwhelp.obj &
    cwchelp.obj &
    cwacc.obj &
    misc7386.obj &
    x86cpu.obj

!include $(trap_dir)/lcl/dos/dosx/dosx.mif
!include $(trap_dir)/master.mif

