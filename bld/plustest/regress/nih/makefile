plustest_name = nih
!include ..\environ.mif

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
cflags_386 += -d2
cflags_i86 += -d2
cflags_axp += -d2
!else
cflags_386 += -ox-d1
cflags_i86 += -ox-d1
cflags_axp += -ox-d1
!endif

cflags_386 += -zq-mf
cflags_i86 += -zq-ml
cflags_axp += -zq

.c.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch)) -i. -dNESTED_TYPES -w0
    
test : .symbolic $(objs)
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
global : .symbolic
    %make common_clean
