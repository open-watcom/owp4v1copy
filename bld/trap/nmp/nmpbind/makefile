#pmake: build remote support nmp os_os2 cpu_i86 cpu_386

host_os  = os2
host_cpu = i86

!include cproj.mif
!include deftarg.mif

!ifndef bin_root
!ifdef bin_root_wv
bin_root = $(bin_root_wv)
!endif
!endif

objs = nmpbind.obj nmpos2.obj

!ifdef on_build_machine
default_targ    = w
!else
default_targ    = d
!endif

first : $(default_targ) .SYMBOLIC
        @%null

w : $(bin_root)\binp\nmpbind.exe .SYMBOLIC
        @%null

d : $(bin_root)\binp\dnmpbind.exe .SYMBOLIC
        @%null

$(bin_root)\binp\nmpbind.exe : $(objs)
    $(linker) sys os2 file {$objs} name $^@ op stack=8k

$(bin_root)\binp\dnmpbind.exe : $(objs)
    $(linker) sys os2 d a file {$objs} name $^@ op stack=8k

.c: ..\c
.c.obj:
    $(comp286) $[@ /d1 /w3 /oaxs /bm /zu /ml /i$(inc_dirs_sys);$(trap_dir)\h
