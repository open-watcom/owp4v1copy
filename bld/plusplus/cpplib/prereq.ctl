# C++ Reference Compilers Prerequisite Tool Build Control File
# ============================================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    echo Building DOS 386 C++ reference compilers
    cdsay <PROJDIR>\..\dos386.i86
    wmake -h -k reference
    cdsay ..\dos386.386
    wmake -h -k reference

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    echo Building OS/2 386 C++ reference compilers
    cdsay <PROJDIR>\..\os2386.i86
    wmake -h -k reference
    cdsay ..\os2386.386
    wmake -h -k reference

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    echo Building Win32 386 C++ reference compilers
    cdsay <PROJDIR>\..\nt386.i86
    wmake -h -k reference
    cdsay ..\nt386.386
    wmake -h -k reference

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    echo Building Win32 AXP C++ reference compilers
    cdsay <PROJDIR>\..\ntaxp.axp
    wmake -h -k reference

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    echo Building Linux 386 C++ reference compilers
    cdsay <PROJDIR>/../linux386.i86
    wmake -h -k reference
    cdsay ../linux386.386
    wmake -h -k reference

[ BLOCK . . ]
#============

cdsay <PROJDIR>
