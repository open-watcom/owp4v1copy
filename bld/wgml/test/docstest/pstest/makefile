#pmake: build pstest

!ifdef hbook
hbpar = hbook=$(hbook)
!endif

all : .SYMBOLIC
    @cd pdf4
    @wmake -h $(hbpar)
    @cd ../pdf
    @wmake -h $(hbpar)
    @cd ..

clean : .SYMBOLIC
    @cd pdf4
    wmake -h clean
    @cd ../pdf
    wmake -h clean
    cd ..

