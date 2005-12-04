#pmake: build flat cpu_386 os_osi
#
# iostmt3s.lib -- stack based multi-thread 32-bit IOSTREAM library
#
processor  = 386
system     = generic
model      = mf
convention = stack
linkage    = static
mthreaded  = 1
define_library_include = $(plusplus_iostream_dir)/iostream.mif
define_deps_include = $(plusplus_iostream_dir)/deps.mif
!include $(cpplib_dir)/mklib.mif
