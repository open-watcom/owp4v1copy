#pmake: all all build lcl os_os2 cpu_386 cpu_386 trap x86
#
# Create the 32-bit OS/2 STD.DLL
#

os      = os2v2
srv     = lcl
cpu     = x86
name    = std

extra_objs =          &
        accmisc.obj   &
        os2v2acc.obj  &
        os2rfx.obj    &
        pgmexec.obj   &
        softmode.obj  &
        dbgthrd.obj   &
        os2conv.obj   &
        loadhelp.obj  &
        pmlock.obj    &
        x86cpu.obj    &
        supp.obj      &
        core.obj

extra_linker = &
        exp TellHandles_.4,TellHardMode_.5 &
        imp _TempStack wdsplice, DoReadWord_ wdsplice &
        imp DoWriteWord_ wdsplice, DoClose_ wdsplice &
        imp DoOpen_ wdsplice, DoDupFile_ wdsplice &
        lib os2386,$(lang_root)\lib286\os2\os2.lib

!include $(trap_dir)\trap.mif
