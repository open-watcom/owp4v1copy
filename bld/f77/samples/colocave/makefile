
# Makefile for Open Watcom Fortran

exeext=.exe     # all platforms currently supported?
cplnk=wfl386    # originally f77
flags=          # originally -o

all: asetup$(exeext) adventure$(exeext) .symbolic
    asetup$(exeext)

asetup$(exeext): asetup.for asubs.for
    $(cplnk) $(flags) -FE=asetup$(exeext) asetup.for asubs.for

adventure$(exeext): aamain.for asubs.for
    $(cplnk) $(flags) -FE=adventure$(exeext) aamain.for asubs.for

clean: .symbolic
    @rm *.obj
    @rm *.err
    @rm *.exe
    @rm advtxt.*
    @echo Clean performed.
