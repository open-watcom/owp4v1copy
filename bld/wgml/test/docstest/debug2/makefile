#pmake: build debug

all : .SYMBOLIC
    @echo Only "clean" is allowed at this level

clean : .SYMBOLIC
    cd dos
    wmake -h hbook=wd clean
    cd ../htmlhelp
    wmake -h hbook=wd clean
    cd ../nt
    wmake -h hbook=wd clean
    cd ../os2
    wmake -h hbook=wd clean
    cd ../pdf
    wmake -h hbook=wd clean
    cd ../win
    wmake -h hbook=wd clean
    cd ..

