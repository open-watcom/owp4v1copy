#pmake/50: docs ps pdf

# postscript files need to be created before pdf files

dotarget=ps

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

