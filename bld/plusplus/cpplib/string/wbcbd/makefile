#pmake: all build large cpu_i86 os_win
#
# strgl.lib -- large memory-model STRING library for Windows
#
host_cpu = i86

suffix = l
target_system = windows
text = -zW
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib16$(suffix).mif
!include $(plusplus_dir)/mklib.mif
