#pmake: all build os_os2 lcl 32bit support x86
#
# Builds WDPMHELP.EXE, WDPMHOOK.DLL
#

host_os  = os2
host_cpu = 386
!include cproj.mif

.EXTENSIONS
.EXTENSIONS : .dll .lbj .obj .c .asm .res .rc

!ifndef bin_root
!ifdef bin_root_wv
bin_root = $(bin_root_wv)
!endif
!endif

bdir = $(bin_root)\binp

w d : $(bdir)\dll\wdpmhook.dll $(bdir)\wdpmhelp.exe .SYMBOLIC
        @%null

tmplnk = tmp.lnk

$(bdir)\dll\wdpmhook.dll: pmhook.obj
    @%write  $(tmplnk) name $(bdir)\dll\wdpmhook.dll sys os2 dll
    @%append $(tmplnk) debug all op symf
    @%append $(tmplnk) option quiet,map
    @%append $(tmplnk) file pmhook
    @%append $(tmplnk) library $(watcom_dir)\lib\os2.lib
    @%append $(tmplnk) export SendMsgHookProc.1,SetHmqDebugee.2
    @%append $(tmplnk) segment class 'DATA' shared
    @%append $(tmplnk) segment class 'BSS' shared
    $(linker) @$(tmplnk)

$(bdir)\wdpmhelp.exe: pmhelp.lbj pmhelp.res
    @%write  $(tmplnk) name $(bdir)\wdpmhelp.exe system os2 pm
    @%append $(tmplnk) debug all op symf
    @%append $(tmplnk) option map
    @%append $(tmplnk) file pmhelp.lbj
    @%append $(tmplnk) library clibl.lib
    @%append $(tmplnk) library $(watcom_dir)\lib\os2.lib
    @%append $(tmplnk) option nodefaultlibs,quiet
    @%append $(tmplnk) option modname=wdpmhelp
    @%append $(tmplnk) option description 'Open Watcom Debugger PM Helper'
    @%append $(tmplnk) option stack=8k
    @%append $(tmplnk) option heapsize=1024
    @%append $(tmplnk) option protmode
    $(linker) @$(tmplnk)
    $(rc_os2) pmhelp.res $(bdir)\wdpmhelp.exe

.c: ..\c

.c.obj:
        $(comp286) $[* -bt=os2 -zq-fpc-d1-oais-s-w3-mc-zu-i$(watcom_dir)\h;$(lang_root)\h\os21x;$(watcom_dir)\os220_h;$(trap_dir)\h

.c.lbj
        $(comp286) $[* -bt=os2 -zq-fpc-d1-ml-zu-w3-s-i$(lang_root)\h\os21x;$(trap_dir)\h -fo=.lbj

pmlock.obj : pmlock.c ..\h\wdpmhelp.h

pmhelp.res: pmhelp.rc ..\h\wdpmhelp.h
        @set oinc=$(%include)
        @set include=$(lang_root)\h\os21x;..\h;$(%include)
        $(rc_os2) -r pmhelp.rc
        @set include=$(%oinc)
