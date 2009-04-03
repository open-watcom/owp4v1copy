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
        lnxacc.obj &
        lnxx86.obj &
        lnxmap.obj &
        lnxcomm.obj &
        lnxfile.obj &
        lnxutl.obj &
        supp.obj &
        core.obj &
        x86cpu.obj

!include $(trap_dir)/lcl/lcl.mif
!include $(trap_dir)/master.mif
