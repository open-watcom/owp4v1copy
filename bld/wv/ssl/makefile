#pmake: build

!include ../make.wv

bin = bin

.extensions:
.extensions: .prs .ssl

!ifndef on_build_machine
verbose = /v
!endif

!ifdef __UNIX__
nulldevice = /dev/null
!else
nulldevice = nul
!endif

all : fortran.prs cpp.prs c.prs java.prs .SYMBOLIC

common = dbgintr.ssl dbgrules.ssl

fortran.prs : fortran.ssl $(common)

cpp.prs : cpp.ssl pp_c.ssl $(common)

c.prs : c.ssl pp_c.ssl $(common)

java.prs : java.ssl pp_c.ssl $(common)

.ssl.prs
        ssl $(verbose) $[@ $^@
!ifndef on_build_machine
    @cp $^@ $(bdir) >$(nulldevice)
!endif

clean: .SYMBOLIC
    @if exist *.prs @del *.prs
