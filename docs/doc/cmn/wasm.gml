.chap *refid=wasm The &asmname.
.*
.if &e'&dohelp eq 0 .do begin
.section Introduction
.do end
.*
.np
.ix 'assembler'
This chapter describes the &asmname..
It takes as input an assembler source file (a file with extension
"&axt") and produces, as output, an object file.
.np
The &asmname command line syntax is the following.
.ix '&asmcmdup' 'command line format'
.ix 'command line format' '&asmcmdup'
.if '&target' eq 'QNX' .do begin
.mbigbox
&asmcmdup [options] asm_file [options] [@env_var]
.embigbox
.np
The square brackets [ ] denote items which are optional.
.begnote
.mnote &asmcmd
is the name of the &asmname..
.mnote asm_file
is the filename specification of the assembler source file to be
assembled.
A default filename extension of ".a" is assumed when no extension is
specified.
A filename extension consists of that portion of a filename containing
the last "." and any characters which follow it.
.exam begin
File Specification              Extension
/home/john.doe/foo              (none)
/home/john.doe/foo.             .
/home/john.doe/foo.bar          .bar
/home/john.doe/foo.goo.bar      .bar
.exam end
.mnote options
is a list of valid &asmname options, each preceded by a dash
("&minus.").
Options may be specified in any order.
.endnote
.do end
.el .do begin
.mbigbox
&asmcmdup [options] [d:][path]filename[.ext] [options] [@env_var]
.embigbox
.np
The square brackets [ ] denote items which are optional.
.begnote
.mnote &asmcmdup
is the name of the &asmname..
.mnote d:
is an optional drive specification such as "A:", "B:", etc.
If not specified, the default drive is assumed.
.mnote path
is an optional path specification such as "\PROGRAMS\ASM\".
If not specified, the current directory is assumed.
.mnote filename
is the file name of the assembler source file to be assembled.
.mnote ext
is the file extension of the assembler source file to be assembled.
If omitted, a file extension of "&axt" is assumed.
If the period "." is specified but not the extension, the file is
assumed to have no file extension.
.mnote options
is a list of valid options, each preceded by a slash
("/") or a dash ("&minus.").
Options may be specified in any order.
.endnote
.do end
.np
The options supported by the &asmname. are:
.begnote $compact
.note {0,1,2,3,4,5,6}{p}{r,s}
.begnote $compact
.note 0
same as ".8086"
.note 1
same as ".186"
.note 2{p}
same as ".286" or ".286p"
.note 3{p}
same as ".386" or ".386p"
(also defines "__386__" and changes the default USE attribute of
segments from "USE16" to "USE32")
.note 4{p}
same as ".486" or ".486p"
(also defines "__386__" and changes the default USE attribute of
segments from "USE16" to "USE32")
.note 5{p}
same as ".586" or ".586p"
(also defines "__386__" and changes the default USE attribute of
segments from "USE16" to "USE32")
.note 6{p}
same as ".686" or ".686p"
(also defines "__386__" and changes the default USE attribute of
segments from "USE16" to "USE32")
.note p
protect mode
.note add r
defines "__REGISTER__"
.note add s
defines "__STACK__"
.endnote
.exam begin
-2      -3p     -4pr    -5p
.exam end
.note bt=<os>
defines "__<os>__" and checks the "<os>_INCLUDE" environment variable
for include files
.note c
do not output OMF COMENT records that allow WDISASM to figure out when
data bytes have been placed in a code segment
.note d<name>[=text]
define text macro
.note d1
line number debugging support
.note e
stop reading assembler source file at END directive.
Normally, anything following the END directive will cause an error.
.note e<number>
set error limit number
.note fe=<file_name>
set error file name
.note fo=<file_name>
set object file name
.note fi=<file_name>
force <file_name> to be included
.note fpc
same as ".no87"
.note fpi
inline 80x87 instructions with emulation
.note fpi87
inline 80x87 instructions
.note fp0
same as ".8087"
.note fp2
same as ".287" or ".287p"
.note fp3
same as ".387" or ".387p"
.note fp5
same as ".587" or ".587p"
.note fp6
same as ".687" or ".687p"
.note i=<directory>
add directory to list of include directories
.note j or s
force signed types to be used for signed values
.note m{t,s,m,c,l,h,f}
memory model: (Tiny, Small, Medium, Compact, Large, Huge, Flat)
.begnote $compact
.note -mt
Same as ".model tiny"
.note -ms
Same as ".model small"
.note -mm
Same as ".model medium"
.note -mc
Same as ".model compact"
.note -ml
Same as ".model large"
.note -mh
Same as ".model huge"
.note -mf
Same as ".model flat"
.endnote
.np
Each of the model directives also defines "__<model>__"
(e.g., ".model small" defines "__SMALL__").
They also affect whether something like "foo proc" is considered a
"far" or "near" procedure.
.note nd=<name>
set data segment name
.note nm=<name>
set module name
.note nt=<name>
set name of text segment
.note o
allow C form of octal constants
.note zcm
set C name mangler to MASM compatible mode
.note zld
remove file dependency information
.note zq or q
operate quietly
.note zz
remove "@size" from STDCALL function names
.note zzo
don't mangle STDCALL symbols (WASM backward compatible)
.note ?  or h
print this message
.note w<number>
set warning level number
.note we
treat all warnings as errors
.note wx
set warning level to maximum setting
.endnote
.*
.section Assembly Directives and Opcodes
.*
.np
It is not the intention of this chapter to describe assembly-language
programming in any detail.
You should consult a book that deals with this topic.
However, we present an alphabetically ordered list of the directives,
opcodes and register names that are recognized by the assembler.
.ix '.186'
.ix '.286'
.ix '.286c'
.ix '.286p'
.ix '.287'
.ix '.386'
.ix '.386p'
.ix '.387'
.ix '.486'
.ix '.486p'
.ix '.586'
.ix '.586p'
.ix '.686'
.ix '.686p'
.ix '.8086'
.ix '.8087'
:cmt. .ix 'aaa'
:cmt. .ix 'aad'
:cmt. .ix 'aam'
:cmt. .ix 'aas'
:cmt. .ix 'abs'
:cmt. .ix 'adc'
:cmt. .ix 'add'
:cmt. .ix 'addr'
:cmt. .ix 'ah'
:cmt. .ix 'al'
:cmt. .ix 'alias'
:cmt. .ix 'align'
.ix '.alpha'
:cmt. .ix 'and'
:cmt. .ix 'arpl'
:cmt. .ix 'assume'
:cmt. .ix 'at'
:cmt. .ix 'ax'
:cmt. .ix 'basic'
:cmt. .ix 'bh'
:cmt. .ix 'bl'
:cmt. .ix 'bound'
:cmt. .ix 'bp'
.ix '.break'
:cmt. .ix 'bsf'
:cmt. .ix 'bsr'
:cmt. .ix 'bswap'
:cmt. .ix 'bt'
:cmt. .ix 'btc'
:cmt. .ix 'btr'
:cmt. .ix 'bts'
:cmt. .ix 'bx'
:cmt. .ix 'byte'
:cmt. .ix 'c'
:cmt. .ix 'call'
:cmt. .ix 'callf'
:cmt. .ix 'casemap'
:cmt. .ix 'catstr'
:cmt. .ix 'cbw'
:cmt. .ix 'cdq'
:cmt. .ix 'ch'
:cmt. .ix 'cl'
:cmt. .ix 'clc'
:cmt. .ix 'cld'
:cmt. .ix 'cli'
:cmt. .ix 'clts'
:cmt. .ix 'cmc'
:cmt. .ix 'cmp'
:cmt. .ix 'cmps'
:cmt. .ix 'cmpsb'
:cmt. .ix 'cmpsd'
:cmt. .ix 'cmpsw'
:cmt. .ix 'cmpxchg'
:cmt. .ix 'cmpxchg8b'
.ix '.code'
:cmt. .ix 'comm'
:cmt. .ix 'comment'
:cmt. .ix 'common'
:cmt. .ix 'compact'
.ix '.const'
.ix '.continue'
:cmt. .ix 'cpuid'
:cmt. .ix 'cr0'
:cmt. .ix 'cr2'
:cmt. .ix 'cr3'
:cmt. .ix 'cr4'
.ix '.cref'
:cmt. .ix 'cs'
:cmt. .ix 'cwd'
:cmt. .ix 'cwde'
:cmt. .ix 'cx'
:cmt. .ix 'daa'
:cmt. .ix 'das'
.ix '.data'
.ix '.data?'
:cmt. .ix 'db'
:cmt. .ix 'dd'
:cmt. .ix 'dec'
:cmt. .ix 'df'
:cmt. .ix 'dh'
:cmt. .ix 'di'
:cmt. .ix 'div'
:cmt. .ix 'dl'
.ix '.dosseg'
:cmt. .ix 'dosseg'
:cmt. .ix 'dp'
:cmt. .ix 'dq'
:cmt. .ix 'dr0'
:cmt. .ix 'dr1'
:cmt. .ix 'dr2'
:cmt. .ix 'dr3'
:cmt. .ix 'dr6'
:cmt. .ix 'dr7'
:cmt. .ix 'ds'
:cmt. .ix 'dt'
:cmt. .ix 'dup'
:cmt. .ix 'dw'
:cmt. .ix 'dword'
:cmt. .ix 'dx'
:cmt. .ix 'eax'
:cmt. .ix 'ebp'
:cmt. .ix 'ebx'
:cmt. .ix 'echo'
:cmt. .ix 'ecx'
:cmt. .ix 'edi'
:cmt. .ix 'edx'
:cmt. .ix '.else'
:cmt. .ix 'else'
:cmt. .ix 'elseif'
:cmt. .ix 'end'
:cmt. .ix '.endif'
:cmt. .ix 'endif'
:cmt. .ix 'endp'
:cmt. .ix 'ends'
.ix '.endw'
:cmt. .ix 'enter'
:cmt. .ix 'eq'
:cmt. .ix 'equ'
:cmt. .ix 'equ2'
.ix '.err'
.ix '.errb'
.ix '.errdef'
.ix '.errdif'
.ix '.errdifi'
.ix '.erre'
.ix '.erridn'
.ix '.erridni'
.ix '.errnb'
.ix '.errndef'
.ix '.errnz'
:cmt. .ix 'error'
:cmt. .ix 'es'
:cmt. .ix 'esi'
:cmt. .ix 'esp'
:cmt. .ix 'even'
.ix '.exit'
:cmt. .ix 'export'
:cmt. .ix 'extern'
:cmt. .ix 'externdef'
:cmt. .ix 'extrn'
:cmt. .ix 'f2xm1'
:cmt. .ix 'fabs'
:cmt. .ix 'fadd'
:cmt. .ix 'faddp'
:cmt. .ix 'far'
.ix '.fardata'
.ix '.fardata?'
:cmt. .ix 'farstack'
:cmt. .ix 'fbld'
:cmt. .ix 'fbstp'
:cmt. .ix 'fchs'
:cmt. .ix 'fclex'
:cmt. .ix 'fcom'
:cmt. .ix 'fcomp'
:cmt. .ix 'fcompp'
:cmt. .ix 'fcos'
:cmt. .ix 'fdecstp'
:cmt. .ix 'fdisi'
:cmt. .ix 'fdiv'
:cmt. .ix 'fdivp'
:cmt. .ix 'fdivr'
:cmt. .ix 'fdivrp'
:cmt. .ix 'feni'
:cmt. .ix 'ffree'
:cmt. .ix 'fiadd'
:cmt. .ix 'ficom'
:cmt. .ix 'ficomp'
:cmt. .ix 'fidiv'
:cmt. .ix 'fidivr'
:cmt. .ix 'fild'
:cmt. .ix 'fimul'
:cmt. .ix 'fincstp'
:cmt. .ix 'finit'
:cmt. .ix 'fist'
:cmt. .ix 'fistp'
:cmt. .ix 'fisub'
:cmt. .ix 'fisubr'
:cmt. .ix 'flat'
:cmt. .ix 'fld'
:cmt. .ix 'fld1'
:cmt. .ix 'fldcw'
:cmt. .ix 'fldenv'
:cmt. .ix 'fldenvd'
:cmt. .ix 'fldenvw'
:cmt. .ix 'fldl2e'
:cmt. .ix 'fldl2t'
:cmt. .ix 'fldlg2'
:cmt. .ix 'fldln2'
:cmt. .ix 'fldpi'
:cmt. .ix 'fldz'
:cmt. .ix 'fmul'
:cmt. .ix 'fmulp'
:cmt. .ix 'fnclex'
:cmt. .ix 'fndisi'
:cmt. .ix 'fneni'
:cmt. .ix 'fninit'
:cmt. .ix 'fnop'
:cmt. .ix 'fnrstor'
:cmt. .ix 'fnrstord'
:cmt. .ix 'fnrstorw'
:cmt. .ix 'fnsave'
:cmt. .ix 'fnsaved'
:cmt. .ix 'fnsavew'
:cmt. .ix 'fnstcw'
:cmt. .ix 'fnstenv'
:cmt. .ix 'fnstenvd'
:cmt. .ix 'fnstenvw'
:cmt. .ix 'fnstsw'
:cmt. .ix 'for'
:cmt. .ix 'forc'
:cmt. .ix 'fortran'
:cmt. .ix 'fpatan'
:cmt. .ix 'fprem'
:cmt. .ix 'fprem1'
:cmt. .ix 'fptan'
:cmt. .ix 'frndint'
:cmt. .ix 'frstor'
:cmt. .ix 'frstord'
:cmt. .ix 'frstorw'
:cmt. .ix 'fs'
:cmt. .ix 'fsave'
:cmt. .ix 'fsaved'
:cmt. .ix 'fsavew'
:cmt. .ix 'fscale'
:cmt. .ix 'fsetpm'
:cmt. .ix 'fsin'
:cmt. .ix 'fsincos'
:cmt. .ix 'fsqrt'
:cmt. .ix 'fst'
:cmt. .ix 'fstcw'
:cmt. .ix 'fstenv'
:cmt. .ix 'fstenvd'
:cmt. .ix 'fstenvw'
:cmt. .ix 'fstp'
:cmt. .ix 'fstsw'
:cmt. .ix 'fsub'
:cmt. .ix 'fsubp'
:cmt. .ix 'fsubr'
:cmt. .ix 'fsubrp'
:cmt. .ix 'ftst'
:cmt. .ix 'fucom'
:cmt. .ix 'fucomp'
:cmt. .ix 'fucompp'
:cmt. .ix 'fwait'
:cmt. .ix 'fword'
:cmt. .ix 'fxam'
:cmt. .ix 'fxch'
:cmt. .ix 'fxtract'
:cmt. .ix 'fyl2x'
:cmt. .ix 'fyl2xp1'
:cmt. .ix 'ge'
:cmt. .ix 'global'
:cmt. .ix 'group'
:cmt. .ix 'gs'
:cmt. .ix 'gt'
:cmt. .ix 'high'
:cmt. .ix 'highword'
:cmt. .ix 'hlt'
:cmt. .ix 'huge'
:cmt. .ix 'idiv'
:cmt. .ix '.if'
:cmt. .ix 'if'
:cmt. .ix 'if1'
:cmt. .ix 'if2'
:cmt. .ix 'ifb'
:cmt. .ix 'ifdef'
:cmt. .ix 'ifdif'
:cmt. .ix 'ifdifi'
:cmt. .ix 'ife'
:cmt. .ix 'ifidn'
:cmt. .ix 'ifidni'
:cmt. .ix 'ifnb'
:cmt. .ix 'ifndef'
:cmt. .ix 'ignore'
:cmt. .ix 'imul'
:cmt. .ix 'in'
:cmt. .ix 'inc'
:cmt. .ix 'include'
:cmt. .ix 'includelib'
:cmt. .ix 'ins'
:cmt. .ix 'insb'
:cmt. .ix 'insd'
:cmt. .ix 'insw'
:cmt. .ix 'int'
:cmt. .ix 'into'
:cmt. .ix 'invd'
:cmt. .ix 'invlpg'
:cmt. .ix 'invoke'
:cmt. .ix 'iret'
:cmt. .ix 'iretd'
:cmt. .ix 'iretdf'
:cmt. .ix 'iretf'
:cmt. .ix 'irp'
:cmt. .ix 'irpc'
:cmt. .ix 'ja'
:cmt. .ix 'jae'
:cmt. .ix 'jb'
:cmt. .ix 'jbe'
:cmt. .ix 'jc'
:cmt. .ix 'jcxz'
:cmt. .ix 'je'
:cmt. .ix 'jecxz'
:cmt. .ix 'jg'
:cmt. .ix 'jge'
:cmt. .ix 'jl'
:cmt. .ix 'jle'
:cmt. .ix 'jmp'
:cmt. .ix 'jmpf'
:cmt. .ix 'jna'
:cmt. .ix 'jnae'
:cmt. .ix 'jnb'
:cmt. .ix 'jnbe'
:cmt. .ix 'jnc'
:cmt. .ix 'jne'
:cmt. .ix 'jng'
:cmt. .ix 'jnge'
:cmt. .ix 'jnl'
:cmt. .ix 'jnle'
:cmt. .ix 'jno'
:cmt. .ix 'jnp'
:cmt. .ix 'jns'
:cmt. .ix 'jnz'
:cmt. .ix 'jo'
:cmt. .ix 'jp'
:cmt. .ix 'jpe'
:cmt. .ix 'jpo'
:cmt. .ix 'js'
:cmt. .ix 'jz'
:cmt. .ix 'label'
:cmt. .ix 'lahf'
:cmt. .ix 'lar'
:cmt. .ix 'large'
:cmt. .ix 'lds'
:cmt. .ix 'le'
:cmt. .ix 'lea'
:cmt. .ix 'leave'
:cmt. .ix 'length'
:cmt. .ix 'lengthof'
:cmt. .ix 'les'
.ix '.lfcond'
:cmt. .ix 'lfs'
:cmt. .ix 'lgdt'
:cmt. .ix 'lgs'
:cmt. .ix 'lidt'
.ix '.list'
.ix '.listall'
.ix '.listif'
.ix '.listmacro'
.ix '.listmacroall'
:cmt. .ix 'lldt'
:cmt. .ix 'lmsw'
:cmt. .ix 'local'
:cmt. .ix 'lock'
:cmt. .ix 'lods'
:cmt. .ix 'lodsb'
:cmt. .ix 'lodsd'
:cmt. .ix 'lodsw'
:cmt. .ix 'loop'
:cmt. .ix 'loope'
:cmt. .ix 'loopne'
:cmt. .ix 'loopnz'
:cmt. .ix 'loopz'
:cmt. .ix 'low'
:cmt. .ix 'lowword'
:cmt. .ix 'lroffset'
:cmt. .ix 'lsl'
:cmt. .ix 'lss'
:cmt. .ix 'lt'
:cmt. .ix 'ltr'
:cmt. .ix 'macro'
:cmt. .ix 'mask'
:cmt. .ix 'medium'
:cmt. .ix 'memory'
:cmt. .ix 'mod'
.ix '.model'
:cmt. .ix 'mov'
:cmt. .ix 'movs'
:cmt. .ix 'movsb'
:cmt. .ix 'movsd'
:cmt. .ix 'movsw'
:cmt. .ix 'movsx'
:cmt. .ix 'movzx'
:cmt. .ix 'mul'
:cmt. .ix 'name'
:cmt. .ix 'ne'
:cmt. .ix 'near'
:cmt. .ix 'nearstack'
:cmt. .ix 'neg'
:cmt. .ix '.no87'
.ix '.nocref'
.ix '.nolist'
:cmt. .ix 'nop'
:cmt. .ix 'not'
:cmt. .ix 'nothing'
:cmt. .ix 'offset'
:cmt. .ix 'opattr'
:cmt. .ix 'option'
:cmt. .ix 'or'
:cmt. .ix 'org'
:cmt. .ix 'os_dos'
:cmt. .ix 'os_os2'
:cmt. .ix 'out'
:cmt. .ix 'outs'
:cmt. .ix 'outsb'
:cmt. .ix 'outsd'
:cmt. .ix 'outsw'
:cmt. .ix 'para'
:cmt. .ix 'pascal'
:cmt. .ix 'pop'
:cmt. .ix 'popa'
:cmt. .ix 'popad'
:cmt. .ix 'popcontext'
:cmt. .ix 'popf'
:cmt. .ix 'popfd'
:cmt. .ix 'private'
:cmt. .ix 'proc'
:cmt. .ix 'proto'
:cmt. .ix 'ptr'
:cmt. .ix 'public'
:cmt. .ix 'purge'
:cmt. .ix 'push'
:cmt. .ix 'pusha'
:cmt. .ix 'pushad'
:cmt. .ix 'pushcontext'
:cmt. .ix 'pushf'
:cmt. .ix 'pushfd'
:cmt. .ix 'pword'
:cmt. .ix 'qword'
.ix '.radix'
:cmt. .ix 'rcl'
:cmt. .ix 'rcr'
:cmt. .ix 'rdmsr'
:cmt. .ix 'rdtsc'
:cmt. .ix 'readonly'
:cmt. .ix 'record'
:cmt. .ix 'rep'
:cmt. .ix 'repe'
.ix '.repeat'
:cmt. .ix 'repeat'
:cmt. .ix 'repne'
:cmt. .ix 'repnz'
:cmt. .ix 'rept'
:cmt. .ix 'repz'
:cmt. .ix 'ret'
:cmt. .ix 'retf'
:cmt. .ix 'retn'
:cmt. .ix 'rol'
:cmt. .ix 'ror'
:cmt. .ix 'rsm'
:cmt. .ix 'sahf'
:cmt. .ix 'sal'
.ix '.sall'
:cmt. .ix 'sar'
:cmt. .ix 'sbb'
:cmt. .ix 'sbyte'
:cmt. .ix 'scas'
:cmt. .ix 'scasb'
:cmt. .ix 'scasd'
:cmt. .ix 'scasw'
:cmt. .ix 'sdword'
:cmt. .ix 'seg'
:cmt. .ix 'segment'
.ix '.seq'
:cmt. .ix 'seta'
:cmt. .ix 'setae'
:cmt. .ix 'setb'
:cmt. .ix 'setbe'
:cmt. .ix 'setc'
:cmt. .ix 'sete'
:cmt. .ix 'setg'
:cmt. .ix 'setge'
:cmt. .ix 'setl'
:cmt. .ix 'setle'
:cmt. .ix 'setna'
:cmt. .ix 'setnae'
:cmt. .ix 'setnb'
:cmt. .ix 'setnbe'
:cmt. .ix 'setnc'
:cmt. .ix 'setne'
:cmt. .ix 'setng'
:cmt. .ix 'setnge'
:cmt. .ix 'setnl'
:cmt. .ix 'setnle'
:cmt. .ix 'setno'
:cmt. .ix 'setnp'
:cmt. .ix 'setns'
:cmt. .ix 'setnz'
:cmt. .ix 'seto'
:cmt. .ix 'setp'
:cmt. .ix 'setpe'
:cmt. .ix 'setpo'
:cmt. .ix 'sets'
:cmt. .ix 'setz'
.ix '.sfcond'
:cmt. .ix 'sgdt'
:cmt. .ix 'shl'
:cmt. .ix 'shld'
:cmt. .ix 'short'
:cmt. .ix 'shr'
:cmt. .ix 'shrd'
:cmt. .ix 'si'
:cmt. .ix 'sidt'
:cmt. .ix 'size'
:cmt. .ix 'sizeof'
:cmt. .ix 'sldt'
:cmt. .ix 'small'
:cmt. .ix 'smsw'
:cmt. .ix 'sp'
:cmt. .ix 'ss'
:cmt. .ix 'st'
.ix '.stack'
.ix '.startup'
:cmt. .ix 'stc'
:cmt. .ix 'std'
:cmt. .ix 'stdcall'
:cmt. .ix 'sti'
:cmt. .ix 'stos'
:cmt. .ix 'stosb'
:cmt. .ix 'stosd'
:cmt. .ix 'stosw'
:cmt. .ix 'str'
:cmt. .ix 'struc'
:cmt. .ix 'struct'
:cmt. .ix 'sub'
:cmt. .ix 'sword'
:cmt. .ix 'syscall'
:cmt. .ix 'tbyte'
:cmt. .ix 'test'
:cmt. .ix 'textequ'
.ix '.tfcond'
:cmt. .ix 'this'
:cmt. .ix 'tiny'
:cmt. .ix 'tr3'
:cmt. .ix 'tr4'
:cmt. .ix 'tr5'
:cmt. .ix 'tr6'
:cmt. .ix 'tr7'
:cmt. .ix 'typedef'
:cmt. .ix 'union'
.ix '.until'
:cmt. .ix 'use16'
:cmt. .ix 'use32'
:cmt. .ix 'uses'
:cmt. .ix 'vararg'
:cmt. .ix 'verr'
:cmt. .ix 'verw'
:cmt. .ix 'wait'
:cmt. .ix 'watcom_c'
:cmt. .ix 'wbinvd'
.ix '.while'
:cmt. .ix 'width'
:cmt. .ix 'word'
:cmt. .ix 'wrmsr'
:cmt. .ix 'xadd'
:cmt. .ix 'xchg'
.ix '.xcref'
:cmt. .ix 'xlat'
:cmt. .ix 'xlatb'
.ix '.xlist'
:cmt. .ix 'xor'
.millust begin
~.186         .286         .286c         .286p        .287         .386
~.386p        .387         .486          .486p        .586         .586p
~.686         .686p        .8086         .8087        aaa          aad
aam          aas          abs           adc          add          addpd
addps        addr         addsd         addss        addsubpd     addsubps
ah           al           alias         align        .alpha       and
andnpd       andnps       andpd         andps        arpl         assume
ax           basic        bh            bl           bound        bp
~.break       bsf          bsr           bswap        bt           btc
btr          bts          bx            byte         c            call
callf        casemap      catstr        cbw          cdq          ch
cl           clc          cld           clflush      cli          clts
cmc          cmova        cmovae        cmovb        cmovbe       cmovc
cmove        cmovg        cmovge        cmovl        cmovle       cmovna
cmovnae      cmovnb       cmovnbe       cmovnc       cmovne       cmovng
cmovnge      cmovnl       cmovnle       cmovno       cmovnp       cmovns
cmovnz       cmovo        cmovp         cmovpe       cmovpo       cmovs
cmovz        cmp          cmpeqpd       cmpeqps      cmpeqsd      cmpeqss
cmplepd      cmpleps      cmplesd       cmpless      cmpltpd      cmpltps
cmpltsd      cmpltss      cmpneqpd      cmpneqps     cmpneqsd     cmpneqss
cmpnlepd     cmpnleps     cmpnlesd      cmpnless     cmpnltpd     cmpnltps
cmpnltsd     cmpnltss     cmpordpd      cmpordps     cmpordsd     cmpordss
cmppd        cmpps        cmps          cmpsb        cmpsd        cmpss
cmpsw        cmpunordpd   cmpunordps    cmpunordsd   cmpunordss   cmpxchg
cmpxchg8b    .code        comisd        comiss       comm         comment
common       compact      .const        .continue    cpuid        cr0
cr2          cr3          cr4           .cref        cs           cvtdq2pd
cvtdq2ps     cvtpd2dq     cvtpd2pi      cvtpd2ps     cvtpi2pd     cvtpi2ps
cvtps2dq     cvtps2pd     cvtps2pi      cvtsd2si     cvtsd2ss     cvtsi2sd
cvtsi2ss     cvtss2sd     cvtss2si      cvttpd2dq    cvttpd2pi    cvttps2dq
cvttps2pi    cvttsd2si    cvttss2si     cwd          cwde         cx
daa          das          .data         .data?       db           dd
dec          df           dh            di           div          divpd
divps        divsd        divss         dl           .dosseg      dosseg
dp           dq           dr0           dr1          dr2          dr3
dr6          dr7          ds            dt           dup          dw
.millust break
dword        dx           eax           ebp          ebx          echo
ecx          edi          edx           .else        else         elseif
emms         end          .endif        endif        endm         endp
ends         .endw        enter         eq           equ          equ2
~.err         .errb        .errdef       .errdif      .errdifi     .erre
~.erridn      .erridni     .errnb        .errndef     .errnz       error
es           esi          esp           even         .exit        exitm
export       extern       externdef     extrn        f2xm1        fabs
fadd         faddp        far           .fardata     .fardata?    farstack
fbld         fbstp        fchs          fclex        fcmovb       fcmovbe
fcmove       fcmovnb      fcmovnbe      fcmovne      fcmovnu      fcmovu
fcom         fcomi        fcomip        fcomp        fcompp       fcos
fdecstp      fdisi        fdiv          fdivp        fdivr        fdivrp
femms        feni         ffree         fiadd        ficom        ficomp
fidiv        fidivr       fild          fimul        fincstp      finit
fist         fistp        fisttp        fisub        fisubr       flat
fld          fld1         fldcw         fldenv       fldenvd      fldenvw
fldl2e       fldl2t       fldlg2        fldln2       fldpi        fldz
fmul         fmulp        fnclex        fndisi       fneni        fninit
fnop         fnrstor      fnrstord      fnrstorw     fnsave       fnsaved
fnsavew      fnstcw       fnstenv       fnstenvd     fnstenvw     fnstsw
for          forc         fortran       fpatan       fprem        fprem1
fptan        frndint      frstor        frstord      frstorw      fs
fsave        fsaved       fsavew        fscale       fsetpm       fsin
fsincos      fsqrt        fst           fstcw        fstenv       fstenvd
fstenvw      fstp         fstsw         fsub         fsubp        fsubr
fsubrp       ftst         fucom         fucomi       fucomip      fucomp
fucompp      fwait        fword         fxam         fxch         fxrstor
fxsave       fxtract      fyl2x         fyl2xp1      ge           global
group        gs           gt            haddpd       haddps       high
highword     hlt          hsubpd        hsubps       huge         idiv
~.if          if           if1           if2          ifb          ifdef
ifdif        ifdifi       ife           ifidn        ifidni       ifnb
ifndef       ignore       imul          in           inc          include
includelib   ins          insb          insd         insw         int
into         invd         invlpg        invoke       iret         iretd
.millust break
iretdf       iretf        irp           irpc         ja           jae
jb           jbe          jc            jcxz         je           jecxz
jg           jge          jl            jle          jmp          jmpf
jna          jnae         jnb           jnbe         jnc          jne
jng          jnge         jnl           jnle         jno          jnp
jns          jnz          jo            jp           jpe          jpo
js           jz           .k3d          label        lahf         lar
large        lddqu        ldmxcsr       lds          le           lea
leave        length       lengthof      les          .lfcond      lfence
lfs          lgdt         lgs           lidt         .list        .listall
~.listif      .listmacro   .listmacroall lldt         lmsw         local
lock         lods         lodsb         lodsd        lodsw        loop
loopd        loope        looped        loopew       loopne       loopned
loopnew      loopnz       loopnzd       loopnzw      loopw        loopz
loopzd       loopzw       low           lowword      lroffset     lsl
lss          lt           ltr           macro        mask         maskmovdqu
maskmovq     maxpd        maxps         maxsd        maxss        medium
memory       mfence       minpd         minps        minsd        minss
mm0          mm1          mm2           mm3          mm4          mm5
mm6          mm7          .mmx          mod          .model       monitor
mov          movapd       movaps        movd         movddup      movdq2q
movdqa       movdqu       movhlps       movhpd       movhps       movlhps
movlpd       movlps       movmskpd      movmskps     movntdq      movnti
movntpd      movntps      movntq        movq         movq2dq      movs
movsb        movsd        movshdup      movsldup     movss        movsw
movsx        movupd       movups        movzx        mul          mulpd
mulps        mulsd        mulss         mwait        name         ne
near         nearstack    neg           .no87        .nocref      .nolist
nop          not          nothing       offset       opattr       option
or           org          orpd          orps         os_dos       os_os2
out          outs         outsb         outsd        outsw        oword
packssdw     packsswb     packuswb      paddb        paddd        paddq
paddsb       paddsw       paddusb       paddusw      paddw        page
pand         pandn        para          pascal       pause        pavgb
pavgusb      pavgw        pcmpeqb       pcmpeqd      pcmpeqw      pcmpgtb
pcmpgtd      pcmpgtw      pextrw        pf2id        pf2iw        pfacc
.millust break
pfadd        pfcmpeq      pfcmpge       pfcmpgt      pfmax        pfmin
pfmul        pfnacc       pfpnacc       pfrcp        pfrcpit1     pfrcpit2
pfrsqit1     pfrsqrt      pfsub         pfsubr       pi2fd        pi2fw
pinsrw       pmaddwd      pmaxsw        pmaxub       pminsw       pminub
pmovmskb     pmulhrw      pmulhuw       pmulhw       pmullw       pmuludq
pop          popa         popad         popcontext   popf         popfd
por          prefetch     prefetchnta   prefetcht0   prefetcht1   prefetcht2
prefetchw    private      proc          proto        psadbw       pshufd
pshufhw      pshuflw      pshufw        pslld        pslldq       psllq
psllw        psrad        psraw         psrld        psrldq       psrlq
psrlw        psubb        psubd         psubq        psubsb       psubsw
psubusb      psubusw      psubw         pswapd       ptr          public
punpckhbw    punpckhdq    punpckhqdq    punpckhwd    punpcklbw    punpckldq
punpcklqdq   punpcklwd    purge         push         pusha        pushad
pushcontext  pushd        pushf         pushfd       pushw        pword
pxor         qword        .radix        rcl          rcpps        rcpss
rcr          rdmsr        rdpmc         rdtsc        readonly     record
rep          repe         .repeat       repeat       repne        repnz
rept         repz         ret           retd         retf         retfd
retn         rol          ror           rsm          rsqrtps      rsqrtss
sahf         sal          .sall         sar          sbb          sbyte
scas         scasb        scasd         scasw        sdword       seg
segment      .seq         seta          setae        setb         setbe
setc         sete         setg          setge        setl         setle
setna        setnae       setnb         setnbe       setnc        setne
setng        setnge       setnl         setnle       setno        setnp
setns        setnz        seto          setp         setpe        setpo
sets         setz         .sfcond       sfence       sgdt         shl
shld         short        shr           shrd         shufpd       shufps
si           sidt         size          sizeof       sldt         small
smsw         sp           sqrtpd        sqrtps       sqrtsd       sqrtss
ss           st           .stack        .startup     stc          std
stdcall      sti          stmxcsr       stos         stosb        stosd
stosw        str          struc         struct       sub          subpd
subps        subsd        subss         subtitle     subttl       sword
syscall      sysenter     sysexit       tbyte        test         textequ
.millust break
~.tfcond      this         tiny          title        tr3          tr4
tr5          tr6          tr7           typedef      ucomisd      ucomiss
union        unpckhpd     unpckhps      unpcklpd     unpcklps     .until
use16        use32        uses          vararg       verr         verw
wait         watcom_c     wbinvd        .while       width        word
wrmsr        xadd         xchg          .xcref       xlat         xlatb
~.xlist       .xmm         xmm0          xmm1         .xmm2        xmm2
~.xmm3        xmm3         xmm4          xmm5         xmm6         xmm7
xor          xorpd        xorps         
.millust end
.*
.section Unsupported Directives
.*
.np
Other assemblers support directives that this assembler does not.
The following is a list of directives that are ignored by the
&asmname. (use of these directives results in a warning message).
.ix '.alpha'
.ix '.cref'
.ix '.lfcond'
.ix '.list'
.ix '.listall'
.ix '.listif'
.ix '.listmacro'
.ix '.listmacroall'
.ix '.nocref'
.ix '.nolist'
.ix 'page'
.ix '.sall'
.ix '.seq'
.ix '.sfcond'
.ix 'subtitle'
.ix 'subttl'
.ix '.tfcond'
.ix 'title'
.ix '.xcref'
.ix '.xlist'
.millust begin
~.alpha         .cref          .lfcond        .list
~.listall       .listif        .listmacro     .listmacroall
~.nocref        .nolist        page           .sall
~.seq           .sfcond        subtitle       subttl
~.tfcond        title          .xcref         .xlist
.millust end
.np
The following is a list of directives that are flagged by the
&asmname. (use of these directives results in an error message).
.ix 'addr'
.ix '.break'
.ix 'casemap'
.ix 'catstr'
.ix '.continue'
.ix 'echo'
.ix '.else'
.ix 'endmacro'
.ix '.endif'
.ix '.endw'
.ix '.exit'
.ix 'high'
.ix 'highword'
.ix '.if'
.ix 'invoke'
.ix 'low'
.ix 'lowword'
.ix 'lroffset'
.ix 'mask'
.ix 'opattr'
.ix 'option'
.ix 'popcontext'
.ix 'proto'
.ix 'purge'
.ix 'pushcontext'
.ix '.radix'
.ix 'record'
.ix '.repeat'
.ix '.startup'
.ix 'this'
.ix 'typedef'
.ix 'union'
.ix '.until'
.ix '.while'
.ix 'width'
.millust begin
addr           .break         casemap        catstr
~.continue      echo           .else          endmacro
~.endif         .endw          .exit          high
highword       .if            invoke         low
lowword        lroffset       mask           opattr
option         popcontext     proto          purge
pushcontext    .radix         record         .repeat
~.startup       this           typedef        union
~.until         .while         width
.millust end
.*
.section &asmname Specific
.*
.np
There are a few specific features in &asmname.
.np
.beglevel
.section Naming convention
.millust begin
                 Procedure   Variable
Convention         Name        Name
---------------  ----------  ---------
C                   '*'         '*'
C (MASM)           '_*'        '_*'    see note 1
WATCOM_C           '*_'        '_*'
SYSCALL             '*'         '*'
STDCALL           '_*@nn'      '_*'
STDCALL            '_*'        '_*'    see note 2
STDCALL             '*'         '*'    see note 3
BASIC               '^'         '^'
FORTRAN             '^'         '^'
PASCAL              '^'         '^'
.millust end
.autonote Notes:
.note
WASM uses MASM compatible names when -zcm command line option is used.
.note
In STDCALL procedures name 'nn' is overall parametrs size in bytes.
'@nn' is suppressed when -zz command line option is used (WATCOM 10.0 compatibility).
.note
STDCALL symbols mangling is suppressed by -zzo command line option (WASM backward compatible).
.endnote
.np
.section &company "C" name mangler
.millust begin
Command line     Procedure     Others
  option           Name        Names
---------------  ----------  ---------
0,1,2              '*_'        '_*'
3,4,5,6 with r     '*_'        '_*'
3,4,5,6 with s      '*'        '*'
.millust end
.np
.section Calling convention
.millust begin
                      Parameters    Parameters   Cleanup caller
Convention   Vararg    passed by       order         stack
-----------  ------  ------------  ------------- --------------
C             yes      stack       right to left    no
WATCOM_C      yes      registers   right to left    no
SYSCALL       yes      stack       right to left    no
STDCALL       yes      stack       right to left    yes see note 1
BASIC         no       stack       left to right    yes
FORTRAN       no       stack       left to right    yes
PASCAL        no       stack       left to right    yes
.millust end
.autonote Notes:
.note
For STDCALL procedures WASM automaticaly cleanup caller stack,
except case when vararg parameter is used.
.endnote
.endlevel
.*
.section &asmname. Diagnostic Messages
.*
.dm errnote begin
.   .se *ctxn=&'substr(&*,1,&'pos(' ',&*)-1)
.   .se *ttl$=&'substr(&*,&'pos(' ',&*)+1)
.   .note &*ctxn &*ttl$
.*  .us &*ttl$
.dm errnote end
.*
.begnote
.*
.errnote 001 Size doesn't match with previous definition
.errnote 002 Invalid instruction with current CPU setting
.errnote 003 LOCK prefix is not allowed on this instruction
.errnote 004 REP prefix is not allowed on this instruction
.errnote 005 Invalid memory pointer
.errnote 006 Cannot use 386 addressing mode with current CPU setting
.errnote 007 Too many base registers
.errnote 008 Invalid index register
.errnote 009 Scale factor must be 1, 2, 4 or 8
.errnote 010 invalid addressing mode with current CPU setting
.errnote 011 ESP cannot be used as index
.errnote 012 Too many base/index registers
.errnote 013 Memory offset cannot reference to more than one label
.errnote 014 Offset must be relocatable
.errnote 015 Memory offset expected
.errnote 016 Invalid indirect memory operand
.errnote 017 Cannot mix 16 and 32-bit registers
.errnote 018 CPU type already set
.errnote 019 Unknown directive
.errnote 020 Expecting comma
.errnote 021 Expecting number
.errnote 022 Invalid label definition
.errnote 023 Invalid use of SHORT, NEAR, FAR operator
.errnote 024 No memory
.errnote 025 Cannot use 386 segment register with current CPU setting
.errnote 026 POP CS is not allowed
.errnote 027 Cannot use 386 register with current CPU setting
.errnote 028 Only MOV can use special register
.errnote 029 Cannot use TR3, TR4, TR5 in current CPU setting
.errnote 030 Cannot use SHORT with CALL
.errnote 031 Only SHORT displacement is allowed
.errnote 032 Syntax error
.errnote 033 Prefix must be followed by an instruction
.errnote 034 No size given before 'PTR' operator
.errnote 035 Invalid IMUL format
.errnote 036 Invalid SHLD/SHRD format
.errnote 037 Too many commas
.errnote 038 Syntax error: Unexpected colon
.errnote 039 Operands must be the same size
.errnote 040 Invalid instruction operands
.errnote 041 Immediate constant too large
.errnote 042 Can not use short or near modifiers with this instruction
.errnote 043 Jump out of range
.errnote 044 Displacement cannot be larger than 32k
.errnote 045 Initializer value too large
.errnote 046 Symbol already defined
.errnote 047 Immediate data too large
.errnote 048 Immediate data out of range
.errnote 049 Can not transfer control to stack symbol
.errnote 050 Offset cannot be smaller than WORD size
.errnote 051 Can not take offset of stack symbol
.errnote 052 Can not take segment of stack symbol
.errnote 053 Segment too large
.errnote 054 Offset cannot be larger than 32k
.errnote 055 Operand 2 too big
.errnote 056 Operand 1 too small
.errnote 057 Too many arithmetic operators
.errnote 058 Too many open square brackets
.errnote 059 Too many close square brackets
.errnote 060 Too many open brackets
.errnote 061 Too many close brackets
.errnote 062 Invalid number digit
.errnote 063 Assembler Code is too long
.errnote 064 Brackets are not balanced
.errnote 065 Operator is expected
.errnote 066 Operand is expected
.errnote 067 Too many tokens in a line
.errnote 068 Bracket is expected
.errnote 069 Illegal use of register
.errnote 070 Illegal use of label
.errnote 071 Invalid operand in addition
.errnote 072 Invalid operand in subtraction
.errnote 073 One operand must be constant
.errnote 074 Constant operand is expected
.errnote 075 A constant operand is expected in addition
.errnote 076 A constant operand is expected in subtraction
.errnote 077 A constant operand is expected in multiplication
.errnote 078 A constant operand is expected in division
.errnote 079 A constant operand is expected after a positive sign
.errnote 080 A constant operand is expected after a negative sign
.errnote 081 Label is not defined
.errnote 082 More than one override
.errnote 083 Label is expected
.errnote 084 Only segment or group label is allowed
.errnote 085 Only register or label is expected in override
.errnote 086 Unexpected end of file
.errnote 087 Label is too long
.errnote 088 This feature has not been implemented yet
.errnote 089 Internal Error #1
.errnote 090 Can not take offset of group
.errnote 091 Can not take offset of segment
.errnote 092 Invalid character found
.errnote 093 Invalid operand size for instruction
.errnote 094 This instruction is not supported
.errnote 095 size not specified -- BYTE PTR is assumed
.errnote 096 size not specified -- WORD PTR is assumed
.errnote 097 size not specified -- DWORD PTR is assumed
.*
.errnote 500 Segment parameter is defined already
.errnote 501 Model parameter is defined already
.errnote 502 Syntax error in segment definition
.errnote 503 'AT' is not supported in segment definition
.errnote 504 Segment definition is changed
.errnote 505 Lname is too long
.errnote 506 Block nesting error
.errnote 507 Ends a segment which is not opened
.errnote 508 Segment option is undefined
.errnote 509 Model option is undefined
.errnote 510 No segment is currently opened
.errnote 511 Lname is used already
.errnote 512 Segment is not defined
.errnote 513 Public is not defined
.errnote 514 Colon is expected
.errnote 515 A token is expected after colon
.errnote 516 Invalid qualified type
.errnote 517 Qualified type is expected
.errnote 518 External definition different from previous one
.errnote 519 Memory model is not found in .MODEL
.errnote 520 Cannot open include file
.errnote 521 Name is used already
.errnote 522 Library name is missing
.errnote 523 Segment name is missing
.errnote 524 Group name is missing
.errnote 525 Data emitted with no segment
.errnote 526 Seglocation is expected
.errnote 527 Invalid register
.errnote 528 Cannot address with assumed register
.errnote 529 Invalid start address
.errnote 530 Label is already defined
.errnote 531 Token is too long
.errnote 532 The line is too long after expansion
.errnote 533 A label is expected after colon
.errnote 534 Must be associated with code
.errnote 535 Procedure must have a name
.errnote 536 Procedure is alreadly defined
.errnote 537 Language type must be specified
.errnote 538 End of procedure is not found
.errnote 539 Local variable must immediately follow PROC or MACRO statement
.errnote 540 Extra character found
.errnote 541 Cannot nest procedures
.errnote 542 No procedure is currently defined
.errnote 543 Procedure name does not match
.errnote 544 Vararg requires C calling convention
.errnote 545 Model declared already
.errnote 546 Model is not declared
.errnote 547 Backquote expected
.errnote 548 COMMENT delimiter expected
.errnote 549 End directive required at end of file
.errnote 550 Nesting level too deep
.errnote 551 Symbol not defined
.errnote 552 Insert Stupid warning #1 here
.errnote 553 Insert Stupid warning #2 here
.errnote 554 Spaces not allowed in command line options
.errnote 555 Error:
.errnote 556 Source File
.errnote 557 No filename specified.
.errnote 558 Out of Memory
.errnote 559 Cannot Open File -
.errnote 560 Cannot Close File -
.errnote 561 Cannot Get Start of Source File -
.errnote 562 Cannot Set to Start of Source File -
.errnote 563 Command Line Contains More Than 1 File To Assemble
.errnote 564 include path %s.
.errnote 565 Unknown option %s. Use /? for list of options.
.errnote 566 read more command line from %s.
.errnote 567 Internal error in %s(%u)
.errnote 568 OBJECT WRITE ERROR !!
.errnote 569 NO LOR PHARLAP !!
.errnote 570 Parameter Required
.errnote 571 Expecting closing square bracket
.errnote 572 Expecting file name
.errnote 573 Floating point instruction not allowed with /fpc
.errnote 574 Too many errors
.errnote 575 Build target not recognised
.errnote 576 Public constants should be numeric
.errnote 577 Expecting symbol
.errnote 578 Do not mix simplified and full segment definitions
.errnote 579 Parms passed in multiple registers must be accessed separately, use %s
.errnote 580 Ten byte variables not supported in register calling convention
.errnote 581 Parameter type not recognised
.errnote 582 forced error:
.errnote 583 forced error: Value not equal to 0 : %d
.errnote 584 forced error: Value equal to 0: %d
.errnote 585 forced error: symbol defined: %s
.errnote 586 forced error: symbol not defined: %s
.errnote 587 forced error: string blank : <%s>
.errnote 588 forced error: string not blank : <%s>
.errnote 589 forced error: strings not equal : <%s> : <%s>
.errnote 590 forced error: strings equal : <%s> : <%s>
.errnote 591 included by file %s(%d)
.errnote 592 macro called from file %s(%d)
.errnote 593 Symbol %s not defined
.errnote 594 Extending jump
.errnote 595 Ignoring inapplicable directive
.errnote 596 Unknown symbol class '%s'
.errnote 597 Symbol class for '%s' already established
.errnote 598 number must be a power of 2
.errnote 599 alignment request greater than segment alignment
.errnote 600 '%s' is already defined
.errnote 601 %u unclosed conditional directive(s) detected
.*
.endnote
