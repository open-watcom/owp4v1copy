#pmake: build lcl os_linux cpu_386 trap x86
#
# Create the Linux STD.TRP
#
host_cpu = 386
host_os  = linux

os      = linux
srv     = lcl
cpu     = x86
which   = TRAP

extra_objs = &
        linuxacc.obj &
        linuxx86.obj &
        linuxmap.obj &
        linuxcomm.obj &
        linuxfile.obj &
        linuxutl.obj &
        supp.obj &
        core.obj &
        x86cpu.obj

!include $(trap_dir)/lcl/lcl.mif
!include $(trap_dir)/master.mif
