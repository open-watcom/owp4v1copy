#pmake: build trap remote os_os2 cpu_386 par
host_CPU = i86
host_OS = os2

# This is a quick and dirty project because it is OS/2 specific and
# intended to be temporary; Yes, I know it breaks all known rules. Once
# WLINK properly supports 16-bit IOPL segments in 32-bit DLLs, the port
# I/O can be built into the parallel trap file/server modules, which is
# where it belongs in the first place.

!include cproj.mif
!include cdirs.mif
!include deftarg.mif

wdio.dll: wdio.obj
    $(linker) sys os2 dll d all op protmode name $@ f $< segment 'IO_TEXT' IOPL export INPORT 2, OUTPORT 4

wdio.obj: wdio.c
    $(cc) -oxs -ml -bd -d2 $<
