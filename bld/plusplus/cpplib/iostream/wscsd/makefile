#pmake: all build small cpu_i86 os_win
#
# iosts.lib -- small memory-model IOSTREAM library for Windows
#
host_cpu = i86

suffix = s
target_system = windows
text = -zW
define_library_include = $(plusplus_iostream_dir)/iostream.mif
define_deps_include = $(plusplus_iostream_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib16$(suffix).mif
!include $(plusplus_dir)/mklib.mif
