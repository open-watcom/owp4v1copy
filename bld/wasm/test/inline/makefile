.CONTINUE
.IGNORE

cmd_file = test_tmp.cmd

objs = &
486 &
adc &
add &
admodes &
admodes1 &
admodes2 &
admodes3 &
admodes4 &
admodes5 &
admodes6 &
amodes &
amodes1 &
amodes2 &
amodes3 &
amodes4 &
amodes5 &
amodes6 &
and &
bt &
bug &
cc2 &
cmp &
cmpx &
dec &
div &
dmodes1 &
dmodes2 &
dmodes3 &
dmodes4 &
dmodes5 &
dmodes6 &
enter &
fadd &
fcom &
fdiv &
fld &
fmul &
fp &
fpmisc &
fpmisc2 &
fsub &
fubar &
fucom &
idiv &
imul &
in &
inc &
lea &
lod &
lxs &
misc &
misc2 &
modes &
modes1 &
modes2 &
modes3 &
modes4 &
modes5 &
modes6 &
mov &
mov2 &
movs &
movx &
mul &
neg &
not &
or &
out &
outs &
pop &
rcl &
sal &
sbb &
scas &
setcc &
shld &
sub &
test &
try &
xadd &
xchg &
xor &
asmbug1 &
asmbug2 &
asmbug3 &
asmbug4 &
asmbug5 &
asmbug6 &
asmbug7 &
asmbug8 &
asmbug9 &
asmbug10 &
asmbug11 &
asmbug12 &
asmbug13

.exe : ..
.c : ..

test : reverse.exe .SYMBOLIC
!ifndef ccomp
ccomp=wcc386
!endif 
	@%make test_files

reverse.exe : reverse.c
	wcl ..\reverse.c -fe=..\reverse.exe

test_files : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off
	@%write $(cmd_file) echo %1 ================================================================
	@%write $(cmd_file) set cmplr=%2
	@%write $(cmd_file) if exist %1.c rm %1.c
	@%write $(cmd_file) vi -d -q -k ":r %1.asm\n:so toc.vi\n:0r cinit.vi\n:w %1.c\n:q!\n"
	@%write $(cmd_file) %cmplr% %1 -s -ot -zm -zq
	@%write $(cmd_file) wdis -a -l %1
	@%write $(cmd_file) vi -d -q -k ":so fixlst.vi\nZZ" %1.lst
	@%write $(cmd_file) ..\reverse %1.lst
	@%write $(cmd_file) if exist %1.chk goto next
	@%write $(cmd_file) diff -b %1.lst %1.asm
	@%write $(cmd_file) goto end
	@%write $(cmd_file) :next
	@%write $(cmd_file) diff -b %1.lst %1.chk
	@%write $(cmd_file) :end
	@%write $(cmd_file) set cmplr=
	@for %x in ($(objs)) do @call $(cmd_file) %x $(ccomp)


clean : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off
	@%write $(cmd_file) @if exist %1.obj del %1.obj
	@%write $(cmd_file) @if exist %1.lst del %1.lst
	@%write $(cmd_file) @if exist %1.c del %1.c
	@%write $(cmd_file) @if exist %1.err del %1.err
	@for %x in ($(objs)) do @call $(cmd_file) %x
	@if exist reverse.obj erase reverse.obj
	@if exist ..\reverse.exe erase ..\reverse.exe
	@if exist test_tmp.cmd erase test_tmp.cmd
	@if exist mem.trk erase mem.trk


