# C++ Reference Compilers Prerequisite Tool Clean Control File
# ============================================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>/../dos386.i86
    wmake -h -k clean refcomp=1
    cdsay ../dos386.386
    wmake -h -k clean refcomp=1
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>/../os2386.i86
    wmake -h -k clean refcomp=1
    cdsay ../os2386.386
    wmake -h -k clean refcomp=1
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>/../nt386.i86
    wmake -h -k clean refcomp=1
    cdsay ../nt386.386
    wmake -h -k clean refcomp=1
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    cdsay <PROJDIR>/../linux386.i86
    wmake -h -k clean refcomp=1
    cdsay ../linux386.386
    wmake -h -k clean refcomp=1
    cdsay <PROJDIR>
