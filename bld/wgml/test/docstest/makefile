#pmake: build docstest

all : .SYMBOLIC
    @echo Only "clean" is allowed at this level

clean : .SYMBOLIC
    cd debug
    wmake -h clean
    cd miftest
    wmake -h clean
    cd ../validate
    wmake -h clean
    cd ../pstest
    wmake -h clean
    cd ../whpass1a
    wmake -h clean
    cd ../whpass1b
    wmake -h clean
    cd ../whpass2a
    wmake -h clean
    cd ..

