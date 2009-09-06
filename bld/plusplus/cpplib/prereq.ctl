# C++ Reference Compilers Prerequisite Tool Build Control File
# ============================================================

[ BLOCK <BUILD_PLATFORM> dos386 ]
#================================
    set REFCOMP_MSG_HOST=DOS 386
    set REFCOMP_WMAKE_HOST_OPTS=host_cpu=386 host_os=dos reference

[ BLOCK <BUILD_PLATFORM> os2386 ]
#================================
    set REFCOMP_MSG_HOST=OS/2 386
    set REFCOMP_WMAKE_HOST_OPTS=host_cpu=386 host_os=os2 reference

[ BLOCK <BUILD_PLATFORM> nt386 ]
#===============================
    set REFCOMP_MSG_HOST=Win32 386
    set REFCOMP_WMAKE_HOST_OPTS=host_cpu=386 host_os=nt reference

[ BLOCK <BUILD_PLATFORM> ntaxp ]
#===============================
    set REFCOMP_MSG_HOST=Win32 AXP
    set REFCOMP_WMAKE_HOST_OPTS=host_cpu=axp host_os=nt reference

[ BLOCK <BUILD_PLATFORM> linux386 ]
#==================================
    set REFCOMP_MSG_HOST=Linux 386
    set REFCOMP_WMAKE_HOST_OPTS=host_cpu=386 host_os=linux reference

[ BLOCK . . ]
#============
    echo Building <REFCOMP_MSG_HOST> C++ reference compilers
    mkdir <PROJDIR>/../rpp.i86
    cdsay <PROJDIR>/../rpp.i86
    wmake -h -k -f ../targ_i86.mif <REFCOMP_WMAKE_HOST_OPTS>
    mkdir <PROJDIR>/../rpp.386
    cdsay <PROJDIR>/../rpp.386
    wmake -h -k -f ../targ_386.mif <REFCOMP_WMAKE_HOST_OPTS>
    mkdir <PROJDIR>/../rpp.axp
    cdsay <PROJDIR>/../rpp.axp
    wmake -h -k -f ../targ_axp.mif <REFCOMP_WMAKE_HOST_OPTS>

cdsay <PROJDIR>
