#pmake: os_dos cpu_i86 i86 intel
#
# WATCOM Debugger makefile for NEC
#

host_OS=dos
host_CPU=i86
machine = nec
EXTRA_CFLAGS = -wo-of -d_OVERLAYED_
ui_library = $(uidir)/dos.nec/ui.lib

!ifeq d 1
EXTRA_CFLAGS += -dDEBUG_NEC
!endif

sysobjs =   &
    fork.obj &
    dosstl.obj &
    doschk.obj &
    doscmd.obj &
    dosfilio.obj &
    doslkup.obj &
    dosmem.obj &
    dosgrow.obj &
    dosalloc.obj &
    dostrap.obj &
    necscrn.obj

!include ../../master.mif

syslnk : .PROCEDURE
    @%append $(lnk) sys dos op dyn,area=0k,stack=3k
