.CONTINUE
.IGNORE

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

!ifndef testcomp
testcomp=wcc386
!endif 

all : clean reverse.exe $(objs) .SYMBOLIC

reverse.exe : reverse.c
	wcl ..\reverse.c -fe=..\reverse.exe

$(objs) : .SYMBOLIC
	@echo $@ ================================================================
	@rm -f $@.c
	@vi -d -q -k ":r $@.asm\n:so toc.vi\n:0r cinit.vi\n:w $@.c\n:q!\n"
	@$(testcomp) $@.c -s -ot -zm -zq -fo=.obj
	@wdis -a -l $@.obj
	@vi -d -q -k ":so fixlst.vi\nZZ" $@.lst
	@..\reverse $@.lst
        @wmake -h -f ../checki.mif name=$@

clean : .SYMBOLIC
	@rm -f *.obj *.lst *.c *.err reverse.obj ..\reverse.exe mem.trk
