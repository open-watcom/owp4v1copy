#pmake: all build compact cpu_i86 os_osi
#
# cplx7c.lib -- compact memory-model COMPLEX library fpi
#
host_cpu = i86

suffix = c
target_system = generic
fp_option = -fpi
define_library_include = $(plusplus_complex_dir)/complex.mif
define_deps_include = $(plusplus_complex_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib16$(suffix).mif
!include $(plusplus_dir)/mklib.mif
