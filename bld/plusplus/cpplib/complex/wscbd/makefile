#pmake: all build compact cpu_i86 os_win
#
# cplxc.lib -- compact memory-model COMPLEX library fpc for Windows
#
host_cpu = i86

suffix = c
target_system = windows
text = -zW
define_library_include = $(plusplus_complex_dir)/complex.mif
define_deps_include = $(plusplus_complex_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib16$(suffix).mif
!include $(plusplus_dir)/mklib.mif
