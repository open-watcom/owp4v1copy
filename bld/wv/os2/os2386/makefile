#pmake: build os_os2 cpu_386
#
# Open Watcom Debugger makefile for OS/2
#

host_OS  = os2
host_CPU = 386
display  = char

syshdir = $(mdir)/$(sysdir)/h;$(inc_dirs_sys_os2)

#trap loader is different for 32-bit OS/2
dig_os_os2_386 = os2v2

sysobjs =            &
        os22ini.obj  &
        os2chk.obj   &
        os2cmd.obj   &
        os22flio.obj &
        os2scrn.obj

!include ../../master.mif

syslnk : .PROCEDURE
    @%append $(lnk) system os2v2
    @%append $(lnk) option verbose
