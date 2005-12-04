#pmake: build flat cpu_386 os_osi os_dos os_linux
#
# cplx3r.lib -- register based 32-bit COMPLEX library fpc
#
processor  = 386
system     = generic
model      = mf
convention = register
linkage    = static
define_library_include = $(plusplus_complex_dir)/complex.mif
define_deps_include = $(plusplus_complex_dir)/deps.mif
!include $(cpplib_dir)/mklib.mif
