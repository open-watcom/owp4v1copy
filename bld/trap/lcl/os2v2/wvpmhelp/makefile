#pmake: build os_os2 cpu_386 lcl support

host_os  = os2
host_cpu = 386

#
# Builds WDPMHELP.EXE, WDPMHOOK.DLL
#

!include cproj.mif
!include defrule.mif
!include deftarg.mif

!ifndef bin_root
!ifdef bin_root_wv
bin_root = $(bin_root_wv)
!endif
!endif

extra_c_flags = -s -fpc

inc_dirs = $(trap_dir)/h

bdir = $(bin_root)/binp

w d : $(bdir)/dll/wdpmhook.dll $(bdir)/wdpmhelp.exe .SYMBOLIC
        @%null

tmplnk = tmp.lnk

$(bdir)/dll/wdpmhook.dll: pmhook.obj
    @%write  $(tmplnk) name $^@ sys os2v2 dll
    @%append $(tmplnk) debug all op symf
    @%append $(tmplnk) option map
    @%append $(tmplnk) file pmhook
    @%append $(tmplnk) library $(os2386)
    @%append $(tmplnk) export SendMsgHookProc.1,SetHmqDebugee.2
    $(linker) @$(tmplnk)

$(bdir)/wdpmhelp.exe: _err.gh _jerr.gh pmhelp.obj pmhelp.res
    @%write  $(tmplnk) name $^@ system os2v2 pm
    @%append $(tmplnk) debug all op symf
    @%append $(tmplnk) option map
    @%append $(tmplnk) import WinLockInput PMMERGE.5225
    @%append $(tmplnk) import WinThreadAssocQueue PMMERGE.5398
    @%append $(tmplnk) file pmhelp
    @%append $(tmplnk) library os2386.lib
    @%append $(tmplnk) option modname=wdpmhelp
    @%append $(tmplnk) option description 'Open Watcom Debugger PM Helper'
    $(linker) @$(tmplnk)
    $(rc_os2) pmhelp.res $^@

.c: ../c

pmhelp.res: ../h/pmhelp.rc ../h/wdpmhelp.h
        @set oinc=$(%include)
        @set include=$(inc_dirs_sys_os2);..\h;$(%include)
        $(rc_os2) -r ..\h\pmhelp.rc .\pmhelp.res
        @set include=$(%oinc)

_err.gh : $(trap_dir)/c/mkstr.c $(trap_dir)/h/trap.msg
        $(bld_cl) $[@ -i"$(trap_dir)\h"
        mkstr $^@
        @rm -f mkstr.*

_jerr.gh : $(trap_dir)/c/mkstr.c $(trap_dir)/h/trap.msg
        $(bld_cl) $[@ -dJAPANESE_MESSAGES -i"$(trap_dir)\h"
        mkstr $^@
        @rm -f mkstr.*
