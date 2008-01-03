plustest_name = nih
!include ../environ.mif

objs =  &
    arrayob.obj  &
    assoc.obj  &
    assocint.obj  &
    bag.obj  &
    bitboard.obj  &
    bitset.obj  &
    class.obj  &
    collecti.obj  &
    date.obj  &
    dictiona.obj  &
    float.obj  &
    fraction.obj  &
    heap.obj  &
    identdic.obj  &
    identset.obj  &
    integer.obj  &
    iterator.obj  &
    keysortc.obj  &
    link.obj  &
    linkedli.obj  &
    linkob.obj  &
    lookupke.obj  &
    nihclerr.obj  &
    nil.obj  &
    object.obj  &
    oiofd.obj  &
    oionih.obj  &
    oiostrea.obj  &
    oiotbl.obj  &
    orderedc.obj  &
    point.obj  &
    random.obj  &
    range.obj  &
    rectangl.obj  &
    seqcltn.obj  &
    set.obj  &
    sharedqu.obj  &
    sortedcl.obj  &
    stack.obj  &
    string.obj  &
    time.obj 
    
!ifdef test1
test_cflags_386 += -d2
test_cflags_i86 += -d2
test_cflags_axp += -d2
alt_error=01
!else
test_cflags_386 += -ox-d1
test_cflags_i86 += -ox-d1
test_cflags_axp += -ox-d1
alt_error=00
!endif

test_cflags_386 += -zq-mf
test_cflags_i86 += -zq-ml
test_cflags_axp += -zq

.c.obj:
    $(wpp_$(arch)) $[@ $(test_cflags_$(arch)) -i. -dNESTED_TYPES -w0
    
test : .symbolic $(objs)
    @%make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
save : .symbolic
    @if exist exec.out cp exec.out s$(arch)_$(alt_error).sav

global : .symbolic
    @%make common_clean
