#pmake: os_qnx cpu_386 flat
#
# iostmt3r.lib -- register based multi-thread 32-bit IOSTREAM library
#
processor  = 386
system     = qnx
model      = ms
convention = register
linkage    = static
mthreaded  = 1
define_library_include = $(plusplus_iostream_dir)/iostream.mif
define_deps_include = $(plusplus_iostream_dir)/deps.mif
!include $(cpplib_dir)/mklib.mif
