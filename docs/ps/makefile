#pmake: all build docs os_nt os_win os_os2 os_dos

dotarget=ps

.mif :  ..\mif

!include books.mif

all : .SYMBOLIC
!ifdef hbook
	@wmake -h -f ..\mif\makebook.mif hbook=$(hbook) dotarget=$(dotarget)
!else
	@for %b in ($(books)) do @wmake -h -f ..\mif\makebook.mif hbook=%b dotarget=$(dotarget)
!endif

!include clean.mif
