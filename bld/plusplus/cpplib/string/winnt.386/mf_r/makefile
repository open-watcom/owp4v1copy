#pmake: build flat cpu_386 os_nt
#
# strg3r.lib -- register based 32-bit STRING library
#
processor  = 386
system     = winnt
model      = mf
convention = register
linkage    = static
fs_registration = yes
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
!include $(cpplib_dir)/mklib.mif
