#pmake: banal

compiler = watcom
	
banal.exe : banal.obj
	wlink d a f banal op map

banal.obj : banal.c
	wcc386 -w4 -we -d2 banal.c
