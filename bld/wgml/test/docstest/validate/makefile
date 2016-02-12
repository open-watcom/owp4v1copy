#pmake: build validate

all : .SYMBOLIC
    @cd dos
    @wmake -h hbook=$(hbook)
    @cd ../htmlhelp
    @wmake -h hbook=$(hbook)
    @cd ../nt
    @wmake -h hbook=$(hbook)
    @cd ../os2
    @wmake -h hbook=$(hbook)
    @cd ../win
    @wmake -h hbook=$(hbook)
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

