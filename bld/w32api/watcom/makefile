#pmake: all build os_nt cpu_386


all: ./mkmh.exe .symbolic
        $<
        cd win
        wmake
        cd ..

./mkmh.exe: lastbld
        bwcl386 -wx -we -zq mkmh.c -fe=$@ -fo=.obj

lastbld: 
        wtouch $@
        
clean: .symbolic
        rm lastbld master.mif deps.mif mkmh.obj mkmh.exe
        -rm -fr win watcom
