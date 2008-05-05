.if &wasmfull. eq 0 .do begin
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
.do end
.contents
~.186~b~b~b~b~b~b~b~b~b~b
~.286~b~b~b~b~b~b~b~b~b~b
~.286c~b~b~b~b~b~b~b~b~b
~.286p~b~b~b~b~b~b~b~b~b
~.287~b~b~b~b~b~b~b~b~b~b
~.386~b~b~b~b~b~b~b~b~b~b
~.386p~b~b~b~b~b~b~b~b~b
~.387~b~b~b~b~b~b~b~b~b~b
~.486~b~b~b~b~b~b~b~b~b~b
~.486p~b~b~b~b~b~b~b~b~b
~.586~b~b~b~b~b~b~b~b~b~b
~.586p~b~b~b~b~b~b~b~b~b
~.686~b~b~b~b~b~b~b~b~b~b
~.686p~b~b~b~b~b~b~b~b~b
~.8086~b~b~b~b~b~b~b~b~b
~.8087~b~b~b~b~b~b~b~b~b
aaa~b~b~b~b~b~b~b~b~b~b~b
aad~b~b~b~b~b~b~b~b~b~b~b
aam~b~b~b~b~b~b~b~b~b~b~b
aas~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
abs~b~b~b~b~b~b~b~b~b~b~b
.do end
adc~b~b~b~b~b~b~b~b~b~b~b
add~b~b~b~b~b~b~b~b~b~b~b
addpd~b~b~b~b~b~b~b~b~b
addps~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
addr~b~b~b~b~b~b~b~b~b~b
.do end
addsd~b~b~b~b~b~b~b~b~b
addss~b~b~b~b~b~b~b~b~b
addsubpd~b~b~b~b~b~b
addsubps~b~b~b~b~b~b
ah~b~b~b~b~b~b~b~b~b~b~b~b
al~b~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
alias~b~b~b~b~b~b~b~b~b
align~b~b~b~b~b~b~b~b~b
~.alpha~b~b~b~b~b~b~b~b
.do end
and~b~b~b~b~b~b~b~b~b~b~b
andnpd~b~b~b~b~b~b~b~b
andnps~b~b~b~b~b~b~b~b
andpd~b~b~b~b~b~b~b~b~b
andps~b~b~b~b~b~b~b~b~b
arpl~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
assume~b~b~b~b~b~b~b~b
.do end
ax~b~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
basic~b~b~b~b~b~b~b~b~b
.do end
bh~b~b~b~b~b~b~b~b~b~b~b~b
bl~b~b~b~b~b~b~b~b~b~b~b~b
bound~b~b~b~b~b~b~b~b~b
bp~b~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.break~b~b~b~b~b~b~b~b
.do end
bsf~b~b~b~b~b~b~b~b~b~b~b
bsr~b~b~b~b~b~b~b~b~b~b~b
bswap~b~b~b~b~b~b~b~b~b
bt~b~b~b~b~b~b~b~b~b~b~b~b
btc~b~b~b~b~b~b~b~b~b~b~b
btr~b~b~b~b~b~b~b~b~b~b~b
bts~b~b~b~b~b~b~b~b~b~b~b
bx~b~b~b~b~b~b~b~b~b~b~b~b
byte~b~b~b~b~b~b~b~b~b~b
c~b~b~b~b~b~b~b~b~b~b~b~b~b
call~b~b~b~b~b~b~b~b~b~b
callf~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
casemap~b~b~b~b~b~b~b
catstr~b~b~b~b~b~b~b~b
.do end
cbw~b~b~b~b~b~b~b~b~b~b~b
cdq~b~b~b~b~b~b~b~b~b~b~b
ch~b~b~b~b~b~b~b~b~b~b~b~b
cl~b~b~b~b~b~b~b~b~b~b~b~b
clc~b~b~b~b~b~b~b~b~b~b~b
cld~b~b~b~b~b~b~b~b~b~b~b
clflush~b~b~b~b~b~b~b
cli~b~b~b~b~b~b~b~b~b~b~b
clts~b~b~b~b~b~b~b~b~b~b
cmc~b~b~b~b~b~b~b~b~b~b~b
cmova~b~b~b~b~b~b~b~b~b
cmovae~b~b~b~b~b~b~b~b
cmovb~b~b~b~b~b~b~b~b~b
cmovbe~b~b~b~b~b~b~b~b
cmovc~b~b~b~b~b~b~b~b~b
cmove~b~b~b~b~b~b~b~b~b
cmovg~b~b~b~b~b~b~b~b~b
cmovge~b~b~b~b~b~b~b~b
cmovl~b~b~b~b~b~b~b~b~b
cmovle~b~b~b~b~b~b~b~b
cmovna~b~b~b~b~b~b~b~b
cmovnae~b~b~b~b~b~b~b
cmovnb~b~b~b~b~b~b~b~b
cmovnbe~b~b~b~b~b~b~b
cmovnc~b~b~b~b~b~b~b~b
cmovne~b~b~b~b~b~b~b~b
cmovng~b~b~b~b~b~b~b~b
cmovnge~b~b~b~b~b~b~b
cmovnl~b~b~b~b~b~b~b~b
cmovnle~b~b~b~b~b~b~b
cmovno~b~b~b~b~b~b~b~b
cmovnp~b~b~b~b~b~b~b~b
cmovns~b~b~b~b~b~b~b~b
cmovnz~b~b~b~b~b~b~b~b
cmovo~b~b~b~b~b~b~b~b~b
cmovp~b~b~b~b~b~b~b~b~b
cmovpe~b~b~b~b~b~b~b~b
cmovpo~b~b~b~b~b~b~b~b
cmovs~b~b~b~b~b~b~b~b~b
cmovz~b~b~b~b~b~b~b~b~b
cmp~b~b~b~b~b~b~b~b~b~b~b
cmpeqpd~b~b~b~b~b~b~b
cmpeqps~b~b~b~b~b~b~b
cmpeqsd~b~b~b~b~b~b~b
cmpeqss~b~b~b~b~b~b~b
cmplepd~b~b~b~b~b~b~b
cmpleps~b~b~b~b~b~b~b
cmplesd~b~b~b~b~b~b~b
cmpless~b~b~b~b~b~b~b
cmpltpd~b~b~b~b~b~b~b
cmpltps~b~b~b~b~b~b~b
cmpltsd~b~b~b~b~b~b~b
cmpltss~b~b~b~b~b~b~b
cmpneqpd~b~b~b~b~b~b
cmpneqps~b~b~b~b~b~b
cmpneqsd~b~b~b~b~b~b
cmpneqss~b~b~b~b~b~b
cmpnlepd~b~b~b~b~b~b
cmpnleps~b~b~b~b~b~b
cmpnlesd~b~b~b~b~b~b
cmpnless~b~b~b~b~b~b
cmpnltpd~b~b~b~b~b~b
cmpnltps~b~b~b~b~b~b
cmpnltsd~b~b~b~b~b~b
cmpnltss~b~b~b~b~b~b
cmpordpd~b~b~b~b~b~b
cmpordps~b~b~b~b~b~b
cmpordsd~b~b~b~b~b~b
cmpordss~b~b~b~b~b~b
cmppd~b~b~b~b~b~b~b~b~b
cmpps~b~b~b~b~b~b~b~b~b
cmps~b~b~b~b~b~b~b~b~b~b
cmpsb~b~b~b~b~b~b~b~b~b
cmpsd~b~b~b~b~b~b~b~b~b
cmpss~b~b~b~b~b~b~b~b~b
cmpsw~b~b~b~b~b~b~b~b~b
cmpunordpd~b~b~b~b
cmpunordps~b~b~b~b
cmpunordsd~b~b~b~b
cmpunordss~b~b~b~b
cmpxchg~b~b~b~b~b~b~b
cmpxchg8b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.code~b~b~b~b~b~b~b~b~b
.do end
comisd~b~b~b~b~b~b~b~b
comiss~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
comm~b~b~b~b~b~b~b~b~b~b
comment~b~b~b~b~b~b~b
common~b~b~b~b~b~b~b~b
compact~b~b~b~b~b~b~b
~.const~b~b~b~b~b~b~b~b
~.continue~b~b~b~b~b
.do end
cpuid~b~b~b~b~b~b~b~b~b
cr0~b~b~b~b~b~b~b~b~b~b~b
cr2~b~b~b~b~b~b~b~b~b~b~b
cr3~b~b~b~b~b~b~b~b~b~b~b
cr4~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.cref~b~b~b~b~b~b~b~b~b
.do end
cs~b~b~b~b~b~b~b~b~b~b~b~b
cvtdq2pd~b~b~b~b~b~b
cvtdq2ps~b~b~b~b~b~b
cvtpd2dq~b~b~b~b~b~b
cvtpd2pi~b~b~b~b~b~b
cvtpd2ps~b~b~b~b~b~b
cvtpi2pd~b~b~b~b~b~b
cvtpi2ps~b~b~b~b~b~b
cvtps2dq~b~b~b~b~b~b
cvtps2pd~b~b~b~b~b~b
cvtps2pi~b~b~b~b~b~b
cvtsd2si~b~b~b~b~b~b
cvtsd2ss~b~b~b~b~b~b
cvtsi2sd~b~b~b~b~b~b
cvtsi2ss~b~b~b~b~b~b
cvtss2sd~b~b~b~b~b~b
cvtss2si~b~b~b~b~b~b
cvttpd2dq~b~b~b~b~b
cvttpd2pi~b~b~b~b~b
cvttps2dq~b~b~b~b~b
cvttps2pi~b~b~b~b~b
cvttsd2si~b~b~b~b~b
cvttss2si~b~b~b~b~b
cwd~b~b~b~b~b~b~b~b~b~b~b
cwde~b~b~b~b~b~b~b~b~b~b
cx~b~b~b~b~b~b~b~b~b~b~b~b
daa~b~b~b~b~b~b~b~b~b~b~b
das~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.data~b~b~b~b~b~b~b~b~b
~.data?~b~b~b~b~b~b~b~b
.do end
db~b~b~b~b~b~b~b~b~b~b~b~b
dd~b~b~b~b~b~b~b~b~b~b~b~b
dec~b~b~b~b~b~b~b~b~b~b~b
df~b~b~b~b~b~b~b~b~b~b~b~b
dh~b~b~b~b~b~b~b~b~b~b~b~b
di~b~b~b~b~b~b~b~b~b~b~b~b
div~b~b~b~b~b~b~b~b~b~b~b
divpd~b~b~b~b~b~b~b~b~b
divps~b~b~b~b~b~b~b~b~b
divsd~b~b~b~b~b~b~b~b~b
divss~b~b~b~b~b~b~b~b~b
dl~b~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.dosseg~b~b~b~b~b~b~b
dosseg~b~b~b~b~b~b~b~b
.do end
dp~b~b~b~b~b~b~b~b~b~b~b~b
dq~b~b~b~b~b~b~b~b~b~b~b~b
dr0~b~b~b~b~b~b~b~b~b~b~b
dr1~b~b~b~b~b~b~b~b~b~b~b
dr2~b~b~b~b~b~b~b~b~b~b~b
dr3~b~b~b~b~b~b~b~b~b~b~b
dr6~b~b~b~b~b~b~b~b~b~b~b
dr7~b~b~b~b~b~b~b~b~b~b~b
ds~b~b~b~b~b~b~b~b~b~b~b~b
dt~b~b~b~b~b~b~b~b~b~b~b~b
dup~b~b~b~b~b~b~b~b~b~b~b
dw~b~b~b~b~b~b~b~b~b~b~b~b
dword~b~b~b~b~b~b~b~b~b
dx~b~b~b~b~b~b~b~b~b~b~b~b
eax~b~b~b~b~b~b~b~b~b~b~b
ebp~b~b~b~b~b~b~b~b~b~b~b
ebx~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
echo~b~b~b~b~b~b~b~b~b~b
.do end
ecx~b~b~b~b~b~b~b~b~b~b~b
edi~b~b~b~b~b~b~b~b~b~b~b
edx~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.else~b~b~b~b~b~b~b~b~b
else~b~b~b~b~b~b~b~b~b~b
elseif~b~b~b~b~b~b~b~b
.do end
emms~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
end~b~b~b~b~b~b~b~b~b~b~b
~.endif~b~b~b~b~b~b~b~b
endif~b~b~b~b~b~b~b~b~b
endm~b~b~b~b~b~b~b~b~b~b
endp~b~b~b~b~b~b~b~b~b~b
ends~b~b~b~b~b~b~b~b~b~b
~.endw~b~b~b~b~b~b~b~b~b
.do end
enter~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
eq~b~b~b~b~b~b~b~b~b~b~b~b
equ~b~b~b~b~b~b~b~b~b~b~b
equ2~b~b~b~b~b~b~b~b~b~b
~.err~b~b~b~b~b~b~b~b~b~b
~.errb~b~b~b~b~b~b~b~b~b
~.errdef~b~b~b~b~b~b~b
~.errdif~b~b~b~b~b~b~b
~.errdifi~b~b~b~b~b~b
~.erre~b~b~b~b~b~b~b~b~b
~.erridn~b~b~b~b~b~b~b
~.erridni~b~b~b~b~b~b
~.errnb~b~b~b~b~b~b~b~b
~.errndef~b~b~b~b~b~b
~.errnz~b~b~b~b~b~b~b~b
error~b~b~b~b~b~b~b~b~b
.do end
es~b~b~b~b~b~b~b~b~b~b~b~b
esi~b~b~b~b~b~b~b~b~b~b~b
esp~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
even~b~b~b~b~b~b~b~b~b~b
~.exit~b~b~b~b~b~b~b~b~b
exitm~b~b~b~b~b~b~b~b~b
export~b~b~b~b~b~b~b~b
extern~b~b~b~b~b~b~b~b
externdef~b~b~b~b~b
extrn~b~b~b~b~b~b~b~b~b
.do end
f2xm1~b~b~b~b~b~b~b~b~b
fabs~b~b~b~b~b~b~b~b~b~b
fadd~b~b~b~b~b~b~b~b~b~b
faddp~b~b~b~b~b~b~b~b~b
far~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.fardata~b~b~b~b~b~b
~.fardata?~b~b~b~b~b
farstack~b~b~b~b~b~b
.do end
fbld~b~b~b~b~b~b~b~b~b~b
fbstp~b~b~b~b~b~b~b~b~b
fchs~b~b~b~b~b~b~b~b~b~b
fclex~b~b~b~b~b~b~b~b~b
fcmovb~b~b~b~b~b~b~b~b
fcmovbe~b~b~b~b~b~b~b
fcmove~b~b~b~b~b~b~b~b
fcmovnb~b~b~b~b~b~b~b
fcmovnbe~b~b~b~b~b~b
fcmovne~b~b~b~b~b~b~b
fcmovnu~b~b~b~b~b~b~b
fcmovu~b~b~b~b~b~b~b~b
fcom~b~b~b~b~b~b~b~b~b~b
fcomi~b~b~b~b~b~b~b~b~b
fcomip~b~b~b~b~b~b~b~b
fcomp~b~b~b~b~b~b~b~b~b
fcompp~b~b~b~b~b~b~b~b
fcos~b~b~b~b~b~b~b~b~b~b
fdecstp~b~b~b~b~b~b~b
fdisi~b~b~b~b~b~b~b~b~b
fdiv~b~b~b~b~b~b~b~b~b~b
fdivp~b~b~b~b~b~b~b~b~b
fdivr~b~b~b~b~b~b~b~b~b
fdivrp~b~b~b~b~b~b~b~b
femms~b~b~b~b~b~b~b~b~b
feni~b~b~b~b~b~b~b~b~b~b
ffree~b~b~b~b~b~b~b~b~b
fiadd~b~b~b~b~b~b~b~b~b
ficom~b~b~b~b~b~b~b~b~b
ficomp~b~b~b~b~b~b~b~b
fidiv~b~b~b~b~b~b~b~b~b
fidivr~b~b~b~b~b~b~b~b
fild~b~b~b~b~b~b~b~b~b~b
fimul~b~b~b~b~b~b~b~b~b
fincstp~b~b~b~b~b~b~b
finit~b~b~b~b~b~b~b~b~b
fist~b~b~b~b~b~b~b~b~b~b
fistp~b~b~b~b~b~b~b~b~b
fisttp~b~b~b~b~b~b~b~b
fisub~b~b~b~b~b~b~b~b~b
fisubr~b~b~b~b~b~b~b~b
flat~b~b~b~b~b~b~b~b~b~b
fld~b~b~b~b~b~b~b~b~b~b~b
fld1~b~b~b~b~b~b~b~b~b~b
fldcw~b~b~b~b~b~b~b~b~b
fldenv~b~b~b~b~b~b~b~b
fldenvd~b~b~b~b~b~b~b
fldenvw~b~b~b~b~b~b~b
fldl2e~b~b~b~b~b~b~b~b
fldl2t~b~b~b~b~b~b~b~b
fldlg2~b~b~b~b~b~b~b~b
fldln2~b~b~b~b~b~b~b~b
fldpi~b~b~b~b~b~b~b~b~b
fldz~b~b~b~b~b~b~b~b~b~b
fmul~b~b~b~b~b~b~b~b~b~b
fmulp~b~b~b~b~b~b~b~b~b
fnclex~b~b~b~b~b~b~b~b
fndisi~b~b~b~b~b~b~b~b
fneni~b~b~b~b~b~b~b~b~b
fninit~b~b~b~b~b~b~b~b
fnop~b~b~b~b~b~b~b~b~b~b
fnrstor~b~b~b~b~b~b~b
fnrstord~b~b~b~b~b~b
fnrstorw~b~b~b~b~b~b
fnsave~b~b~b~b~b~b~b~b
fnsaved~b~b~b~b~b~b~b
fnsavew~b~b~b~b~b~b~b
fnstcw~b~b~b~b~b~b~b~b
fnstenv~b~b~b~b~b~b~b
fnstenvd~b~b~b~b~b~b
fnstenvw~b~b~b~b~b~b
fnstsw~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
for~b~b~b~b~b~b~b~b~b~b~b
forc~b~b~b~b~b~b~b~b~b~b
fortran~b~b~b~b~b~b~b
.do end
fpatan~b~b~b~b~b~b~b~b
fprem~b~b~b~b~b~b~b~b~b
fprem1~b~b~b~b~b~b~b~b
fptan~b~b~b~b~b~b~b~b~b
frndint~b~b~b~b~b~b~b
frstor~b~b~b~b~b~b~b~b
frstord~b~b~b~b~b~b~b
frstorw~b~b~b~b~b~b~b
fs~b~b~b~b~b~b~b~b~b~b~b~b
fsave~b~b~b~b~b~b~b~b~b
fsaved~b~b~b~b~b~b~b~b
fsavew~b~b~b~b~b~b~b~b
fscale~b~b~b~b~b~b~b~b
fsetpm~b~b~b~b~b~b~b~b
fsin~b~b~b~b~b~b~b~b~b~b
fsincos~b~b~b~b~b~b~b
fsqrt~b~b~b~b~b~b~b~b~b
fst~b~b~b~b~b~b~b~b~b~b~b
fstcw~b~b~b~b~b~b~b~b~b
fstenv~b~b~b~b~b~b~b~b
fstenvd~b~b~b~b~b~b~b
fstenvw~b~b~b~b~b~b~b
fstp~b~b~b~b~b~b~b~b~b~b
fstsw~b~b~b~b~b~b~b~b~b
fsub~b~b~b~b~b~b~b~b~b~b
fsubp~b~b~b~b~b~b~b~b~b
fsubr~b~b~b~b~b~b~b~b~b
fsubrp~b~b~b~b~b~b~b~b
ftst~b~b~b~b~b~b~b~b~b~b
fucom~b~b~b~b~b~b~b~b~b
fucomi~b~b~b~b~b~b~b~b
fucomip~b~b~b~b~b~b~b
fucomp~b~b~b~b~b~b~b~b
fucompp~b~b~b~b~b~b~b
fwait~b~b~b~b~b~b~b~b~b
fword~b~b~b~b~b~b~b~b~b
fxam~b~b~b~b~b~b~b~b~b~b
fxch~b~b~b~b~b~b~b~b~b~b
fxrstor~b~b~b~b~b~b~b
fxsave~b~b~b~b~b~b~b~b
fxtract~b~b~b~b~b~b~b
fyl2x~b~b~b~b~b~b~b~b~b
fyl2xp1~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
ge~b~b~b~b~b~b~b~b~b~b~b~b
global~b~b~b~b~b~b~b~b
group~b~b~b~b~b~b~b~b~b
.do end
gs~b~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
gt~b~b~b~b~b~b~b~b~b~b~b~b
.do end
haddpd~b~b~b~b~b~b~b~b
haddps~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
high~b~b~b~b~b~b~b~b~b~b
highword~b~b~b~b~b~b
.do end
hlt~b~b~b~b~b~b~b~b~b~b~b
hsubpd~b~b~b~b~b~b~b~b
hsubps~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
huge~b~b~b~b~b~b~b~b~b~b
.do end
idiv~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.if~b~b~b~b~b~b~b~b~b~b~b
if~b~b~b~b~b~b~b~b~b~b~b~b
if1~b~b~b~b~b~b~b~b~b~b~b
if2~b~b~b~b~b~b~b~b~b~b~b
ifb~b~b~b~b~b~b~b~b~b~b~b
ifdef~b~b~b~b~b~b~b~b~b
ifdif~b~b~b~b~b~b~b~b~b
ifdifi~b~b~b~b~b~b~b~b
ife~b~b~b~b~b~b~b~b~b~b~b
ifidn~b~b~b~b~b~b~b~b~b
ifidni~b~b~b~b~b~b~b~b
ifnb~b~b~b~b~b~b~b~b~b~b
ifndef~b~b~b~b~b~b~b~b
ignore~b~b~b~b~b~b~b~b
.do end
imul~b~b~b~b~b~b~b~b~b~b
in~b~b~b~b~b~b~b~b~b~b~b~b
inc~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
include~b~b~b~b~b~b~b
includelib~b~b~b~b
.do end
ins~b~b~b~b~b~b~b~b~b~b~b
insb~b~b~b~b~b~b~b~b~b~b
insd~b~b~b~b~b~b~b~b~b~b
insw~b~b~b~b~b~b~b~b~b~b
int~b~b~b~b~b~b~b~b~b~b~b
into~b~b~b~b~b~b~b~b~b~b
invd~b~b~b~b~b~b~b~b~b~b
invlpg~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
invoke~b~b~b~b~b~b~b~b
.do end
iret~b~b~b~b~b~b~b~b~b~b
iretd~b~b~b~b~b~b~b~b~b
iretdf~b~b~b~b~b~b~b~b
iretf~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
irp~b~b~b~b~b~b~b~b~b~b~b
irpc~b~b~b~b~b~b~b~b~b~b
.do end
ja~b~b~b~b~b~b~b~b~b~b~b~b
jae~b~b~b~b~b~b~b~b~b~b~b
jb~b~b~b~b~b~b~b~b~b~b~b~b
jbe~b~b~b~b~b~b~b~b~b~b~b
jc~b~b~b~b~b~b~b~b~b~b~b~b
jcxz~b~b~b~b~b~b~b~b~b~b
je~b~b~b~b~b~b~b~b~b~b~b~b
jecxz~b~b~b~b~b~b~b~b~b
jg~b~b~b~b~b~b~b~b~b~b~b~b
jge~b~b~b~b~b~b~b~b~b~b~b
jl~b~b~b~b~b~b~b~b~b~b~b~b
jle~b~b~b~b~b~b~b~b~b~b~b
jmp~b~b~b~b~b~b~b~b~b~b~b
jmpf~b~b~b~b~b~b~b~b~b~b
jna~b~b~b~b~b~b~b~b~b~b~b
jnae~b~b~b~b~b~b~b~b~b~b
jnb~b~b~b~b~b~b~b~b~b~b~b
jnbe~b~b~b~b~b~b~b~b~b~b
jnc~b~b~b~b~b~b~b~b~b~b~b
jne~b~b~b~b~b~b~b~b~b~b~b
jng~b~b~b~b~b~b~b~b~b~b~b
jnge~b~b~b~b~b~b~b~b~b~b
jnl~b~b~b~b~b~b~b~b~b~b~b
jnle~b~b~b~b~b~b~b~b~b~b
jno~b~b~b~b~b~b~b~b~b~b~b
jnp~b~b~b~b~b~b~b~b~b~b~b
jns~b~b~b~b~b~b~b~b~b~b~b
jnz~b~b~b~b~b~b~b~b~b~b~b
jo~b~b~b~b~b~b~b~b~b~b~b~b
jp~b~b~b~b~b~b~b~b~b~b~b~b
jpe~b~b~b~b~b~b~b~b~b~b~b
jpo~b~b~b~b~b~b~b~b~b~b~b
js~b~b~b~b~b~b~b~b~b~b~b~b
jz~b~b~b~b~b~b~b~b~b~b~b~b
~.k3d~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
label~b~b~b~b~b~b~b~b~b
.do end
lahf~b~b~b~b~b~b~b~b~b~b
lar~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
large~b~b~b~b~b~b~b~b~b
.do end
lddqu~b~b~b~b~b~b~b~b~b
ldmxcsr~b~b~b~b~b~b~b
lds~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
le~b~b~b~b~b~b~b~b~b~b~b~b
.do end
lea~b~b~b~b~b~b~b~b~b~b~b
leave~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
length~b~b~b~b~b~b~b~b
lengthof~b~b~b~b~b~b
.do end
les~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.lfcond~b~b~b~b~b~b~b
.do end
lfence~b~b~b~b~b~b~b~b
lfs~b~b~b~b~b~b~b~b~b~b~b
lgdt~b~b~b~b~b~b~b~b~b~b
lgs~b~b~b~b~b~b~b~b~b~b~b
lidt~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.list~b~b~b~b~b~b~b~b~b
~.listall~b~b~b~b~b~b
~.listif~b~b~b~b~b~b~b
~.listmacro~b~b~b~b
~.listmacroall~b
.do end
lldt~b~b~b~b~b~b~b~b~b~b
lmsw~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
local~b~b~b~b~b~b~b~b~b
.do end
lock~b~b~b~b~b~b~b~b~b~b
lods~b~b~b~b~b~b~b~b~b~b
lodsb~b~b~b~b~b~b~b~b~b
lodsd~b~b~b~b~b~b~b~b~b
lodsw~b~b~b~b~b~b~b~b~b
loop~b~b~b~b~b~b~b~b~b~b
loopd~b~b~b~b~b~b~b~b~b
loope~b~b~b~b~b~b~b~b~b
looped~b~b~b~b~b~b~b~b
loopew~b~b~b~b~b~b~b~b
loopne~b~b~b~b~b~b~b~b
loopned~b~b~b~b~b~b~b
loopnew~b~b~b~b~b~b~b
loopnz~b~b~b~b~b~b~b~b
loopnzd~b~b~b~b~b~b~b
loopnzw~b~b~b~b~b~b~b
loopw~b~b~b~b~b~b~b~b~b
loopz~b~b~b~b~b~b~b~b~b
loopzd~b~b~b~b~b~b~b~b
loopzw~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
low~b~b~b~b~b~b~b~b~b~b~b
lowword~b~b~b~b~b~b~b
lroffset~b~b~b~b~b~b
.do end
lsl~b~b~b~b~b~b~b~b~b~b~b
lss~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
lt~b~b~b~b~b~b~b~b~b~b~b~b
.do end
ltr~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
macro~b~b~b~b~b~b~b~b~b
mask~b~b~b~b~b~b~b~b~b~b
.do end
maskmovdqu~b~b~b~b
maskmovq~b~b~b~b~b~b
maxpd~b~b~b~b~b~b~b~b~b
maxps~b~b~b~b~b~b~b~b~b
maxsd~b~b~b~b~b~b~b~b~b
maxss~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
medium~b~b~b~b~b~b~b~b
memory~b~b~b~b~b~b~b~b
.do end
mfence~b~b~b~b~b~b~b~b
minpd~b~b~b~b~b~b~b~b~b
minps~b~b~b~b~b~b~b~b~b
minsd~b~b~b~b~b~b~b~b~b
minss~b~b~b~b~b~b~b~b~b
mm0~b~b~b~b~b~b~b~b~b~b~b
mm1~b~b~b~b~b~b~b~b~b~b~b
mm2~b~b~b~b~b~b~b~b~b~b~b
mm3~b~b~b~b~b~b~b~b~b~b~b
mm4~b~b~b~b~b~b~b~b~b~b~b
mm5~b~b~b~b~b~b~b~b~b~b~b
mm6~b~b~b~b~b~b~b~b~b~b~b
mm7~b~b~b~b~b~b~b~b~b~b~b
~.mmx~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
mod~b~b~b~b~b~b~b~b~b~b~b
~.model~b~b~b~b~b~b~b~b
.do end
monitor~b~b~b~b~b~b~b
mov~b~b~b~b~b~b~b~b~b~b~b
movapd~b~b~b~b~b~b~b~b
movaps~b~b~b~b~b~b~b~b
movd~b~b~b~b~b~b~b~b~b~b
movddup~b~b~b~b~b~b~b
movdq2q~b~b~b~b~b~b~b
movdqa~b~b~b~b~b~b~b~b
movdqu~b~b~b~b~b~b~b~b
movhlps~b~b~b~b~b~b~b
movhpd~b~b~b~b~b~b~b~b
movhps~b~b~b~b~b~b~b~b
movlhps~b~b~b~b~b~b~b
movlpd~b~b~b~b~b~b~b~b
movlps~b~b~b~b~b~b~b~b
movmskpd~b~b~b~b~b~b
movmskps~b~b~b~b~b~b
movntdq~b~b~b~b~b~b~b
movnti~b~b~b~b~b~b~b~b
movntpd~b~b~b~b~b~b~b
movntps~b~b~b~b~b~b~b
movntq~b~b~b~b~b~b~b~b
movq~b~b~b~b~b~b~b~b~b~b
movq2dq~b~b~b~b~b~b~b
movs~b~b~b~b~b~b~b~b~b~b
movsb~b~b~b~b~b~b~b~b~b
movsd~b~b~b~b~b~b~b~b~b
movshdup~b~b~b~b~b~b
movsldup~b~b~b~b~b~b
movss~b~b~b~b~b~b~b~b~b
movsw~b~b~b~b~b~b~b~b~b
movsx~b~b~b~b~b~b~b~b~b
movupd~b~b~b~b~b~b~b~b
movups~b~b~b~b~b~b~b~b
movzx~b~b~b~b~b~b~b~b~b
mul~b~b~b~b~b~b~b~b~b~b~b
mulpd~b~b~b~b~b~b~b~b~b
mulps~b~b~b~b~b~b~b~b~b
mulsd~b~b~b~b~b~b~b~b~b
mulss~b~b~b~b~b~b~b~b~b
mwait~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
name~b~b~b~b~b~b~b~b~b~b
ne~b~b~b~b~b~b~b~b~b~b~b~b
.do end
near~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
nearstack~b~b~b~b~b
.do end
neg~b~b~b~b~b~b~b~b~b~b~b
~.no87~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.nocref~b~b~b~b~b~b~b
~.nolist~b~b~b~b~b~b~b
.do end
nop~b~b~b~b~b~b~b~b~b~b~b
not~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
nothing~b~b~b~b~b~b~b
.do end
offset~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
opattr~b~b~b~b~b~b~b~b
option~b~b~b~b~b~b~b~b
.do end
or~b~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
org~b~b~b~b~b~b~b~b~b~b~b
.do end
orpd~b~b~b~b~b~b~b~b~b~b
orps~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
os_dos~b~b~b~b~b~b~b~b
os_os2~b~b~b~b~b~b~b~b
.do end
out~b~b~b~b~b~b~b~b~b~b~b
outs~b~b~b~b~b~b~b~b~b~b
outsb~b~b~b~b~b~b~b~b~b
outsd~b~b~b~b~b~b~b~b~b
outsw~b~b~b~b~b~b~b~b~b
oword~b~b~b~b~b~b~b~b~b
packssdw~b~b~b~b~b~b
packsswb~b~b~b~b~b~b
packuswb~b~b~b~b~b~b
paddb~b~b~b~b~b~b~b~b~b
paddd~b~b~b~b~b~b~b~b~b
paddq~b~b~b~b~b~b~b~b~b
paddsb~b~b~b~b~b~b~b~b
paddsw~b~b~b~b~b~b~b~b
paddusb~b~b~b~b~b~b~b
paddusw~b~b~b~b~b~b~b
paddw~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
page~b~b~b~b~b~b~b~b~b~b
.do end
pand~b~b~b~b~b~b~b~b~b~b
pandn~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
para~b~b~b~b~b~b~b~b~b~b
pascal~b~b~b~b~b~b~b~b
.do end
pause~b~b~b~b~b~b~b~b~b
pavgb~b~b~b~b~b~b~b~b~b
pavgusb~b~b~b~b~b~b~b
pavgw~b~b~b~b~b~b~b~b~b
pcmpeqb~b~b~b~b~b~b~b
pcmpeqd~b~b~b~b~b~b~b
pcmpeqw~b~b~b~b~b~b~b
pcmpgtb~b~b~b~b~b~b~b
pcmpgtd~b~b~b~b~b~b~b
pcmpgtw~b~b~b~b~b~b~b
pextrw~b~b~b~b~b~b~b~b
pf2id~b~b~b~b~b~b~b~b~b
pf2iw~b~b~b~b~b~b~b~b~b
pfacc~b~b~b~b~b~b~b~b~b
pfadd~b~b~b~b~b~b~b~b~b
pfcmpeq~b~b~b~b~b~b~b
pfcmpge~b~b~b~b~b~b~b
pfcmpgt~b~b~b~b~b~b~b
pfmax~b~b~b~b~b~b~b~b~b
pfmin~b~b~b~b~b~b~b~b~b
pfmul~b~b~b~b~b~b~b~b~b
pfnacc~b~b~b~b~b~b~b~b
pfpnacc~b~b~b~b~b~b~b
pfrcp~b~b~b~b~b~b~b~b~b
pfrcpit1~b~b~b~b~b~b
pfrcpit2~b~b~b~b~b~b
pfrsqit1~b~b~b~b~b~b
pfrsqrt~b~b~b~b~b~b~b
pfsub~b~b~b~b~b~b~b~b~b
pfsubr~b~b~b~b~b~b~b~b
pi2fd~b~b~b~b~b~b~b~b~b
pi2fw~b~b~b~b~b~b~b~b~b
pinsrw~b~b~b~b~b~b~b~b
pmaddwd~b~b~b~b~b~b~b
pmaxsw~b~b~b~b~b~b~b~b
pmaxub~b~b~b~b~b~b~b~b
pminsw~b~b~b~b~b~b~b~b
pminub~b~b~b~b~b~b~b~b
pmovmskb~b~b~b~b~b~b
pmulhrw~b~b~b~b~b~b~b
pmulhuw~b~b~b~b~b~b~b
pmulhw~b~b~b~b~b~b~b~b
pmullw~b~b~b~b~b~b~b~b
pmuludq~b~b~b~b~b~b~b
pop~b~b~b~b~b~b~b~b~b~b~b
popa~b~b~b~b~b~b~b~b~b~b
popad~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
popcontext~b~b~b~b
.do end
popf~b~b~b~b~b~b~b~b~b~b
popfd~b~b~b~b~b~b~b~b~b
por~b~b~b~b~b~b~b~b~b~b~b
prefetch~b~b~b~b~b~b
prefetchnta~b~b~b
prefetcht0~b~b~b~b
prefetcht1~b~b~b~b
prefetcht2~b~b~b~b
prefetchw~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
private~b~b~b~b~b~b~b
proc~b~b~b~b~b~b~b~b~b~b
proto~b~b~b~b~b~b~b~b~b
.do end
psadbw~b~b~b~b~b~b~b~b
pshufd~b~b~b~b~b~b~b~b
pshufhw~b~b~b~b~b~b~b
pshuflw~b~b~b~b~b~b~b
pshufw~b~b~b~b~b~b~b~b
pslld~b~b~b~b~b~b~b~b~b
pslldq~b~b~b~b~b~b~b~b
psllq~b~b~b~b~b~b~b~b~b
psllw~b~b~b~b~b~b~b~b~b
psrad~b~b~b~b~b~b~b~b~b
psraw~b~b~b~b~b~b~b~b~b
psrld~b~b~b~b~b~b~b~b~b
psrldq~b~b~b~b~b~b~b~b
psrlq~b~b~b~b~b~b~b~b~b
psrlw~b~b~b~b~b~b~b~b~b
psubb~b~b~b~b~b~b~b~b~b
psubd~b~b~b~b~b~b~b~b~b
psubq~b~b~b~b~b~b~b~b~b
psubsb~b~b~b~b~b~b~b~b
psubsw~b~b~b~b~b~b~b~b
psubusb~b~b~b~b~b~b~b
psubusw~b~b~b~b~b~b~b
psubw~b~b~b~b~b~b~b~b~b
pswapd~b~b~b~b~b~b~b~b
ptr~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
public~b~b~b~b~b~b~b~b
.do end
punpckhbw~b~b~b~b~b
punpckhdq~b~b~b~b~b
punpckhqdq~b~b~b~b
punpckhwd~b~b~b~b~b
punpcklbw~b~b~b~b~b
punpckldq~b~b~b~b~b
punpcklqdq~b~b~b~b
punpcklwd~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
purge~b~b~b~b~b~b~b~b~b
.do end
push~b~b~b~b~b~b~b~b~b~b
pusha~b~b~b~b~b~b~b~b~b
pushad~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
pushcontext~b~b~b
.do end
pushd~b~b~b~b~b~b~b~b~b
pushf~b~b~b~b~b~b~b~b~b
pushfd~b~b~b~b~b~b~b~b
pushw~b~b~b~b~b~b~b~b~b
pword~b~b~b~b~b~b~b~b~b
pxor~b~b~b~b~b~b~b~b~b~b
qword~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.radix~b~b~b~b~b~b~b~b
.do end
rcl~b~b~b~b~b~b~b~b~b~b~b
rcpps~b~b~b~b~b~b~b~b~b
rcpss~b~b~b~b~b~b~b~b~b
rcr~b~b~b~b~b~b~b~b~b~b~b
rdmsr~b~b~b~b~b~b~b~b~b
rdpmc~b~b~b~b~b~b~b~b~b
rdtsc~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
readonly~b~b~b~b~b~b
record~b~b~b~b~b~b~b~b
.do end
rep~b~b~b~b~b~b~b~b~b~b~b
repe~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.repeat~b~b~b~b~b~b~b
repeat~b~b~b~b~b~b~b~b
.do end
repne~b~b~b~b~b~b~b~b~b
repnz~b~b~b~b~b~b~b~b~b
rept~b~b~b~b~b~b~b~b~b~b
repz~b~b~b~b~b~b~b~b~b~b
ret~b~b~b~b~b~b~b~b~b~b~b
retd~b~b~b~b~b~b~b~b~b~b
retf~b~b~b~b~b~b~b~b~b~b
retfd~b~b~b~b~b~b~b~b~b
retn~b~b~b~b~b~b~b~b~b~b
rol~b~b~b~b~b~b~b~b~b~b~b
ror~b~b~b~b~b~b~b~b~b~b~b
rsm~b~b~b~b~b~b~b~b~b~b~b
rsqrtps~b~b~b~b~b~b~b
rsqrtss~b~b~b~b~b~b~b
sahf~b~b~b~b~b~b~b~b~b~b
sal~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.sall~b~b~b~b~b~b~b~b~b
.do end
sar~b~b~b~b~b~b~b~b~b~b~b
sbb~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
sbyte~b~b~b~b~b~b~b~b~b
.do end
scas~b~b~b~b~b~b~b~b~b~b
scasb~b~b~b~b~b~b~b~b~b
scasd~b~b~b~b~b~b~b~b~b
scasw~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
sdword~b~b~b~b~b~b~b~b
.do end
seg~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
segment~b~b~b~b~b~b~b
~.seq~b~b~b~b~b~b~b~b~b~b
.do end
seta~b~b~b~b~b~b~b~b~b~b
setae~b~b~b~b~b~b~b~b~b
setb~b~b~b~b~b~b~b~b~b~b
setbe~b~b~b~b~b~b~b~b~b
setc~b~b~b~b~b~b~b~b~b~b
sete~b~b~b~b~b~b~b~b~b~b
setg~b~b~b~b~b~b~b~b~b~b
setge~b~b~b~b~b~b~b~b~b
setl~b~b~b~b~b~b~b~b~b~b
setle~b~b~b~b~b~b~b~b~b
setna~b~b~b~b~b~b~b~b~b
setnae~b~b~b~b~b~b~b~b
setnb~b~b~b~b~b~b~b~b~b
setnbe~b~b~b~b~b~b~b~b
setnc~b~b~b~b~b~b~b~b~b
setne~b~b~b~b~b~b~b~b~b
setng~b~b~b~b~b~b~b~b~b
setnge~b~b~b~b~b~b~b~b
setnl~b~b~b~b~b~b~b~b~b
setnle~b~b~b~b~b~b~b~b
setno~b~b~b~b~b~b~b~b~b
setnp~b~b~b~b~b~b~b~b~b
setns~b~b~b~b~b~b~b~b~b
setnz~b~b~b~b~b~b~b~b~b
seto~b~b~b~b~b~b~b~b~b~b
setp~b~b~b~b~b~b~b~b~b~b
setpe~b~b~b~b~b~b~b~b~b
setpo~b~b~b~b~b~b~b~b~b
sets~b~b~b~b~b~b~b~b~b~b
setz~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.sfcond~b~b~b~b~b~b~b
.do end
sfence~b~b~b~b~b~b~b~b
sgdt~b~b~b~b~b~b~b~b~b~b
shl~b~b~b~b~b~b~b~b~b~b~b
shld~b~b~b~b~b~b~b~b~b~b
short~b~b~b~b~b~b~b~b~b
shr~b~b~b~b~b~b~b~b~b~b~b
shrd~b~b~b~b~b~b~b~b~b~b
shufpd~b~b~b~b~b~b~b~b
shufps~b~b~b~b~b~b~b~b
si~b~b~b~b~b~b~b~b~b~b~b~b
sidt~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
size~b~b~b~b~b~b~b~b~b~b
sizeof~b~b~b~b~b~b~b~b
.do end
sldt~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
small~b~b~b~b~b~b~b~b~b
.do end
smsw~b~b~b~b~b~b~b~b~b~b
sp~b~b~b~b~b~b~b~b~b~b~b~b
sqrtpd~b~b~b~b~b~b~b~b
sqrtps~b~b~b~b~b~b~b~b
sqrtsd~b~b~b~b~b~b~b~b
sqrtss~b~b~b~b~b~b~b~b
ss~b~b~b~b~b~b~b~b~b~b~b~b
st~b~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.stack~b~b~b~b~b~b~b~b
~.startup~b~b~b~b~b~b
.do end
stc~b~b~b~b~b~b~b~b~b~b~b
std~b~b~b~b~b~b~b~b~b~b~b
stdcall~b~b~b~b~b~b~b
sti~b~b~b~b~b~b~b~b~b~b~b
stmxcsr~b~b~b~b~b~b~b
stos~b~b~b~b~b~b~b~b~b~b
stosb~b~b~b~b~b~b~b~b~b
stosd~b~b~b~b~b~b~b~b~b
stosw~b~b~b~b~b~b~b~b~b
str~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
struc~b~b~b~b~b~b~b~b~b
struct~b~b~b~b~b~b~b~b
.do end
sub~b~b~b~b~b~b~b~b~b~b~b
subpd~b~b~b~b~b~b~b~b~b
subps~b~b~b~b~b~b~b~b~b
subsd~b~b~b~b~b~b~b~b~b
subss~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
subtitle~b~b~b~b~b~b
subttl~b~b~b~b~b~b~b~b
sword~b~b~b~b~b~b~b~b~b
.do end
syscall~b~b~b~b~b~b~b
sysenter~b~b~b~b~b~b
sysexit~b~b~b~b~b~b~b
tbyte~b~b~b~b~b~b~b~b~b
test~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
textequ~b~b~b~b~b~b~b
~.tfcond~b~b~b~b~b~b~b
this~b~b~b~b~b~b~b~b~b~b
tiny~b~b~b~b~b~b~b~b~b~b
title~b~b~b~b~b~b~b~b~b
.do end
tr3~b~b~b~b~b~b~b~b~b~b~b
tr4~b~b~b~b~b~b~b~b~b~b~b
tr5~b~b~b~b~b~b~b~b~b~b~b
tr6~b~b~b~b~b~b~b~b~b~b~b
tr7~b~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
typedef~b~b~b~b~b~b~b
.do end
ucomisd~b~b~b~b~b~b~b
ucomiss~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
union~b~b~b~b~b~b~b~b~b
.do end
unpckhpd~b~b~b~b~b~b
unpckhps~b~b~b~b~b~b
unpcklpd~b~b~b~b~b~b
unpcklps~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.until~b~b~b~b~b~b~b~b
use16~b~b~b~b~b~b~b~b~b
use32~b~b~b~b~b~b~b~b~b
uses~b~b~b~b~b~b~b~b~b~b
vararg~b~b~b~b~b~b~b~b
.do end
verr~b~b~b~b~b~b~b~b~b~b
verw~b~b~b~b~b~b~b~b~b~b
wait~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
watcom_c~b~b~b~b~b~b
.do end
wbinvd~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.while~b~b~b~b~b~b~b~b
width~b~b~b~b~b~b~b~b~b
.do end
word~b~b~b~b~b~b~b~b~b~b
wrmsr~b~b~b~b~b~b~b~b~b
xadd~b~b~b~b~b~b~b~b~b~b
xchg~b~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.xcref~b~b~b~b~b~b~b~b
.do end
xlat~b~b~b~b~b~b~b~b~b~b
xlatb~b~b~b~b~b~b~b~b~b
.if &wasmfull. eq 1 .do begin
~.xlist~b~b~b~b~b~b~b~b
.do end
~.xmm~b~b~b~b~b~b~b~b~b~b
xmm0~b~b~b~b~b~b~b~b~b~b
xmm1~b~b~b~b~b~b~b~b~b~b
~.xmm2~b~b~b~b~b~b~b~b~b
xmm2~b~b~b~b~b~b~b~b~b~b
~.xmm3~b~b~b~b~b~b~b~b~b
xmm3~b~b~b~b~b~b~b~b~b~b
xmm4~b~b~b~b~b~b~b~b~b~b
xmm5~b~b~b~b~b~b~b~b~b~b
xmm6~b~b~b~b~b~b~b~b~b~b
xmm7~b~b~b~b~b~b~b~b~b~b
xor~b~b~b~b~b~b~b~b~b~b~b
xorpd~b~b~b~b~b~b~b~b~b
xorps~b~b~b~b~b~b~b~b~b
.econtents
