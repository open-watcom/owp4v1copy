#pmake: all build os_nt cpu_386


all: mkmh.exe .symbolic

mkmh.exe: lastbld
	wcc386 /zq mkmh.c
	wlink option quiet system nt file mkmh.obj
	mkmh
	cd win
	$(MAKE)
        cd ..

lastbld: 
	touch $@
        
clean: .symbolic
	rm lastbld master.mif deps.mif mkmh.obj mkmh.exe
	-rm -fr win watcom
