#pmake: build cpu_386 os_dos

host_os = dos
host_cpu = 386

proj_name = cwdll

cwdll_autodepends = .autodepend

!include cproj.mif
!include defrule.mif

extra_a_flags = -ms

cwdll.lib: dllfunc.obj cwapi.obj
    $(librarian) -b -c -n -p=512 $@ +dllfunc.obj +cwapi.obj

clean: .SYMBOLIC
    rm -f *.lib
    rm -f dllfunc.obj
