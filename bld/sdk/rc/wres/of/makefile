#pmake: build os_dos os_nt os_win os_os2 os_linux cpu_386


host_OS = dos
host_CPU = 386
libname = f

!include ../master.mif

lexusbld : .symbolic all
!ifdef __LINUX__
    mkdir -p $(lexlib_dir)/wres/lib
!else
    @if not exist $(lexlib_dir)\wres\lib mkdir $(lexlib_dir)\wres\lib
!endif
    @newer ../lib/wresf.lib $(lexlib_dir)/wres/lib "^cp &f $#f > nul"

