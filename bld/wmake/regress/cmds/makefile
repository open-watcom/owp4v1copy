proj_name = wmake_regress_cmds

!include cproj.mif
!include deftarg.mif
!include defrule.mif

targets = mkerr.exe envarg.exe ifeq.exe

all: $(targets) .SYMBOLIC
	@%null
	
mkerr.exe: mkerr.c
        $(bld_cl) $(inc_path) $[@ $(wcl_util_opts)

envarg.exe: envarg.c
        $(bld_cl) $(inc_path) $[@ $(wcl_util_opts)

ifeq.exe: ifeq.c
        $(bld_cl) $(inc_path) $[@ $(wcl_util_opts)
