        include system.inc
        include cw.inc

; MED 1/21/2003, WASM compatibility change
; only macros left used from the once mighty EQUMAC.INC follow
;*******************************************************************************
;Lazy variable access equates.
;*******************************************************************************
b       equ     byte ptr
w       equ     word ptr
d       equ     dword ptr
f       equ     fword ptr

;*******************************************************************************
;Similar to PROC but it resets the variables needed for ESP local variable and
;stack parameter addressing.
;
;Usage:   PROCS TestProc
;
; Does:   TestProc PROC   and some initialisation.
;
;*******************************************************************************
procs   macro name
;curproc        textequ <name>
curproc textequ name
curproc proc    private
        endm

;*******************************************************************************
;Similar to ENDP but doesn't need a name. It closes a PROCS routine.
;*******************************************************************************
endps   macro
curproc endp
        endm

;*******************************************************************************
;Replacement for PUSH that maintains the stack offset for PARAMS,LOCALS &
;MLOCAL and allows multiple parameters.
;*******************************************************************************
pushs   macro r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16
        irp     x,<r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16> ;REPEAT FOR EACH PARM
        ifnb    <x>
        push    x
        endif
        endm
        endm

;*******************************************************************************
;A replacement for POP that maintains the stack offset for PARAMS,LOCALS &
;MLOCAL and allows multiple parameters. POP's in reverse order.
;*******************************************************************************
pops    macro r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16
        irp     x,<r16,r15,r14,r13,r12,r11,r10,r9,r8,r7,r6,r5,r4,r3,r2,r1> ;REPEAT FOR EACH PARM
        ifnb    <x>
        pop     x
        endif
        endm
        endm

;*******************************************************************************
;Call a C routine with stacked parameters and clean the stack afterwards. Also
;preserves all registers except EAX.
;*******************************************************************************
callc   macro name,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16
        pushs   ebx,ecx,edx,esi,edi,ebp
callsize        = 0
        irp     x,<p16,p15,p14,p13,p12,p11,p10,p9,p8,p7,p6,p5,p4,p3,p2,p1>
        ifnb    <x>
callsize        = callsize+4
        pushs   x
        endif
        endm
        call    name
        if      callsize
        lea     esp,[esp+callsize]
        endif
        pops    ebx,ecx,edx,esi,edi,ebp
        endm

;*******************************************************************************
;Add a memory region to the auto-lock list.
;*******************************************************************************
autolock        macro p1,p2
        extrn __autolock:near
        extrn __autounlock:near
_AUTOLOCKB      segment dword public 'DATA'
_AUTOLOCKB      ends
_AUTOLOCK       segment dword public 'DATA'
        dd p1,p2
_AUTOLOCK       ends
_AUTOLOCKE      segment dword public 'DATA'
_AUTOLOCKE      ends
        endm


;
;Hardware break point table entry structure.
;
HBRK    struc
HBRK_Flags      dw 0    ;padding.
HBRK_Handle     dw 0    ;DPMI break point handle.
HBRK_Address    dd 0    ;Linear break point address.
HBRK_Size       db 0    ;DPMI size code to use.
HBRK_Type       db 0    ;DPMI type code to use.
HBRK    ends


MaxWatches      equ     256


;
;Software watch point table entry structure.
;
WATCH   struc
WATCH_Flags     dd 0
WATCH_Address   dd 0
WATCH_Length    dd 0
WATCH_Check     dd 0
WATCH   ends


        .data

; MED 1/21/2003
; kill need for C2.INC by appropriate external declarations
EXTERN  __psp:WORD, strcpy:NEAR, strcat:NEAR, strlen:NEAR

public PSPSegment,DebugLevel

dLockStart      label byte


PSPSegment      dw ?

public ReqTable

ReqTable        label dword
        dd 0    ;0
        dd 0    ;1
        dd 0    ;2
        dd 0    ;3
        dd 0    ;4
        dd 0    ;5
        dd 0;   REQ_GET_SYS_CONFIG   ;6
        dd REQ_MAP_ADDR         ;7
        dd 0;   REQ_ADDR_INFO        ;8
        dd REQ_CHECKSUM_MEM     ;9
        dd REQ_READ_MEM         ;10
        dd REQ_WRITE_MEM        ;11
        dd REQ_READ_IO          ;12
        dd REQ_WRITE_IO         ;13
        dd REQ_READ_CPU         ;14
        dd REQ_READ_FPU         ;15
        dd REQ_WRITE_CPU        ;16
        dd REQ_WRITE_FPU        ;17
        dd REQ_PROG_GO          ;18
        dd REQ_PROG_STEP        ;19
        dd REQ_PROG_LOAD        ;20
        dd REQ_PROG_KILL        ;21
        dd REQ_SET_WATCH        ;22
        dd REQ_CLEAR_WATCH      ;23
        dd REQ_SET_BREAK        ;24
        dd REQ_CLEAR_BREAK      ;25
        dd REQ_GET_NEXT_ALIAS   ;26
        dd 0                    ;27
        dd 0                    ;28
        dd 0                    ;29
        dd REQ_GET_LIB_NAME     ;30
        dd REQ_GET_ERR_TEXT     ;31
        dd REQ_GET_MESSAGE_TEXT ;32
        dd REQ_REDIRECT_STDIN   ;33
        dd REQ_REDIRECT_STDOUT  ;34
        dd 0    ;35

        dd REQ_READ_REGS        ;36
        dd REQ_WRITE_REGS       ;37
        dd 0;   REQ_MACHINE_DATA     ;38

        dd 0    ;39
        dd 0    ;40
        dd 0    ;41
        dd 0    ;42
        dd 0    ;43
        dd 0    ;44
        dd 0    ;45
        dd 0    ;46
        dd 0    ;47
        dd 0    ;48
        dd 0    ;49
        dd 0    ;50
        dd 0    ;51
        dd 0    ;52
        dd 0    ;53
        dd 0    ;54
        dd 0    ;55
        dd 0    ;56
        dd 0    ;57
        dd 0    ;58
        dd 0    ;59
        dd 0    ;60
        dd 0    ;61
        dd 0    ;62
        dd 0    ;63
        dd 0    ;64
        dd 0    ;65
        dd 0    ;66
        dd 0    ;67
        dd 0    ;68
        dd 0    ;69
        dd 0    ;70
        dd 0    ;71
        dd 0    ;72
        dd 0    ;73
        dd 0    ;74
        dd 0    ;75
        dd 0    ;76
        dd 0    ;77
        dd 0    ;78
        dd 0    ;79
        dd 0    ;80
        dd 0    ;81
        dd 0    ;82
        dd 0    ;83
        dd 0    ;84
        dd 0    ;85
        dd 0    ;86
        dd 0    ;87
        dd 0    ;88
        dd 0    ;89
        dd 0    ;90
        dd 0    ;91
        dd 0    ;92
        dd 0    ;93
        dd 0    ;94
        dd 0    ;95
        dd 0    ;96
        dd 0    ;97
        dd 0    ;98
        dd 0    ;99
        dd 0    ;100
        dd 0    ;101
        dd 0    ;102
        dd 0    ;103
        dd 0    ;104
        dd 0    ;105
        dd 0    ;106
        dd 0    ;107
        dd 0    ;108
        dd 0    ;109
        dd 0    ;110
        dd 0    ;111
        dd 0    ;112
        dd 0    ;113
        dd 0    ;114
        dd 0    ;115
        dd 0    ;116
        dd 0    ;117
        dd 0    ;118
        dd 0    ;119
        dd 0    ;120
        dd 0    ;121
        dd 0    ;122
        dd 0    ;123
        dd 0    ;124
        dd 0    ;125
        dd 0    ;126
        dd 0    ;127

ProgName        db 128 dup (0)
ProgCommand     db 256 dup (0)

DebugPSP        dw 0
DebugSegs       dd 0

DebugRegsStart  label byte
DebugEAX        dd 0
DebugEBX        dd 0
DebugECX        dd 0
DebugEDX        dd 0
DebugESI        dd 0
DebugEDI        dd 0
DebugEBP        dd 0
DebugESP        dd 0
DebugEIP        dd 0
DebugEFL        dd 0
DebugCR0        dd 0
DebugCR2        dd 0
DebugCR3        dd 0
DebugDS         dw 0
DebugES         dw 0
DebugSS         dw 0
DebugCS         dw 0
DebugFS         dw 0
DebugGS         dw 0
DebugRegsEnd    label byte

DebugZero       dw 0

TerminationFlag db 0
TerminateCode   db 0
Executing       db 0
ExceptionFlag   db 0
BreakFlag       db 0
TraceFlag       db 0
DebuggerESP     dd 0
DebuggerSS      dw 0
ExecuteFlags    dd 0
BreakKeyFlag    db 0

HBRKTable       db size HBRK * 4 dup (0)
NumWatches      dd 0
WatchTable      db size WATCH * 256 dup (0)

ErrorNumber     dd 0
ErrorMessage    dd 0
ErrorList       dd 0,ErrorM01,ErrorM02,ErrorM03,ErrorM04,ErrorM05,ErrorM06,ErrorM07
                dd ErrorM08,ErrorM09,ErrorM10,ErrorM11,ErrorM12
ErrorM01        db "DOS reported a file access error",0
ErrorM02        db "Unknown file format",0
ErrorM03        db "Not enough memory",0
ErrorM04        db "Invalid task handle",0
ErrorM05        db "Not enough WATCH table space",0
ErrorM06        db "Function not implemented",0
ErrorM07        db "Divide by zero exception (00h)",0
ErrorM08        db "Stack access exception (0Ch)",0
ErrorM09        db "General protection exception (0Dh)",0
ErrorM10        db "Page access exception (0Eh)",0
ErrorM11        db "Unknown exception",0
ErrorM12        db "Hardware break point triggered",0

LinearAddressCheck db 0

InInt09 db 0
KeyTable        db 128 dup (0)  ;keypress table.


ConfigFile      db "cwhelp.cfg",0
ConfigName      db "cwhelp.cfg", 128 dup (0)
SetupName       db "setup",0
BreakKeyVar     db "BreakKeys",0
BreakKeyList    dd 1dh,38h,0,0,0
ResetTimerVAR   db "ResetTimer",0
ResetTimer      dd 0
DebugVar        db "debug",0
DebugLevel      dd 0
;DebugLevel     dd 1    ; MED 1/20/2003


DebugBuffer     db 256 dup (0)
CarriageReturn  db 13,0
SpaceText       db " ",0
LogFileName     db "cwhelp.log",0
LogFileHandle   dd 0


dLockEnd        label byte

        autolock cLockStart,cLockEnd-cLockStart
        autolock dLockStart,dLockEnd-dLockStart


        .code


cLockStart      label byte


;*******************************************************************************
;
;Convert selector number/offset into real address.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_MAP_ADDR
        local   @@incount:DWORD,@@inaddr:DWORD,@@outaddr:DWORD

; MED 1/20/2003
;        cmp     DebugLevel,0
;        jz      @@pastaddr
;        call    DumpReply2File
;@@pastaddr:

        pushad
;
;Setup new input count/address.
;
        mov     @@incount,ecx
        mov     @@inaddr,esi
        sub     @@incount,1+6+4
        add     @@inaddr,1+6+4
;
;Setup output address and default contents.
;
        mov     @@outaddr,edi
        add     @@outaddr,6+4+4
        mov     eax,[esi+1]
        mov     [edi],eax
        mov     w[edi+4],0
        mov     d[edi+6],0
        mov     d[edi+6+4],0
;
;Check the module handle.
;
        mov     bx,DebugPSP
        sys     GetSelDet32
        cmp     edx,[esi+1+6]       ;right module handle?
        jnz     @@9addr
;
;Convert the selector.
;
        movsx   ebx,w[esi+1+4]
        cmp     ebx,0
        jge     @@NotFlat0
        neg     ebx
@@NotFlat0:
        dec     ebx
        shl     ebx,3
        add     bx,[edx+EPSP_SegBase]
        or      bx,3
        mov     [edi+4],bx              ;set the selector.
;
;Convert the offset.
;
        movsx   ebx,w[esi+1+4]
        cmp     ebx,0
        jge     @@NotFlat1
        neg     ebx
@@NotFlat1:
        dec     ebx
        shl     ebx,3
        add     ebx,DebugSegs           ;point to segment details.
        mov     eax,[ebx]               ;get base offset.
        add     eax,[edx+EPSP_MemBase]
        add     [edi],eax               ;add real address.

; MED 1/23/2003
; horrible hackery to fix offset+code size passed for symbol offset in global vars
        cmp     WORD PTR [esi+1+4],-1
        jge     addrsetb                ; only adjust MAP_FLAT_DATA_SELECTOR
        mov     ecx,DebugSegs
        add     ecx,8
        mov     ecx,[ecx]               ; 2nd, hopefully DGROUP, segment base offset
        add     ecx,65535
        xor     cx,cx                   ; round up to next 64K
        sub     [edi],ecx               ; subtract off rounded up DGROUP offset
addrsetb:

;
;Set the bounds.
;
        mov     d[edi+6],0              ;set low bound.
        mov     eax,[ebx+4]             ;get limit.
        and     eax,0fffffh             ;mask to 20 bits.
        test    d[ebx+4],1 shl 20       ;G bit set?
        jz      @@NoGBitaddr
        shl     eax,12
        or      eax,4095
@@NoGBitaddr:
        or      eax,eax
        jz      @@NoDecLimaddr
        cmp     eax,-1
        jz      @@NoDecLimaddr
        dec     eax
@@NoDecLimaddr: mov     d[edi+6+4],eax  ;set high bound.
;
;Return to caller.
;
@@9addr:
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        ret
        endps


;*******************************************************************************
;
;Check-sum some memory.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_CHECKSUM_MEM
        push    ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
        mov     ebp,ecx
        pop     ecx
        mov     eax,0
        jc      @@9mem
        movzx   eax,w[esi+1+6]          ;get length.
        add     eax,d[esi+1]            ;include the offset.
        cmp     eax,ebp
        jc      @@1mem
        sub     eax,ebp
        sub     w[esi+1+6],ax
        mov     eax,0
        js      @@0mem
        jz      @@0mem
@@1mem: add     edx,[esi+1]             ;get linear address.
        movzx   ebp,w[esi+1+6]          ;get length.
        xor     eax,eax
        xor     ebx,ebx
        push    esi
        mov     esi,edx
@@0mem: cli
        or      LinearAddressCheck,1
        mov     bl,[esi]
        cmp     LinearAddressCheck,0
        mov     LinearAddressCheck,0
        sti
        jz      @@2mem
        add     eax,ebx
        inc     esi
        dec     ebp
        jnz     @@0mem
@@2mem:
        pop     esi
@@9mem: mov     [edi],eax               ;store result.
        add     edi,4
        add     esi,1+6+2
        sub     ecx,1+6+2
        ret
        endps


;*******************************************************************************
;
;Read some memory.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_READ_MEM
        xor     eax,eax
        mov     ax,[esi+1+4]
        or      eax,[esi+1]
        jz      @@0rmem
        push    ecx
        mov     bx,[esi+1+4]
        sys     GetSelDet32
        mov     ebp,ecx
        pop     ecx
        jc      @@0rmem
        movzx   eax,w[esi+1+6]          ;get length.
        add     eax,d[esi+1]            ;include the offset.
        cmp     eax,ebp
        jc      @@1rmem
        sub     eax,ebp
        sub     w[esi+1+6],ax
        js      @@0rmem
        jz      @@0rmem
@@1rmem:
        add     edx,[esi+1]
        pushs   ecx,esi
        movzx   ecx,w[esi+1+6]
        mov     esi,edx
        or      ecx,ecx
@@2rmem:
        jz      @@3rmem
        cli
        mov     LinearAddressCheck,1
        mov     al,[esi]
        cmp     LinearAddressCheck,0
        mov     LinearAddressCheck,0
        sti
        jz      @@3rmem
        mov     [edi],al
        inc     esi
        inc     edi
        dec     ecx
        jmp     @@2rmem
@@3rmem:
        pops    ecx,esi
@@0rmem:
        add     esi,1+6+2
        sub     ecx,1+6+2
        ret
        endps


;*******************************************************************************
;
;Write some memory.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_WRITE_MEM
        xor     eax,eax
        mov     ax,[esi+1+4]
        or      eax,[esi+1]
        jz      @@0wmem
        push    ecx
        mov     bx,w[esi+1+4]
        sys     GetSelDet32
        mov     ebp,ecx
        pop     ecx
        mov     eax,0
        jc      @@0wmem
        mov     eax,ecx                 ;get length.
        sub     eax,1+6
        add     eax,d[esi+1]            ;include the offset.
        cmp     eax,ebp
        jc      @@1wmem
        sub     eax,ebp
        sub     ecx,eax
        mov     eax,0
        js      @@0wmem
        jz      @@0wmem
@@1wmem:
        add     edx,d[esi+1]
        push    edi
        mov     edi,edx
        add     esi,1+6
        sub     ecx,1+6
        xor     eax,eax
        xchg    esi,edi
        or      ecx,ecx
@@2wmem:
        jz      @@3wmem
        cli
        mov     LinearAddressCheck,1
        mov     bl,[esi]
        cmp     LinearAddressCheck,0
        mov     LinearAddressCheck,0
        sti
        jz      @@3wmem
        mov     bl,[edi]
        mov     [esi],bl
        inc     esi
        inc     edi
        inc     eax
        dec     ecx
        jmp     @@2wmem
@@3wmem:
        xchg    esi,edi
        pops    edi
@@0wmem:
        mov     [edi],ax
        add     edi,2
        xor     ecx,ecx
        ret
        endps


;*******************************************************************************
;
;Read from I/O port.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_READ_IO
        mov     edx,[esi+1]             ;get I/O port.
        cmp     b[esi+1+4],1
        jz      @@byteio
        cmp     b[esi+1+4],2
        jz      @@wordio
        cmp     b[esi+1+4],4
        jz      @@dwordio
        jmp     @@9io
;
@@byteio:
        in      al,dx
        mov     [edi],al
        inc     edi
        jmp     @@9io
;
@@wordio:
        in      ax,dx
        mov     [edi],ax
        add     edi,2
        jmp     @@9io
;
@@dwordio:
        in      eax,dx
        mov     [edi],eax
        add     edi,4
;
@@9io:  add     esi,1+4+1
        sub     ecx,1+4+1
        ret
        endps


;*******************************************************************************
;
;Write to I/O port.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_WRITE_IO
        mov     edx,[esi+1]             ;get I/O address.
        add     esi,4
        sub     ecx,4
        cmp     ecx,1
        jz      @@bytewio
        cmp     ecx,2
        jz      @@wordwio
        cmp     ecx,4
        jz      @@dwordwio
        xor     al,al
        jmp     @@9wio
;
@@bytewio:
        mov     al,[esi]
        out     dx,al
        mov     al,1
        jmp     @@9wio
;
@@wordwio:
        mov     ax,[esi]
        out     dx,ax
        mov     al,2
        jmp     @@9wio
;
@@dwordwio:
        mov     eax,[esi]
        out     dx,eax
        mov     al,4
;
@@9wio: mov     [edi],al
        inc     edi
        add     esi,ecx
        xor     ecx,ecx
        ret
        endps


;*******************************************************************************
;
;Get main CPU register contents.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_READ_CPU
        local   @@incount:DWORD,@@inaddr:DWORD,@@outaddr:DWORD

        pushad
;
        inc     esi             ;skip REQ_READ_CPU
        dec     ecx
;
        mov     @@incount,ecx
        mov     @@inaddr,esi
;
;Copy CPU register values.
;
        mov     esi,offset DebugRegsStart
        mov     ecx,offset DebugRegsEnd-DebugRegsStart
        rep     movsb
        mov     @@outaddr,edi
;
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        ret
        endps


;*******************************************************************************
;
;Get FPU register contents.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_READ_FPU
        local   @@incount:DWORD,@@inaddr:DWORD,@@outaddr:DWORD

        pushad
;
        inc     esi             ;skip REQ_READ_FPU
        dec     ecx
;
        mov     @@incount,ecx
        mov     @@inaddr,esi
;
;Get FPU register value.
;
        fsave   [edi]
        frstor [edi]
        fwait
        add     edi,108
        mov     @@outaddr,edi
;
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        ret
        endps


;*******************************************************************************
;
;Set main CPU register contents.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_WRITE_CPU
;
        inc     esi             ;skip REQ_WRITE_CPU
        dec     ecx
;
;Copy CPU register values.
;
        pushs   ecx,edi
        mov     edi,offset DebugRegsStart
        mov     ecx,offset DebugRegsEnd-DebugRegsStart
        rep     movsb
        pops    ecx,edi
;
        sub     ecx,offset DebugRegsEnd-DebugRegsStart
        ret
        endps


;*******************************************************************************
;
;Set FPU register contents.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_WRITE_FPU
;
        inc     esi             ;skip REQ_WRITE_FPU
        dec     ecx
;
;Set FPU register values.
;
        frstor [esi]
        fwait
        add     esi,108
        sub     ecx,108
;
        ret
        endps


;*******************************************************************************
; REQ_READ_REGS processing, follows REQ_READ_CPU, MED 1/20/2003
;*******************************************************************************
        procs   REQ_READ_REGS
;        cmp     DebugLevel,0
;        jz      @@pastrregs
;        call    DumpReply2File
;@@pastrregs:
        call    REQ_READ_CPU
        dec     esi             ; REQ_READ_CPU already incremented esi and
        inc     ecx             ; decremented ecx - don't do it twice
        call    REQ_READ_FPU
        ret
        endps


;*******************************************************************************
; REQ_WRITE_REGS processing, follows REQ_WRITE_CPU, MED 1/20/2003
;*******************************************************************************
        procs   REQ_WRITE_REGS
;        cmp     DebugLevel,0
;        jz      @@pastwregs
;        call    DumpReply2File
;@@pastwregs:
        call    REQ_WRITE_CPU
        dec     esi             ; REQ_READ_CPU already incremented esi and
        inc     ecx             ; decremented ecx - don't do it twice
        call    REQ_WRITE_FPU
        add     edi,132         ; gotta account for XMM regs
        sub     ecx,132
        ret
        endps


;*******************************************************************************
;
;Run the program.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_PROG_GO
        local   @@result:DWORD

        inc     esi
        dec     ecx             ;skip REQ_PROG_GO
        pushad
;
        xor     eax,eax         ;set code for GO.
        call    Execute
        mov     @@result,eax
;
        popad
        mov     eax,DebugESP
        mov     [edi],eax
        add     edi,4
        mov     ax,DebugSS
        mov     [edi],ax
        add     edi,2
        mov     eax,DebugEIP
        mov     [edi],eax
        add     edi,4
        mov     ax,DebugCS
        mov     [edi],ax
        add     edi,2
        mov     eax,@@result
        mov     [edi],ax
        add     edi,2
        ret
        endps


;*******************************************************************************
;
;Run the program.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_PROG_STEP
        local   @@result:DWORD

        inc     esi
        dec     ecx             ;skip REQ_PROG_STEP
        pushad
;
        mov     eax,1           ;set code for STEP.
        call    Execute
        mov     @@result,eax
;
        popad
        mov     eax,DebugESP
        mov     [edi],eax
        add     edi,4
        mov     ax,DebugSS
        mov     [edi],ax
        add     edi,2
        mov     eax,DebugEIP
        mov     [edi],eax
        add     edi,4
        mov     ax,DebugCS
        mov     [edi],ax
        add     edi,2
        mov     eax,@@result
        mov     [edi],ax
        add     edi,2
        ret
        endps


;*******************************************************************************
;
;Load a program ready for debugging.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_PROG_LOAD
        local   @@incount:DWORD,@@inaddr:DWORD,@@outaddr:DWORD

        pushad
        mov     @@outaddr,edi
;
        inc     esi             ;skip REQ_PROG_LOAD
        dec     ecx
;
        inc     esi
        dec     ecx             ;skip argv flag
;
;Get the program name.
;
        mov     edi,offset ProgName
@@0load:
        movsb
        dec     ecx
        cmp     b[esi-1],0
        jnz     @@0load
;
;Get the command line.
;
        mov     edi,offset ProgCommand+1
@@1load:
        or      ecx,ecx
        jz      @@2load
        movsb
        dec     ecx
        cmp     b[esi-1],0
        jnz     @@1load
        mov     b[edi-1]," "
        jmp     @@1load
@@2load:
        cmp     edi,offset ProgCommand+1
        jz      @@5load
        mov     b[edi],13
@@5load:
        sub     edi,offset ProgCommand+1
        mov     eax,edi
        mov     ProgCommand,al  ;set command line length.
        mov     @@incount,ecx
        mov     @@inaddr,esi
;
;Load the program ready for debugging.
;
        mov     edx,offset ProgName
        mov     esi,offset ProgCommand
        xor     cx,cx
        push    ebp             ; save critical register chewed by debug EXEC (fffdh) API
        sys     ExecDebug
        pushs   ds,ds,ds
        pops    es,fs,gs
        jnc     @@3load
        ;
        ;Some sort of error occured so set status.
        ;
        pop     ebp                     ; restore crit ebp value
        mov     edi,@@outaddr
        mov     [edi],eax
        mov     ErrorNumber,eax
        call    SetErrorText
        add     @@outaddr,4+4+4+1
        jmp     @@9load
        ;
        ;Setup initial register values.
        ;
@@3load:
        mov     DebugSegs,ebp
        pop     ebp                     ; restore crit ebp value
        mov     DebugCS,cx
        mov     DebugEIP,edx
        mov     DebugSS,bx
        mov     DebugESP,eax
        mov     DebugPSP,si
        mov     DebugDS,di
        mov     DebugES,si
        pushfd
        pop     eax
        mov     DebugEFL,eax
        mov     DebugEAX,0
        mov     DebugEBX,0
        mov     DebugECX,0
        mov     DebugEDX,0
        mov     DebugESI,0
        mov     DebugEDI,0
        mov     DebugEBP,0
;
;Setup a new transfer buffer to stop debugger interfering.
;
        mov     bx,8192/16
        sys     GetMemDOS
        jc      @@4load
        push    eax
        mov     bx,DebugPSP
        mov     ah,50h
        int     21h
        pop     eax
        mov     bx,ax
        mov     ecx,8192
        sys     SetDOSTrans
        mov     bx,PSPSegment
        mov     ah,50h
        int     21h
;
;Setup results.
;
@@4load:
        mov     edi,@@outaddr
        mov     d[edi],0                ;error code=zero.
        movzx   ebx,DebugPSP
        mov     d[edi+4],ebx            ;task ID=PSP.
        sys     GetSelDet32
        mov     d[edi+8],edx            ;module handle=linear PSP.
        mov     b[edi+12],1+2           ;flags=32-bit+protected
        add     @@outaddr,4+4+4+1
;
;Return results to caller.
;
@@9load:
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        ret
        endps


;*******************************************************************************
;
;Lose a program loaded for debugging.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_PROG_KILL
        mov     ebx,[esi+1]
        add     esi,1+4
        sub     ecx,1+4
        sys     RelSel
        mov     d[edi],0
        jnc     @@0kill
        mov     d[edi],4
        mov     ErrorNumber,4
        call    SetErrorText
@@0kill:        add     edi,4
;
;Reset the timer if required.
;
        cmp     ResetTimer,0
        jz      @@1kill
        pushad
        or      eax,-1
        call    LoadTimer
        popad
;
@@1kill:        ret
        endps


;*******************************************************************************
;
;Set a watch point.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_SET_WATCH
;
;Check if size is OK for HBRK
;
        cmp     b[esi+1+6],1
        jz      @@0watch
        cmp     b[esi+1+6],2
        jz      @@0watch
        cmp     b[esi+1+6],4
        jnz     @@3watch
;
;Size is OK so see if we can find a free entry.
;
@@0watch:
        mov     ebx,offset HBRKTable
        mov     ebp,4
@@1watch:
        test    HBRK_Flags[ebx],1       ;free?
        jz      @@2watch
        add     ebx,size HBRK
        dec     ebp
        jnz     @@1watch
        jmp     @@3watch                ;have to be software watch then.
;
;Fill in hardware break point details.
;
@@2watch:
        mov     HBRK_Flags[ebx],1       ;mark it in use.
        pushs   ebx,ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
        pops    ebx,ecx
        add     edx,[esi+1]             ;include offset.
        mov     HBRK_Address[ebx],edx   ;set linear address of break.
        mov     al,[esi+1+6]
        mov     HBRK_Size[ebx],al       ;set break point size.
        mov     HBRK_Type[ebx],1        ;set type to write.
        mov     d[edi],0                ;clear error field.
        add     edi,4
        mov     d[edi],10+(1 shl 31)
        add     edi,4
        add     esi,1+6+1
        sub     ecx,1+6+1
        jmp     @@9watch
;
;OK, either the size won't work for a HBRK or all HBRK's are in use so set up
;a software WATCH.
;
@@3watch:
        cmp     NumWatches,MaxWatches   ;all watches in use?
        jnz     @@4watch
        ;
        ;No more watches either so return an error.
        ;
@@7watch:
        add     esi,1+6+1
        sub     ecx,1+6+1
        mov     d[edi],5
        add     edi,4
        mov     d[edi],0
        add     edi,4
        mov     ErrorNumber,5
        call    SetErrorText
        jmp     @@9watch
;
;Must be a free WATCH entry so find it.
;
@@4watch:
        mov     ebx,offset WATCHTable
        mov     ebp,MaxWatches
@@5watch:
        test    WATCH_Flags[ebx],1
        jz      @@6watch
        add     ebx,size WATCH
        dec     ebp
        jnz     @@5watch
        jmp     @@7watch                ;can't happen but...
;
;Found next free WATCH so fill in the details.
;
@@6watch:
        mov     WATCH_Flags[ebx],1
        pushs   ebx,ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
        pops    ebx,ecx
        add     edx,[esi+1]             ;include offset.
        mov     WATCH_Address[ebx],edx  ;set linear address of WATCH.
        xor     eax,eax
        mov     al,[esi+1+6]
        mov     WATCH_Length[ebx],eax   ;set WATCH length.
        ;
        ;Need to setup checksum.
        ;
        pushs   esi,edi
        xor     edi,edi
        mov     esi,eax
        xor     eax,eax
@@8watch:       mov     al,[edx]
        add     edi,eax
        inc     edx
        dec     esi
        jnz     @@8watch
        mov     eax,edi
        pops    esi,edi
        mov     WATCH_Check[ebx],eax    ;set check-sum.
;
        inc     NumWatches              ;update WATCH count.
;
;set return details.
;
        mov     d[edi],0                ;clear error field.
        add     edi,4
        mov     d[edi],5000             ;copy DOS4GW slow down value.
        add     edi,4
        add     esi,1+6+1
        sub     ecx,1+6+1
;
;Return to caller.
;
@@9watch:       ret
        endps


;*******************************************************************************
;
;Clear a watch point.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_CLEAR_WATCH
;
;Get the linear address ready for comparison.
;
        push    ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
        pop     ecx
        add     edx,[esi+1]             ;include offset.
        xor     eax,eax
        mov     al,[esi+1+6]            ;get size.
;
;Check all HBRK's
;
        mov     ebx,offset HBRKTable
        mov     ebp,4
@@3cwatch:      test    HBRK_Flags[ebx],1       ;in use?
        jz      @@4cwatch
        cmp     edx,HBRK_Address[ebx]   ;right address?
        jnz     @@4cwatch
        cmp     al,HBRK_Size[ebx]       ;right size?
        jnz     @@4cwatch
        mov     HBRK_Flags[ebx],0       ;free this entry.
        jmp     @@2cwatch
@@4cwatch:      add     ebx,size HBRK
        dec     ebp
        jnz     @@3cwatch
;
;Check all WATCH's.
;
        cmp     NumWatches,0            ;no point if no WATCH's in use.
        jz      @@2cwatch
        mov     ebx,offset WATCHTable
        mov     ebp,MaxWatches
@@0cwatch:
        test    WATCH_Flags[ebx],1      ;in use?
        jz      @@1cwatch
        cmp     edx,WATCH_Address[ebx]  ;right address?
        jnz     @@1cwatch
        cmp     eax,WATCH_Length[ebx]   ;right length?
        jnz     @@1cwatch
        mov     WATCH_Flags[ebx],0      ;clear WATCH.
        dec     NumWatches              ;update number of WATCH's.
        jmp     @@2cwatch
@@1cwatch:      add     ebx,size WATCH
        dec     ebp
        jnz     @@0cwatch
;
@@2cwatch:      add     esi,1+6+1
        sub     ecx,1+6+1
        ret
        endps


;*******************************************************************************
;
;Set a break point.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_SET_BREAK
        inc     esi             ;skip REQ_SET_BREAK
        dec     ecx
;
;Get selector base.
;
        mov     bx,[esi+4]
        push    ecx
        sys     GetSelDet32
        pop     ecx
;
;Include offset.
;
        add     edx,[esi]
;
;Get current value.
;
        xor     eax,eax
        mov     al,[edx]
        mov     [edi],eax
        add     edi,4
;
;Set break point
;
        mov     b[edx],0cch
;
;Update input values.
;
        add     esi,6
        sub     ecx,6
;
        ret
        endps


;*******************************************************************************
;
;Clear a break point.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_CLEAR_BREAK
        inc     esi
        dec     ecx             ;skip REQ_CLEAR_BREAK
;
;Get selector base.
;
        mov     bx,[esi+4]
        push    ecx
        sys     GetSelDet32
        pop     ecx
;
;Include offset.
;
        add     edx,[esi]
;
;Restore value.
;
        mov     al,[esi+6]
        mov     [edx],al
;
;Update input values.
;
        add     esi,6+4
        sub     ecx,6+4
;
        ret
        endps


;*******************************************************************************
;
;Return the alias of a selector.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_GET_NEXT_ALIAS
        movzx   eax,w[esi+1]            ;get alias requested
        add     esi,1+2
        sub     ecx,1+2
        mov     w[edi],0
        add     edi,2
        mov     w[edi],0
        add     edi,2
        ret
        endps


;*******************************************************************************
;
;Return the name of a module.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_GET_LIB_NAME
        add     esi,1+4
        sub     ecx,1+4
        mov     d[edi],0
        add     edi,4
        mov     b[edi],0
        inc     edi
        ret
        endps


;*******************************************************************************
;
;Return the text for an error number.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_GET_ERR_TEXT
        mov     edx,[esi+1]
        mov     edx,[ErrorList+edx*4]
        add     esi,1+4
        sub     ecx,1+4
@@0text:
        mov     al,[edx]
        mov     [edi],al
        inc     edx
        inc     edi
        or      al,al
        jnz     @@0text
        ret
        endps


;*******************************************************************************
;
;Return current message/error text.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_GET_MESSAGE_TEXT
        mov     edx,ErrorMessage
        add     esi,1
        sub     ecx,1
        mov     b[edi],0                ;set flags.
        inc     edi
@@0mtext:       mov     al,[edx]
        mov     [edi],al
        inc     edx
        inc     edi
        or      al,al
        jnz     @@0mtext
        ret
        endps


;*******************************************************************************
;
;Redirect standard input.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_REDIRECT_STDIN
        add     esi,ecx
        xor     ecx,ecx
        mov     d[edi],6
        mov     ErrorNumber,6
        call    SetErrorText
        add     edi,4
        ret
        endps


;*******************************************************************************
;
;Redirect standard output.
;
;On Entry:
;
;ECX    - remaining request bytes.
;ESI    - current request data.
;EDI    - result buffer position.
;
;Returns:
;
;ECX,ESI & EDI updated.
;
;*******************************************************************************
        procs   REQ_REDIRECT_STDOUT
        add     esi,ecx
        xor     ecx,ecx
        mov     d[edi],6
        mov     ErrorNumber,6
        call    SetErrorText
        add     edi,4
        ret
        endps


;*******************************************************************************
;
;Setup ErrorMessage with text for ErrorNumber.
;
;*******************************************************************************
        procs   SetErrorText
        push    eax
        mov     eax,ErrorNumber
        mov     eax,d[ErrorList+eax*4]
        mov     ErrorMessage,eax
        pops    eax
        ret
        endps


;*******************************************************************************
;
;Execute the debugee.
;
;On Entry:
;
;EAX    - mode, 0=go, 1=step.
;
;Returns:
;
;EAX    - status (see REQ_PROG_GO/STEP return flags)
;
;*******************************************************************************
        procs   Execute
        pushs   ebx,ecx,edx,esi,edi,ebp
        mov     ExecuteFlags,eax
;
;Switch to debuggee's PSP.
;
        mov     bx,DebugPSP
        mov     ah,50h
        int     21h
;
;Install hardware break points.
;
        mov     esi,offset HBRKTable
        mov     ebp,4
@@hbrk0:        test    HBRK_Flags[esi],1
        jz      @@hbrk1
        mov     ax,0b00h
        mov     ebx,HBRK_Address[esi]
        mov     cx,bx
        shr     ebx,16
        mov     dl,HBRK_Size[esi]
        mov     dh,HBRK_Type[esi]
        int     31h
        jc      @@hbrk1
        mov     HBRK_Handle[esi],bx
        or      HBRK_Flags[esi],2
        mov     ax,0b03h
        int     31h
@@hbrk1:        add     esi,size HBRK
        dec     ebp
        jnz     @@hbrk0
;
;Force watch point checking if watches are present.
;
        cmp     NumWatches,0
        jz      @@7exec
        or      ExecuteFlags,2  ;force single steping.
;
;Set debuggee trap flag if it's a single instruction trace else clear it if
;not.
;
@@7exec:
        cmp     ExecuteFlags,0
        jz      @@0exec
        or      DebugEFL,256
        jmp     @@11exec
@@0exec:        and     DebugEFL,not 256
;
;Set flags ready for execution.
;
@@11exec:
        mov     Executing,1
        mov     ExceptionFlag,-1
        mov     BreakFlag,0
        mov     TraceFlag,0
        mov     BreakKeyFlag,0
;
;Put return address on the stack.
;
        mov     eax,offset @@backexec   ;store return address for int 3.
        push    eax
        mov     w[DebuggerSS],ss
        mov     d[DebuggerESP],esp
;
;Execute the program.
;
        mov     ss,DebugSS
        mov     esp,DebugESP
        push    d[DebugEFL]
        push    w[DebugZero]
        push    w[DebugCS]
        push    d[DebugEIP]
        mov     eax,DebugEAX
        mov     ebx,DebugEBX
        mov     ecx,DebugECX
        mov     edx,DebugEDX
        mov     esi,DebugESI
        mov     edi,DebugEDI
        mov     ebp,DebugEBP
        mov     gs,DebugGS
        mov     fs,DebugFS
        mov     es,DebugES
        mov     ds,DebugDS
        iretd
;
;Clear execution flag.
;
@@backexec:
        mov     Executing,0
;
;Check if we're single stepping to allow for watches.
;
        test    ExecuteFlags,2
        jz      @@8exec
        cmp     TerminationFlag,0       ;terminated?
        jnz     @@8exec
        cmp     ExceptionFlag,-1        ;exception triggered?
        jnz     @@8exec
        cmp     BreakKeyFlag,0
        jnz     @@8exec
;
;Check state of all watches.
;
        mov     esi,offset WatchTable
        mov     ebp,MaxWatches
@@hbrk6:        test    WATCH_Flags[esi],1      ;in use?
        jz      @@hbrk7
        ;
        ;Check if this watch changed.
        ;
        mov     edi,WATCH_Address[esi]
        mov     ecx,WATCH_Length[esi]
        xor     eax,eax
        xor     ebx,ebx
@@hbrk8:        mov     bl,[edi]
        add     eax,ebx
        inc     edi
        dec     ecx
        jnz     @@hbrk8
        cmp     eax,WATCH_Check[esi]
        jnz     @@10exec                ;signal COND_WATCH
@@hbrk7:        add     esi,size WATCH
        dec     ebp
        jnz     @@hbrk6
;
;Check it wasn't a single step anyway.
;
        test    ExecuteFlags,1  ;single steping anyway?
        jnz     @@8exec
        jmp     @@7exec

;
;Set vars to trigger COND_WATCH
;
@@10exec:       mov     ExceptionFlag,1 ;force trace flag setting.
        or      TraceFlag,-1
;
;Remove HBRK's
;
@@8exec:

        mov     al,20h  ; MED 08/06/96, re-enable interrupts
        out     20h,al

        mov     esi,offset HBRKTable
        mov     ebp,4
@@hbrk4:        test    HBRK_Flags[esi],2
        jz      @@hbrk5
        and     HBRK_Flags[esi],not 2
        mov     bx,HBRK_Handle[esi]
        mov     ax,0b01h
        int     31h
@@hbrk5:        add     esi,size HBRK
        dec     ebp
        jnz     @@hbrk4
;
;Store PSP incase it changed.
;
        mov     ah,62h
        int     21h
        mov     DebugPSP,bx
;
;Switch back to helpers PSP.
;
        mov     bx,PSPSegment
        mov     ah,50h
        int     21h
;
;Now setup return value to reflect why we stopped execution.
;
        xor     eax,eax
        cmp     TerminationFlag,0       ;program terminated?
        jz      @@1exec
        or      eax,1 shl 10            ;COND_TERMINATE
        jmp     @@9exec
@@1exec:        cmp     BreakKeyFlag,0
        jz      @@20exec
        or      eax,1 shl 9             ;COND_USER
        jmp     @@9exec
@@20exec:       cmp     BreakFlag,0             ;break point?
        jz      @@2exec
        or      eax,1 shl 7             ;COND_BREAK
        jmp     @@9exec
@@2exec:        cmp     TraceFlag,0             ;trace point?
        jz      @@3exec
        cmp     ExceptionFlag,1 ;hardware break point?
        jnz     @@5exec
        or      eax,1 shl 8             ;COND_WATCH
        or      eax,1 shl 11            ;COND_EXCEPTION
        mov     ErrorNumber,12
        call    SetErrorText
        jmp     @@9exec
@@5exec:        or      eax,1 shl 6             ;COND_TRACE
        jmp     @@9exec
@@3exec:        cmp     ExceptionFlag,-1        ;exception?
        jz      @@4exec
        or      eax,1 shl 11                    ;COND_EXCEPTION
        mov     ErrorNumber,7
        cmp     ExceptionFlag,0 ;div zero?
        jz      @@12exec
        mov     ErrorNumber,8
        cmp     ExceptionFlag,12        ;stack?
        jz      @@12exec
        mov     ErrorNumber,9
        cmp     ExceptionFlag,13        ;general?
        jz      @@12exec
        mov     ErrorNumber,10
        cmp     ExceptionFlag,14        ;page?
        jz      @@12exec
        mov     ErrorNumber,11
@@12exec:       call    SetErrorText
        jmp     @@9exec
@@4exec:        or      eax,1 shl 8             ;COND_WATCH = dunno!
;
;Return to caller.
;
@@9exec:
        pops    ebx,ecx,edx,esi,edi,ebp
        ret
        endps


;*******************************************************************************
;
;Check if hardware break point executed.
;
;*******************************************************************************
IsHardBreak     proc    near    private
        pushad
        push    ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     esi,offset HBRKTable
        mov     ecx,4
@@0hb:  cmp     HBRK_Flags[esi],1+2
        jnz     @@1hb
        mov     bx,HBRK_Handle[esi]
        mov     ax,0b02h
        int     31h
        jc      @@1hb
        test    ax,1
        jnz     @@8hb
@@1hb:  add     esi,size HBRK
        dec     ecx
        jnz     @@0hb
        or      eax,-1
        jmp     @@10hb
        ;
@@8hb:  xor     eax,eax
        ;
@@10hb: pop     ds
        popad
        ret
IsHardBreak     endp


;*******************************************************************************
;Catch single instruction trace and debug register traps.
;*******************************************************************************
Int01Handler    proc    near    public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Oldi01
        mov     Executing,0
        call    IsHardBreak
        jnz     @@0i01
        mov     ExceptionFlag,1
@@0i01: or      TraceFlag,-1
        mov     ebp,esp         ;make stack addresable.
        ;
        ;Need a stack alias for DPMI.
        ;
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP
        ;
        mov     eax,es:[ebp+(4+4+4+4)+(4+4)]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     eax,es:[ebp+(4+4+4+4)+(4)]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4+4)+(0)]
        mov     DebugEIP,eax
        ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returni01
        mov     es:[ebp+(4+4+4+4)+(0)],eax
        mov     es:w[ebp+(4+4+4+4)+(4)],cs
        and     es:w[ebp+(4+4+4+4)+(4+4)],65535-256
        pops    eax,ebp,ds,es
        iretd
        ;
@@returni01:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
;
@@Oldi01:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldInt01]
        assume ds:DGROUP
OldInt01        df 0
Int01Handler    endp
public OldInt01

;*******************************************************************************
;Catch single step or debug register traps.
;*******************************************************************************
EInt01Handler   proc    near    public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Olde01
        mov     Executing,0
        call    IsHardBreak
        jnz     @@0e01
        mov     ExceptionFlag,1
@@0e01: or      TraceFlag,-1
        mov     ebp,esp         ;make stack addresable.
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP
        ;
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(4+4)+4]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     ax,es:[ebp+(4+4+4)+(4+4+4)+(4)+4]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(0)+4]
        mov     DebugEIP,eax
        ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returne01
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
        pops    eax,ebp,ds,es
        retf
        ;
@@returne01:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
;
@@Olde01:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldEInt01]
        assume ds:DGROUP
OldEInt01       df 0
EInt01Handler   endp
public OldEInt01


;*******************************************************************************
;Catch INT 3's.
;*******************************************************************************
Int03Handler    proc    near    public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Oldi03
        mov     Executing,0
        or      BreakFlag,-1
        mov     ebp,esp         ;make stack addresable.
        ;
        ;Need a stack alias for DPMI.
        ;
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP

; MED 08/15/96
; attempt to allow debugging of hardware interrupt handler
;  which has extra values placed on stack
KernalSS                equ     68h+3                   ;Kernal SS
        mov     ax,ss
        cmp     ax,KernalSS
        jne     med2a
        mov     eax,es:[ebp+(4+4+4+4)+(4+4)+(4+4+4)]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     eax,es:[ebp+(4+4+4+4)+(4)+(4+4+4)]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4+4)+(0)+(4+4+4)]
        mov     DebugEIP,eax
        jmp     med2b
med2a:

        mov     eax,es:[ebp+(4+4+4+4)+(4+4)]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     eax,es:[ebp+(4+4+4+4)+(4)]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4+4)+(0)]
        mov     DebugEIP,eax

med2b:
        cmp     ExceptionFlag,-1
        jnz     @@NoEIPDec
        dec     DebugEIP                ;account for int 3 instruction length.
        cmp     TerminationFlag,-1
        jnz     @@NoEIPDec
        dec     DebugEIP
@@NoEIPDec:
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returni03
        mov     es:[ebp+(4+4+4+4)+(0)],eax
        mov     es:w[ebp+(4+4+4+4)+(4)],cs
        and     es:w[ebp+(4+4+4+4)+(4+4)],65535-256
        pops    eax,ebp,ds,es
        iretd
        ;
@@returni03:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
;
@@Oldi03:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldInt03]
        assume ds:DGROUP
OldInt03        df 0
Int03Handler    endp
public OldInt03


;*******************************************************************************
;Catch break exceptions.
;*******************************************************************************
EInt03Handler   proc    near    public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Olde03
        mov     Executing,0
        or      BreakFlag,-1
        mov     ebp,esp         ;make stack addresable.
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP
        ;
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(4+4)+4]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     ax,es:[ebp+(4+4+4)+(4+4+4)+(4)+4]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(0)+4]
        mov     DebugEIP,eax
        ;
        cmp     ExceptionFlag,-1
        jnz     @@NoEIPDece03
        dec     DebugEIP                ;account for int 3 instruction length.
        cmp     TerminationFlag,-1
        jnz     @@NoEIPDece03
        dec     DebugEIP
@@NoEIPDece03:  ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returne03
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
        pops    eax,ebp,ds,es
        retf
        ;
@@returne03:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
;
@@Olde03:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldEInt03]
        assume ds:DGROUP
OldEInt03       df 0
EInt03Handler   endp
public OldEInt03


;*******************************************************************************
;Catch divide by zero faults.
;*******************************************************************************
Exc00Handler    proc    near public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Olde00
        mov     Executing,0
        mov     ExceptionFlag,0
        mov     ebp,esp                 ;make stack addresable.
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP
        ;
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(4+4)+4]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     ax,es:[ebp+(4+4+4)+(4+4+4)+(4)+4]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(0)+4]
        mov     DebugEIP,eax
        ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returne00
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
        pops    eax,ebp,ds,es
        retf
        ;
@@returne00:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
        ;
@@Olde00:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldExc00]
        assume ds:DGROUP
OldExc00        df 0
Exc00Handler    endp
public OldExc00


;*******************************************************************************
;Catch stack exceptions.
;*******************************************************************************
Exc12Handler    proc    near public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Olde12
        mov     Executing,0
        mov     ExceptionFlag,12
        mov     ebp,esp         ;make stack addresable.
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP
        ;
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(4+4)+4]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     ax,es:[ebp+(4+4+4)+(4+4+4)+(4)+4]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(0)+4]
        mov     DebugEIP,eax
        ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returne12
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
        pops    eax,ebp,ds,es
        retf
        ;
@@returne12:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
        ;
@@Olde12:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldExc12]
        assume ds:DGROUP
OldExc12        df 0
Exc12Handler    endp
public OldExc12

;*******************************************************************************
;Catch general protection faults.
;*******************************************************************************
Exc13Handler    proc    near public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Olde13
        mov     Executing,0
        mov     ExceptionFlag,13
        mov     ebp,esp         ;make stack addresable.
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP
        ;
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(4+4)+4]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     ax,es:[ebp+(4+4+4)+(4+4+4)+(4)+4]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(0)+4]
        mov     DebugEIP,eax
        ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returne13
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
        pops    eax,ebp,ds,es
        retf
        ;
@@returne13:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
        ;
@@Olde13:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldExc13]
        assume ds:DGROUP
OldExc13        df 0
Exc13Handler    endp
public OldExc13

;*******************************************************************************
;Catch page faults.
;*******************************************************************************
Exc14Handler    proc    near    public
        pushs   eax,ebp,ds,es
        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     Executing,0
        jz      @@Olde14
        mov     Executing,0
        mov     ExceptionFlag,14
        mov     ebp,esp                 ;make stack addresable.
        mov     ax,ss
        mov     es,ax
        ;
        ;Retrieve general registers.
        ;
        mov     eax,es:[ebp+4+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,es:[ebp+4+4]
        mov     DebugEBP,eax
        mov     ax,es:[ebp+4]
        mov     DebugDS,ax
        mov     ax,es:[ebp+0]
        mov     DebugES,ax
        mov     DebugFS,fs
        mov     DebugGS,gs
        ;
        ;Fetch origional Flags:CS:EIP
        ;
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(4+4)+4]
        and     eax,not 256
        mov     DebugEFL,eax
        mov     ax,es:[ebp+(4+4+4)+(4+4+4)+(4)+4]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4)+(4+4+4)+(0)+4]
        mov     DebugEIP,eax
        ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset @@returne14
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
        pops    eax,ebp,ds,es
        retf
        ;
@@returne14:    ;Now return control to exec caller.
        ;
        mov     ax,DGROUP
        mov     ds,ax
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        mov     DebugSS,ss
        mov     DebugESP,esp
        lss     esp,f[DebuggerESP]
        ret
        ;
@@Olde14:
        cmp     LinearAddressCheck,0
        jz      @@shite14
        mov     LinearAddressCheck,0
        xor     esi,esi
        pops    eax,ebp,ds,es
        retf
@@shite14:
        pops    eax,ebp,ds,es
        assume ds:nothing
        jmp     cs:f[OldExc14]
        assume ds:DGROUP
OldExc14        df 0
Exc14Handler    endp
public OldExc14

;*******************************************************************************
;INT 21h handler to catch program termination.
;*******************************************************************************
Int21Handler    proc    far     public
        pushfd
        cmp     ah,4ch  ;terminate?
        jnz     @@Oldi21
;
        pushs   ax,bx,ds
        mov     ah,62h
        int     21h
        mov     ds,bx
        lds     bx,ds:[EPSP_ExecCount]
        cmp     b[bx],0
        pops    ax,bx,ds
        jnz     @@Oldi21
;
        popfd
        pushs   ds,eax
        mov     ax,DGROUP
        mov     ds,ax
        mov     TerminationFlag,-1
        mov     eax,esp
        mov     al,ss:[eax]
        mov     TerminateCode,al
        pops    ds,eax
        jmp     Int03Handler
@@Oldi21:       ;
        popfd
        assume ds:nothing
        jmp     cs:f[OldInt21h]
        assume ds:DGROUP
OldInt21h       df 0
Int21Handler    endp
public OldInt21h


;*******************************************************************************
;
;This should recieve ALL keyboard interrupts before anything else gets to see
;them.
;
;*******************************************************************************
BreakChecker    proc    near    public
        pushs   eax,ebx,ebp,ds
        mov     ax,DGROUP
        mov     ds,ax
        inc     InInt09
;
;Update the key table.
;
        in      al,60h          ;get the scan code.
        mov     bl,al
        and     ebx,127         ;isolate scan code.
        add     ebx,offset KeyTable
        and     al,128
        xor     al,128
        mov     [ebx],al                ;set key state.
;
;Check we havn't already been here.
;
        cmp     InInt09,2
        jnc     @@oldbc
;
;Check if anything is running.
;
        cmp     Executing,0
        jz      @@oldbc
;
;Check if our break combination is set.
;
        mov     ebx,offset BreakKeyList
        cmp     d[ebx],0                ;check if any keys in the list.
        jz      @@oldbc
@@6bc:  cmp     d[ebx],0                ;End of the list?
        jz      @@7bc
        mov     eax,d[ebx]              ;Get scan code.
        cmp     b[KeyTable+eax],0
        jz      @@oldbc
        add     ebx,4
        jmp     @@6bc
;
;Check if return CS:EIP & stack belong to the program we're running.
;
@@7bc:  pushad
        mov     ebp,esp
        add     ebp,4+4+4+4+(4*8)+4+4+4
        mov     ecx,d[ebp+4]            ;return CS
        mov     edx,d[ebp]
        ;
        push    es
        mov     es,DebugPSP
        mov     ax,es:[EPSP_SegBase]
        pop     es
        cmp     cx,ax
        jc      @@nopebc
        mov     DebugEIP,edx
        mov     DebugCS,cx
;
;Want to break into the program so swollow this key press.
;
        in      al,61h
        mov     ah,al
        or      al,1 shl 7
        out     61h,al          ;say comming back.
        xchg    ah,al
        out     61h,al          ;code we got.
        ;
        mov     al,32
        out     20h,al          ;re-enable interupts.
;
;Swap the return address for our own.
;
        mov     ebp,esp
        add     ebp,4+4+4+4+(4*8)+4+4+4
        mov     w[ebp+4],cs             ;return CS
        mov     d[ebp],offset @@3bc
        popad
        pops    eax,ebx,ebp,ds
        iretd
;
;We should be running on the applications stack with the applications registers
;now.
;
@@3bc:  pushfd
        pushs   eax,ebp,ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     ebp,esp
        mov     eax,[ebp+4+4]
        mov     DebugEAX,eax
        mov     DebugEBX,ebx
        mov     DebugECX,ecx
        mov     DebugEDX,edx
        mov     DebugESI,esi
        mov     DebugEDI,edi
        mov     eax,[ebp+4]
        mov     DebugEBP,eax
        mov     eax,[ebp]
        mov     DebugDS,ax
        mov     DebugES,es
        mov     DebugFS,fs
        mov     DebugGS,gs
        mov     eax,[ebp+4+4+4]
        mov     DebugEFL,eax
        mov     DebugSS,ss
        mov     DebugESP,esp
        add     DebugESP,4+4+4+4
;
;Return to the debugger exec routine.
;
        mov     ax,DGROUP
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        or      BreakKeyFlag,-1
        mov     Executing,0
        dec     InInt09
;
;Clean up the key table.
;
        mov     ebx,offset BreakKeyList
@@60bc: cmp     d[ebx],0                ;End of the list?
        jz      @@70bc
        mov     eax,d[ebx]              ;Get scan code.
        mov     b[KeyTable+eax],0
        add     ebx,4
        jmp     @@60bc
;
@@70bc: lss     esp,f[DebuggerESP]
        ret
;
@@nopebc:       popad
;
;Pass control to the origional handler.
;
@@oldbc:
        dec     InInt09
        pops    eax,ebx,ebp,ds
        assume ds:nothing
        jmp     cs:f[OldInt09]
        assume ds:DGROUP
OldInt09        df 0
BreakChecker    endp
public OldInt09

;*******************************************************************************
;
;This must be just before cwAPIpatch
;
;*******************************************************************************
cwIdentity      db "CAUSEWAY"
cwMajorVersion  db 0
cwMinorVersion  db 0

public cwMajorVersion

;*******************************************************************************
;
;This intercepts get/set int 9 vector stuff and makes sure BreakChecker stays
;first in the chain. Also monitor exec calls.
;
;*******************************************************************************
Int31Intercept  proc    near    public
        cmp     ax,0205h                ;Set vector?
        jnz     @@notseti31
        cmp     bl,9            ;INT 9?
        jnz     @@oldi31
        pushs   eax,edx,ds
        mov     ax,DGROUP
        mov     ds,ax
        assume ds:_TEXT
        mov     d[OldInt09],edx
        mov     w[OldInt09+4],cx
        assume ds:DGROUP
        pops    eax,edx,ds
        jmp     @@reti31
        ;
@@notseti31:
        cmp     ax,0204h                ;Get vector?
        jnz     @@oldi31
        cmp     bl,9            ;INT 9?
        jnz     @@oldi31
        assume ds:nothing
        mov     edx,cs:d[OldInt09]
        mov     cx,cs:w[OldInt09+4]
        assume ds:DGROUP
        ;
@@reti31:       pushs   eax,ebp,ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     ebp,esp
        add     ebp,4+4+4+4+4
        and     ss:w[ebp],not 1 ;clear carry.
        pops    eax,ebp,ds
        iretd
        ;
@@oldi31:       assume ds:nothing
        jmp     cs:f[OldInt31]
        assume ds:DGROUP
OldInt31        df 0
Int31Intercept  endp
public OldInt31


;*******************************************************************************
;
;Read config file if one exists.
;
;*******************************************************************************
ReadConfig      proc    near    public
;
;Try in the current directory.
;
;       calls   OpenFile,offset ConfigFile,0
        push    0
        push    offset ConfigFile
        call    OpenFile
        add     esp,8

        or      eax,eax
        jz      @@0rc

;       calls   CloseFile,eax
        push    eax
        call    CloseFile
        add     esp,4

        jmp     @@3rc
;
;Get the execution path and use it to find the configuration file.
;
@@0rc:  mov     bx,__psp
        sys     GetSelDet32
        mov     bx,[edx+2ch]
        sys     GetSelDet32
        mov     edi,edx
        or      ecx,-1
        xor     al,al
@@4rc:  repne   scasb
        cmp     b[edi],0
        jnz     @@4rc
        add     edi,2
        mov     esi,edi
        mov     edi,offset ConfigName
        callc   strcpy,edi,esi
        callc   strlen,edi
        mov     esi,edi
        add     edi,eax
@@1rc:  dec     edi
        cmp     esi,edi
        jnc     @@2rc
        cmp     b[edi],"\"
        jnz     @@1rc
@@2rc:  mov     b[edi+1],0
        mov     edi,esi
        mov     esi,offset ConfigFile
        callc   strcat,edi,esi
        ;

;       calls   OpenFile,offset ConfigName,0
        push    0
        push    offset ConfigName
        call    OpenFile
        add     esp,8

        or      eax,eax
        jz      @@9rc

;       calls   CloseFile,eax
        push    eax
        call    CloseFile
        add     esp,4

;
;config file exists so fetch our variables.
;
@@3rc:
;       calls   cfgGetInts,offset ConfigName,offset SetupName,offset BreakKeyVar, offset BreakKeyList, 4
        push    4
        push    offset BreakKeyList
        push    offset BreakKeyVar
        push    offset SetupName
        push    offset ConfigName
        call    cfgGetInts
        add     esp,20

;       calls cfgGetOnOff,offset ConfigName,offset SetupName, offset ResetTimerVar, offset ResetTimer
        push    offset ResetTimer
        push    offset ResetTimerVar
        push    offset SetupName
        push    offset Configname
        call    cfgGetOnOff
        add     esp,16

;       calls   cfgGetOnOff,offset ConfigName,offset SetupName, offset DebugVar, offset DebugLevel
        push    offset DebugLevel
        push    offset DebugVar
        push    offset SetupName
        push    offset ConfigName
        call    cfgGetOnOff
        add     esp,16

;
@@9rc:  ret
ReadConfig      endp


;*******************************************************************************
;
;Loads the timer with value specified.
;
;On Entry:
;
;AX - Value to load timer with.
;
;On Exit:
;
;All registers preserved.
;
;*******************************************************************************
LoadTimer       proc    near    private
        cli
        jmp     @@8lt
@@8lt:  jmp     @@9lt
@@9lt:  push    ax
        mov     al,36h
        out     43h,al
        pop     ax
        jmp     @@0lt
@@0lt:  jmp     @@1lt
@@1lt:  out     40h,al
        mov     al,ah
        jmp     @@2lt
@@2lt:  jmp     @@3lt
@@3lt:  out     40h,al
        jmp     @@4lt
@@4lt:  jmp     @@5lt
@@5lt:  in      al,21h
        and     al,254
        jmp     @@6lt
@@6lt:  jmp     @@7lt
@@7lt:  out     21h,al
        sti
        ret
LoadTimer       endp


;*******************************************************************************
;
;Change the border colour. Provided mainly for simplistic debugging.
;
;On Entry:
;
;AL     - colour to set.
;
;Returns:
;
;ALL registers preserved.
;
;*******************************************************************************
Bord    proc    near    private
        push    ax
        push    dx
        mov     ah,al
        mov     dx,3dah
        in      al,dx
        mov     dl,0c0h
        mov     al,11h
        out     dx,al
        mov     al,ah
        out     dx,al
        mov     al,20h
        out     dx,al
        pop     dx
        pop     ax
        ret
Bord    endp

cLockEnd        label byte


        end
