#pmake: all build flat cpu_386 os_nt
#
# strg3r.lib -- register based 32-bit STRING library
#
host_cpu = 386

fs_registration = yes
target_system = nt
text = -d__USE_FS
suffix = 3r
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
define_target_include = $(plusplus_dir)/lib32$(suffix).mif
!include $(plusplus_dir)/mklib.mif
