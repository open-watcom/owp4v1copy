#pmake: cpu_386

!include deftarg.mif

!ifeq release 0
debug_info = debug line
!else
debug_info =
!endif

.BEFORE:
    @set include=$(%include);$(watcom_dir)\h;$(dig_dir)\h

.c.obj:
    $(comp386) $[@ -mf -d1 -ox -w4

smpdump.exe : smpdump.obj
    wlink NAME $^@ $(debug_info) FILE $[@
