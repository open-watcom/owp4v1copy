#pmake: all build flat cpu_386 os_nt
#
# iost3r.lib -- register based 32-bit IOSTREAM library
#
processor  = 386
system     = winnt
model      = f
convention = register
linkage    = static
fs_registration = yes
define_library_include = $(plusplus_iostream_dir)/iostream.mif
define_deps_include = $(plusplus_iostream_dir)/deps.mif
!include $(plusplus_dir)/mklib.mif
