#pmake: all os_dos cpu_386
host_os = dos
host_cpu = 386

proj_name = defgen
xname = defgen.exe

lnk = $(name).lnk

!include cproj.mif
!include deftarg.mif
!include defrule.mif

inc_dirs = ..;

objs = ytab.obj main.obj scan.obj error.obj semantic.obj output.obj

$(xname) : ytab.h ytab.c $(objs) $(__MAKEFILES__)
        @%create $(lnk)
        @%append $(lnk) name $^@
        @for %i in ($(objs)) do @%append $(lnk) file %i
        $(linker) $(lflags) @$(lnk)

.c: ..

ytab.c ytab.h: ../yydriver.c ../defgen.y
        $(byacc) -d $]@ $[@
