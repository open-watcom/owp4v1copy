plustest_name = yacl
!include ..\..\environ.mif

OBJECTS =  binfile.obj bytstore.obj dskbtree.obj slotfile.obj 

!ifdef test1
test_cflags = -ot-op-d1
alt_error=01
!else ifdef test2
test_cflags = -ot-op-d1-xds
alt_error=02
!else ifdef test3
test_cflags = -op-d2-xds
alt_error=03
!else
test_cflags = -op-d2
alt_error=00
!endif

test_cflags += -ei -w0 -zq -i.. -d__OS2__

test_cflags_i86 = -ml

.cpp.obj:
    $(wpp_$(arch)) $[@ $(test_cflags) $(test_cflags_$(arch))
    
test : .symbolic ..\io.lib
    %make global
    %append $(log_file) PASS $(%__CWD__) $(arch)/$(alt_error)
    
..\io.lib : $(OBJECTS)
    *$(librarian_$(arch)) /n/q/b $^@ $(OBJECTS)

global : .symbolic
    %make common_clean
