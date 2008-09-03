[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    cdsay <PROJDIR>/cwc/dos386
    wmake -h
    <CPCMD> bcwc.exe <OWBINDIR>/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    cdsay <PROJDIR>/cwc/os2386
    wmake -h
    <CPCMD> bcwc.exe <OWBINDIR>/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    cdsay <PROJDIR>/cwc/nt386
    wmake -h
    <CPCMD> bcwc.exe <OWBINDIR>/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    cdsay <PROJDIR>/cwc/linux386
    wmake -h
    <CPCMD> bcwc.exe <OWBINDIR>/bcwc
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> dos386clean ]
#=====================================
    cdsay <PROJDIR>/cwc/dos386
    wmake -h clean
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386clean ]
#=====================================
    cdsay <PROJDIR>/cwc/os2386
    wmake -h clean
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386clean ]
#====================================
    cdsay <PROJDIR>/cwc/nt386
    wmake -h clean
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386clean ]
#=======================================
    cdsay <PROJDIR>/cwc/linux386
    wmake -h clean
    cdsay <PROJDIR>
