!ifdef __NT__
CFLAGS = /w4 /d2 /zq /k16000
comp = wcl386
!else
!ifdef __OS2__
CFLAGS = /w4 /d2 /zq /k16000
comp = wcl386
!else
CFLAGS = /w4 /d2 /lr /zq /mc /k8192
comp = wcl
!endif
!endif

all : .symbolic
all : mkdisk.exe splitfil.exe packsize.exe

mkdisk.exe : mkdisk.c
    $(COMP) mkdisk $(CFLAGS)

packsize.exe : packsize.c
    $(COMP) packsize

mkdisk.exp : mkdisk.c
    wcl386 /d2 /l=pharlap mkdisk

splitfil.exe : splitfil.c
    $(COMP) splitfil $(CFLAGS)

genmkfil.exe : genmkfil.c
    $(COMP) genmkfil $(CFLAGS)
