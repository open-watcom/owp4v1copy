#pmake: all build os_linux cpu_386 386 intel
#
# Open Watcom Debugger makefile for Linux
#

host_OS  = linux
host_CPU = 386

display = char

sysobjs = linuxcmd.obj linuxio.obj lnxinit.obj lnxscrn.obj lnxlkup.obj lnxchk.obj peloader.obj

!include ../../master.mif

syslnk : .PROCEDURE
!ifdef __LINUX__
    @%append $(lnk) sys bld_linux
!else
    @%append $(lnk) sys linux
!endif
