# C++ Reference Compilers Prerequisite Tool Clean Control File
# ============================================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>\..\dosi86
    wmake -h -k clean
    cdsay ..\dos386
    wmake -h -k clean
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>\..\os2i86
    wmake -h -k clean
    cdsay ..\os2386
    wmake -h -k clean
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>\..\nti86
    wmake -h -k clean
    cdsay ..\nt386
    wmake -h -k clean
    cdsay <PROJDIR>
