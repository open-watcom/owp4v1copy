#pmake: os_nt cpu_386
#
# WATCOM Debugger makefile for NT (real GUI version)
#

host_OS=nt
host_cpu=386
display=gui

sys_windowed = 1
dll = 1
extra_c_flags= -br

all1 : $(exe_name) wd.lib .SYMBOLIC
        @%null

sysobjs = ntstub.obj ntfilio.obj winlkup.obj winchk.obj ntcmd.obj ntfinfo.obj stubdb.obj

!include ../../master.mif

extra_l_flags_nt = lib wd.lib
