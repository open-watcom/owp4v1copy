#pmake: all build large cpu_i86 os_win
#
# cplx7l.lib -- large memory-model COMPLEX library fpi for Windows
#
host_cpu = i86

suffix = l
target_system = windows
text = -zW
fp_option = -fpi
define_library_include = $(plusplus_complex_dir)/complex.mif
define_deps_include = $(plusplus_complex_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib16$(suffix).mif
!include $(plusplus_dir)/mklib.mif
