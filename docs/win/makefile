#pmake: all build docs os_win

dotarget=win

.mif:  ..\mif

!include books.mif

!ifdef hbook
books=$(hbook)
!endif

all: .SYMBOLIC $(books)
    @%null

$(books) : .SYMBOLIC
    @wmake -h -f ..\mif\master.mif hbook=$@ dotarget=$(dotarget)

!include clean.mif
