#pmake: all build flat cpu_386 os_os2 dynamic
#
# contmt3r.lib -- register based multi-thread 32-bit CONTAINER library
#
host_cpu = 386

fs_registration = yes
target_system = os2v2
text = -d__USE_FS
suffix = mt3r
linkage = dynamic
define_library_include = $(plusplus_contain_dir)/contain.mif
define_target_include = $(plusplus_dir)/lib$(suffix).mif
!include $(plusplus_dir)/mklib.mif
