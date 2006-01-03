#pmake: lcl core os_linux cpu_386 trap x86
#
# Create the Linux core.trp
#
host_cpu = 386
host_os  = linux

os      = linux
srv     = lcl
cpu     = x86
name    = core
which   = TRAP

extra_objs = &
        linuxcore.obj &
        linuxcomm.obj &
        linuxfile.obj &
        supp.obj &
        core.obj
	
!include $(trap_dir)/lcl/lcl.mif
!include $(trap_dir)/master.mif
