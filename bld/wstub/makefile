#pmake: all build os_dos os_win cpu_i86 cpu_386

host_OS = DOS
host_CPU = i86

all : wstub.exe wstubq.exe

!include cproj.mif
!include defrule.mif
!include deftarg.mif

wstub.exe : wstub.obj
        $(linker) file $[@ name $^@ op quiet, map sys dos
wstubq.exe : wstubq.obj
        $(linker) file $[@ name $^@ op quiet, map sys dos
wstub.obj : wstub.c
        $(cc) -bt=dos $[@ -oaxs
wstubq.obj : wstub.c
        $(cc) -bt=dos $[@ -oaxs -dQUIET -fo=$^@
