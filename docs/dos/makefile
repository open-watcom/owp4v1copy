#pmake: all build docs os_dos

dotarget=dos

.mif :  ..\mif

!include books.mif

all: .SYMBOLIC
#	@for %b in ($(books)) do @wmake -d -f ..\mif\timestamp.mif hbook=%b dotarget=$(dotarget)
!ifdef hbook
	@wmake -h -f ..\mif\makebook.mif hbook=$(hbook) dotarget=$(dotarget)
!else
	@for %b in ($(books)) do @wmake -f ..\mif\makebook.mif hbook=%b dotarget=$(dotarget)
!endif

!include clean.mif
