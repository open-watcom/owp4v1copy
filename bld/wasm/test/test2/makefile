.CONTINUE
.IGNORE

cmd_file = test_tmp.cmd

objs = &
a &
adc &
arithop &
asmbug &
asmbug1 &
b &
bar &
bitwise &
bunk &
c &
dasm &
data &
dup &
enter &
fadd &
foo &
fubar &
help &
imul &
j2 &
jmp &
jmptst &
jnk &
label &
new &
old1 &
old2 &
ptrtest &
reljmp &
reljmp16 &
scal &
set386 &
setjmp &
struct &
stupid &
t &
t01 &
t02 &
t03 &
t04 &
t05 &
t06 &
t07 &
t08 &
t09 &
t10 &
t11 &
t12 &
t13 &
t14 &
t15 &
t16 &
t17 &
t18 &
t19 &
t20 &
t21 &
t22 &
t23 &
t24 &
t25 &
t26 &
t27 &
t28 &
t29 &
t30 &
t31 &
t32 &
t33 &
t34 &
t35 &
t36 &
t37 &
t38 &
t51 &
test &
timer &
tmp

test : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off
	@%write $(cmd_file) @echo %1.asm ======================================================
	@%write $(cmd_file) @wasm %1.asm -q
	@%write $(cmd_file) @if not exist %1.err goto next1
	@%write $(cmd_file) @if not exist %1.chk goto next2
        @%write $(cmd_file) @diff -b %1.err %1.chk
	@%write $(cmd_file) goto next1
	@%write $(cmd_file) :next2
	@%write $(cmd_file) type %1.err
	@%write $(cmd_file) :next1
	@for %x in ($(objs)) do @call $(cmd_file) %x

debug : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off
	@%write $(cmd_file) @echo %1.asm ======================================================
	@%write $(cmd_file) @wasm %1.asm -q -d6
	@%write $(cmd_file) @if not exist %1.err goto next1
	@%write $(cmd_file) @if not exist %1.chk goto next2
        @%write $(cmd_file) @diff -b %1.err %1.chk
	@%write $(cmd_file) goto next1
	@%write $(cmd_file) :next2
	@%write $(cmd_file) type %1.err
	@%write $(cmd_file) :next1
	@for %x in ($(objs)) do @call $(cmd_file) %x

clean : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off
	@%write $(cmd_file) @if exist %1.obj del %1.obj
	@%write $(cmd_file) @if exist %1.err del %1.err
	@for %x in ($(objs)) do @call $(cmd_file) %x
	@if exist test_tmp.cmd del test_tmp.cmd
	@if exist mem.trk erase mem.trk
