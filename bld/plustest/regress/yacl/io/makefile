plustest_name = yacl
!include ..\..\environ.mif

OBJECTS =  binfile.obj bytstore.obj dskbtree.obj slotfile.obj 

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
    
test : .symbolic ..\io.lib
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
..\io.lib : $(OBJECTS)
    *$(librarian_$(arch)) /n/q/b $^@ $(OBJECTS)

global : .symbolic
    %make common_clean
