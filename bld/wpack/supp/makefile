#pmake: build os_nt os_dos os_os2 os_linux cpu_386

!ifeq %BUILD_PLATFORM dos386
host_os = dos
host_cpu = 386
!else ifeq %BUILD_PLATFORM nt386
host_os = nt
host_cpu = 386
!else ifeq %BUILD_PLATFORM os2386
host_os = os2
host_cpu = 386
!else ifeq %BUILD_PLATFORM linux386
host_os = linux
host_cpu = 386
!else
!endif

nowildargv = 1

!include ../master.mif
