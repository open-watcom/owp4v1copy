#pmake: os_qnx cpu_386 386 intel
#
# Open Watcom Debugger makefile for QNX
#

host_os  = qnx
host_cpu = 386

display = char
extra_cflags = -ms

sysobjs = &
        qnxinit.obj  &
        qnxlkup.obj  &
        qnxchk.obj   &
        qnxcmd.obj   &
        qnxfilio.obj &
        qnxscrn.obj

!include ..\..\makefile

syslnk : .PROCEDURE
    @%append $(lnk) sys qnx386 flat
    @%append $(lnk) opt stack = 0x8000, offset=0x9000, resource=..\wvqnx.u
