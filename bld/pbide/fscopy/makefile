#pmake: all build cpu_i86 os_dos os_nt os_win os_os2

!include deftarg.mif

wfscopy.exe : fscopy.obj
        wlink op map name wfscopy.exe file fscopy

fscopy.obj : fscopy.c
        wcc /w4 fscopy

