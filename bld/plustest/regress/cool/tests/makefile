plustest_name = cool
!include ../../environ.mif

tests = &
    test_as1.$(exe) &
    test_ass.$(exe) &
    test_avl.$(exe) &
    test_big.$(exe) &
    test_bit.$(exe) &
    test_bt.$(exe) &
    test_cha.$(exe) &
    test_com.$(exe) &
    test_dt.$(exe) &
    test_gs.$(exe) &
    test_has.$(exe) &
    test_lis.$(exe) &
    test_mat.$(exe) &
    test_nod.$(exe) &
    test_nt.$(exe) &
    test_pai.$(exe) &
    test_pro.$(exe) &
    test_que.$(exe) &
    test_ran.$(exe) &
    test_rat.$(exe) &
    test_reg.$(exe) &
    test_rng.$(exe) &
    test_set.$(exe) &
    test_sta.$(exe) &
    test_str.$(exe) &
    test_tim.$(exe) &
    test_val.$(exe) &
    test_vec.$(exe) &
!ifneq arch i86       # Eager template instantiation causes problems for i86.
    test_han.$(exe) &
    test_ite.$(exe) &
    test_m_v.$(exe) &
    test_qua.$(exe) &
!endif

!ifdef test1
test_cflags = -op-d2
alt_error=01
!else ifdef test2
test_cflags = -ot-op-d1-xds
alt_error=02
!else ifdef test3
test_cflags = -op-d2-xds
alt_error=03
!else
test_cflags = -ot-op-d1
alt_error=00
!endif

test_cflags += -zq -I../.. -w0

test_cflags_i86 = -ml -zm
    
stack_386=opt stack=32k
stack_i86=opt stack=24k
stack_axp=opt stack=32k

.obj.$(exe):
    $(linker) @ex.lnk FILE $[@
    
.c.obj:
    $(wpp_$(arch)) $[@ $(test_cflags) $(test_cflags_$(arch))
    
test : .symbolic ex.lnk $(tests)
    %create test.out
    %append test.out test_as1.$(exe)
    $(run) test_as1.$(exe) >>test.out
    %append test.out test_ass.$(exe)
    $(run) test_ass.$(exe) >>test.out
    %append test.out test_avl.$(exe)
    $(run) test_avl.$(exe) >>test.out
    %append test.out test_big.$(exe)
    $(run) test_big.$(exe) >>test.out
    %append test.out test_bit.$(exe)
    $(run) test_bit.$(exe) >>test.out
    %append test.out test_bt.$(exe)
    $(run) test_bt.$(exe) >>test.out
    %append test.out test_cha.$(exe)
    $(run) test_cha.$(exe) >>test.out
    %append test.out test_com.$(exe)
    $(run) test_com.$(exe) >>test.out
    %append test.out test_dt.$(exe)
    $(run) test_dt.$(exe) >>test.out
    %append test.out test_gs.$(exe)
    $(run) test_gs.$(exe) >>test.out
    %append test.out test_has.$(exe)
    $(run) test_has.$(exe) >>test.out
    %append test.out test_lis.$(exe)
    $(run) test_lis.$(exe) >>test.out
    %append test.out test_mat.$(exe)
    $(run) test_mat.$(exe) >>test.out
    %append test.out test_nod.$(exe)
    $(run) test_nod.$(exe) >>test.out
    %append test.out test_nt.$(exe)
    $(run) test_nt.$(exe) >>test.out
    %append test.out test_pai.$(exe)
    $(run) test_pai.$(exe) >>test.out
    %append test.out test_pro.$(exe)
    $(run) test_pro.$(exe) >>test.out
    %append test.out test_que.$(exe)
    $(run) test_que.$(exe) >>test.out
    %append test.out test_ran.$(exe)
    $(run) test_ran.$(exe) >>test.out
    %append test.out test_rat.$(exe)
    $(run) test_rat.$(exe) >>test.out
    %append test.out test_reg.$(exe)
    $(run) test_reg.$(exe) >>test.out
    %append test.out test_rng.$(exe)
    $(run) test_rng.$(exe) >>test.out
    %append test.out test_set.$(exe)
    $(run) test_set.$(exe) >>test.out
    %append test.out test_sta.$(exe)
    $(run) test_sta.$(exe) >>test.out
    %append test.out test_str.$(exe)
    $(run) test_str.$(exe) >>test.out
    %append test.out test_tim.$(exe)
    $(run) test_tim.$(exe) >>test.out
    %append test.out test_val.$(exe)
    $(run) test_val.$(exe) >>test.out
    %append test.out test_vec.$(exe)
    $(run) test_vec.$(exe) >>test.out
!ifneq arch i86
    %append test.out test_han.$(exe)
    $(run) test_han.$(exe) >>test.out
    %append test.out test_ite.$(exe)
    $(run) test_ite.$(exe) >>test.out
    %append test.out test_m_v.$(exe)
    $(run) test_m_v.$(exe) >>test.out
    %append test.out test_qua.$(exe)
    $(run) test_qua.$(exe) >>test.out
!endif
!ifneq arch i86
    diff test.out test.chk
!else
    diff test.out testi86.chk
!endif
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
ex.lnk : makefile
    %create $^@
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(stack_$(arch))
    @%append $^@ LIBRARY ../cool.lib
    
global : .symbolic
    @%make common_clean
