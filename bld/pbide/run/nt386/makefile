#pmake: all build cpu_386 os_nt

runnt.exe : ../run.c
        wcc386 -bt=nt -mf -i$(lang_root)\h\nt $< -d2 -w4 /fo=runnt.obj
        wlink deb all sys nt_win file runnt.obj op map
