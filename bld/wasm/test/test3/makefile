.CONTINUE
.IGNORE

cmd_file = test_tmp.cmd

objs = &
allign &
datamix1 &
datamix2 &
mseg &
plsintr &
segatrep &
structdd &
testequ

dev_objs = &
irpexitm &
lea &
lsxxx &
retiret

!ifndef asm
asm=wasm -q -zld
!endif

all : $(cmd_file) $(objs) $(dev_objs)
	@call $(cmd_file)

$(cmd_file) : .SYMBOLIC
	@%create $(cmd_file)
	@%write $(cmd_file) @echo off

$(objs) : .SYMBOLIC
	@%append $(cmd_file) @echo $^@.asm ======================================================
	@%append $(cmd_file) @$(asm) $(asm_flags_$^@) $^@.asm >$^@.tmp 2>&1
	@%append $(cmd_file) @if not exist $^@.chk goto next$^@a
        @%append $(cmd_file) @diff -i -b $^@.tmp $^@.chk
	@%append $(cmd_file) goto next$^@c
	@%append $(cmd_file) :next$^@a
	@%append $(cmd_file) type $^@.tmp
	@%append $(cmd_file) :next$^@c
	@%append $(cmd_file) @if not exist $^@.obj goto next$^@d
	@%append $(cmd_file) @wdis -l=$^@.dis -a $^@.obj
	@%append $(cmd_file) @if not exist $^@.esm goto next$^@b
        @%append $(cmd_file) @diff -i -b $^@.dis $^@.esm
	@%append $(cmd_file) :next$^@b
	@%append $(cmd_file) @dmpobj -q -l -rec=FIXUPP $^@.obj
	@%append $(cmd_file) @if not exist $^@.emp goto next$^@d
        @%append $(cmd_file) @diff -i -b $^@.lst $^@.emp
	@%append $(cmd_file) :next$^@d

$(dev_objs) : .SYMBOLIC
	@%append $(cmd_file) @echo .
	@%append $(cmd_file) @echo ==============================================================
	@%append $(cmd_file) @echo DEVELOPMENT - $^@.asm 
	@%append $(cmd_file) @echo ==============================================================
	@%append $(cmd_file) @echo .
	@%append $(cmd_file) @$(asm) $(asm_flags_$^@) $^@.asm >$^@.tmp 2>&1
	@%append $(cmd_file) @if not exist $^@.chk goto next$^@a
        @%append $(cmd_file) @diff -i -b $^@.tmp $^@.chk
	@%append $(cmd_file) goto next$^@c
	@%append $(cmd_file) :next$^@a
	@%append $(cmd_file) type $^@.tmp
	@%append $(cmd_file) :next$^@c
	@%append $(cmd_file) @if not exist $^@.obj goto next$^@d
	@%append $(cmd_file) @wdis -l=$^@.dis -a $^@.obj
	@%append $(cmd_file) @if not exist $^@.esm goto next$^@b
        @%append $(cmd_file) @diff -i -b $^@.dis $^@.esm
	@%append $(cmd_file) :next$^@b
	@%append $(cmd_file) @dmpobj -q -l -rec=FIXUPP $^@.obj
	@%append $(cmd_file) @if not exist $^@.emp goto next$^@d
        @%append $(cmd_file) @diff -i -b $^@.lst $^@.emp
	@%append $(cmd_file) :next$^@d

clean : .SYMBOLIC
	rm -f *.obj *.err *.dis *.lst *.tmp test_tmp.cmd mem.trk
