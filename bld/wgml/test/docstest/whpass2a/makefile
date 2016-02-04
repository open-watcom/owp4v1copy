#pmake: build whpass2a

!ifdef hbook
booktodo = (hbook=$(hbook))
!endif

all : .SYMBOLIC
    @cd dos
    @wmake -h $(booktodo)
    @cd ../htmlhelp
    @wmake -h $(booktodo)
    @cd ../nt
    @wmake -h $(booktodo)
    @cd ../os2
    @wmake -h $(booktodo)
    @cd ../win
    @wmake -h $(booktodo)
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

