#pmake/50: build os_dos os_os2 os_win os_nt os_linux os_nov os_osi os_qnx

wlsystem.lnk: specs.sp
    wsplice -k Pwlsystem $< $@

clean: .symbolic
    rm -f wlsystem.lnk
