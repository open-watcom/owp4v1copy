.CONTINUE
.IGNORE

cmd_file = test_tmp.cmd

objs = &
387 &
a &
alias &
alignorg &
anon &
anthony &
b &
backquot &
bar &
beep &
bigext &
bug &
bunk &
c &
cmps &
comm &
current &
data &
dos16m &
dosinite &
dosstart &
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
fubar &
g &
getenv &
hello &
ifs &
include &
ins &
irp &
jmp &
jmpext &
jmpfword &
jmpsize &
jmptst &
kyb &
l2 &
l3 &
labelbug &
loader2 &
lods &
maaak &
macro &
macroloc &
macrotst &
main &
mainnew &
mangle &
masmsuks &
masmtest &
movs &
new &
newbug &
newmif &
newproc &
notcomm &
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
t2 &
test &
testing &
tmp &
tn &
wincall &
word &
x &
xx

test : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off
	@%write $(cmd_file) @echo %1.asm ======================================================
	@%write $(cmd_file) @wasm %1.asm -q
	@for %x in ($(objs)) do @call $(cmd_file) %x

clean : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off
	@%write $(cmd_file) @if exist %1.obj del %1.obj
	@%write $(cmd_file) @if exist %1.err del %1.err
	@for %x in ($(objs)) do @call $(cmd_file) %x
	@if exist test_tmp.cmd del test_tmp.cmd
	@if exist mem.trk erase mem.trk
