#pmake: all build os_os2 cpu_386

host_os  = os2
host_cpu = 386

proj_name = editdll

!ifndef editdll_autodepends
editdll_autodepends = .AUTODEPEND
!endif

sys_dll = 1

!include cproj.mif
!include deftarg.mif
!include defrule.mif

extra_l_flags_os2 = initinstance terminstance pm
extra_l_flags_os2 += op description 'Open Watcom Editor DLL for EPM'
extra_l_flags_os2 += op manyautodata

all: epmlink.dll wedit.dll .SYMBOLIC

epmlink.dll: epmlink.obj
    $(linker) $(lflags) name $^& file $[@

wedit.dll: epmlink.obj
    $(linker) $(lflags) name $^& file $[@
    $(librarian) -n -b -c $^*.lib +$^*.dll

epmtest.exe : epmtest.obj
    $(linker) d a op st=16k, case sys os2v2_pm file epmtest.obj lib wedit.lib

epmspy.exe : epmspy.obj
    $(linker) sys os2v2_pm file epmspy.obj
