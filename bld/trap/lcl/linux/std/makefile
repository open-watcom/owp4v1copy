#pmake: all no_build lcl os_linux cpu_386 trap x86
#
# Create the Linux STD.TRP
#

os      = linux
srv     = lcl
cpu     = x86

extra_objs = &
        linuxacc.obj &
        linuxcomm.obj &
        linuxfile.obj &
        squish87.obj &
        supp.obj &
        core.obj

extra_incs = $(trap_dir)\lcl\h

!include $(trap_dir)\trap.mif

