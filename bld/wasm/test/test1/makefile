.CONTINUE
.IGNORE

cmd_file = test_tmp.cmd

objs = &
387 &
a &
alias &
anon &
b &
backquot &
beep &
bigext &
bug &
bunk &
c &
cmps &
comm &
current &
data &
dosinite &
do_ax &
dup &
empty &
error &
f &
farjmp &
fcom &
five &
float &
foo &
g &
getenv &
hello &
ifs &
include &
ins &
irp &
jmpext &
jmpfword &
jmpsize &
jmptst &
kyb &
l2 &
l3 &
labelbug &
lods &
maaak &
macro &
macroloc &
macrotst &
mainnew &
mangle &
masmsuks &
masmtest &
movs &
new &
newbug &
newmif &
offset &
org &
orgage &
outs &
passtest &
proc &
proc3 &
pub &
pub2 &
pubdef &
q &
qnxstrt &
quote &
scas &
segoff &
shell &
size &
sizebug &
sizebug2 &
stos &
strmang &
struct &
structjn &
testing &
tmp &
tn &
wincall &
word &
x &
xx

dev_objs = &
alignorg &
anthony &
bar &
dos16m &
dosstart &
fubar &
jmp &
loader2 &
main &
newproc &
notcomm &
t2 &
test

asm_flags_x = -c

!ifndef asm
asm=wasm -q -zld
!endif 

count=1

all : $(cmd_file) $(objs) $(dev_objs)
	@call $(cmd_file)

$(cmd_file) : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off

$(objs) : .SYMBOLIC
	@%append $(cmd_file) @echo $^@.asm ======================================================
	@%append $(cmd_file) @$(asm) $(asm_flags_$^@) $^@.asm >$^@.tmp 2>&1
	@%append $(cmd_file) @if not exist $^@.chk goto next$^@a
        @%append $(cmd_file) @diff -b $^@.tmp $^@.chk
	@%append $(cmd_file) goto next$^@c
	@%append $(cmd_file) :next$^@a
	@%append $(cmd_file) type $^@.tmp
	@%append $(cmd_file) :next$^@c
	@%append $(cmd_file) @if not exist $^@.obj goto next$^@d
	@%append $(cmd_file) @wdis -l=$^@.dis -a $^@.obj
	@%append $(cmd_file) @if not exist $^@.esm goto next$^@b
        @%append $(cmd_file) @diff -b $^@.dis $^@.esm
	@%append $(cmd_file) :next$^@b
	@%append $(cmd_file) @dmpobj -q -l -rec=FIXUPP $^@.obj
	@%append $(cmd_file) @if not exist $^@.emp goto next$^@d
        @%append $(cmd_file) @diff -b $^@.lst $^@.emp
	@%append $(cmd_file) :next$^@d

$(dev_objs) : .SYMBOLIC
	@%append $(cmd_file) @echo .
	@%append $(cmd_file) @echo ==============================================================
	@%append $(cmd_file) @echo DEVELOPMENT - $^@.asm 
	@%append $(cmd_file) @echo ==============================================================
	@%append $(cmd_file) @echo .
	@%append $(cmd_file) @$(asm) $(asm_flags_$^@) $^@.asm >$^@.tmp 2>&1
	@%append $(cmd_file) @if not exist $^@.chk goto next$^@a
        @%append $(cmd_file) @diff -b $^@.tmp $^@.chk
	@%append $(cmd_file) goto next$^@c
	@%append $(cmd_file) :next$^@a
	@%append $(cmd_file) type $^@.tmp
	@%append $(cmd_file) :next$^@c
	@%append $(cmd_file) @if not exist $^@.obj goto next$^@d
	@%append $(cmd_file) @wdis -l=$^@.dis -a $^@.obj
	@%append $(cmd_file) @if not exist $^@.esm goto next$^@b
        @%append $(cmd_file) @diff -b $^@.dis $^@.esm
	@%append $(cmd_file) :next$^@b
	@%append $(cmd_file) @dmpobj -q -l -rec=FIXUPP $^@.obj
	@%append $(cmd_file) @if not exist $^@.emp goto next$^@d
        @%append $(cmd_file) @diff -b $^@.lst $^@.emp
	@%append $(cmd_file) :next$^@d

clean : .SYMBOLIC
	rm -f *.obj *.err *.dis *.lst *.tmp test_tmp.cmd mem.trk
