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
cflags = -ot-op-d1
!else ifdef test2
cflags = -ot-op-d1-xds
!else ifdef test3
cflags = -op-d2-xds
!else
cflags = -op-d2
!endif

cflags += -ei -w0 -zq -i.. -d__OS2__

cflags_i86 = -ml

.cpp.obj:
    $(wpp_$(arch)) $[@ $(cflags) $(cflags_$(arch))
    
test : .symbolic ..\base.lib
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
..\base.lib : $(OBJECTS)
    *$(librarian_$(arch)) /n/q/b $^@ $(OBJECTS)

global : .symbolic
    %make common_clean
