#pmake: os_qnx cpu_386 flat
#
# strgmt3r.lib -- register based multi-thread 32-bit STRING library
#
host_cpu = 386

target_system = qnx
suffix = mt3r
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib$(suffix).mif
!include $(plusplus_dir)/mklib.mif
