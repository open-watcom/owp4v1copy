plustest_name = dcmat
!include ../environ.mif

objs = &
    compmat.obj &
    cosft.obj &
    doubmat.obj &
    four1.obj &
    realft.obj &
    svdcmp.obj
    
!ifdef test1
cflags_386 = -d2
cflags_i86 = -d2
cflags_axp = -d2
!else
cflags_386 = -ox-d1
cflags_i86 = -ox-d1
cflags_axp = -ox-d1
!endif

cflags_386 += -zq -e10 -w0 -i. -d__ATT_complex__
cflags_i86 += -zq -e10 -w0 -i. -d__ATT_complex__ -mm -zm
cflags_axp += -zq -e10 -w0 -i. -d__ATT_complex__

stack_386=opt stack=16k
stack_i86=opt stack=8k
stack_axp=opt stack=16k

.c.$(exe):
    $(wpp_$(arch)) $[@ $(cflags_$(arch))
    $(linker) @test.lnk FILE $[*
    
.c.obj:
    $(wpp_$(arch)) $[@ $(cflags_$(arch))
    
test : .symbolic matrix.lib test.lnk test.$(exe) tmatrix.$(exe)
    $(run) test >test.out
    $(run) tmatrix >>test.out
    diff test.out test.chk
    %make global
    %append $(log_file) PASS $(%__CWD__)
    
matrix.lib : $(objs) matrix.lbc
    $(librarian_$(arch)) /b/n/q $^@ @matrix.lbc
    
matrix.lbc : makefile
    %create $^@
    @for %i in ( $(objs) ) do @%append $^@ -+%i
    
test.lnk : makefile
    %create $^@
    @%append $^@ $(ldebug_$(arch))
    @%append $^@ $(lnk_$(arch))
    @%append $^@ $(stack_$(arch))
    @%append $^@ LIBRARY matrix.lib
    
global : .symbolic
    @%make common_clean
