#pmake: all build flat cpu_386 os_osi
#
# cplx7r.lib -- register based 32-bit COMPLEX library -fpi
#
host_cpu = 386

suffix = 3r
target_system = generic
fp_option = -fpi
define_library_include = $(plusplus_complex_dir)/complex.mif
define_deps_include = $(plusplus_complex_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib32$(suffix).mif
!include $(plusplus_dir)/mklib.mif
