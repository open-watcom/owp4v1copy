#pmake: all build medium cpu_i86 os_win
#
# strgm.lib -- medium memory-model STRING library for Windows
#
host_cpu = i86

suffix = m
target_system = windows
text = -zW
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib16$(suffix).mif
!include $(plusplus_dir)/mklib.mif
