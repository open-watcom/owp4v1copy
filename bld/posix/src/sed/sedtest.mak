# testsed.mak
#
# Makefile to test sed

!ifdef %testsed
sed = $(%testsed
!else
sed = sed
!endif

# testsed.mak
#
# Makefile to test sed

!ifdef %testsed
sed = $(%testsed
!else
sed = sed
!endif

all: .symbolic sedcomp sedexec
    @%null

sedcomp: .symbolic main compile cmdcomp rhscomp recomp cmdline address gettext resolve ycomp
    @%null

clean: .symbolic
    rm -f afore after scrip

main: .symbolic noargs badarg enarg fgarg noef badsquig labels
    @%null

# I am unconvinced this should succeed. Bad in SUSV3. GNU sed 3.02 outputs
# help and fails. WFB 20040730
noargs: .symbolic
    $(sed)

badarg: .symbolic
    > afore echo hello
    type afore | $(sed) -e > after 2>&1 || true
    > xpect echo sed: error processing: -e
    diff after xpect
    type afore | $(sed) -f > after 2>&1 || true
    > xpect echo sed: error processing: -f
    diff after xpect
    type afore | $(sed) -? > after 2>&1 || true
    > xpect echo sed: unknown flag ?
    >>xpect echo sed: error processing: -?
    diff after xpect

enarg: .symbolic
    > afore echo hello
    $(sed) -e p -n afore > after
    diff afore after

fgarg: .symbolic
    > afore echo hello
    > xpect echo heLLo
    > scrip echo s/l/L/
    $(sed) -g -f scrip afore > after # -g only applies to subsequent arguments
    diff after xpect

noef: .symbolic
    > afore echo hello
    $(sed) -n p afore > after
    diff afore after

badsquig: .symbolic
    > afore echo hello
    $(sed) -e "}" afore > after 2>&1 || true
    > xpect echo sed: too many }'s
    diff after xpect
    $(sed) -e "{p" afore > after 2>&1 || true
    > xpect echo sed: too many {'s
    diff after xpect

labels: .symbolic
    > afore echo hello
    $(sed) -e :label -e p -n afore > after
    diff afore after

compile: .symbolic semicolon AGMSG emptyre TRAIL
    @%null

semicolon: .symbolic
    > afore echo hello
    $(sed) -e "$# This is a comment" -e "; p" -n afore > after
    diff afore after

AGMSG: .symbolic
    > afore echo hello
    > xpect echo sed: garbled address /\)/p
    $(sed) -e /\)/p afore >after 2>&1 || true
    diff after xpect

emptyre: .symbolic
    > afore echo hello
    > xpect echo sed: first RE must be non-null
    $(sed) -e //p afore >after 2>&1 || true
    diff after xpect
    $(sed) -e /hello/!d -e //p -n afore >after
    diff afore after
    $(sed) -e 1!d -e //p -n afore >after # This looks wrong! WFB 20040730
    diff afore after
    $(sed) -n -e $$p afore # This failed before 20040730. WFB
    diff afore after
    >>afore echo world
    $(sed) -n -e "1,2 p" afore > after 2>&1
    diff afore after
    $(sed) -n -e 1;2p afore > after 2>&1
    diff afore after
    $(sed) -n -e 1;p afore > after 2>&1 || true
    > xpect echo sed: garbled address 1;p
    diff after xpect
    > xpect echo hello
    $(sed) -n -e " $$ ! p" afore > after 2>&1
    diff after xpect

TRAIL: .symbolic
    > afore echo hello
    > xpect echo sed: command "pq" has trailing garbage
    $(sed) -e pq afore > after 2>&1 || true
    diff after xpect
    $(sed) -n -e /foo/d;p afore > after 2>&1
    diff afore after
    $(sed) -n -e "/foo/d;p$# This is a comment" afore > after 2>&1
    diff afore after

cmdcomp: .symbolic brace equals q label branch airc D s l wW y
    > afore echo hello # Quick test of all commands
    $(sed) -e q;{p;};=;:label -e b -e t -e T -e a;i;r;c;D;s/a/a/l;w;W;y/a/b/ afore > after 2>&1
    diff afore after

brace: .symbolic
    > afore echo hello
    >>afore echo world
    $(sed) -n {p;} afore > after 2>&1
    diff afore after
    $(sed) -n -e { -e p -e } afore > after 2>&1
    diff afore after
    > xpect echo hello
    $(sed) -n 1{p;} afore > after 2>&1
    diff after xpect
    > xpect echo world
    $(sed) -n 1!{p;} afore > after 2>&1
    diff after xpect
    > xpect echo sed: no addresses allowed for 1}
    $(sed) -n 1} afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: too many }'s
    $(sed) -n } afore > after 2>&1 || true
    diff after xpect

equals: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo 1
    >>xpect echo 2
    $(sed) -n /h/,/l/= afore > after 2>&1 || true
    diff after xpect

q: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo sed: only one address allowed for /h/,/l/q
    $(sed) /h/,/l/q afore > after 2>&1 || true
    diff after xpect
    > xpect echo hello
    $(sed) /h/q afore > after 2>&1 || true
    diff after xpect

label: .symbolic
    > afore echo hello
    > xpect echo sed: no addresses allowed for 1:label
    $(sed) 1:label nul > after 2>&1 || true
    diff after xpect
    > xpect echo sed: duplicate label :label
    $(sed) -e :label -e :label nul > after 2>&1 || true
    diff after xpect
    $(sed) :label afore > after 2>&1 || true
    diff afore after

branch: .symbolic
    > afore echo hello
    $(sed) -n -e "/d/ blabel" -e p -e ":label" afore > after 2>&1
    diff afore after
    $(sed) -e "/d/ ! blabel" -e p -e ":label" afore > after 2>&1
    diff afore after

airc: .symbolic
    > afore echo hello
    > xpect echo sed: only one address allowed for 1,2a
    # 1addr on air seems restrictive but complies with SUSV3 WFB 20040731
    $(sed) 1,2a nul > after 2>&1 || true
    diff after xpect
    > xpect echo sed: only one address allowed for 1,2i
    $(sed) 1,2i nul > after 2>&1 || true
    diff after xpect
    > xpect echo sed: only one address allowed for 1,2r
    $(sed) 1,2r nul > after 2>&1 || true
    diff after xpect
    # Impossibly? difficult to do multi-line command portably WFB 20040731
    > scrip echo c\
    >>scrip echo world
    > xpect echo world
    $(sed) -f scrip afore > after 2>&1
    diff after xpect

D: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo world
    $(sed) N;D afore > after 2>&1
    diff after xpect

s: .symbolic
    > afore echo hello
    > xpect echo sed: garbled command s/\)//
    $(sed) s/\)// afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: garbled command s/a/\1/
    $(sed) s/a/\1/ afore > after 2>&1 || true
    diff after xpect
    > xpect echo heLLo
    $(sed) -g s/l/L/ afore > after 2>&1
    diff after xpect
    $(sed) s/l/L/g afore > after 2>&1
    diff after xpect
    >>afore echo world
    > xpect echo heLlo
    $(sed) -n N;s/l/L/P afore > after 2>&1
    diff after xpect
    >>xpect echo world
    $(sed) -n N;s/l/L/p afore > after 2>&1
    diff after xpect
    > xpect echo sed: cannot create after
    # This fails due to share conflicts between stdout redirection and file WFB 20040803
    $(sed) -n N;s/l/L/pwafter afore > after 2>&1 || true
    diff after xpect
    > xpect echo heLlo
    >>xpect echo world
    >>xpect echo heLlo
    >>xpect echo world
    $(sed) -n N;s/l/L/pw afore > after 2>&1
    diff after xpect
    # Seems strange that s supports w flag but not W WFB 20040803
    > xpect echo sed: command "N;s/l/L/W" has trailing garbage
    $(sed) -n N;s/l/L/W afore > after 2>&1 || true
    diff after xpect
    > xpect echo heLlo
    >>xpect echo heLlo
    >>xpect echo world
    $(sed) -n N;s/l/L/Pw afore > after 2>&1
    diff after xpect

l: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo hello\nworld
    $(sed) -n N;l afore > after 2>&1
    diff after xpect
    $(sed) -n N;lwafter afore
    diff after xpect

wW: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo sed: too many w files
    $(sed) -n -e w -e wa -e wb -e wc -e wd -e we -e wf -e wg -e wh -e wi -e Wj afore > after 2>&1 || true
    diff after xpect
    > xpect echo hello
    $(sed) -n N;Wafter afore
    diff after xpect
    >>xpect echo world
    $(sed) -n N;wafter afore
    diff after xpect

y: .symbolic
    > afore echo hello
    > xpect echo sed: garbled command y/abcdefghijklmnopqrstuvwxyz/A-Z/
    $(sed) y/abcdefghijklmnopqrstuvwxyz/A-Z/ afore > after 2>&1 || true
    diff after xpect
    > xpect echo HELLO
    $(sed) y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/ afore > after 2>&1
    diff after xpect

rhscomp: .symbolic
    > afore echo hello
    > xpect echo sed: garbled command s/l/\1/
    $(sed) s/l/\1/ afore > after 2>&1 || true
    diff after xpect
    > xpect echo helo
    $(sed) s/\(l\)\1/\1/ afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: garbled command s/l/l
    $(sed) s/l/l afore > after 2>&1 || true
    diff after xpect

recomp: .symbolic starplusdol set
    > afore echo hello
    > xpect echo heLo
    $(sed) /ll*/s//L/ afore > after 2>&1
    diff after xpect
    $(sed) /l+/s//L/ afore > after 2>&1
    diff after xpect
    > xpect echo ello
    $(sed) s/^h// afore > after 2>&1
    diff after xpect
    > xpect echo sed: garbled command s/\(h//
    $(sed) s/\(h// afore > after 2>&1 || true
    diff after xpect
    > afore echo hello world
    > xpect echo row ollehld
    $(sed) s/\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)/\9\8\7\6\5\4\3\2\1/ afore > after
    diff after xpect
    > xpect echo sed: garbled command s/\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)/\9\8\7\6\5\4\3\2\1/
    $(sed) s/\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)/\9\8\7\6\5\4\3\2\1/ afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: garbled command s/\(.\)\)//
    $(sed) s/\(.\)\)// afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: garbled command s/\(.\)/\2/
    $(sed) s/\(.\)/\2/ afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: garbled command s/h\
    >>xpect echo ello//
    > scrip echo s/h\
    >>scrip echo ello//
    $(sed) -f scrip afore > after 2>&1 || true
    diff after xpect
    > afore echo hello
    >>afore echo wor	ld # There is an intentional tab character on this line WFB 20040801
    > xpect echo hello world
    $(sed) "N;s/o\nw/o w/;s/\t//" afore > after 2>&1
    diff after xpect
    > xpect echo sed: garbled address /h
    > scrip echo /h
    $(sed) -f scrip afore > after 2>&1 || true
    diff after xpect
    > afore echo hello
    >>afore echo world
    $(sed) "N;s/o.w/o w/" afore > after 2>&1
    diff afore after

starplusdol: .symbolic
    > afore echo ++aa
    > xpect echo hello world
    $(sed) "s/++a+/hello world/" afore > after 2>&1
    diff after xpect
    > xpect echo sed: garbled command s/\(+a+/hello world/
    $(sed) "s/\(+a+/hello world/" afore > after 2>&1 || true
    diff after xpect
    > afore echo **
    > xpect echo hello world
    $(sed) "s/**a*/hello world/" afore > after 2>&1
    diff after xpect
    > xpect echo sed: garbled command s/\(*a+/hello world/
    $(sed) "s/\(*a+/hello world/" afore > after 2>&1 || true
    diff after xpect
    > xpect echo *hello world
    $(sed) "s/*$$/hello world/" afore > after 2>&1
    diff after xpect

set: .symbolic
    > afore echo hello
    > xpect echo haaaa
    $(sed) "s/[^h]/a/g" afore > after 2>&1
    diff after xpect
    > xpect echo sed: garbled command /[h
    $(sed) /[h afore > after 2>&1 || true
    > afore echo h-llo
    > xpect echo haaao
    $(sed) s/[-l]/a/g afore > after 2>&1
    diff after xpect
    > afore echo hello
    > xpect echo aaaao
    $(sed) s/[e-l]/a/g afore > after 2>&1
    diff after xpect
    > afore echo hello
    >>afore echo w\r	ld # There is an intentional tab character on this line WFB 20040802
    > xpect echo halloawarald
    $(sed) N;s/[\e\n\t]/a/g afore > after 2>&1
    diff after xpect
    > afore echo h]llo
    > xpect echo a]aaa
    $(sed) "s/[^]]/a/g" afore > after 2>&1
    diff after xpect

cmdline: .symbolic
    > afore echo hello
    > xpect echo sed: error processing: -e
    $(sed) -e < afore > after 2>&1 || true
    diff after xpect
    # This used to seize. WFB 20040803
    $(sed) -e s/h/b\ afore > after 2>&1 || true
    > xpect echo sed: error processing: s/h/b\
    diff after xpect

address: .symbolic
    > afore echo hello
    $(sed) -n $$p afore > after 2>&1
    diff afore after
    $(sed) -n $$p afore afore > after 2>&1
    diff afore after
    $(sed) -n /e/p afore > after 2>&1
    diff afore after
    $(sed) -n 1p afore > after 2>&1
    diff afore after
    $(sed) -n p afore > after 2>&1
    diff afore after

gettext: .symbolic
    > afore echo hello
    > scrip echo a\
    >>scrip echo     world
    > xpect echo hello
    >>xpect echo world
    $(sed) -f scrip afore > after 2>&1
    diff after xpect
    > scrip echo a\
    >>scrip echo     w\or \\ ld\
    >>scrip echo universe
    > xpect echo hello
    >>xpect echo wor \ ld
    >>xpect echo universe
    $(sed) -f scrip afore > after 2>&1
    diff after xpect

resolve: .symbolic
    > afore echo hello
    > xpect echo sed: undefined label label
    $(sed) blabel  afore > after 2>&1 || true
    diff after xpect
    $(sed) -e blabel -e :label afore > after 2>&1
    diff afore after

ycomp: .symbolic
    > afore echo he\lo
    > scrip echo y/h
    > xpect echo sed: garbled command y/h
    $(sed) -f scrip afore > after 2>&1 || true
    diff after xpect
    > xpect echo abclo
    $(sed) y/h\e\\/abc/ afore > after 2>&1
    diff after xpect
    > afore echo hello
    >>afore echo world
    > xpect echo hello world
    $(sed) "N;y/\n/ /" afore > after 2>&1
    diff after xpect
    > afore echo hello
    > xpect echo sed: garbled command y/a//
    $(sed) y/a// afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: garbled command y/a/ 
    $(sed) y/a/ afore > after 2>&1 || true
    diff after xpect
    > xpect echo sed: garbled command y/a/b
    $(sed) y/a/b afore > after 2>&1 || true
    diff after xpect

sedexec: .symbolic execute selected match advance substitute dosub place listto command getline memeql readout
    @%null

execute: .symbolic
    > afore echo hello
    > xpect echo sed: can't open nosuch
    >>xpect echo hello
    $(sed) -n p nosuch afore > after 2>&1 || true
    diff after xpect
    > xpect echo hello hello
    $(sed) -n "N;$$s/\n/ /p" afore afore > after 2>&1
    diff after xpect
    >>afore echo world
    > xpect echo world
    $(sed) N;D afore > after 2>&1
    diff after xpect
    $(sed) -e b -e p afore > after 2>&1
    diff afore after
    $(sed) -e blab -e p;:lab afore > after 2>&1
    diff afore after
    > afore echo hello
    > scrip echo a\
    >>scrip echo world
    > xpect echo hello
    >>xpect echo world
    $(sed) -f scrip afore > after 2>&1
    diff after xpect

selected: .symbolic
    > afore echo hello
    $(sed) -n 1{p;} afore > after 2>&1
    diff afore after
    >>afore echo world
    > xpect echo world
    $(sed) -n 1!{p;} afore > after 2>&1
    diff after xpect
    $(sed) -n 2,$$p afore > after 2>&1
    diff after xpect
    >>afore echo universe
    $(sed) -n 2,2p afore > after 2>&1
    diff after xpect
    $(sed) -n /world/,2p afore > after 2>&1
    diff after xpect

match: .symbolic
    > afore echo hello
    > xpect echo heLLo
    $(sed) -g s/l/L/ afore > after 2>&1
    diff after xpect

advance: .symbolic
    > afore echo hello
    > xpect echo Hello
    $(sed) s/h/H/ afore > after 2>&1    # CCHR
    diff after xpect
    $(sed) s/./H/ afore > after 2>&1    # CDOT
    diff after xpect
    >>afore echo world
    > xpect echo HHHHHnHHHHH
    $(sed) N;s/./H/g;s/\n/n/ afore > after 2>&1 # CDOT, CDOT | STAR
    diff after xpect
    > afore echo heho
    >>afore echo horld
    > xpect echo Hehonhorld
    $(sed) "N;s/^h/H/g;s/\n/n/" afore > after 2>&1 # CNL
    diff after xpect
    > afore echo delld
    >>afore echo hodld
    > xpect echo delldnhodlD
    $(sed) N;s/d$$/D/g;s/\n/n/ afore > after 2>&1 # CDOL
    diff after xpect
    > afore echo hello
    > xpect echo hellO
    $(sed) s/o/O/ afore > after 2>&1    # CEOF
    diff after xpect
    $(sed) s/[o][o]*/O/ afore > after 2>&1  # CCL, CCL | STAR, star
    diff after xpect
    $(sed) s/\(o\)/O/ afore > after 2>&1 # CBRA, CKET
    diff after xpect
    > xpect echo helo
    $(sed) s/\(l\)\1/\1/ afore > after 2>&1 # CBACK
    diff after xpect
    $(sed) s/\(o\)\1/\1/ afore > after 2>&1 # CBACK
    diff afore after
    $(sed) s/\(l\)\1/\1/ afore > after 2>&1 # CBACK | STAR
    diff after xpect

substitute: .symbolic
    > afore echo hello
    $(sed) s/a/b/ afore > after 2>&1
    diff afore after
    > xpect echo heLLo
    $(sed) -g s/l/L/ afore > after 2>&1
    diff after xpect

dosub: .symbolic
    > afore echo hello
    > xpect echo hellooa
    $(sed) "s/.*\(.\)/&\1a/" afore > after 2>&1
    diff after xpect

place: .symbolic # already tested by dosub
    @%null

listto: .symbolic
    # BS gets "Illegal character value 08H in file" from wmake
    # ESC gets "Illegal character value 1bH in file" from wmake
    # CR completes the command list
    > afore echo hello TAB	NL # There is an intentional tab character on this line WFB 20040801
    >>afore echo line end
    > xpect echo hello TAB\tNL\nline end
    $(sed) -n N;l afore > after 2>&1
    diff after xpect

command: .symbolic
    > afore echo hello
    > scrip echo a\
    >>scrip echo world
    > xpect echo hello
    >>xpect echo world
    $(sed) -f scrip afore > after 2>&1
    diff after xpect
    > scrip echo c\
    >>scrip echo world
    > xpect echo world
    $(sed) -f scrip afore > after 2>&1
    diff after xpect
    > afore echo hello
    >>afore echo world
    > xpect echo world
    $(sed) 1d afore > after 2>&1
    diff after xpect
    $(sed) N;D afore > after 2>&1
    diff after xpect
    > xpect echo 2
    $(sed) -n $$= afore > after 2>&1
    diff after xpect
    > xpect echo hello
    $(sed) -n 1h;2{g;p;} afore > after 2>&1
    diff after xpect
    > xpect echo world
    >>xpect echo hello
    $(sed) -n 1h;2{G;p;} afore > after 2>&1
    diff after xpect
    > xpect echo\
    >>xpect echo hello
    $(sed) -n 1H;2{g;p;} afore > after 2>&1
    diff after xpect
    > afore echo hello
    > scrip echo i\
    >>scrip echo world
    > xpect echo world
    >>xpect echo hello
    $(sed) -f scrip afore > after 2>&1
    diff after xpect
    $(sed) b afore > after 2>&1
    diff afore after
    $(sed) -n l afore > after 2>&1
    diff afore after
    >>afore echo world
    > scrip echo a\
    >>scrip echo universe
    > xpect echo hello
    >>xpect echo universe
    >>xpect echo world
    $(sed) -f scrip -e n afore > after 2>&1
    diff after xpect
    > xpect echo universe
    >>xpect echo hello
    >>xpect echo world
    $(sed) -f scrip -e N afore > after 2>&1
    diff after xpect
    > xpect echo hello
    $(sed) -n 1p  afore > after 2>&1
    diff after xpect
    $(sed) -n N;P  afore > after 2>&1
    diff after xpect
    $(sed) q afore > after 2>&1
    diff after xpect
    > xpect echo hello
    >>xpect echo hello
    >>xpect echo world
    >>xpect echo world
    $(sed) 1rafore afore > after 2>&1
    diff after xpect
    > xpect echo Hello
    >>xpect echo world
    $(sed) s/h/H/ afore > after 2>&1
    diff after xpect
    $(sed) -n N;s/h/H/p afore > after 2>&1
    diff after xpect
    > xpect echo Hello
    $(sed) -n N;s/h/H/P afore > after 2>&1
    diff after xpect
    $(sed) -n s/h/H/wafter afore
    diff after xpect
    $(sed) -n -e s/h/H/;T -e p afore > after 2>&1
    diff after xpect
    $(sed) -n -e s/h/H/;t -e p afore > after 2>&1
    > xpect echo world
    diff after xpect
    > xpect echo hello
    $(sed) -n N;W afore > after 2>&1
    diff after xpect
    >>xpect echo world
    $(sed) -n N;w afore > after 2>&1
    diff after xpect
    > xpect echo hello
    $(sed) -n 1h;2x;$$p afore > after 2>&1
    diff after xpect
    > xpect echo heLLo
    >>xpect echo worLd
    $(sed) y/l/L/ afore > after 2>&1
    diff after xpect

getline: .symbolic # already deemed tested WFB 20040802
    @%null

memeql: .symbolic # already deemed tested WFB 20040802
    @%null

readout: .symbolic # already deemed tested WFB 20040802
    @%null
