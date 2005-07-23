#pmake: build os_nt os_dos os_os2 cpu_386

proj_name = mkdisk

mkdisk_autodepends = .autodepend

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

!include cproj.mif
!include deftarg.mif
!include defrule.mif

extra_l_flags = op map

inc_dirs = -I"$(libzip_dir)/lib" -I"$(zlib_dir)"

commands = mkdisk.exe mkexezip.exe splitfil.exe packsize.exe genmkfil.exe

all : $(commands) .SYMBOLIC

mkdisk.exe : mkdisk.obj
    $(linker) $(lflags) name $@ file {$<}

mkexezip.exe : mkexezip.obj
    $(linker) $(lflags) name $@ file {$<}

packsize.exe : packsize.obj
    $(linker) $(lflags) name $@ file {$<}

splitfil.exe : splitfil.obj
    $(linker) $(lflags) name $@ file {$<}

genmkfil.exe : genmkfil.obj
    $(linker) $(lflags) name $@ file {$<}
