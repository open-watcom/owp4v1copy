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
t39 &
t51 &
test &
timer &
tmp

!ifndef asm
asm=wasm -q
!endif

all : $(cmd_file) $(objs)
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
	@%append $(cmd_file) @wdis -l=$^@.dis -a $^@.obj
	@%append $(cmd_file) @if not exist $^@.esm goto next$^@b
        @%append $(cmd_file) @diff -b $^@.dis $^@.esm
	@%append $(cmd_file) :next$^@b
	@%append $(cmd_file) @dmpobj -q -l -rec=FIXUPP $^@.obj
	@%append $(cmd_file) @if not exist $^@.emp goto next$^@c
        @%append $(cmd_file) @diff -b $^@.lst $^@.emp
	@%append $(cmd_file) :next$^@c

clean : .SYMBOLIC
	rm -f *.obj *.err *.dis *.lst *.tmp test_tmp.cmd mem.trk
