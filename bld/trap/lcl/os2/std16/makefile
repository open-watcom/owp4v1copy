#pmake: lcl os_os2 cpu_i86 trap
#
# Create the OS/2 STD16.DLL
#
host_CPU = i86

os      = os2
srv     = lcl
cpu     = x86
name    = std16

extra_objs = &
        accmisc.obj &
        os2v1acc.obj &
        os2rfx.obj &
        loadstd.obj &
        npxtype.obj &
        squish87.obj &
        x86cpu.obj &
        supp.obj &
        core.obj

!include $(trap_dir)/trap.mif
