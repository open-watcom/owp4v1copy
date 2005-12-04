#pmake: build flat cpu_386 os_os2
#
# strg3s.lib -- stack based 32-bit STRING library
#
processor  = 386
system     = os2
model      = mf
convention = stack
linkage    = static
fs_registration = yes
define_library_include = $(plusplus_string_dir)/string.mif
define_deps_include = $(plusplus_string_dir)/deps.mif
!include $(cpplib_dir)/mklib.mif
