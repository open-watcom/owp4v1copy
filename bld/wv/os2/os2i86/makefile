#pmake: os_os2 cpu_i86
#
# Open Watcom Debugger makefile for OS/2
#

host_OS  = os2
host_CPU = i86
display  = char

syshdir  = $(mdir)\$(sysdir)\h;$(inc_dirs_sys_os2)
extra_cflags = /zc

#add '16' suffix to avoid conflicts
tail_char_os2 = 16

sysobjs =            &
        os2init.obj  &
        os2lkup.obj  &
        os2chk.obj   &
        os2cmd.obj   &
        os2filio.obj &
        os2scrn.obj

!include ../../makefile

syslnk : .PROCEDURE
    @%append $(lnk) system os2 pmc
    @%append $(lnk) option newfile
    @%append $(lnk) option stack=16k
