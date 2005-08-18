
# Makefile for Open Watcom Fortran

exeext=.exe    # all platforms currently supported?
cplnk=wfl386   # originally f77
flags=         # originally -o

all: asetup$(exeext) adventure$(exeext) ADVTXT .symbolic
    @echo Adventure compile done.

asetup$(exeext): asetup.for asubs.for
    $(cplnk) $(flags) -FE=asetup$(exeext) asetup.for asubs.for

adventure$(exeext): aamain.for asubs.for
    $(cplnk) $(flags) -FE=adventure$(exeext) aamain.for asubs.for

ADVTXT: ADVDAT asetup$(exeext)
    asetup$(exeext)

clean: .symbolic
    @rm *.obj
    @rm *.err
    @rm *.exe
    @echo Clean done.
