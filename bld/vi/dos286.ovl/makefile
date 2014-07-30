#pmake: build os_dos cpu_i86
host_os  = dos
host_cpu = i86

# Build a 286 version; includes extended memory and XMS support
is286 = 1

# Define overlay flags (-of is critical!)
extra_c_flags_dos_i86 = -D__V__ -of

# Linker flags to create a dynamic overlay
start_flags  = op dynamic, area=24k
start_flags += begin section
more_flags   = autosection
final_flags  = end

# Use Watcom debug format for overlaid code
vi_debugfmt = watcom

!include ../mif/master.mif
