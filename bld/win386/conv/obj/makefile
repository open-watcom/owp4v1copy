#pmake/50: all build os_win cpu_i86 cpu_386
#host_os = win
host_cpu = 386

!include cproj.mif
!include deftarg.mif
!include defrule.mif

.EXTENSIONS:
.EXTENSIONS: .exe .obj .asm .def .c .h .mif

.def : ..\def
.asm : ..\asm
.c : ..\c
.h : ..\h

def_files = &
clib.def &
commdlg.def &
ddeml.def &
dpmi.def &
lzexpand.def &
mmsystem.def &
odbc.def &
ole.def &
penwin.def &
shell.def &
stress.def &
toolhelp.def &
ver.def &
win31.def &
win31_16.def &

all : conv.exe win386.lib .SYMBOLIC
        @%null

conv.exe : conv.c makefile
    $(bld_cl) $[* $(wcl_util_opts)

win386.lib : $(def_files) ..\def\win386 conv.exe fldenv.obj _call16.obj
        conv ..\def\win386 ..\def -s -q
        ls -1 win*.obj >tmp.lbc
        $(librarian) /n /b /p=16 win386.lib +fldenv.obj +_call16.obj @tmp.lbc
        @erase tmp.lbc
        @copy winglue.asm ..\..\asm
        @copy dllthunk.c ..\..\c
        @copy dllthk.asm ..\..\asm
        @copy winglue.inc ..\..\h
        @erase winglue.asm
        @erase dllthk.asm
        @erase dllthunk.c
        @erase winglue.inc

fldenv.obj : fldenv.asm
        $(as) -i$(watcomdir)h $[*
_call16.obj : _call16.asm
        $(as) $[*
