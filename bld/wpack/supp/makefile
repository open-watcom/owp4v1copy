#pmake: build os_dos os_linux os_nt os_os2 cpu_386 cpu_i86

!     ifeq  %BUILD_PLATFORM dos386
host_os = dos
host_cpu = i86                       # 16 bit version
!else ifeq  %BUILD_PLATFORM linux386
host_os = linux
host_cpu = 386
!else ifeq  %BUILD_PLATFORM nt386
host_os = nt
host_cpu = 386
!else ifeq  %BUILD_PLATFORM os2386
host_os = os2
host_cpu = 386
!else
!error BUILD_PLATFORM environment variable unset or unrecognised.
!endif

nowildargv = 1

!include ../master.mif
