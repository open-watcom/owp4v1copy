#pmake: all build medium cpu_i86 os_win
#
# cplx7m.lib -- medium memory-model COMPLEX library fpi for Windows
#
host_cpu = i86

suffix = m
target_system = windows
target_lib = $(prefix)7$(suffix).lib
production_lib = lib286\win
define_library_include = $(plusplus_complex_dir)/complex.mif
define_deps_include = $(plusplus_complex_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib16$(suffix).mif
text = -zW
fp_option = -fpi
!include $(plusplus_dir)/mklib.mif
