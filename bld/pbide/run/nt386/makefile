#pmake: build cpu_386 os_nt

host_os  = nt
host_cpu = 386

!include cproj.mif
!include defrule.mif
!include deftarg.mif

.c: ..

runnt.exe : run.obj
        wlink debug all sys nt_win name $@ file run op quiet,map
