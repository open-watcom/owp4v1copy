#pmake: build flat cpu_386 os_os2 dynamic debug
#
# iostmt3r.lib -- register based multi-thread 32-bit IOSTREAM library
#
processor  = 386
system     = os2
model      = mf
convention = register
linkage    = dynamic
mthreaded  = 1
fs_registration = yes
cpplib_release  = 0

define_library_include = $(plusplus_iostream_dir)/iostream.mif
define_deps_include    = $(plusplus_iostream_dir)/deps.mif
!include $(plusplus_dir)/mklib.mif
