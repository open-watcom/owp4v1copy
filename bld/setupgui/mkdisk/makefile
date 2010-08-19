#pmake: build os_nt os_dos os_os2 os_linux cpu_386

proj_name = mkdisk

mkdisk_autodepends = .autodepend

!ifeq %BUILD_PLATFORM dos386
host_os  = dos
host_cpu = 386
ziplib = ../ziplib/osi386/ziplib.lib
!else ifeq %BUILD_PLATFORM nt386
host_os  = nt
host_cpu = 386
ziplib = ../ziplib/osi386/ziplib.lib
!else ifeq %BUILD_PLATFORM os2386
host_os  = os2
host_cpu = 386
ziplib = ../ziplib/osi386/ziplib.lib
!else ifeq %BUILD_PLATFORM linux386
host_os  = linux
host_cpu = 386
ziplib = ../ziplib/linux386/ziplib.lib
!else
!endif

!include cproj.mif
!include deftarg.mif
!include defrule.mif

inc_dirs = -I"$(libzip_dir)/lib" -I"$(zlib_dir)" 

commands = mkexezip.exe uzip.exe splitfil.exe packsize.exe genmkfil.exe

all : $(commands) .SYMBOLIC

mkexezip.exe : mkexezip.obj
    $(cl) $(clflags) $< $(cl_libs)

packsize.exe : packsize.obj
    $(cl) $(clflags) $< $(cl_libs)

splitfil.exe : splitfil.obj
    $(cl) $(clflags) $< $(cl_libs)

genmkfil.exe : genmkfil.obj
    $(cl) $(clflags) $< $(cl_libs)

uzip.exe : microzip.obj $(ziplib) ../zlib/osi$(host_cpu)/zlib.lib
    $(cl) $(clflags) $< $(cl_libs)
