ctest_name = positive
!include ..\environ.mif

positive_dir = source

!include $(positive_dir)\positive.mif

!ifdef test1
cflags_386 = -d2-mf
cflags_i86 = -d2-mc-2
cflags_axp = -d2-si
!ifndef __MSDOS__
add_br_ = -br
!endif
alt_error = 01
!else ifdef test2
cflags_386 = -d1-mf
cflags_i86 = -d1-mm
cflags_axp = -d1-si
alt_error = 02
!else ifdef test3
cflags_386 = -d2-ms
cflags_i86 = -d2-ml
cflags_axp = -d2
alt_error = 03
!else ifdef test4
cflags_386 = -d1-mf
cflags_i86 = -d1-ms
cflags_axp = -d1-si
alt_error = 04
!else ifdef test5
cflags_386 = -d1-mf
cflags_i86 = -d1-mc-2
cflags_axp = -d1
alt_error = 05
!else ifdef test6
cflags_386 = -d2-mf
cflags_i86 = -d2-mm
cflags_axp = -d2
alt_error = 06
!else ifdef test7
cflags_386 = -d1-ms
cflags_i86 = -d1-ml
cflags_axp = -d1
alt_error = 07
!else ifdef test8
cflags_386 = -d2-mf
cflags_i86 = -d2-ms
cflags_axp = -d2
alt_error = 08
!else ifdef test9
cflags_386 = -d1-oe-mf
cflags_i86 = -d1-oe-ms
cflags_axp = -d1-oe
alt_error = 09
!else
cflags_386 = -ox-d1-ms
cflags_i86 = -ox-d1-ms
cflags_axp = -ox-d1
alt_error = 00
!ifndef __MSDOS__
add_br_ = -br
!endif
!endif

!ifndef noisy
run_quietly = -zq
!endif

cflags_386 += -we-w0 -i$(positive_dir) $(run_quietly)
cflags_i86 += -we-w0 -i$(positive_dir) $(run_quietly)
cflags_axp += -we-w0 -i$(positive_dir) $(run_quietly)

!ifeq arch 386
cflags_386 += $(add_br_$(no_br_$^*))
!endif

.error
    @%append $(%error_file) $(%error_msg)

.c: $(positive_dir)
.c.obj:
    @set error_msg=failure to compile $^@
    $(wcc_$(arch)) $[@ $(cflags_$(arch)) $(opt_$^*) $(opt_EH_$(EH_$^*))

.obj.$(exe):
    @set error_msg=failure to link $^@
    $(linker) @test.lnk $(opt_link_$^*) FILE $[@
    @set error_msg=failure to execute $^@
    @%append exec.out PASS executing $^@
    $(run) $^@ >>exec.out
    del $[@
    wstrip -q $^@

test : .symbolic actual_test
    @set error_file=error.out
    @set error_msg=failure to complete test stream
    echo OK >error.out
    $(bld_cl) chk_exec.c -ox-d1
    chk_exec exec.out >>error.out
    diff -b error.out error.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)

actual_test : .symbolic start_test $(positive_exes)
    if exist *.obj del *.obj
    if exist *.$(exe) del *.$(exe)

fnstat1.$(exe) : fnstat1a.obj fnstat1b.obj
    @set error_msg=failure to link $^@
    $(linker) @test.lnk NAME $^@ FILE $[@, $]@
    @set error_msg=failure to execute $^@
    @%append exec.out PASS executing $^@
    $(run) $^@ >>exec.out

start_test : .symbolic test.lnk
    %create exec.out
    @set error_file=exec.out

test.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ OPTION stack=8k, quiet, caseexact, noredef

save : .symbolic
    if exist exec.out copy exec.out s$(arch)_$(alt_error).sav

global : .symbolic
    @if exist failure del failure
    @%make common_clean
