#pmake: all build docs os_nt

dotarget=nt

.mif :  ..\mif

!include books.mif

all: .SYMBOLIC
!ifdef hbook
	@wmake -h -f ..\mif\makebook.mif hbook=$(hbook) dotarget=$(dotarget)
!else
	@for %b in ($(books)) do @wmake -f ..\mif\makebook.mif hbook=%b dotarget=$(dotarget)
!endif

!include clean.mif
