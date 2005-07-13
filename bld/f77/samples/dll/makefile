
dllsamp.dll:	dllsamp.obj
	%create dllsamp.lnk
	%append dllsamp.lnk system nt_dll initinstance terminstance
	%append dllsamp.lnk export DLL_ENTRY_1
	%append dllsamp.lnk export DLL_ENTRY_2
	%append dllsamp.lnk file dllsamp
	wlink @dllsamp.lnk
	%erase dllsamp.lnk

dllsamp.obj:	dllsamp.for
	wfc386 -bd dllsamp
