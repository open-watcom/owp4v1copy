#pmake: build lcl os_os2 cpu_386 cpu_386 trap x86
host_cpu = 386

#
# Create the 32-bit OS/2 STD.DLL
#

os      = os2v2
srv     = lcl
cpu     = x86
name    = std

#extra_cflags = -d2

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
        exp TellHandles_.4,TellHardMode_.5             &
        imp _TempStack wdsplice, DoReadWord_ wdsplice  &
        imp DoWriteWord_ wdsplice, DoClose_ wdsplice   &
        imp DoOpen_ wdsplice, DoWritePgmScrn_ wdsplice &
        imp _XferBuff wdsplice, DoDupFile_ wdsplice    &
        lib $(os2386),$(os2286)

!include $(trap_dir)\trap.mif
