#pmake: build os_os2 cpu_386 lcl support
#
# Builds WDPMHELP.EXE, WDPMHOOK.DLL
#

host_os  = os2
host_cpu = 386

!include cproj.mif
!include defrule.mif
!include deftarg.mif

!ifndef bin_root
!ifdef bin_root_wv
bin_root = $(bin_root_wv)
!endif
!endif

inc_dirs = $(trap_dir)\h

bdir = $(bin_root)\binp

w d : $(bdir)\dll\wdpmhook.dll $(bdir)\wdpmhelp.exe .SYMBOLIC
        @%null

tmplnk = tmp.lnk

$(bdir)\dll\wdpmhook.dll: pmhook.obj
    @%write  $(tmplnk) name $(bdir)\dll\wdpmhook.dll sys os2v2 dll
    @%append $(tmplnk) debug all op symf
    @%append $(tmplnk) option quiet,map,caseex
    @%append $(tmplnk) file pmhook
    @%append $(tmplnk) library $(os2386)
    @%append $(tmplnk) export SendMsgHookProc.1,SetHmqDebugee.2
    $(linker) @$(tmplnk)

$(bdir)\wdpmhelp.exe: pmhelp.obj pmhelp.res
    @%write  $(tmplnk) name $(bdir)\wdpmhelp.exe system os2v2 pm
    @%append $(tmplnk) debug all op symf
    @%append $(tmplnk) option map
    @%append $(tmplnk) import WinLockInput PMMERGE.5225
    @%append $(tmplnk) import WinThreadAssocQueue PMMERGE.5398
    @%append $(tmplnk) file pmhelp
    @%append $(tmplnk) library os2386.lib
    @%append $(tmplnk) option quiet
    @%append $(tmplnk) option modname=wdpmhelp
    @%append $(tmplnk) option description 'Open Watcom Debugger PM Helper'
    $(linker) @$(tmplnk)
    $(rc_os2) pmhelp.res $(bdir)\wdpmhelp.exe

.c: ..\c

#.c.obj:
#        $(comp286) $[* -bt=os2 -zq-fpc-d1-oais-s-w3-mc-zu-i$(watcom_dir)\h;$(lang_root)\h\os21x;$(watcom_dir)\os220_h;$(trap_dir)\h

#.c.lbj
#        $(comp286) $[* -bt=os2 -zq-fpc-d1-ml-zu-w3-s-i$(lang_root)\h\os21x;$(trap_dir)\h -fo=.lbj

pmlock.obj : pmlock.c ..\h\wdpmhelp.h

pmhelp.res: ..\h\pmhelp.rc ..\h\wdpmhelp.h
        @set oinc=$(%include)
        @set include=$(inc_dirs_sys_os2);..\h;$(%include)
        $(rc_os2) -r ..\h\pmhelp.rc .\pmhelp.res
        @set include=$(%oinc)
