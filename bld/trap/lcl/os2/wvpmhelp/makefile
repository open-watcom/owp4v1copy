#pmake: os_os2 lcl cpu_386 support
#
# Create WDPMHELP.EXE, WDPMHOOK.DLL
#

host_os  = os2
host_cpu = i86

!include cproj.mif
!include deftarg.mif

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

$(bdir)\dll\wdpmhook.dll: pmhook.obj loadstd.obj
    @%write  $(tmplnk) name $(bdir)\dll\wdpmhook.dll sys os2 dll initglobal
    @%append $(tmplnk) debug all op symfile
    @%append $(tmplnk) option dosseg,quiet
    @%append $(tmplnk) option map
    @%append $(tmplnk) file loadstd,pmhook
    @%append $(tmplnk) library $(watcom_dir)\lib\os2.lib
    @%append $(tmplnk) export SendMsgHookProc.1,SetHmqDebugee.2
    @%append $(tmplnk) segment class 'DATA' shared
    @%append $(tmplnk) segment class 'BSS' shared
    $(linker) @$(tmplnk)

$(bdir)\wdpmhelp.exe: pmhelp.lbj pmhelp.res
    @%write  $(tmplnk) name $(bdir)\wdpmhelp.exe system os2 pm
    @%append $(tmplnk) debug all op symfile
    @%append $(tmplnk) file pmhelp.lbj
    @%append $(tmplnk) library clibl.lib
    @%append $(tmplnk) library os2.lib
    @%append $(tmplnk) option alignment=16
    @%append $(tmplnk) option nodefaultlibs,quiet
    @%append $(tmplnk) option modname=wdpmhelp
    @%append $(tmplnk) option description 'OS/2 Presentation Manager Sample'
    @%append $(tmplnk) option manyautodata
    @%append $(tmplnk) option stack=4096
    @%append $(tmplnk) option heapsize=1024
    @%append $(tmplnk) option protmode
    $(linker) @$(tmplnk)
    $(lang_root)\binp\rc pmhelp.res $(bdir)\wdpmhelp.exe

.c: ..\c

.c.obj:
        $(comp286) $[* -bt=os2 -zq-fpc-d1-oais-s-w3-mc-zu-i$(watcom_dir)\h;$(lang_root)\h\os21x;$(lang_root)\h\os2;$(trap_dir)\h

.c.lbj
        $(comp286) $[* -bt=os2 -zq-fpc-d1-ml-zu-w3-s-i$(lang_root)\h\os21x;$(trap_dir)\h -fo=.lbj

.asm: ..\asm

.asm.obj:
        wasm -zq -2 -bt=os2 $[*

pmlock.obj : pmlock.c ..\h\wdpmhelp.h

pmhelp.res: ..\h\pmhelp.rc ..\h\wdpmhelp.h
        @set oinc=$(%include)
        @set include=$(lang_root)\h\os21x;..\h;$(%include)
        @set opath=$(%path)
        @set path=$(lang_root)\binp
        $(lang_root)\binp\rc -r ..\h\pmhelp.rc .\pmhelp.res
        @set path=$(%opath)
        @set include=$(%oinc)
