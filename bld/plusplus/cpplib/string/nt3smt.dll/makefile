#pmake: all build flat cpu_386 os_nt dynamic
#
# strgmt3s.lib -- stack based multi-thread 32-bit STRING library
#
host_cpu = 386

fs_registration = yes
target_system = nt
text = -d__USE_FS
suffix = mt3s
linkage = dynamic
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib$(suffix).mif
!include $(plusplus_dir)/mklib.mif
