#pmake: all build lcl os_linux cpu_386 trap x86
#
# Create the Linux STD.TRP
#
host_CPU = 386
host_OS = linux

os      = linux
srv     = lcl
cpu     = x86
which = TRAP

extra_objs = &
        linuxacc.obj &
        linuxcomm.obj &
        linuxfile.obj &
        squish87.obj &
        supp.obj &
        core.obj &
        x86cpu.obj

inc_dirs = $(trap_dir)/lcl/h

!include $(trap_dir)/master.mif

