#pmake: all build docs os_win

dotarget=win

.mif:  ..\mif

!include books.mif

all: .SYMBOLIC
!ifdef hbook
	@wmake -h -f ..\mif\makebook.mif hbook=$(hbook) dotarget=$(dotarget)
!else
	@for %b in ($(books)) do @wmake -h -f ..\mif\makebook.mif hbook=%b dotarget=$(dotarget)
!endif

!include clean.mif
