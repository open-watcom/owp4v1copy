#pmake: build os_linux cpu_386 i386 intel

host_OS         = linux
host_CPU        = 386

!ifeq bootstrap 1
target_CPU = 386
all : libcg$(target_CPU).a .SYMBOLIC
!endif

!include ../mif/master.mif

!ifeq bootstrap 1
cc=cc
bld_cl = cc

INCLUDE = -I../h -I../../h -I../../../h -I../../../../watcom/h &
-I../../../../cfloat/h -I../../../../dwarf/dw/h -I../../../../comp_cfg/h &
-I../../../../owl/h
CFLAGS = -c -o $@ -funsigned-char -pipe -O $(INCLUDE) -DNDEBUG -D__SMALL__
gen_hdrs = $(bit_sets) $(cg_dir)/h/cgprotos.gh $(cg_dir)/h/feprotos.gh
bitset_opts = -o $@ -g
protos_opts = -E -traditional -o $@

libcg$(target_CPU).a : $(gen_hdrs) $(cg_objs) $(qnx_objs) $(intel_objs) $(mem_objs) $($(target_CPU)_objs)
        ar crs $@ $<

!endif
