!include cproj.mif
!include deftarg.mif

INCLUDE = h
OPTS = -ols -s

CFLAGS = $(OPTS) -d1-hw -3 -fpc -wx -we
cflags_nt = -ox -bt=NT
cflags_os2 = -bt=os2 -mc
cflags_dos = -bt=dos -ms -d_DOS
lflags_nt = nt option stack=12k,caseexact,map,symfile
lflags_dos = dos option stack=8k,caseexact,symfile
lflags_os2 = os2 pmc option stack=12k,caseexact,heap=8k,newfile,map,symfile

wbind.exe: wbind.obj
    $(linker) debug watcom all name wbind file wbind system $(lflags_$(host_OS))
wbind.obj : ..\wbind.c
    $(CC) $[* $(CFLAGS) $(cflags_$(host_OS))

