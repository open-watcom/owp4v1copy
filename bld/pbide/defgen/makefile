#pmake: all os_dos cpu_386
name = defgen
lnk = $(name).lnk

!include deftarg.mif

objs = ytab.obj main.obj scan.obj error.obj semantic.obj output.obj

cflags = -w4 -mf -d2 -fo=obj\$[&.obj

.EXTENSIONS:
.EXTENSIONS: .exe
.EXTENSIONS: .obj
.EXTENSIONS: .asm .c .h

$(name).exe : $(objs) $(lnk)
        wlink @$(lnk)

$(name).lnk : makefile
        %create $(lnk)
!ifeq release 0
        @%append $(lnk) debug all
!endif
        @%append $(lnk) system dos4g
        @%append $(lnk) name $(name).exe
        @%append $(lnk) option map
        @for %i in ($(objs)) do @%append $(lnk) file obj\%i

ytab.c : yydriver.c defgen.y
        yacc -d defgen.y yydriver.c

.obj : obj

.c.obj :
        $(comp386) $[* $(cflags)
