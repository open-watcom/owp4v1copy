# C++ Reference Compilers Prerequisite Tool Build Control File
# ============================================================

[ BLOCK <1> dos386 ]
#================================
    echo Building DOS 386 C++ reference compilers
    cdsay <projdir>\..\c
    cdsay ..\dosi86
    wmake -h -k
    wmake -h -k reference
    cdsay ..\dos386
    wmake -h -k
    wmake -h -k reference
    cdsay <projdir>

[ BLOCK <1> os2386 ]
#================================
    echo Building OS/2 386 C++ reference compilers
    cdsay <projdir>\..\c
    cdsay ..\os2i86
    wmake -h -k
    wmake -h -k reference
    cdsay ..\os2386
    wmake -h -k
    wmake -h -k reference
    cdsay <projdir>

[ BLOCK <1> nt386 ]
#===============================
    echo Building Win32 386 C++ reference compilers
    cdsay <projdir>\..\c
    cdsay ..\nti86
    wmake -h -k
    wmake -h -k reference
    cdsay ..\nt386
    wmake -h -k
    wmake -h -k reference
    cdsay <projdir>
