#pmake: build cpu_i86 os_win

host_os  = win
host_cpu = i86

!include cproj.mif
!include defrule.mif
!include deftarg.mif

.c: ..

run.exe : run.obj
        wlink deb all sys windows name $@ file run op quiet,map

