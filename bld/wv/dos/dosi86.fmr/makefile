#pmake: os_dos cpu_i86 i86 intel
#
# WATCOM Debugger makefile for DOS on the FMR PC
#

host_OS=dos
host_CPU=i86
display=char
machine=fmr
EXTRA_CFLAGS = -wo-of-d_OVERLAYED_

sysobjs =   &
    fork.obj &
    dosstl.obj &
    doschk.obj &
    doscmd.obj &
    dosfilio.obj &
    doslkup.obj &
    dosmem.obj &
    $(arch)scrn.obj &
    dosgrow.obj &
    dosalloc.obj &
    dostrap.obj

!include ../../master.mif

syslnk : .PROCEDURE
    @%append $(lnk) sys dos op dyn,area=0k,stack=3k
