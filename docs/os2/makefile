#pmake: build docs os_os2

dotarget=os2

.mif :  ..\mif

!include books.mif

!ifdef hbook
books=$(hbook)
!endif

all: .SYMBOLIC $(books)
    @%null

$(books) : .SYMBOLIC
    @wmake -h -f ..\mif\master.mif hbook=$@ dotarget=$(dotarget)

!include clean.mif
