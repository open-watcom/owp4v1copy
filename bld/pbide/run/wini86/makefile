#pmake: all build cpu_i86 os_win

!include cproj.mif
!include deftarg.mif

run.exe : ../run.c
        wcc -bt=windows -ml -i$(lang_root)\h\win $< -d2 -w4
        wlink deb all sys windows file run.obj op map
