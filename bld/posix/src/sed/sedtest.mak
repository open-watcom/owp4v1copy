# testsed.mak
#
# Makefile to test sed

!ifdef %testsed
sed = $(%testsed)
!else
sed = sed
!endif

!include cproj.mif

all: .symbolic sedcomp sedexec spencer.bat susv3 bre
    .\spencer.bat 2>&1 | tee spencer.log
    @$(sed) -n "1s/.*/spencer.bat failed!/p" spencer.log

sedcomp: .symbolic main
    @%null

sedexec: .symbolic execute
    @%null

clean: .symbolic
    rm -f afore after xpect scrip scr0p scr1p
    rm -f spencer.bat spencer.log spen?.??? listto.* a b c d e f g h i written

main: .symbolic noargs badarg enarg fgarg noef badsquig labels resolve compile recomp
    @%null

# I am unconvinced this should succeed. Bad in SUSV3. GNU sed 3.02 outputs
# help and fails. WFB 20040730
# Changed to fail with usage message. WFB 20040901
noargs: .symbolic
    $(sed) 2> after || true

badarg: .symbolic
    > afore echo hello
    > xpect echo sed: error processing: -e
    type afore | $(sed) -e 2> after || true
    diff after xpect
    > xpect echo sed: error processing: -f
    type afore | $(sed) -f 2> after || true
    diff after xpect
    > xpect echo sed: unknown flag ?
    >>xpect echo Usage: sed [-g] [-n] script file ...
    >>xpect echo        sed [-g] [-n] -e script ... -f script_file ... file ...
    >>xpect echo.
    >>xpect echo   -g: global substitute             -n: no default copy,
    >>xpect echo   script: append to commands,       script_file: append contents to commands
    >>xpect echo   file: a text file - lines of upto 8K bytes, \n not needed on last line
    >>xpect echo         default is to read from standard input
    >>xpect echo.
    >>xpect echo General command format:
    >>xpect echo     [addr1][,addr2] [!]function [arguments]
    >>xpect echo     [addr1][,addr2] [!]s/[RE]/replace/[p^|P][g][w file]
    >>xpect echo.
    >>xpect echo Command list:
    >>xpect echo     a: append,      b: branch,    c: change,   d/D: delete,
    >>xpect echo     g/G: get,       h/H: hold,    i: insert,   l: list,
    >>xpect echo     n/N: next,      p/P: print,   q: quit,     r: read,
    >>xpect echo     s: substitute,  t/T: test,    w/W: write,  x: xchange,
    >>xpect echo     y: translate,   :label,       =: number,   {}: grouping
    >>xpect echo.
    >>xpect echo Regular expression elements:
    >>xpect echo     \n, and \t: newline, and tab
    >>xpect echo     ^^: line begin,             $$: line end,
    >>xpect echo     [a-z$$0-9]: class,          [^^a-z$$]: inverse-class,
    >>xpect echo     .: one character,          *: 0 or more repeats,
    >>xpect echo     +: one or more repeats,    \{n,m\} n to m repeats,
    >>xpect echo     \(..\): subexpression,     \1..\9: matched subexpression,
    >>xpect echo.
    >>xpect echo Substitution patterns:
    >>xpect echo     \n,\t: newline and tab
    >>xpect echo     ^&: match pattern,          \1..\9: matched subexpression
    type afore | $(sed) -? 2> after || true
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
    $(sed) -e "" afore > after
    diff afore after

badsquig: .symbolic
    > afore echo hello
    > xpect echo sed: too many }'s
    $(sed) -e "}" afore 2> after || true
    diff after xpect
    > xpect echo sed: too many {'s
    $(sed) -e "{p" afore 2> after || true
    diff after xpect

labels: .symbolic
    # labels can be unused or duplicated.
    # One can jump forward to a defined label, forward to the end of the script,
    # forward to a missing label or backward.
    # unused
    > afore echo hello
    $(sed) -e :label -e p -n afore > after
    diff afore after
    # duplicated
    > xpect echo sed: duplicate label ":label"
    $(sed) -e :label -e :label afore 2> after || true
    diff after xpect
    # forward to a defined label
    $(sed) -neblabel -ep;:label -ep afore > after
    diff afore after
    # forward to the end of the script
    $(sed) -eb -ep afore > after
    diff afore after
    # forward to a missing label
    > xpect echo sed: undefined label ":label"
    $(sed) -eblabel afore 2> after || true
    diff after xpect
    # backward
    $(sed) -eblabel1 -e:label -eq;:label1 -eblabel afore > after
    diff afore after

compile: .symbolic semicolon AGMSG emptyre TRAIL cmdcomp cmdline address
    @%null

semicolon: .symbolic
    > afore echo hello
    $(sed) -e "$# This is a comment" -e "; p" -n afore > after
    diff afore after

AGMSG: .symbolic
    > afore echo hello
    > xpect echo sed: garbled address /\)/p
    $(sed) -e /\)/p afore 2> after || true
    diff after xpect

emptyre: .symbolic
    > afore echo hello
    > xpect echo sed: first RE must be non-null
    $(sed) -e //p afore 2> after || true
    diff after xpect
    $(sed) -e /hello/!d -e //p -n afore > after
    diff afore after
    $(sed) -e 1!d -e //p -n afore 2> after || true
    diff after xpect
    # last regular expression is dynamic rather than analysable at compile time
    $(sed) -n -e b1 -e:0 -e //p;q;:1 -e /h/b0 afore > after
    diff afore after
    $(sed) -n -e $$p afore > after # This failed before 20040730. WFB
    diff afore after
    >>afore echo world
    $(sed) -n -e "1,2 p" afore > after
    diff afore after
    # Curiously, this scenario should not give a diagnostic WFB 20040815
    $(sed) -n -e "2,1 p" afore > after
    > xpect echo world
    diff after xpect
    $(sed) -n -e 1;2p afore > after
    diff afore after
    > xpect echo sed: garbled address 1;p
    $(sed) -n -e 1;p afore 2> after || true
    diff after xpect
    > xpect echo hello
    $(sed) -n -e " $$ ! p" afore > after
    diff after xpect

TRAIL: .symbolic
    > afore echo hello
    > xpect echo sed: command "pq" has trailing garbage
    $(sed) -e pq afore 2> after || true
    diff after xpect
    $(sed) -n -e /foo/d;p afore > after
    diff afore after
    $(sed) -n -e "/foo/d;p$# This is a comment" afore > after
    diff afore after

cmdcomp: .symbolic brace equals q label branch airc D s l wW y rhscomp gettext ycomp
    > afore echo hello # Quick test of all commands
    $(sed) -e q;{p;};=;:label -e b -e t -e T -e a;i;r;c;D;s/a/a/l;w;W;y/a/b/ afore > after
    diff afore after

brace: .symbolic
    > afore echo hello
    >>afore echo world
    $(sed) -n {p;} afore > after
    diff afore after
    $(sed) -n -e { -e p -e } afore > after
    diff afore after
    > xpect echo hello
    $(sed) -n 1{p;} afore > after
    diff after xpect
    $(sed) -n 1,2!{p;};p afore | diff afore -
    > xpect echo world
    $(sed) -n 1!{p;} afore > after
    diff after xpect
    > xpect echo sed: no addresses allowed for 1}
    $(sed) -n 1} afore 2> after || true
    diff after xpect
    > xpect echo sed: too many }'s
    $(sed) -n } afore 2> after || true
    diff after xpect

equals: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo 1
    >>xpect echo 2
    $(sed) -n /h/,/l/= afore > after
    diff after xpect

q: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo sed: only one address allowed for /h/,/l/q
    $(sed) /h/,/l/q afore 2> after || true
    diff after xpect
    > xpect echo hello
    $(sed) /h/q afore > after
    diff after xpect

label: .symbolic
    > afore echo hello
    > xpect echo sed: no addresses allowed for 1:label
    $(sed) 1:label nul 2> after || true
    diff after xpect
    > xpect echo sed: duplicate label ":label"
    $(sed) -e :label -e :label nul 2> after || true
    diff after xpect
    $(sed) :label afore > after
    diff afore after

branch: .symbolic
    > afore echo hello
    $(sed) -n -e "/d/ blabel" -e p -e ":label" afore > after
    diff afore after
    $(sed) -e "/d/ ! blabel" -e p -e ":label" afore > after
    diff afore after

airc: .symbolic
    > afore echo hello
    > xpect echo sed: only one address allowed for 1,2a
    # 1addr on air seems restrictive but complies with SUSV3 WFB 20040731
    $(sed) 1,2a nul 2> after || true
    diff after xpect
    > xpect echo sed: only one address allowed for 1,2i
    $(sed) 1,2i nul 2> after || true
    diff after xpect
    > xpect echo sed: only one address allowed for 1,2r
    $(sed) 1,2r nul 2> after || true
    diff after xpect
    # Impossibly? difficult to do multi-line -e command portably WFB 20040731
    > scrip echo c\
    >>scrip echo world
    > xpect echo world
    $(sed) -f scrip afore > after
    diff after xpect

D: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo world
    $(sed) N;D afore > after
    diff after xpect

s: .symbolic
    > afore echo hello
    > xpect echo sed: garbled command s/\)//
    $(sed) s/\)// afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command s/a/\1/
    $(sed) s/a/\1/ afore 2> after || true
    diff after xpect
    > xpect echo heLLo
    $(sed) -g s/l/L/ afore > after
    diff after xpect
    $(sed) s/l/L/g afore > after
    diff after xpect
    >>afore echo world
    > xpect echo heLlo
    $(sed) -n N;s/l/L/P afore > after
    diff after xpect
    >>xpect echo world
    $(sed) -n N;s/l/L/p afore > after
    diff after xpect
    > xpect echo sed: cannot create after
    # This fails due to share conflicts between stdout redirection and file WFB 20040803
    $(sed) -n N;s/l/L/pwafter afore 2> after || true
    diff after xpect
    > xpect echo heLlo
    >>xpect echo world
    >>xpect echo heLlo
    >>xpect echo world
    $(sed) -n N;s/l/L/pw afore > after
    diff after xpect
    # Seems strange that s supports w flag but not W WFB 20040803
    > xpect echo sed: command "N;s/l/L/W" has trailing garbage
    $(sed) -n N;s/l/L/W afore 2> after || true
    diff after xpect
    > xpect echo heLlo
    >>xpect echo heLlo
    >>xpect echo world
    $(sed) -n N;s/l/L/Pw afore > after
    diff after xpect
    > xpect echo helLo
    $(sed) -n s/l/L/2p afore > after
    diff after xpect
    > xpect echo sed: bad value for match count on s command s/l/L/0
    $(sed) s/l/L/0 afore 2> after || true
    diff after xpect
    > xpect echo sed: bad value for match count on s command s/l/L/2048
    $(sed) s/l/L/2047 afore > after
    diff afore after
    $(sed) s/l/L/2048 afore 2> after || true
    diff after xpect

l: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo hello
    >>xpect echo world$$
    $(sed) -n N;l afore > after
    diff after xpect
    $(sed) -n N;lwafter afore
    diff after xpect

wW: .symbolic
    > afore echo hello
    >>afore echo world
    > xpect echo sed: too many w files
    rm -f a b c d e f g h i
    $(sed) -n -e w -e wa -e wb -e wc -e wd -e we -e wf -e wg -e wh -e wi -e Wj afore 2> after || true
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
    $(sed) y/abcdefghijklmnopqrstuvwxyz/A-Z/ afore 2> after || true
    diff after xpect
    > xpect echo HELLO
    $(sed) y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/ afore > after
    diff after xpect

rhscomp: .symbolic
    > afore echo hello
    > xpect echo sed: garbled command s/l/\1/
    $(sed) s/l/\1/ afore 2> after || true
    diff after xpect
    > xpect echo helo
    $(sed) s/\(l\)\1/\1/ afore > after
    diff after xpect
    > xpect echo sed: garbled command s/l/l
    $(sed) s/l/l afore 2> after || true
    diff after xpect

recomp: .symbolic starplusdol set handle_cket
    > afore echo hello
    > xpect echo heLo
    $(sed) /ll*/s//L/ afore > after
    diff after xpect
    $(sed) /l+/s//L/ afore > after
    diff after xpect
    > xpect echo ello
    $(sed) s/^h// afore > after
    diff after xpect
    > xpect echo sed: garbled command s/\(h//
    $(sed) s/\(h// afore 2> after || true
    diff after xpect
    > afore echo hello world
    > xpect echo row ollehld
    $(sed) s/\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)/\9\8\7\6\5\4\3\2\1/ afore > after
    diff after xpect
    > xpect echo row ollehd
    $(sed) s/\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)\(.\)/\9\8\7\6\5\4\3\2\1/ afore > after
    diff after xpect
    > xpect echo sed: garbled command s/\(.\)\)//
    $(sed) s/\(.\)\)// afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command s/\(.\)/\2/
    $(sed) s/\(.\)/\2/ afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled address /\{1,2\}/p
    $(sed) /\{1,2\}/p afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled address /.\{,2\}/p
    $(sed) /.\{,2\}/p afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled address /.\{256\}/p
    $(sed) /.\{256\}/p afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled address /.\{1,256\}/p
    $(sed) /.\{1,256\}/p afore 2> after || true
    diff after xpect
    > xpect echo hello world
    $(sed) /.\{255\}!/p afore > after
    diff afore after
    $(sed) /.\{255,\}!/p afore > after
    diff afore after
    > xpect echo sed: garbled address /.\{255,2a\}!/p
    $(sed) /.\{255,2a\}!/p afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled address /.\{2,1\}/p
    $(sed) /.\{2,1\}/p afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled address /.\{2a\}/p
    $(sed) /.\{2a\}/p afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command s/h\
    >>xpect echo ello//
    > scrip echo s/h\
    >>scrip echo ello//
    $(sed) -f scrip afore 2> after || true
    diff after xpect
    > afore echo hello
    >>afore echo wor	ld # There is an intentional tab character on this line WFB 20040801
    > xpect echo hello world
    $(sed) "N;s/o\nw/o w/;s/\t//" afore > after
    diff after xpect
    > xpect echo sed: garbled address /h
    > scrip echo /h
    $(sed) -f scrip afore 2> after || true
    diff after xpect
    > afore echo hello
    >>afore echo world
    > xpect echo hello world
    $(sed) "N;s/o.w/o w/" afore > after
    diff after xpect
    > afore echo \hello
    > xpect echo sed: garbled address /\h/p
    $(sed) -n /\h/p afore > after
    diff afore after # \<non-special> is erroneous in IEEE, <non-special> here
    $(sed) -n /\\h/p afore > after
    diff afore after

starplusdol: .symbolic
    > afore echo ++aa
    > xpect echo hello world
    $(sed) "s/++a+/hello world/" afore > after
    diff after xpect
    > xpect echo sed: garbled command s/\(+a+/hello world/
    $(sed) "s/\(+a+/hello world/" afore 2> after || true
    diff after xpect
    > afore echo **
    > xpect echo hello world
    $(sed) "s/**a*/hello world/" afore > after
    diff after xpect
    > xpect echo sed: garbled command s/\(*a+/hello world/
    $(sed) "s/\(*a+/hello world/" afore 2> after || true
    diff after xpect
    > xpect echo *hello world
    $(sed) "s/*$$/hello world/" afore > after
    diff after xpect

set: .symbolic
    > afore echo hello
    > xpect echo haaaa
    $(sed) "s/[^h]/a/g" afore > after
    diff after xpect
    > xpect echo sed: garbled command /[h
    $(sed) /[h afore 2> after || true
    diff after xpect
    > afore echo h-llo
    > xpect echo haaao
    $(sed) s/[-l]/a/g afore > after
    diff after xpect
    > afore echo hello
    > xpect echo aaaao
    $(sed) s/[e-l]/a/g afore > after
    diff after xpect
    > xpect echo sed: garbled command s/[l-e]/a/g
    $(sed) s/[l-e]/a/g afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command /[e-l-o]/p
    $(sed) -n /[e-l-o]/p afore 2> after || true
    diff after xpect
    > afore echo hello
    >>afore echo w\r	ld # There is an intentional tab character on this line WFB 20040802
    > xpect echo halloawarald
    $(sed) N;s/[\e\n\t]/a/g afore > after
    diff after xpect
    > afore echo h]llo
    > xpect echo a]aaa
    $(sed) "s/[^]]/a/g" afore > after
    diff after xpect
    $(sed) -n /[[:alpha:]]/p afore > after
    diff afore after
    > xpect echo sed: invalid character class name /[[:foo:]]/p
    $(sed) /[[:foo:]]/p afore 2> after || true
    diff after xpect

handle_cket: .symbolic
    > afore echo hello
    $(sed) -n /a*/p afore > after # CCHR
    diff afore after
    $(sed) -n /.*/p afore > after # CDOT
    diff afore after
    $(sed) -n /[aeiou]*/p afore > after # CCL
    diff afore after
    $(sed) -n /^*/!p afore > after # * is literal after special ^
    diff afore after
    $(sed) -n /$$*/p afore > after # $ is literal if not last
    diff afore after
    > xpect echo sed: garbled address /\(*\)/p
    $(sed) -n /\(*\)/p afore 2> after || true # CBRA
    diff after xpect
    $(sed) -n /\([aeiou]\)*/p afore > after # CKET
    diff afore after
    $(sed) -n /\([aeiou]\)\1*/p afore > after # CBACK
    diff afore after
    > xpect echo sed: no such command as 1*p
    $(sed) -n 1*p afore 2> after || true # * after line number is not special
    diff after xpect
    # By definition, CEND* can't happen CEND is always followed by CEOF
    # By definition, CEOF* can't happen

cmdline: .symbolic
    > afore echo hello
    > xpect echo sed: error processing: -e
    $(sed) -e < afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command s/h/b\
    >>xpect echo afore
    # This used to seize. WFB 20040803
    $(sed) -e s/h/b\ afore 2> after || true
    diff after xpect
    # This used to seize. WFB 20040813
    > xpect echo bello
    $(sed) -f << afore > after
s/h/b/
<<
    diff after xpect

address: .symbolic
    > afore echo hello
    $(sed) -n $$p afore > after
    diff afore after
    $(sed) -n $$p afore afore > after
    diff afore after
    $(sed) -n \hehp afore > after
    diff afore after
    $(sed) -n /e/p afore > after
    diff afore after
    $(sed) -n 1p afore > after
    diff afore after
    $(sed) -n p afore > after
    diff afore after

gettext: .symbolic
    > afore echo hello
    > scrip echo a\
    >>scrip echo     world
    > xpect echo hello
    >>xpect echo world
    $(sed) -f scrip afore > after
    diff after xpect
    > scrip echo a\
    >>scrip echo     w\or \\ ld\
    >>scrip echo universe
    > xpect echo hello
    >>xpect echo wor \ ld
    >>xpect echo universe
    $(sed) -f scrip afore > after
    diff after xpect

resolve: .symbolic
    > afore echo hello
    > xpect echo sed: undefined label ":label"
    $(sed) blabel  afore 2> after || true
    diff after xpect
    $(sed) -e blabel -e :label afore > after
    diff afore after

ycomp: .symbolic
    > afore echo he\lo
    > scrip echo y/h
    > xpect echo sed: garbled command y/h
    $(sed) -f scrip afore 2> after || true
    diff after xpect
    > xpect echo abclo
    $(sed) y/he\\/abc/ afore > after
    diff after xpect
    > afore echo hello
    >>afore echo world
    > xpect echo hello world
    # "If a backslash followed by an 'n' appear in string1 or string2,
    # the two characters shall be handled as a single <newline>."
    $(sed) "N;y/\n/ /" afore > after
    diff after xpect
    > afore echo hello
    > xpect echo h
    >>xpect echo llo
    $(sed) y/e/\n/ afore > after
    diff after xpect
    # "If the number of characters in string1 and string2 are not equal,
    # or if any of the characters in string1 appear more than once,
    # the results are undefined."
    > afore echo hello
    > xpect echo sed: garbled command y/a//
    $(sed) y/a// afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y//a/
    $(sed) y//a/ afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y/hh/el/
    $(sed) y/hh/el/ afore 2> after || true
    diff after xpect
    # Any character other than backslash or <newline>
    # can be used instead of slash to delimit the strings.
    > xpect echo HELLO
    $(sed) yaheloaHELOa afore > after
    diff after xpect
    > xpect echo sed: garbled command y\helo\HELO\
    >>xpect echo afore
    $(sed) y\helo\HELO\ afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y\\helo\\HELO\\
    $(sed) y\\helo\\HELO\\ afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y
    # "If the delimiter is not 'n' , within string1 and string2,
    # the delimiter itself can be used as a literal character
    # if it is preceded by a backslash."
    # [ "If the delimiter is not 'n'" because \n has a special meaning.]
    > xpect echo HELLh
    $(sed) yh\helohHEL\hh afore > after
    diff after xpect
    # "If a backslash character is immediately followed by a backslash
    # character in string1 or string2, the two backslash characters shall be
    # counted as a single literal backslash character. The meaning of a
    # backslash followed by any character that is not 'n' , a backslash, or the
    # delimiter character is undefined."
    >>afore echo w\rld
    > xpect echo h
    >>xpect echo llo wo\ld
    $(sed) "N;y/\\r\ne/o\\ \n/" afore > after
    diff after xpect
    > xpect echo sed: garbled command yh\h\?elohHEL\hh
    $(sed) yh\h\?elohHEL\hh afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command yh\helohH\?L\hh
    $(sed) yh\helohH\?L\hh afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y
    > scrip echo y
    >>scrip echo helo
    >>scrip echo HELO
    $(sed) -f scrip afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y/a/ 
    $(sed) y/a/ afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y/a/b
    $(sed) y/a/b afore 2> after || true
    diff after xpect
    > xpect echo sed: garbled command y/\h/\h/
    $(sed) y/\h/\h/ afore 2> after || true
    diff after xpect

execute: .symbolic selected command match
    > afore echo hello
    > xpect echo sed: can't open nosuch
    >>xpect echo hello
    $(sed) -n p nosuch afore > after 2>&1
    diff after xpect
    > xpect echo hello hello
    $(sed) -n "N;$$s/\n/ /p" afore afore > after
    diff after xpect
    >>afore echo world
    > xpect echo world
    $(sed) N;D afore > after
    diff after xpect
    $(sed) -e b -e p afore > after
    diff afore after
    $(sed) -e blab -e p;:lab afore > after
    diff afore after
    > afore echo hello
    > scrip echo a\
    >>scrip echo world
    > xpect echo hello
    >>xpect echo world
    $(sed) -f scrip afore > after
    diff after xpect

selected: .symbolic
    > afore echo hello
    $(sed) -n 1{p;} afore > after
    diff afore after
    >>afore echo world
    > xpect echo world
    $(sed) -n 1!{p;} afore > after
    diff after xpect
    $(sed) -n 2,$$p afore > after
    diff after xpect
    >>afore echo universe
    $(sed) -n 2,2p afore > after
    diff after xpect
    $(sed) -n /world/,2p afore > after
    diff after xpect

match: .symbolic advance
    > afore echo hello
    > xpect echo heLLo
    $(sed) -g s/l/L/ afore > after
    diff after xpect
    > xpect echo heLlo
    $(sed) s/l/L/ afore > after
    diff after xpect
    > xpect echo hellO
    $(sed) s/./O/5 afore > after
    diff after xpect

advance: .symbolic mtype memeql
    > afore echo hello
    > xpect echo Hello
    $(sed) s/h/H/ afore > after    # CCHR
    diff after xpect
    $(sed) s/./H/ afore > after    # CDOT
    diff after xpect
    >>afore echo world
    > xpect echo HHHHHHHHHHH
    $(sed) N;s/./H/g afore > after # CDOT, CDOT | STAR
    diff after xpect # . should match \n as well as other characters
    > afore echo heho
    >>afore echo horld
    > xpect echo Hehonhorld
    $(sed) "N;s/^h/H/g;s/\n/n/" afore > after # CNL
    diff after xpect
    > afore echo delld
    >>afore echo hodld
    > xpect echo delldnhodlD
    $(sed) N;s/d$$/D/g;s/\n/n/ afore > after # CDOL
    diff after xpect
    > afore echo hello
    > xpect echo hellO
    $(sed) s/o/O/ afore > after    # CEOF
    diff after xpect
    $(sed) s/[o][o]*/O/ afore > after  # CCL, CCL | STAR, star
    diff after xpect
    $(sed) s/\(o\)/O/ afore > after # CBRA, CKET
    diff after xpect
    > xpect echo helo
    $(sed) s/\(l\)\1/\1/ afore > after # CBACK
    diff after xpect
    $(sed) s/\(o\)\1/\1/ afore > after # CBACK
    diff afore after
    $(sed) s/\(l\)\1/\1/ afore > after # CBACK
    diff after xpect
    > afore echo abababg
    > xpect echo abg
    $(sed) s/\(..\)\1*/\1/ afore > after # CBACK | STAR
    diff after xpect
    $(sed) -n /\(c*\)\1*/p afore > after # CBACK | STAR - zero width \1
    diff afore after
    # \(...\)* sets \n to last match
    > afore echo abcdefg
    > xpect echo efg
    $(sed) s/\(..\)*/\1/ afore > after # CBRA | STAR, CKET | STAR
    diff after xpect
    $(sed) -n /\(foo\)*/p afore > after # CBRA | STAR, CKET | STAR
    diff afore after
    $(sed) -n /\(foo\)*\1*/p afore > after # CBRA | STAR, CKET | STAR
    diff afore after

mtype: .symbolic
    > afore echo aaaaaaa
    > xpect echo b
    $(sed) -n s/a\{0,\}/b/p afore > after # CCHR | MTYPE
    diff after xpect
    $(sed) -n s/a\{1,\}/b/p afore > after
    diff after xpect
    > xpect echo baaaaaa
    $(sed) -n s/a\{1,1\}/b/p afore > after
    diff after xpect
    > xpect echo baa
    $(sed) -n s/a\{3,5\}/b/p afore > after
    diff after xpect
    > xpect echo b
    $(sed) -n s/a\{3,8\}/b/p afore > after
    diff after xpect
    $(sed) -n /a\{8,\}/!p afore > after
    diff afore after
    > xpect echo baaaaaaa
    $(sed) -n s/b\{0,\}/b/p afore > after
    diff after xpect
    > afore echo abcdefg
    > xpect echo b
    $(sed) -n s/.\{0,\}/b/p afore > after # CDOT | MTYPE
    diff after xpect
    $(sed) -n s/.\{1,\}/b/p afore > after
    diff after xpect
    > xpect echo bbcdefg
    $(sed) -n s/.\{1,1\}/b/p afore > after
    diff after xpect
    > xpect echo bfg
    $(sed) -n s/.\{3,5\}/b/p afore > after
    diff after xpect
    > xpect echo b
    $(sed) -n s/.\{3,8\}/b/p afore > after
    diff after xpect
    $(sed) -n /.\{8,\}/!p afore > after
    diff afore after
    > xpect echo b
    $(sed) -n s/.\{0,\}/b/p afore > after
    diff after xpect
    > afore echo abcdefg
    > xpect echo b
    $(sed) -n s/[a-g]\{0,\}/b/p afore > after # CCL | MTYPE
    diff after xpect
    $(sed) -n s/[a-g]\{1,\}/b/p afore > after
    diff after xpect
    > xpect echo bbcdefg
    $(sed) -n s/[a-g]\{1,1\}/b/p afore > after
    diff after xpect
    > xpect echo bfg
    $(sed) -n s/[a-g]\{3,5\}/b/p afore > after
    diff after xpect
    > xpect echo b
    $(sed) -n s/[a-g]\{3,8\}/b/p afore > after
    diff after xpect
    $(sed) -n /[a-g]\{8,\}/!p afore > after
    diff afore after
    > xpect echo b
    $(sed) -n s/[a-g]\{0,\}/b/p afore > after
    diff after xpect
    > afore echo aaaaaaa
    > xpect echo b
    $(sed) -n s/\(.\)\1\{0,\}/b/p afore > after # CBACK | MTYPE
    diff after xpect
    $(sed) -n s/\(.\)\1\{1,\}/b/p afore > after
    diff after xpect
    > xpect echo baaaaa
    $(sed) -n s/\(.\)\1\{1,1\}/b/p afore > after
    diff after xpect
    > xpect echo ba
    $(sed) -n s/\(.\)\1\{3,5\}/b/p afore > after
    diff after xpect
    > xpect echo b
    $(sed) -n s/\(.\)\1\{3,8\}/b/p afore > after
    diff after xpect
    $(sed) -n /\(.\)\1\{8,\}/!p afore > after
    diff afore after
    > xpect echo b
    $(sed) -n s/\(.\)\1\{0,\}/b/p afore > after
    diff after xpect
    > afore echo aaaaaa
    > xpect echo b
    $(sed) -n s/\(.\)\{0,\}/b/p afore > after # CKET | MTYPE
    diff after xpect
    $(sed) -n s/\(.\)\{1,\}/b/p afore > after
    diff after xpect
    > xpect echo baaaaa
    $(sed) -n s/\(.\)\{1,1\}/b/p afore > after
    diff after xpect
    > xpect echo ba
    $(sed) -n s/\(.\)\{3,5\}/b/p afore > after
    diff after xpect
    > xpect echo b
    $(sed) -n s/\(.\)\{3,8\}/b/p afore > after
    diff after xpect
    $(sed) -n /\(.\)\{8,\}/!p afore > after
    diff afore after
    > xpect echo b
    $(sed) -n s/\(.\)\{0,\}/b/p afore > after
    diff after xpect

substitute: .symbolic dosub
    > afore echo hello
    $(sed) s/a/b/ afore > after
    diff afore after
    > xpect echo heLLo
    $(sed) -g s/l/L/ afore > after
    diff after xpect

dosub: .symbolic place
    > afore echo hello
    > xpect echo hellooa
    $(sed) "s/.*\(.\)/&\1a/" afore > after
    diff after xpect

place: .symbolic # already tested by dosub
    @%null

listto: .symbolic listto.exe
    # BS gets "Illegal character value 08H in file" from wmake
    # ESC gets "Illegal character value 1bH in file" from wmake
    # CR completes the command list
    > afore echo hello TAB	NL # There is an intentional tab character on this line WFB 20040801
    >>afore echo line end
    > xpect echo hello TAB\tNL\nline end$$
    > xpect echo hello TAB\tNL
    >>xpect echo line end$$
    $(sed) -n N;l afore > after
    diff after xpect
    > xpect echo $$
    >>xpect echo This sed quietly terminates a line with a decimal 26 character$$
    >>xpect echo \01\02\03\04\05\06\a\b\t$$
    >>xpect echo \v\f\0e\0f\10\11\12\13\14\15\16\17\18\19$$
    >>xpect echo \1b\1c\1d\1e\1f !"$#$$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNO\
    >>xpect echo PQRSTUVWXYZ[\\]^^_`abcdefghijklmnopqrstuvwxyz{^|}~\7f\80\81\82\83\
    >>xpect echo \84\85\86\87\88\89\8a\8b\8c\8d\8e\8f\90\91\92\93\94\95\96\97\98\
    >>xpect echo \99\9a\9b\9c\9d\9e\9f\a0\a1\a2\a3\a4\a5\a6\a7\a8\a9\aa\ab\ac\ad\
    >>xpect echo \ae\af\b0\b1\b2\b3\b4\b5\b6\b7\b8\b9\ba\bb\bc\bd\be\bf\c0\c1\c2\
    >>xpect echo \c3\c4\c5\c6\c7\c8\c9\ca\cb\cc\cd\ce\cf\d0\d1\d2\d3\d4\d5\d6\d7\
    >>xpect echo \d8\d9\da\db\dc\dd\de\df\e0\e1\e2\e3\e4\e5\e6\e7\e8\e9\ea\eb\ec\
    >>xpect echo \ed\ee\ef\f0\f1\f2\f3\f4\f5\f6\f7\f8\f9\fa\fb\fc\fd\fe\ff$$
    listto | sed -n l > after
    diff after xpect

listto.exe: $(__MAKEFILES__)
    # Writing < and > to a file using Microsoft tools in non-trivial
    > listto.c echo $#include "stdio.h"
    >>listto.c echo void main( void ) {
    >>listto.c echo     int i = 0;
    >>listto.c echo     putc( i, stdout );
    >>listto.c echo     fprintf( stdout, "%c%s\n", 0, "This sed silently ignores data between NUL and NL" );
    >>listto.c echo     fprintf( stdout, "%s%c", "This sed quietly terminates a line with a decimal 26 character", 26 );
    >>listto.c echo     while( ++i != 256 )
    >>listto.c echo         putc( i, stdout );
    >>listto.c echo }
    $(bld_cl) listto.c $(wcl_util_opts)

command: .symbolic substitute listto readout getline
    > afore echo hello
    > scrip echo a\
    >>scrip echo world
    > xpect echo hello
    >>xpect echo world
    $(sed) -f scrip afore > after
    diff after xpect
    > scrip echo c\
    >>scrip echo world
    > xpect echo world
    $(sed) -f scrip afore > after
    diff after xpect
    > afore echo hello
    >>afore echo world
    > xpect echo world
    $(sed) 1d afore > after
    diff after xpect
    $(sed) N;D afore > after
    diff after xpect
    > xpect echo 2
    $(sed) -n $$= afore > after
    diff after xpect
    > xpect echo hello
    $(sed) -n 1h;2{g;p;} afore > after
    diff after xpect
    > xpect echo world
    >>xpect echo hello
    $(sed) -n 1h;2{G;p;} afore > after
    diff after xpect
    > xpect echo\
    >>xpect echo hello
    $(sed) -n 1H;2{g;p;} afore > after
    diff after xpect
    > afore echo hello
    > scrip echo i\
    >>scrip echo world
    > xpect echo world
    >>xpect echo hello
    $(sed) -f scrip afore > after
    diff after xpect
    $(sed) b afore > after
    diff afore after
    > xpect echo hello$$
    $(sed) -n l afore > after
    diff after xpect
    >>afore echo world
    > scrip echo a\
    >>scrip echo universe
    > xpect echo hello
    >>xpect echo universe
    >>xpect echo world
    $(sed) -f scrip -e n afore > after
    diff after xpect
    > xpect echo universe
    >>xpect echo hello
    >>xpect echo world
    $(sed) -f scrip -e N afore > after
    diff after xpect
    > xpect echo hello
    $(sed) -n 1p  afore > after
    diff after xpect
    $(sed) -n N;P  afore > after
    diff after xpect
    $(sed) q afore > after
    diff after xpect
    > xpect echo hello
    >>xpect echo hello
    >>xpect echo world
    >>xpect echo world
    $(sed) 1rafore afore > after
    diff after xpect
    > xpect echo Hello
    >>xpect echo world
    $(sed) s/h/H/ afore > after
    diff after xpect
    $(sed) -n N;s/h/H/p afore > after
    diff after xpect
    > xpect echo Hello
    $(sed) -n N;s/h/H/P afore > after
    diff after xpect
    $(sed) -n s/h/H/wafter afore
    diff after xpect
    $(sed) -n -e s/h/H/;T -e p afore > after
    diff after xpect
    $(sed) -n -e s/h/H/;t -e p afore > after
    > xpect echo world
    diff after xpect
    > xpect echo hello
    $(sed) -n N;W afore > after
    diff after xpect
    >>xpect echo world
    $(sed) -n N;w afore > after
    diff after xpect
    > xpect echo hello
    $(sed) -n 1h;2x;$$p afore > after
    diff after xpect
    > xpect echo heLLo
    >>xpect echo worLd
    $(sed) y/l/L/ afore > after
    diff after xpect

getline: .symbolic # already deemed tested WFB 20040802
    @%null

memeql: .symbolic # already deemed tested WFB 20040802
    @%null

readout: .symbolic
    > afore echo hello
    # Test call from main loop
    > scrip echo rwritten
    >>scrip echo a\
    >>scrip echo append0\
    >>scrip echo append1
    >>scrip echo wwritten
    > xpect echo hello
    >>xpect echo hello
    >>xpect echo append0
    >>xpect echo append1
    $(sed) -f scrip afore > after
    diff after xpect
    # Call from n command
    >>afore echo world
    > xpect echo hello
    >>xpect echo hello
    >>xpect echo append0
    >>xpect echo append1
    >>xpect echo world
    $(sed) -f scrip -e n afore > after
    diff after xpect
    # Call from N command
    > xpect echo hello
    >>xpect echo append0
    >>xpect echo append1
    >>xpect echo hello
    >>xpect echo world
    $(sed) -f scrip -e N afore > after
    diff after xpect
    # Call from q command
    $(sed) -f scrip -e q afore > after
    > xpect echo hello
    >>xpect echo hello
    >>xpect echo append0
    >>xpect echo append1
    diff after xpect

spen0.sed: $(__MAKEFILES__)
    > spen0.sed echo $# Scrap lines which start $#
    >>spen0.sed echo /^^$#/d
    >>spen0.sed echo $# ( -> \( and ) -> \)
    >>spen0.sed echo s/[()]/\\^&/g
    >>spen0.sed echo $# ^^0 introduces a matching pattern
    >>spen0.sed echo s,^^0:\(.*\):\(.*\),echo \2^| $(sed) "/\1/d"  2^>\^&1,
    >>spen0.sed echo $# ^^1 introduces a non-matching pattern
    >>spen0.sed echo s,^^1:\(.*\):\(.*\),echo \2^| $(sed) "/\1/!d" 2^>\^&1,
    >>spen0.sed echo $# ^^2 introduces an unacceptable pattern
    >>spen0.sed echo s,^^2:\(.*\):\(.*\),echo \2^| $(sed) "/\1/h"  2^>nul,
    >>spen0.sed echo $# Output the line and the line number and go for the next line
    >>spen0.sed echo p;=;d

spen0.dat: $(__MAKEFILES__)
    > spen0.dat echo 0:abc:abc
    >>spen0.dat echo 1:abc:xbc
    >>spen0.dat echo 1:abc:axc
    >>spen0.dat echo 1:abc:abx
    >>spen0.dat echo 0:abc:xabcy
    >>spen0.dat echo 0:abc:ababc
    >>spen0.dat echo 0:ab*c:abc
    >>spen0.dat echo 0:ab*bc:abc
    >>spen0.dat echo 0:ab*bc:abbc
    >>spen0.dat echo 0:ab*bc:abbbbc
    >>spen0.dat echo 0:ab+bc:abbc
    >>spen0.dat echo 1:ab+bc:abc
    >>spen0.dat echo 1:ab+bc:abq
    >>spen0.dat echo 0:ab+bc:abbbbc
    >>spen0.dat echo $#0:ab?bc:abbc                          ? is not special in THIS sed.
    >>spen0.dat echo $#0:ab?bc:abc
    >>spen0.dat echo 1:ab?bc:abbbbc
    >>spen0.dat echo $#0:ab?c:abc
    >>spen0.dat echo 0:^^abc$$:abc
    >>spen0.dat echo 1:^^abc$$:abcc
    >>spen0.dat echo 0:^^abc:abcc
    >>spen0.dat echo 1:^^abc$$:aabc
    >>spen0.dat echo 0:abc$$:aabc
    >>spen0.dat echo 0:^^:abc
    >>spen0.dat echo 0:$$:abc
    >>spen0.dat echo 0:a.c:abc
    >>spen0.dat echo 0:a.c:axc
    >>spen0.dat echo 0:a.*c:axyzc
    >>spen0.dat echo 1:a.*c:axyzd
    >>spen0.dat echo 1:a[bc]d:abc
    >>spen0.dat echo 0:a[bc]d:abd
    >>spen0.dat echo 1:a[b-d]e:abd
    >>spen0.dat echo 0:a[b-d]e:ace
    >>spen0.dat echo 0:a[b-d]:aac
    >>spen0.dat echo 0:a[-b]:a-
    >>spen0.dat echo 0:a[b-]:a-
    >>spen0.dat echo 2:a[b-a]:-
    >>spen0.dat echo 2:a[]b:-
    >>spen0.dat echo 2:a[:-
    >>spen0.dat echo 0:a]:a]
    >>spen0.dat echo 0:a[]]b:a]b
    >>spen0.dat echo 0:a[^^bc]d:aed
    >>spen0.dat echo 1:a[^^bc]d:abd
    >>spen0.dat echo 0:a[^^-b]c:adc
    >>spen0.dat echo 1:a[^^-b]c:a-c
    >>spen0.dat echo 1:a[^^]b]c:a]c
    >>spen0.dat echo 0:a[^^]b]c:adc
    >>spen0.dat echo $#0:ab^|cd:abc
    >>spen0.dat echo $#0:ab^|cd:abcd
    >>spen0.dat echo 0:()ef:def
    >>spen0.dat echo 0:()*:-
    >>spen0.dat echo 1:*a:-
    >>spen0.dat echo $#0:^^*:-                                Test wrongly says * special after ^^
    >>spen0.dat echo 0:$$*:-
    >>spen0.dat echo $#1:(*)b:-                              Spurious test; substituted test below
    >>spen0.dat echo 1:$$b:b
    >>spen0.dat echo 2:a\:-
    >>spen0.dat echo 0:a\(b:a(b
    >>spen0.dat echo $#0:a\(*b:ab
    >>spen0.dat echo $#0:a\(*b:a((b
    >>spen0.dat echo 1:a\x:a\x
    >>spen0.dat echo $#1:abc):-                              Spurious test; substituted test below
    >>spen0.dat echo 2:(abc:-
    >>spen0.dat echo 0:((a)):abc
    >>spen0.dat echo 0:(a)b(c):abc
    >>spen0.dat echo 0:a+b+c:aabbabc
    >>spen0.dat echo 0:a**:-
    >>spen0.dat echo $#0:a*?:-
    >>spen0.dat echo 0:(a*)*:-
    >>spen0.dat echo 0:(a*)+:-
    >>spen0.dat echo $#0:(a^|)*:-                             ^| is not special in THIS sed.
    >>spen0.dat echo $#0:(a*^|b)*:-
    >>spen0.dat echo $#0:(a+^|b)*:ab
    >>spen0.dat echo $#0:(a+^|b)+:ab
    >>spen0.dat echo $#0:(a+^|b)?:ab
    >>spen0.dat echo 0:[^^ab]*:cde
    >>spen0.dat echo 0:(^^)*:-
    >>spen0.dat echo $#0:(ab^|)*:-
    >>spen0.dat echo 2:)(:-
    >>spen0.dat echo 1:abc:
    >>spen0.dat echo 1:abc:
    >>spen0.dat echo 0:a*:
    >>spen0.dat echo 0:([abc])*d:abbbcd
    >>spen0.dat echo 0:([abc])*bcd:abcd
    >>spen0.dat echo $#0:a^|b^|c^|d^|e:e
    >>spen0.dat echo $#0:(a^|b^|c^|d^|e)f:ef
    >>spen0.dat echo $#0:((a*^|b))*:-
    >>spen0.dat echo 0:abcd*efg:abcdefg
    >>spen0.dat echo 0:ab*:xabyabbbz
    >>spen0.dat echo 0:ab*:xayabbbz
    >>spen0.dat echo $#0:(ab^|cd)e:abcde
    >>spen0.dat echo 0:[abhgefdc]ij:hij
    >>spen0.dat echo $#1:^^(ab^|cd)e:abcde
    >>spen0.dat echo $#0:(abc^|)ef:abcdef
    >>spen0.dat echo $#0:(a^|b)c*d:abcd
    >>spen0.dat echo $#0:(ab^|ab*)bc:abc
    >>spen0.dat echo 0:a([bc]*)c*:abc
    >>spen0.dat echo 0:a([bc]*)(c*d):abcd
    >>spen0.dat echo 0:a([bc]+)(c*d):abcd
    >>spen0.dat echo 0:a([bc]*)(c+d):abcd
    >>spen0.dat echo 0:a[bcd]*dcdcde:adcdcde
    >>spen0.dat echo 1:a[bcd]+dcdcde:adcdcde
    >>spen0.dat echo $#0:(ab^|a)b*c:abc
    >>spen0.dat echo 0:((a)(b)c)(d):abcd
    >>spen0.dat echo 0:[A-Za-z_][A-Za-z0-9_]*:alpha
    >>spen0.dat echo $#0:^^a(bc+^|b[eh])g^|.h$$:abh
    >>spen0.dat echo $##0:(bc+d$$^|ef*g.^|h?i(j^|k)):effgz
    >>spen0.dat echo $#0:(bc+d$$^|ef*g.^|h?i(j^|k)):ij
    >>spen0.dat echo $#1:(bc+d$$^|ef*g.^|h?i(j^|k)):effg
    >>spen0.dat echo $#1:(bc+d$$^|ef*g.^|h?i(j^|k)):bcdd
    >>spen0.dat echo $#0:(bc+d$$^|ef*g.^|h?i(j^|k)):reffgz
    >>spen0.dat echo 1:((((((((((a)))))))))):-
    >>spen0.dat echo 0:(((((((((a))))))))):a
    >>spen0.dat echo 1:multiple words of text:uh-uh
    >>spen0.dat echo 0:multiple words:multiple words, yeah
    >>spen0.dat echo 0:(.*)c(.*):abcde
    >>spen0.dat echo 1:\((.*),:(.*)\)
    >>spen0.dat echo 1:[k]:ab
    >>spen0.dat echo 0:abcd:abcd
    >>spen0.dat echo 0:a(bc)d:abcd
    >>spen0.dat echo $#0:a[^^A-^^C]?c:a^^Bc wmake can't handle SOH, STX or ETX in an inline file
    >>spen0.dat echo 0:(....).*\1:beriberi
    >>spen0.dat echo 2:(*)b:-
    >>spen0.dat echo 2:abc):-

spen1.sed: $(__MAKEFILES__)
    > spen1.sed echo 1i\
    >>spen1.sed echo @echo %debug% off
    >>spen1.sed echo $# The file contains line pairs: a sed command on the 1st; a number on the 2nd.
    >>spen1.sed echo $# Transform each pair into one line with a conditional failure diagnostic.
    >>spen1.sed echo N;s,\n\(.*\), ^| $(sed) -n \"1s/.*/Test $#\1 failed!/p\",

spencer.bat: spen0.sed spen0.dat spen1.sed
    $(sed) -f spen0.sed spen0.dat | $(sed) -f spen1.sed > spencer.bat

# This rule tests things, not otherwise tested, specified in IEEE Std 1003.1, 2004 Edition
# http://www.opengroup.org/onlinepubs/000095399/utilities/sed.html
susv3: .symbolic
    # "the order of presentation of the -e and -f options is significant."
    > afore echo hello
    > xpect echo help
    > scrip echo p
    $(sed) -n -e s/lo/p/ -f scrip afore > after
    diff after xpect
    # "Multiple -e and -f options may be specified."
    > xpect echo Help
    > scr0p echo s/o/p/
    > scr1p echo p
    $(sed) -n -e s/l// -f scr0p -e s/h/H/ -f scr1p afore > after
    diff after xpect
    # "If multiple file operands are specified, the named files shall be read in the order specified"
    > xpect echo hello world
    > scr0p echo hello
    > scr1p echo world
    $(sed) -n "N;s/ //g;s/\n/ /p" scr0p scr1p > after
    diff after xpect

    # "INPUT FILES The input files shall be text files. The
    # script_files named by the -f option shall consist of editing commands."
    # This implementation allows \n absent from all input files.

    # "ENVIRONMENT VARIABLES The following environment variables
    # shall affect the execution of sed: ...".
    # All the variables are setlocale() oriented. None are supported as Open
    # Watcom C library documentation says: "Watcom C/C++ supports only the "C"
    # locale and so invoking this function will have no effect upon the
    # behavior of a program at present.#

    # "The command can be preceded by <blank>s and/or semicolons. The function can be preceded by <blank>s."
    $(sed) -n "    ;  1  p  " afore > after
    diff afore after

    # "The pattern and hold spaces shall each be able to hold at least 8192 bytes."
    > afore echo 12345678 1 2345678 2 2345678 3 2345678 4 2345678 5 2345678 6 23
    #          128  256  512 1024  2048 4096 8192
    $(sed) -n "h;G; h;G; h;G; h;G; h;G; h;G; h;G; h;P" afore > after
    diff afore after
    > xpect echo sed: can only fit 8192 bytes at line 1
    >>xpect echo 12345678 1 2345678 2 2345678 3 2345678 4 2345678 5 2345678 6 23
    #          128  256  512 1024  2048 4096 8192 16384
    $(sed) -n "h;G; h;G; h;G; h;G; h;G; h;G; h;G; h;G;P" afore > after 2>&1
    diff after xpect
    > xpect echo sed: can only fit 8192 bytes at line 1
    >>xpect echo ?12345678 1 2345678 2 2345678 3 2345678 4 2345678 5 2345678 6 23
    #          128  256  512 1024  2048 4096 8192 8193
    $(sed) -n "h;G; h;G; h;G; h;G; h;G; h;G; h;G; s/^/?/P" afore > after 2>&1
    diff after xpect

    # "In a context address, the construction "\cBREc" , where c is any
    # character other than backslash or <newline>, shall be identical to "/BRE/" ."
    > afore echo hello
    $(sed) -n \chcp afore > after
    diff afore after

    # "The r and w command verbs, and the w flag to the s command, take an
    # optional rfile (or wfile) parameter, separated from the command verb
    # letter or flag by one or more <blank>s; implementations may allow
    # zero separation as an extension."
    # This implementation does so allow.

    # "The argument rfile or the argument wfile shall terminate the editing
    # command. Each wfile shall be created before processing begins.
    # Implementations shall support at least ten wfile arguments in the
    # script; the actual number (greater than or equal to 10) that is
    # supported by the implementation is unspecified. The use of the wfile
    # parameter shall cause that file to be initially created, if it does not
    # exist, or shall replace the contents of an existing file."
    # This implementation supports 10 such files.

    # "The contents of the file specified for the r command shall be as of
    # the time the output is written, not the time the r command is applied."
    > xpect echo Hello
    >>xpect echo hello
    $(sed) -e rscrip -e wscrip -e s/h/H/ afore > after
    diff afore scrip
    diff after xpect

    # "It is unspecified whether <blank>s can follow a '!' character, and
    # conforming applications shall not follow a '!' character with <blank>s."
    # This implementation allows that unspecified behavior.
    $(sed) -e "/h/ ! d" afore > after
    diff afore after

    # "[2addr]b [label] ... The implementation shall support labels recognized
    # as unique up to at least 8 characters; the actual length (greater than
    # or equal to 8) that shall be supported by the implementation is
    # unspecified. It is unspecified whether exceeding a label length causes
    # an error or a silent truncation."
    # This implementation imposes no length restrictions.
    $(sed) -n -e "ba very long label" -e b -e ":a very long label" -e p afore > after
    diff afore after

    # "[2addr]l ... Non-printable characters not in that table shall be written
    # as one three-digit octal number (with a preceding backslash) for
    # each byte in the character (most significant byte first)."
    # This implementation uses hex instead.

    # "[2addr]s/BRE/replacement/flags ... The value of flags shall be zero or
    # more of: ... g Globally substitute for all non-overlapping instances of
    # the BRE rather than just the first one.
    # If both g and n are specified, the results are unspecified."
    # If both g and n are specified, this implementation does n and subsequent.
    > xpect echo helaa
    $(sed) -e s/./a/4g afore > after
    diff after xpect

    # In [2addr]s/BRE/replacement/flags, an extra P flag is supported.
    > afore echo hello
    >>afore echo world
    > xpect echo hello
    $(sed) -n -e N;s/d/D/P afore > after
    diff after xpect

    # [0addr] Ignore this empty command.
    > afore echo hello
    $(sed) -e "" afore > after
    diff afore after

    # "[0addr]# Ignore the '#' and the remainder of the line
    # (treat them as a comment), with the single exception that if the first
    # two characters in the script are "#n" , the default output shall be
    # suppressed; this shall be the equivalent of specifying -n on the command
    # line."
    > afore echo hello
    $(sed) -e "$#n" -e p afore > after
    diff afore after
    > afore echo hello
    $(sed) -e "$#N" afore > after
    diff afore after

bre: .symbolic # Note compliance with http://www.opengroup.org/onlinepubs/000095399/basedefs/xbd_chap09.html#tag_09_03
    # " 9.3.1 BREs Matching a Single Character or Collating Element
    # A BRE ordinary character, a special character preceded by a backslash, or
    # a period shall match a single character. A bracket expression shall match
    # a single character or a single collating element."
    # This implementation does not support collating elements.

    # "9.3.3 BRE Special Characters
    # This implementation does not recognise .[\ as special
    > xpect echo sed: garbled command /.[\/p
    $(sed) -n /.[\/p afore 2> after || true
    diff after xpect

    # "9.3.5 RE Bracket Expression
    #   collating elements, collating symbols and equivalence classes are
    #   unsupported;
    #   character sequences "[.", "[=", and "[:" are not special.

    # " 9.3.7 BRE Precedence
    # The order of precedence shall be as shown in the following table:
    # BRE Precedence (from high to low)
    # Collation-related bracket symbols [==] [::] [..]
    # Escaped characters                \<special character>
    # Bracket expression                []
    # Subexpressions/back-references    \(\) \n
    # Single-character-BRE duplication  * \{m,n\}
    # Concatenation
    # Anchoring                         ^ $"
    # This implementation does not recognise Collation-related bracket symbols.

    # "9.3.8 BRE Expression Anchoring
    # This does not treat a circumflex as an anchor when used as the first
    # character of a subexpression.
    # This does not treat a dollar sign as an anchor when used as the last
    # character of a subexpression.
