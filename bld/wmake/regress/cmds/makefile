.extensions:
.extensions: . .exe .obj .c

.c.obj :
    $(comp286) $[* -d1 -oailt -s -zq -w4

.obj.exe :
!ifdef __OS2__
    $(linker) sys os2 debug all file $[* op q
!else
    $(linker) sys dos debug all file $[* op q
!endif

.exe. : .symbolic
    @%null

all :   ifeq mkerr envarg .symbolic
    @%null
