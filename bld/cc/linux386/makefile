#pmake: build 386 linux cpu_386 os_linux
host_CPU = 386
host_OS = linux
target_CPU = 386
cc_version = tree

!ifeq bootstrap 1
target = wcc386
all: $(target)
!endif

!include ../master.mif

!ifeq bootstrap 1
cg_inc_dirs = -I$(cfloat_dir)/h -I$(watcom_dir)/h -I$(owl_dir)/h &
  -I../../cg/h -I../../cg/intel/h -I../../cg/intel/$(target_CPU)/h
inc_dirs = -I. -I../h
inc_dirs += -I$(cc_dir)/h $(cg_inc_dirs)
inc_dirs += -I$(inline_asm_h)
inc_dirs += -I$(dwarf_dir)/dw/h
inc_dirs += -I$(comp_cfg_dir)/h
inc_dirs += -I$(fe_misc_dir)/h

cc = cc
bld_cl = cc

CPPFLAGS = $(inc_dirs) -DNDEBUG -Dlocal=static -D__UNIX__ -D__LINUX__ -D__SMALL__ -D_M_IX86
CFLAGS = $(CPPFLAGS) -c -funsigned-char -pipe -std=gnu99 -O -o $@
LDFLAGS = -L$(cg_dir)/intel/$(target_CPU)/$(%OBJDIR) -L$(dwarf_dir)/dw/$(%OBJDIR) &
-L$(cfloat_dir)/$(%OBJDIR) -L$(owl_dir)/$(%OBJDIR) -L$(clib_dir)/$(%OBJDIR) &
-L$(wres_dir)/$(%OBJDIR)
LIBS = -lcg$(target_cpu) -ldwarf -lcfloat -lowl -lwatcom -lwres
wcl_util_opts = $(inc_dirs) $(LDFLAGS) -o $[&.exe -D__UNIX__ $(LIBS)

code$(target_CPU).s: ../a/code$(target_CPU).asm ../asm2s.sed
        sed -f ../asm2s.sed $[@ > $@

../h/code$(target_CPU).gh : code$(target_CPU).s ../a/mkcode.c
        $(CC) -D__$(target_CPU)__ $(INCLUDE) ../a/mkcode.c code$(target_CPU).s -o mkcode
        ./mkcode
	rm -f mkcode

weights.gh keywords.gh : ../h/c.key ../h/pc.key ../h/seh.key ../h/dummy.key
	$(CC) $(INCLUDE) ../../fe_misc/c/findhash.c -o findhash
	./findhash -q $<
	rm -f findhash

$(target): ../h/code$(target_CPU).gh weights.gh keywords.gh msgtxt.gh msgdefs.gh msgattr.gh errors01.int $(intname)01.int usage.gh $(cc_objs)
	$(cc) $(LDFLAGS) -o $@ $(cc_objs) $(LIBS)

!endif
