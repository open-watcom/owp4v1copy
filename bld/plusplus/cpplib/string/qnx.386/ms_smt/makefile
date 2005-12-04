#pmake: os_qnx cpu_386 flat
#
# strgmt3s.lib -- stack based multi-thread 32-bit STRING library
#
processor  = 386
system     = qnx
model      = ms
convention = stack
linkage    = static
mthreaded  = 1
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
!include $(cpplib_dir)/mklib.mif
