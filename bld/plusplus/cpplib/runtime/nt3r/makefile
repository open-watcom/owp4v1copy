#pmake: all build flat cpu_386 os_nt
#
# plib3r -- 32-bit 3r PLIB, PLBX
#
host_cpu = 386

fs_registration = yes
suffix = 3r
target_system = nt
text = -D__USE_FS
define_library_include = $(plusplus_runtime_dir)/plib.mif
define_target_include = $(plusplus_dir)/lib32$(suffix).mif
!include $(plusplus_dir)/mklib.mif
