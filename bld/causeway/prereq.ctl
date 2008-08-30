[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    mkdir <PROJDIR>/cwc/<PREOBJDIR>
    cdsay <PROJDIR>/cwc/<PREOBJDIR>
    wmake -h -f ../dos386/makefile
    <CPCMD> bcwc.exe <OWBINDIR>/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    mkdir <PROJDIR>/cwc/<PREOBJDIR>
    cdsay <PROJDIR>/cwc/<PREOBJDIR>
    wmake -h -f ../os2386/makefile
    <CPCMD> bcwc.exe <OWBINDIR>/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    mkdir <PROJDIR>/cwc/<PREOBJDIR>
    cdsay <PROJDIR>/cwc/<PREOBJDIR>
    wmake -h -f ../nt386/makefile
    <CPCMD> bcwc.exe <OWBINDIR>/
    cdsay <PROJDIR>

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    mkdir <PROJDIR>/cwc/<PREOBJDIR>
    cdsay <PROJDIR>/cwc/<PREOBJDIR>
    wmake -h -f ../linux386/makefile
    <CPCMD> bcwc.exe <OWBINDIR>/bcwc
    cdsay <PROJDIR>
