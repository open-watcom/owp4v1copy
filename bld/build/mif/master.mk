SDK_DIR = $(DEVDIR)/sdk
SDK_DIR = $(DEVDIR)/sdk
RC_DIR = $(SDK_DIR)/rc/rc
WRES_DIR = $(SDK_DIR)/rc/wres
WCLIB_DIR = $(DEVDIR)/clib
WATCOMH = $(DEVDIR)/watcom/h
H_DIR = $(DEVDIR)/watcom/h
CPP_DIR = $(DEVDIR)/cpp
DWARF_DIR = $(DEVDIR)/dwarf
DIP_DIR = $(DEVDIR)/dip
FE_MISC_DIR = $(DEVDIR)/fe_misc
LIB_MISC_DIR = $(DEVDIR)/lib_misc
ORL_DIR = $(DEVDIR)/orl
CPPFLAGS = $(addprefix -I,$(INCDIRS)) $(DEFS)
LIBS = $(addprefix -L,$(LIBDIRS))
LDLIBS = $(LIBS)

ifndef TARG_CPU
TARG_CPU=__386__
endif

CFLAGS = -funsigned-char -Wall -g -O2
CPPFLAGS = $(addprefix -I,$(INCDIRS)) $(DEFS)
DEFS = -D$(TARG_CPU) -D__LINUX__ -DLINUX -D__UNIX__= -DUNIX -D_WCUNALIGNED="__attribute__((packed))" -D_LINKER=_WLINK -D__FLAT__ -Uunix -DBOOTSTRAP

$(OBJDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -I$(OBJDIR) -o $@ $<
