# C++ Reference Compilers Prerequisite Tool Clean Control File
# ============================================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>/../dos386.i86
    wmake -h -k clean
    cdsay ../dos386.386
    wmake -h -k clean
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>/../os2386.i86
    wmake -h -k clean
    cdsay ../os2386.386
    wmake -h -k clean
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>/../nt386.i86
    wmake -h -k clean
    cdsay ../nt386.386
    wmake -h -k clean
    cdsay <PROJDIR>
