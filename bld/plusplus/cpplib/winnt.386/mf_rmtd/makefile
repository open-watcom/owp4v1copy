#pmake: build debug flat cpu_386 os_nt

processor  = 386
system     = winnt
model      = mf
convention = register
linkage    = static
mthreaded  = 1
subdir     = mf_rmtd

cpplib_release = 0
debug_lib_page_size = -p=128

define_library_include = $(cpplib_dir)/plib.mif
!include $(cpplib_dir)/mrglib.mif
