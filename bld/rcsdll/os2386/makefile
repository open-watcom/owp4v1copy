#pmake: all build cpu_386 os_os2

!include cproj.mif
!include deftarg.mif

CC = wpp386
CFLAGS = -d2 -w4 -bd -zq -mf -bt=os2 -i="$(lang_root)/h;$(inc_dirs_sys_os2_386)"
OBJS = rcsdll.obj inifile.obj

.EXTENSIONS:
.EXTENSIONS: .obj
.EXTENSIONS: .cpp

rcsdll.dll : $(OBJS)
    $(linker) op map d all sys os2v2 dll initinstance terminstance pm debug all option manyautodata fi { $(OBJS) }
    $(librarian) -n -b -c $^*.lib +$^*.dll

.cpp : ..;../mkssi

.cpp.obj : .AUTODEPEND
    $(CC) $< $(CFLAGS)
