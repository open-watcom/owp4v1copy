#pmake: all build flat cpu_386 os_osi os_dos os_linux
#
# cont3s.lib -- stack based 32-bit CONTAINER library
#
host_cpu = 386

suffix = 3s
target_system = generic
target_lib = $(prefix)$(suffix).lib
production_lib = lib386
define_library_include = $(plusplus_contain_dir)/contain.mif
define_target_include = $(plusplus_dir)/lib32$(suffix).mif
!include $(plusplus_dir)/mklib.mif
