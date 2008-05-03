#pmake: build flat cpu_386 os_os2 dynamic debug
processor  = 386
system     = os2
model      = mf
convention = register
linkage    = dynamic
mthreaded  = 1
cpplib_release = 0
debug_lib_page_size = -p=128
subdir     = mf_rmtd.dll
additional_cleanup  = export*.def
define_library_include = $(cpplib_dir)/plib.mif
!include $(cpplib_dir)/mrglib.mif
