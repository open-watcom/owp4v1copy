#pmake: os_os2 cpu_i86
#
# Open Watcom Debugger makefile for OS/2 PM
#

# trmem = 1
host_OS  = os2
host_CPU = i86
display  = gui
syshdir  = $(mdir)/$(sysdir)/h;$(inc_dirs_sys_os2)
extra_cflags = -zc -zu

#add '16' suffix to avoid conflicts
tail_gui_os2 = w16

sysobjs =            &
        os2init.obj  &
        os2lkup.obj  &
        os2chk.obj   &
        os2cmd.obj   &
        os2filio.obj &
        pmscrn.obj

!include ../../master.mif

syslnk : .PROCEDURE
    @%append $(lnk) system os2 pm
    @%append $(lnk) option newfile
    @%append $(lnk) option stack=20k
    @%append $(lnk) option map
