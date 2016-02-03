#pmake: build whpass1b

!ifndef hbook
target = all
!else
target = $(hbook)
!endif

$(target).ptf : .SYMBOLIC
    cd dos
    wmake hbook=$(hbook)
    cd ../htmlhelp
    wmake hbook=$(hbook)
    cd ../nt
    wmake hbook=$(hbook)
    cd ../os2
    wmake hbook=$(hbook)
    cd ../win
    wmake hbook=$(hbook)
    cd ..

clean : .SYMBOLIC
    cd dos
    wmake clean
    cd ../htmlhelp
    wmake clean
    cd ../nt
    wmake clean
    cd ../os2
    wmake clean
    cd ../win
    wmake clean
    cd ..

