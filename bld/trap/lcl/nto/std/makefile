#pmake: lcl os_nto cpu_386 trap x86
#
# Create the QNX Neutrino std.trp
#
host_os  = nto
host_cpu = 386

os    = nto
srv   = lcl
cpu   = x86
which = TRAP

extra_objs = &
        ntoacc.obj &
	ntomap.obj &
        ntocomm.obj &
        ntofile.obj &
        misc7386.obj &
        supp.obj &
        core.obj

inc_dirs = -I"$(trap_dir)/lcl/h"

!include $(trap_dir)/master.mif
