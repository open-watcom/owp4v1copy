#pmake: build cpu_386 os_dos

host_os = dos
host_cpu = 386

proj_name = cwdll

cwdll_autodepends = .autodepend
cwdll_distrib_lib = 1

!include cproj.mif
!include defrule.mif

extra_a_flags = -ms

cwdll.lib: dllfunc.obj cwapi.obj
    $(librarian) $(libflags) $<

clean: .SYMBOLIC
    rm -f *.lib
    rm -f dllfunc.obj
