#pmake: build small cpu_386 os_nov
#
# cplx73s.lib -- Netware stack based 32-bit COMPLEX library fpi, fpr
#
host_cpu = 386

suffix = 3s
target_system = netware
fp_option = -fpi
define_library_include = $(plusplus_complex_dir)/complex.mif
define_deps_include = $(plusplus_complex_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib32$(suffix).mif
!include $(plusplus_dir)/mklib.mif
