# C++ Reference Compilers Prerequisite Tool Clean Control File
# ============================================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <projdir>\..\dosi86
    wmake -h -k clean
    cdsay ..\dos386
    wmake -h -k clean
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <projdir>\..\os2i86
    wmake -h -k clean
    cdsay ..\os2386
    wmake -h -k clean
    cdsay <projdir>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <projdir>\..\nti86
    wmake -h -k clean
    cdsay ..\nt386
    wmake -h -k clean
    cdsay <projdir>
