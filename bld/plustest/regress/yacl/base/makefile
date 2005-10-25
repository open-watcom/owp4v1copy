plustest_name = yacl
!include ..\..\environ.mif

OBJECTS =    &
basicops.obj &
bitset.obj   &
bytarray.obj &
bytestrm.obj &
bytstrng.obj &
clntset.obj  &
date.obj     &
error.obj    &
gbtree.obj   &
genseq.obj   &
integer.obj  &
intseq.obj   &
intset.obj   &
iofilter.obj &
map.obj      &
memory.obj   &
object.obj   &
objseq.obj   &
objset.obj   &
sequence.obj &
stream.obj   &
strgseq.obj  &
strgset.obj  &
string.obj   &
strsplit.obj &
tbtree.obj   &
timeofda.obj &
tree.obj     &
treewalk.obj

!ifdef test1
test_cflags = -ot-op-d1
!else ifdef test2
test_cflags = -ot-op-d1-xds
!else ifdef test3
test_cflags = -op-d2-xds
!else
test_cflags = -op-d2
!endif

test_cflags += -ei -w0 -zq -i.. -d__OS2__

test_cflags_i86 = -ml

# Required to keep code size under 64KB
test_cflags_i86_map = -zm -oxs

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags) $(test_cflags_$(arch)) $(test_cflags_$(arch)_$^*) 
    
test : .symbolic ..\base.lib
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
..\base.lib : $(OBJECTS)
    *$(librarian_$(arch)) /n/q/b $^@ $(OBJECTS)

global : .symbolic
    %make common_clean
