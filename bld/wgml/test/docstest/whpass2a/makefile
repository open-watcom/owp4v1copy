#pmake: build whpass2a

!ifdef hbook
hbpar = hbook=$(hbook)
!endif

all : .SYMBOLIC
    @cd dos
    @wmake -h $(hbpar)
    @cd ../htmlhelp
    @wmake -h $(hbpar)
    @cd ../nt
    @wmake -h $(hbpar)
    @cd ../os2
    @wmake -h $(hbpar)
    @cd ../win
    @wmake -h $(hbpar)
    @cd ..

clean : .SYMBOLIC
    cd dos
    wmake -h clean
    cd ../htmlhelp
    wmake -h clean
    cd ../nt
    wmake -h clean
    cd ../os2
    wmake -h clean
    cd ../win
    wmake -h clean
    cd ..

