        include system.inc
        include cw.inc

.387

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



        .code


cLockStart      label byte


Copyright       db 13,10
        db 79 dup ("Ä"),13,10
        db "CauseWay Trap helper v2.0 Copyright 1995-96 Michael Devore; all rights reserved",13,10
        db 79 dup ("Ä"),13,10
        db 13,10,"$"


;******************************************************************************
;Main entry point. Just has to find the trap file signature and note the
;buffer details before passing control back to real mode.
;******************************************************************************
_ASMMain        proc    near
        public _ASMMain
;        pushs   esi,edi,ebp
        push    esi
        push    edi
        push    ebp

        mov     ax,__psp
        mov     PSPSegment,ax
;
;Read configuration.
;
        call    ReadConfig
;
;Look for trap file signature off vector 6.
;
        mov     esi,6*4         ;point to int 6 vector.
        mov     esi,[esi]               ;retrieve contents.
        mov     al,1
        cmp     d[esi],0deb0deb0h       ;right signature?
        jnz     AM@@exit
;
;Get the buffer address.
;
        mov     eax,d[esi+4]
        mov     ReqAddress,eax
;
;Get the real mode call-back address.
;
        mov     eax,d[esi+8]
        mov     RealCall,eax
        mov     edi,offset RealModeRegs
        mov     Real_IP[edi],ax
        shr     eax,16
        mov     Real_CS[edi],ax
;
;If in debug mode we need to delete the last log file.
;
        cmp     dword DebugLevel,0
        jz      AM@@0
        mov     edx,offset LogFileName
        mov     ah,41h
        int     21h
;
;Patch exception 0 interupt.
;
AM@@0:    mov     bl,0
        sys     GetVect
        assume ds:_TEXT
        mov     d[OldInt00],edx
        mov     w[OldInt00+4],cx
        assume ds:DGROUP
        mov     bl,0
        mov     cx,cs
        mov     edx,offset Int00Handler
        sys     SetVect
;
;Patch debug interupt.
;
        mov     bl,1
        sys     GetVect
        assume ds:_TEXT
        mov     d[OldInt01],edx
        mov     w[OldInt01+4],cx
        assume ds:DGROUP
        mov     bl,1
        mov     cx,cs
        mov     edx,offset Int01Handler
        sys     SetVect
;
;Patch trap interupt.
;
        mov     bl,3
        sys     GetVect
        assume ds:_TEXT
        mov     d[OldInt03],edx
        mov     w[OldInt03+4],cx
        assume ds:DGROUP
        mov     bl,3
        mov     cx,cs
        mov     edx,offset Int03Handler
        sys     SetVect
;
;Patch debug exception.
;
        mov     bl,1
        sys     GetEVect
        assume ds:_TEXT
        mov     d[OldEInt01],edx
        mov     w[OldEInt01+4],cx
        assume ds:DGROUP
        mov     bl,1
        mov     cx,cs
        mov     edx,offset EInt01Handler
        sys     SetEVect
;
;Patch trap exception.
;
        mov     bl,3
        sys     GetEVect
        assume ds:_TEXT
        mov     d[OldEInt03],edx
        mov     w[OldEInt03+4],cx
        assume ds:DGROUP
        mov     bl,3
        mov     cx,cs
        mov     edx,offset EInt03Handler
        sys     SetEVect
;
;Patch exception 12.
;
        mov     bl,12
        sys     GetEVect
        assume ds:_TEXT
        mov     d[OldExc12],edx
        mov     w[OldExc12+4],cx
        assume ds:DGROUP
        mov     bl,12
        mov     cx,cs
        mov     edx,offset Exc12Handler
        sys     SetEVect
;
;Patch exception 13.
;
        mov     bl,13
        sys     GetEVect
        assume ds:_TEXT
        mov     d[OldExc13],edx
        mov     w[OldExc13+4],cx
        assume ds:DGROUP
        mov     bl,13
        mov     cx,cs
        mov     edx,offset Exc13Handler
        sys     SetEVect
;
;Patch exception 14.
;
        mov     bl,14
        sys     GetEVect
        assume ds:_TEXT
        mov     d[OldExc14],edx
        mov     w[OldExc14+4],cx
        assume ds:DGROUP
        mov     bl,14
        mov     cx,cs
        mov     edx,offset Exc14Handler
        sys     SetEVect
;
;Patch int 21h to catch termination.
;
        mov     bl,21h
        sys     GetVect
        assume ds:_TEXT
        mov     d[OldInt21h],edx
        mov     w[OldInt21h+4],cx
        assume ds:DGROUP
        mov     edx,offset Int21Handler
        mov     cx,cs
        mov     bl,21h
        sys     SetVect
;
;Patch INT 9 & INT 31h so we always see keyboard activity
;and can interrupt the program.
;
        mov     bl,9
        sys     GetVect
        assume ds:_TEXT
        mov     d[OldInt09],edx
        mov     w[OldInt09+4],cx
        assume ds:DGROUP
        mov     edx,offset BreakChecker
        mov     cx,cs
        mov     bl,9
        sys     SetVect
        mov     bl,31h
        sys     GetVect
        assume ds:_TEXT
        mov     d[OldInt31],edx
        mov     w[OldInt31+4],cx
        push    es
        mov     es,cx
        mov     ax,es:[edx-2]
        pop     es
        mov     w[cwMajorVersion],ax
        assume ds:DGROUP
        mov     edx,offset Int31Intercept
        mov     cx,cs
        mov     bl,31h
        sys     SetVect
;
;Say hello.
;
        mov     edx,offset Copyright
        mov     ah,9
        int     21h
;
;Pass control to real mode call handler.
;
        call    Dispatcher
;
;Restore INT 21h vector.
;
        assume ds:_TEXT
        mov     edx,d[OldInt21h]
        mov     cx,w[OldInt21h+4]
        assume ds:DGROUP
        mov     bl,21h
        sys     SetVect
;
;Restore int 0 handler.
;
        assume ds:_TEXT
        mov     edx,d[OldInt00]
        mov     cx,w[OldInt00+4]
        assume ds:DGROUP
        mov     bl,0
        sys     SetVect
;
;Restore debug interupt handler.
;
        assume ds:_TEXT
        mov     edx,d[OldInt01]
        mov     cx,w[OldInt01+4]
        assume ds:DGROUP
        mov     bl,1
        sys     SetVect
;
;Restore trap interupt handler.
;
        assume ds:_TEXT
        mov     edx,d[OldInt03]
        mov     cx,w[OldInt03+4]
        assume ds:DGROUP
        mov     bl,3
        sys     SetVect
;
;Restore debug exception handler.
;
        assume ds:_TEXT
        mov     edx,d[OldEInt01]
        mov     cx,w[OldEInt01+4]
        assume ds:DGROUP
        mov     bl,1
        sys     SetEVect
;
;Restore trap exception handler.
;
        assume ds:_TEXT
        mov     edx,d[OldEInt03]
        mov     cx,w[OldEInt03+4]
        assume ds:DGROUP
        mov     bl,3
        sys     SetEVect
;
;Restore exception 12 handler.
;
        assume ds:_TEXT
        mov     edx,d[OldExc12]
        mov     cx,w[OldExc12+4]
        assume ds:DGROUP
        mov     bl,12
        sys     SetEVect
;
;Restore exception 13 handler.
;
        assume ds:_TEXT
        mov     edx,d[OldExc13]
        mov     cx,w[OldExc13+4]
        assume ds:DGROUP
        mov     bl,13
        sys     SetEVect
;
;Restore exception 14 handler.
;
        assume ds:_TEXT
        mov     edx,d[OldExc14]
        mov     cx,w[OldExc14+4]
        assume ds:DGROUP
        mov     bl,14
        sys     SetEVect
;
;Better return an errorlevel of zero.
;
        xor     al,al
;
;Back to caller.
;
AM@@exit:
        pop     ebp
        pop     edi
        pop     esi
;pops    esi,edi,ebp
        ret
_ASMMain        endp


;*******************************************************************************
;Takes care of low-level link to the trap file and dishes out control to
;appropriate functions.
;*******************************************************************************
Dispatcher      proc    near
D@@0:    mov     edi,offset RealModeRegs
        sys     FarCallReal             ;transfer back to real mode.
;
;Copy commands up into the local buffer.
;
        mov     esi,ReqAddress
        mov     edi,offset ReqBuffer
        mov     ecx,[esi+4]             ;get data length.
        mov     esi,[esi]               ;point to data.
        mov     ReqLength,ecx
        rep     movsb
;
;If we're doing debug info dump this request.
;
        cmp     dword DebugLevel,0
        jz      D@@1
        call    DumpRequest2File
;
;Check if this is a get lost message.
;
D@@1:    cmp     b[ReqBuffer],0
        jz      D@@9
;
;Check if any results are needed and skip processing on this pass if they are.
;
        test    b[ReqBuffer],128
        jnz     D@@2
;
;Pass control back to real mode ready for result transfer.
;
        mov     edi,offset RealModeRegs
        sys     FarCallReal
;
;Process specified commands.
;
D@@2:    mov     esi,offset ReqBuffer
        mov     ecx,ReqLength
        mov     edi,ReqAddress
        mov     edi,[edi]
;
;Go through all commands.
;
D@@3:    xor     eax,eax
        mov     al,[esi]                ;get the command number.
        and     al,127
        cmp     d[ReqTable+eax*4],0
        jz      D@@oops
        call    d[ReqTable+eax*4]
        or      ecx,ecx
        jnz     D@@3
        jmp     D@@4
;
;Display request buffer if we don't understand it.
;
D@@oops: cmp     dword DebugLevel,0
        jz      D@@4
        call    DumpRequest
        xor     ah,ah
        int     16h
;
;Set output length.
;
D@@4:    mov     esi,ReqAddress
        sub     edi,[esi]
        mov     [esi+4],edi
;
;Send result to log file.
;
        cmp     dword DebugLevel,0
        jz      D@@0
        call    DumpReply2File
        jmp     D@@0
;
D@@9:    ret
Dispatcher      endp


;*******************************************************************************
;
;Get system configuration info (processor type etc).
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
REQ_GET_SYS_CONFIG         proc near
        local @@incount:dword,@@inaddr:dword,@@outaddr:dword
;        locals @@incount,@@inaddr,@@outaddr

;        pushads
        pushad
;
        inc     esi             ;skip REQ_GET_SYS_CONFIG
        dec     ecx
;
        mov     @@incount,ecx
        mov     @@inaddr,esi
        mov     @@outaddr,edi
;
;Get the main processor type.
;
        mov     edx,esp         ; save current stack
        and     esp,not 3               ; align it to avoid faults

        pushfd          ; save flags

        pushfd
        pushfd
        pop     eax
        xor     eax,40000H              ; flip AC bit
        push    eax
        popfd
        pushfd          ; get new flags
        pop     ecx
        popfd
        xor     eax,ecx         ; see if AC bit has changed
        shr     eax,18
        and     eax,1           ; nonzero if 386
        mov     al,3            ; assume 386
        jne     medCPU

; try to flip ID bit to detect 486 or above
        pushfd
        pushfd
        pop     eax
        xor     eax,200000h
        push    eax
        popfd
        pushfd
        pop     ecx
        popfd
        xor     eax,ecx
        shr     eax,21
        and     eax,1           ; nonzero if 486
        mov     al,4
        jne     medCPU

; 586+, can use the CPUID instruction
        push    edx
        mov     eax,1
.586
        cpuid
        shr     eax,8
        and     eax,0fh
        pop     edx

;       push    ecx             ; restore EFLAGS
;       popfd                   ; ...

medCPU:
        popfd                   ; restore original flags

        mov     esp,edx         ; restore stack pointer
        mov     edi,@@outaddr
        mov     [edi],al
        inc     dword @@outaddr
;
;Get the math co-processor type.
;
;        pushs   ebp             ; save bp
        push    ebp
        sub     eax,eax         ; set initial control word to 0
;        pushs   eax             ; push it on stack
        push    eax
        mov     ebp,esp         ; point to control word
        fninit          ; initialize math coprocessor
        fnstcw [ebp]            ; store control word in memory
        mov     al,0            ; assume no coprocessor present
        mov     ah,[ebp+1]              ; upper byte is 03h if
        cmp     ah,03h          ; coprocessor is present
        jne     GSC@@0             ; exit if no coprocessor present
        mov     al,1            ; assume it is an 8087
        and     d[ebp],NOT 0080h        ; turn interrupts on (IEM=0)
        fldcw   [ebp]           ; load control word
        fdisi                   ; disable interrupts (IEM=1)
        fstcw   [ebp]           ; store control word
        test    d[ebp],080h             ; if IEM=1, then 8087
        jnz     GSC@@0
        finit                   ; use default infinity mode
        fld1                    ; generate infinity by
        fldz                    ;   dividing 1 by 0
        fdiv                    ; ...
        fld     st              ; form negative infinity
        fchs                    ; ...
        fcompp          ; compare +/- infinity
        fstsw   [ebp]           ; equal for 87/287
        fwait                   ; wait fstsw to complete
        mov     eax,[ebp]               ; get NDP control word
        mov     al,2            ; assume 80287
        sahf                    ; store condition bits in flags
        jz      GSC@@0             ; it's 287 if infinities equal
        mov     al,3            ; indicate 80387
GSC@@0:
;    pops    ebp
;        pops    ebp
        pop     ebp
        pop     ebp

        mov     edi,@@outaddr
        mov     [edi],al
        inc     dword @@outaddr
;
;Set OS values.
;
        mov     ah,30h
        int     21h             ;Get DOS version.
        mov     edi,@@outaddr
        mov     [edi],al
        mov     [edi+1],ah
        mov     b[edi+2],7              ;OS_RATIONAL
        add     dword @@outaddr,1+1+1
;
;Set huge shift value.
;
        mov     edi,@@outaddr
        mov     b[edi],12               ;HUGE_SHIFT!
        inc     dword @@outaddr
;
;Return results to caller.
;
;        popads
        popad

        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        rets
REQ_GET_SYS_CONFIG        endp


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
   REQ_MAP_ADDR        proc near
         local @@incount:dword,@@inaddr:dword,@@outaddr:dword
;        locals @@incount,@@inaddr,@@outaddr
;        pushads
        pushad
;
;Setup new input count/address.
;
        mov     @@incount,ecx
        mov     @@inaddr,esi
        sub     dword @@incount,1+6+4
        add     dword @@inaddr,1+6+4
;
;Setup output address and default contents.
;
        mov     @@outaddr,edi
        add     dword @@outaddr,6+4+4
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
        cmp     edx,[esi+1+6]   ;right module handle?
        jnz     MA@@9
;
;Convert the selector.
;
        cmp     w[esi+1+4],-1
        jnz     MA@@0
        mov     w[esi+1+4],1
MA@@0:    cmp     w[esi+1+4],-2
        jnz     MA@@1
        mov     w[esi+1+4],2
;
MA@@1:    movzx   ebx,w[esi+1+4]
        dec     ebx
        shl     ebx,3
        add     bx,[edx+EPSP_SegBase]
        or      bx,3
        mov     [edi+4],bx              ;set the selector.
;
;Convert the offset.
;
        movzx   ebx,w[esi+1+4]
        dec     ebx
        shl     ebx,3
        add     ebx,DebugSegs   ;point to segment details.
        mov     eax,[ebx]               ;get base offset.
        add     eax,[edx+EPSP_MemBase]
        add     [edi],eax               ;add real address.
;
;Set the bounds.
;
        mov     d[edi+6],0              ;set low bound.
        mov     eax,[ebx+4]             ;get limit.
        and     eax,0fffffh             ;mask to 20 bits.
        test    d[ebx+4],1 shl 20       ;G bit set?
        jz      MA@@NoGBit
        shl     eax,12
        or      eax,4095
MA@@NoGBit:       or      eax,eax
        jz      MA@@NoDecLim
        cmp     eax,-1
        jz      MA@@NoDecLim
        dec     eax
MA@@NoDecLim:     mov     d[edi+6+4],eax  ;set high bound.
;
;Return to caller.
;
MA@@9:
        popad
;    popads
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        rets
REQ_MAP_ADDR        endp


;*******************************************************************************
;
;Work out if specified address has big bit set.
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
   REQ_ADDR_INFO        proc near
        movzx   eax,w[esi+1+4]
        .586p
        lar     eax,eax
        .386
        test    eax,00400000h
        mov     eax,1
        jnz     AI@@0
        xor     eax,eax
AI@@0:    mov     b[edi],al
        inc     edi
        add     esi,1+6
        sub     ecx,1+6
        rets
REQ_ADDR_INFO        endp


; MED, 01/24/99
;*******************************************************************************
   REQ_MACHINE_DATA        proc near
        xor     eax,eax
        mov     [edi],eax
        add     edi,4
        mov     eax,-1
        mov     [edi],eax
        add     edi,4
        movzx   eax,w[esi+1+1+4]
        .586p
        lar     eax,eax
        .386
        test    eax,00400000h
        mov     eax,1
        jnz     MD@@0
        xor     eax,eax
MD@@0:
        mov     b[edi],al
        inc     edi
        add     esi,8+1
        sub     ecx,8+1
        rets
REQ_MACHINE_DATA        endp


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
   REQ_CHECKSUM_MEM        proc near
;        pushs   ecx
        push    ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
        mov     ebp,ecx
;        pops    ecx
        pop     ecx
        mov     eax,0
        jc      CM@@9
        movzx   eax,w[esi+1+6]  ;get length.
        add     eax,d[esi+1]            ;include the offset.
        cmp     eax,ebp
        jc      CM@@1
        sub     eax,ebp
        sub     w[esi+1+6],ax
        mov     eax,0
        js      CM@@0
        jz      CM@@0
CM@@1:    add     edx,[esi+1]             ;get linear address.
        movzx   ebp,w[esi+1+6]  ;get length.
        xor     eax,eax
        xor     ebx,ebx
;        pushs   esi
        push    esi
        mov     esi,edx
CM@@0:    cli
        or      byte LinearAddressCheck,1
        mov     bl,[esi]
        cmp     byte LinearAddressCheck,0
        mov     byte LinearAddressCheck,0
        sti
        jz      CM@@2
        add     eax,ebx
        inc     esi
        dec     ebp
        jnz     CM@@0
CM@@2:
;    pops    esi
        pop     esi
CM@@9:    mov     [edi],eax               ;store result.
        add     edi,4
        add     esi,1+6+2
        sub     ecx,1+6+2
        rets
   REQ_CHECKSUM_MEM        endp


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
   REQ_READ_MEM        proc     near
        xor     eax,eax
        mov     ax,[esi+1+4]
        or      eax,[esi+1]
        jz      RM@@0
;        pushs   ecx
        push    ecx
        mov     bx,[esi+1+4]
        sys     GetSelDet32
        mov     ebp,ecx
;        pops    ecx
        pop     ecx
        jc      RM@@0
        movzx   eax,w[esi+1+6]  ;get length.
        add     eax,d[esi+1]            ;include the offset.
        cmp     eax,ebp
        jc      RM@@1
        sub     eax,ebp
        sub     w[esi+1+6],ax
        js      RM@@0
        jz      RM@@0
RM@@1:    add     edx,[esi+1]
;        pushs   ecx,esi
        push    ecx
        push    esi
        movzx   ecx,w[esi+1+6]
        mov     esi,edx
        or      ecx,ecx
RM@@2:    jz      RM@@3
        cli
        mov     byte LinearAddressCheck,1
        mov     al,[esi]
        cmp     byte LinearAddressCheck,0
        mov     byte LinearAddressCheck,0
        sti
        jz      RM@@3
        mov     [edi],al
        inc     esi
        inc     edi
        dec     ecx
        jmp     RM@@2
RM@@3:
;    pops    ecx,esi
        pop     esi
        pop     ecx
RM@@0:    add     esi,1+6+2
        sub     ecx,1+6+2
        rets
   REQ_READ_MEM        endp


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
   REQ_WRITE_MEM        proc near
        xor     eax,eax
        mov     ax,[esi+1+4]
        or      eax,[esi+1]
        jz      WM@@0
;        pushs   ecx
        push    ecx
        mov     bx,w[esi+1+4]
        sys     GetSelDet32
        mov     ebp,ecx
;        pops    ecx
        pop     ecx
        mov     eax,0
        jc      WM@@0
        mov     eax,ecx         ;get length.
        sub     eax,1+6
        add     eax,d[esi+1]            ;include the offset.
        cmp     eax,ebp
        jc      WM@@1
        sub     eax,ebp
        sub     ecx,eax
        mov     eax,0
        js      WM@@0
        jz      WM@@0
WM@@1:    add     edx,d[esi+1]
;        pushs   edi
        push    edi
        mov     edi,edx
        add     esi,1+6
        sub     ecx,1+6
        xor     eax,eax
        xchg    esi,edi
        or      ecx,ecx
WM@@2:    jz      WM@@3
        cli
        mov     byte LinearAddressCheck,1
        mov     bl,[esi]
        cmp     byte LinearAddressCheck,0
        mov     byte LinearAddressCheck,0
        sti
        jz      WM@@3
        mov     bl,[edi]
        mov     [esi],bl
        inc     esi
        inc     edi
        inc     eax
        dec     ecx
        jmp     WM@@2
WM@@3:    xchg    esi,edi
;        pops    edi
        pop     edi
WM@@0:    mov     [edi],ax
        add     edi,2
        xor     ecx,ecx
        rets
   REQ_WRITE_MEM        endp


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
REQ_READ_IO        proc   near
        mov     edx,[esi+1]             ;get I/O port.
        cmp     b[esi+1+4],1
        jz      RIO@@byte
        cmp     b[esi+1+4],2
        jz      RIO@@word
        cmp     b[esi+1+4],4
        jz      RIO@@dword
        jmp     RIO@@9
;
RIO@@byte: in      al,dx
        mov     [edi],al
        inc     edi
        jmp     RIO@@9
;
RIO@@word: in      ax,dx
        mov     [edi],ax
        add     edi,2
        jmp     RIO@@9
;
RIO@@dword:        in      eax,dx
        mov     [edi],eax
        add     edi,4
;
RIO@@9:    add     esi,1+4+1
        sub     ecx,1+4+1
        rets
REQ_READ_IO        endp


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
REQ_WRITE_IO        proc        near
        mov     edx,[esi+1]             ;get I/O address.
        add     esi,4
        sub     ecx,4
        cmp     ecx,1
        jz      WIO@@byte
        cmp     ecx,2
        jz      WIO@@word
        cmp     ecx,4
        jz      WIO@@dword
        xor     al,al
        jmp     WIO@@9
;
WIO@@byte: mov     al,[esi]
        out     dx,al
        mov     al,1
        jmp     WIO@@9
;
WIO@@word: mov     ax,[esi]
        out     dx,ax
        mov     al,2
        jmp     WIO@@9
;
WIO@@dword:        mov     eax,[esi]
        out     dx,eax
        mov     al,4
;
WIO@@9:    mov     [edi],al
        inc     edi
        add     esi,ecx
        xor     ecx,ecx
        rets
REQ_WRITE_IO        endp


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
   REQ_READ_CPU        proc near
        local @@incount:dword,@@inaddr:dword,@@outaddr:dword
;        pushads
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
;        popads
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        rets
   REQ_READ_CPU        endp


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
REQ_READ_FPU        proc   near
        local @@incount:dword,@@inaddr:dword,@@outaddr:dword
;        pushads
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
;        popads
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        rets
REQ_READ_FPU        endp


; MED, 01/24/99
;*******************************************************************************
REQ_READ_REGS        proc near
        local @@incount:dword,@@inaddr:dword,@@outaddr:dword
;        pushads
        pushad

        inc     esi             ;skip REQ_READ_REGS
        dec     ecx

        mov     @@incount,ecx
        mov     @@inaddr,esi

;Copy CPU register values.
        mov     esi,offset DebugRegsStart
        mov     ecx,offset DebugRegsEnd-DebugRegsStart
        rep     movsb
;       mov     @@outaddr,edi

;Get FPU register value.
        fsave   [edi]
        frstor [edi]
        fwait
        add     edi,108
        mov     @@outaddr,edi

;        popads
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        rets
REQ_READ_REGS        endp


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
REQ_WRITE_CPU        proc       near
;
        inc     esi             ;skip REQ_WRITE_CPU
        dec     ecx
;
;Copy CPU register values.
;
;        pushs   ecx,edi
        push    ecx
        push    edi
        mov     edi,offset DebugRegsStart
        mov     ecx,offset DebugRegsEnd-DebugRegsStart
        rep     movsb
;        pops    ecx,edi
        pop     edi
        pop     ecx
;
        sub     ecx,offset DebugRegsEnd-DebugRegsStart
        rets
REQ_WRITE_CPU        endp


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
REQ_WRITE_FPU        proc   near
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
        rets
REQ_WRITE_FPU        endp


; MED, 01/24/99
;*******************************************************************************
REQ_WRITE_REGS        proc      near

        inc     esi             ;skip REQ_WRITE_REGS
        dec     ecx

;Copy CPU register values.
;        pushs   ecx,edi
        push    ecx
        push    edi

        mov     edi,offset DebugRegsStart
        mov     ecx,offset DebugRegsEnd-DebugRegsStart
        rep     movsb
;        pops    ecx,edi
        pop     edi
        pop     ecx

        sub     ecx,offset DebugRegsEnd-DebugRegsStart

;Set FPU register values.
        frstor [esi]
        fwait
        add     esi,108
        sub     ecx,108

        rets
REQ_WRITE_REGS        endp


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
 REQ_PROG_GO        proc        near
        local @@result:dword
        inc     esi
        dec     ecx             ;skip REQ_PROG_GO
;        pushads
        pushad
;
        xor     eax,eax         ;set code for GO.
        call    Execute
        mov     @@result,eax
;
;        popads
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
        rets
 REQ_PROG_GO        endp


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
REQ_PROG_STEP        proc       near
        local @@result:dword
        inc     esi
        dec     ecx             ;skip REQ_PROG_STEP
;        pushads
        pushad
;
        mov     eax,1           ;set code for STEP.
        call    Execute
        mov     @@result,eax
;
;        popads
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
        rets
REQ_PROG_STEP        endp


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
REQ_PROG_LOAD        proc   near
         local @@incount:dword,@@inaddr:dword,@@outaddr:dword
;        locals @@incount,@@inaddr,@@outaddr
;        pushads
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
PL@@0:    movsb
        dec     ecx
        cmp     b[esi-1],0
        jnz     PL@@0
;
;Get the command line.
;
        mov     edi,offset ProgCommand+1
PL@@1:    or      ecx,ecx
        jz      PL@@2
        movsb
        dec     ecx
        cmp     b[esi-1],0
        jnz     PL@@1
        mov     b[edi-1]," "
        jmp     PL@@1
PL@@2:    cmp     edi,offset ProgCommand+1
        jz      PL@@5
        mov     b[edi],13
PL@@5:    sub     edi,offset ProgCommand+1
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
        mov     ax,0fffdh
        int     31h
;        pushs   ds,ds,ds
;        pops    es,fs,gs
        push    ds
        push    ds
        push    ds
        pop     gs
        pop     fs
        pop     es

        jnc     PL@@3
        ;
        ;Some sort of error occured so set status.
        ;
        mov     edi,@@outaddr
        mov     [edi],eax
        mov     ErrorNumber,eax
        call    SetErrorText
        add     dword @@outaddr,4+4+4+1
        jmp     PL@@9
        ;
        ;Setup initial register values.
        ;
PL@@3:    mov     DebugSegs,ebp
        mov     DebugCS,cx
        mov     DebugEIP,edx
        mov     DebugSS,bx
        mov     DebugESP,eax
        mov     DebugPSP,si
        mov     DebugDS,di
        mov     DebugES,si
        pushfd
        pop     eax
        mov     dword DebugEFL,eax
        mov     dword DebugEAX,0
        mov     dword DebugEBX,0
        mov     dword DebugECX,0
        mov     dword DebugEDX,0
        mov     dword DebugESI,0
        mov     dword DebugEDI,0
        mov     dword DebugEBP,0
;
;Setup a new transfer buffer to stop debugger interfering.
;
        mov     bx,8192/16
        sys     GetMemDOS
        jc      PL@@4
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
PL@@4:    mov     edi,@@outaddr
        mov     d[edi],0                ;error code=zero.
        movzx   ebx,DebugPSP
        mov     d[edi+4],ebx            ;task ID=PSP.
        sys     GetSelDet32
        mov     d[edi+8],edx            ;module handle=linear PSP.
        mov     b[edi+12],1+2   ;flags=32-bit+protected
        add     dword @@outaddr,4+4+4+1
;
;Return results to caller.
;
PL@@9:
;    popads
        popad
        mov     ecx,@@incount
        mov     esi,@@inaddr
        mov     edi,@@outaddr
        rets
REQ_PROG_LOAD        endp


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
REQ_PROG_KILL        proc       near
        mov     ebx,[esi+1]
        add     esi,1+4
        sub     ecx,1+4
        sys     RelSel
        mov     d[edi],0
        jnc     PK@@0
        mov     d[edi],4
        mov     dword ErrorNumber,4
        call    SetErrorText
PK@@0:    add     edi,4
;
;Reset the timer if required.
;
        cmp     dword ResetTimer,0
        jz      PK@@1
;        pushads
        pushad
        or      eax,-1
        call    LoadTimer
;        popads
        popad
;
PK@@1:    rets
REQ_PROG_KILL        endp


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
REQ_SET_WATCH        proc       near
;
;Check if size is OK for HBRK
;
        cmp     b[esi+1+6],1
        jz      SW@@0
        cmp     b[esi+1+6],2
        jz      SW@@0
        cmp     b[esi+1+6],4
        jnz     SW@@3
;
;Size is OK so see if we can find a free entry.
;
SW@@0:    mov     ebx,offset HBRKTable
        mov     ebp,4
SW@@1:    test    HBRK_Flags[ebx],1       ;free?
        jz      SW@@2
        add     ebx,size HBRK
        dec     ebp
        jnz     SW@@1
        jmp     SW@@3             ;have to be software watch then.
;
;Fill in hardware break point details.
;
SW@@2:    mov     HBRK_Flags[ebx],1       ;mark it in use.
;        pushs   ebx,ecx
        push    ebx
        push    ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
;        pops    ebx,ecx
        pop     ecx
        pop     ebx
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
        jmp     SW@@9
;
;OK, either the size won't work for a HBRK or all HBRK's are in use so set up
;a software WATCH.
;
SW@@3:    cmp     dword NumWatches,MaxWatches   ;all watches in use?
        jnz     SW@@4
        ;
        ;No more watches either so return an error.
        ;
SW@@7:    add     esi,1+6+1
        sub     ecx,1+6+1
        mov     d[edi],5
        add     edi,4
        mov     d[edi],0
        add     edi,4
        mov     dword ErrorNumber,5
        call    SetErrorText
        jmp     SW@@9
;
;Must be a free WATCH entry so find it.
;
SW@@4:    mov     ebx,offset WATCHTable
        mov     ebp,MaxWatches
SW@@5:    test    WATCH_Flags[ebx],1
        jz      SW@@6
        add     ebx,size WATCH
        dec     ebp
        jnz     SW@@5
        jmp     SW@@7             ;can't happen but...
;
;Found next free WATCH so fill in the details.
;
SW@@6:    mov     WATCH_Flags[ebx],1
;        pushs   ebx,ecx
        push    ebx
        push    ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
;        pops    ebx,ecx
        pop     ecx
        pop     ebx
        add     edx,[esi+1]             ;include offset.
        mov     WATCH_Address[ebx],edx  ;set linear address of WATCH.
        xor     eax,eax
        mov     al,[esi+1+6]
        mov     WATCH_Length[ebx],eax   ;set WATCH length.
        ;
        ;Need to setup checksum.
        ;
;        pushs   esi,edi
        push    esi
        push    edi
        xor     edi,edi
        mov     esi,eax
        xor     eax,eax
SW@@8:    mov     al,[edx]
        add     edi,eax
        inc     edx
        dec     esi
        jnz     SW@@8
        mov     eax,edi
;        pops    esi,edi
        pop     edi
        pop     esi
        mov     WATCH_Check[ebx],eax    ;set check-sum.
;
        inc     dword NumWatches              ;update WATCH count.
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
SW@@9:    rets
REQ_SET_WATCH        endp


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
REQ_CLEAR_WATCH        proc     near
;
;Get the linear address ready for comparison.
;
;        pushs   ecx
        push    ecx
        mov     bx,[esi+1+4]            ;get selector.
        sys     GetSelDet32
;        pops    ecx
        pop     ecx
        add     edx,[esi+1]             ;include offset.
        xor     eax,eax
        mov     al,[esi+1+6]            ;get size.
;
;Check all HBRK's
;
        mov     ebx,offset HBRKTable
        mov     ebp,4
CW@@3:    test    HBRK_Flags[ebx],1       ;in use?
        jz      CW@@4
        cmp     edx,HBRK_Address[ebx]   ;right address?
        jnz     CW@@4
        cmp     al,HBRK_Size[ebx]       ;right size?
        jnz     CW@@4
        mov     HBRK_Flags[ebx],0       ;free this entry.
        jmp     CW@@2
CW@@4:    add     ebx,size HBRK
        dec     ebp
        jnz     CW@@3
;
;Check all WATCH's.
;
        cmp     dword NumWatches,0            ;no point if no WATCH's in use.
        jz      CW@@2
        mov     ebx,offset WATCHTable
        mov     ebp,MaxWatches
CW@@0:    test    WATCH_Flags[ebx],1      ;in use?
        jz      CW@@1
        cmp     edx,WATCH_Address[ebx]  ;right address?
        jnz     CW@@1
        cmp     eax,WATCH_Length[ebx]   ;right length?
        jnz     CW@@1
        mov     WATCH_Flags[ebx],0      ;clear WATCH.
        dec     dword NumWatches              ;update number of WATCH's.
        jmp     CW@@2
CW@@1:    add     ebx,size WATCH
        dec     ebp
        jnz     CW@@0
;
CW@@2:    add     esi,1+6+1
        sub     ecx,1+6+1
        rets
REQ_CLEAR_WATCH        endp


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
REQ_SET_BREAK        proc   near
        inc     esi             ;skip REQ_SET_BREAK
        dec     ecx
;
;Get selector base.
;
        mov     bx,[esi+4]
;        pushs   ecx
        push    ecx
        sys     GetSelDet32
;        pops    ecx
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
        rets
REQ_SET_BREAK        endp


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
REQ_CLEAR_BREAK        proc     near
        inc     esi
        dec     ecx             ;skip REQ_CLEAR_BREAK
;
;Get selector base.
;
        mov     bx,[esi+4]
;        pushs   ecx
        push    ecx
        sys     GetSelDet32
;        pops    ecx
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
        rets
REQ_CLEAR_BREAK        endp


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
REQ_GET_NEXT_ALIAS        proc  near
        movzx   eax,w[esi+1]            ;get alias requested
        add     esi,1+2
        sub     ecx,1+2
        mov     w[edi],0
        add     edi,2
        mov     w[edi],0
        add     edi,2
        rets
REQ_GET_NEXT_ALIAS        endp


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
REQ_GET_LIB_NAME        proc    near
        add     esi,1+4
        sub     ecx,1+4
        mov     d[edi],0
        add     edi,4
        mov     b[edi],0
        inc     edi
        rets
REQ_GET_LIB_NAME        endp


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
REQ_GET_ERR_TEXT        proc    near
        mov     edx,[esi+1]
        mov     edx,[ErrorList+edx*4]
        add     esi,1+4
        sub     ecx,1+4
GE@@0:    mov     al,[edx]
        mov     [edi],al
        inc     edx
        inc     edi
        or      al,al
        jnz     GE@@0
        rets
REQ_GET_ERR_TEXT        endp


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
REQ_GET_MESSAGE_TEXT        proc   near
        mov     edx,ErrorMessage
        add     esi,1
        sub     ecx,1
        mov     b[edi],0                ;set flags.
        inc     edi
GM@@0:    mov     al,[edx]
        mov     [edi],al
        inc     edx
        inc     edi
        or      al,al
        jnz     GM@@0
        rets
REQ_GET_MESSAGE_TEXT        endp


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
REQ_REDIRECT_STDIN        proc
        add     esi,ecx
        xor     ecx,ecx
        mov     d[edi],6
        mov     dword ErrorNumber,6
        call    SetErrorText
        add     edi,4
        rets
REQ_REDIRECT_STDIN        endp


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
REQ_REDIRECT_STDOUT        proc near
        add     esi,ecx
        xor     ecx,ecx
        mov     d[edi],6
        mov     dword ErrorNumber,6
        call    SetErrorText
        add     edi,4
        rets
   REQ_REDIRECT_STDOUT        endp


;*******************************************************************************
;
;Setup ErrorMessage with text for ErrorNumber.
;
;*******************************************************************************
SetErrorText        proc        near
;        pushs   eax
        push    eax
        mov     eax,ErrorNumber
        mov     eax,d[ErrorList+eax*4]
        mov     ErrorMessage,eax
;        pops    eax
        pop     eax
        rets
SetErrorText        endp


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
Execute        proc     near
;        pushs   ebx,ecx,edx,esi,edi,ebp
        push    ebx
        push    ecx
        push    edx
        push    esi
        push    edi
        push    ebp


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
E@@hbrk0:        test    HBRK_Flags[esi],1
        jz      E@@hbrk1
        mov     ax,0b00h
        mov     ebx,HBRK_Address[esi]
        mov     cx,bx
        shr     ebx,16
        mov     dl,HBRK_Size[esi]
        mov     dh,HBRK_Type[esi]
        int     31h
        jc      E@@hbrk1
        mov     HBRK_Handle[esi],bx
        or      HBRK_Flags[esi],2
        mov     ax,0b03h
        int     31h
E@@hbrk1:        add     esi,size HBRK
        dec     ebp
        jnz     E@@hbrk0
;
;Force watch point checking if watches are present.
;
        cmp     dword NumWatches,0
        jz      E@@7
        or      dword ExecuteFlags,2  ;force single steping.
;
;Set debuggee trap flag if it's a single instruction trace else clear it if
;not.
;
E@@7:    cmp     dword ExecuteFlags,0

        jz      E@@0
        or      dword DebugEFL,256
        jmp     E@@11
E@@0:    and     dword DebugEFL,not 256
;
;Set flags ready for execution.
;
E@@11:   mov     byte Executing,1
        mov     byte ExceptionFlag,-1
        mov     byte BreakFlag,0
        mov     byte TraceFlag,0
        mov     byte BreakKeyFlag,0
;
;Put return address on the stack.
;
        mov     eax,offset E@@back       ;store return address for int 3.
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
E@@back: mov     byte Executing,0
;
;Check if we're single stepping to allow for watches.
;
        test    dword ExecuteFlags,2
        jz      E@@8
        cmp     byte TerminationFlag,0       ;terminated?
        jnz     E@@8
        cmp     byte ExceptionFlag,-1        ;exception triggered?
        jnz     E@@8
        cmp     byte BreakKeyFlag,0
        jnz     E@@8
;
;Check state of all watches.
;
        mov     esi,offset WatchTable
        mov     ebp,MaxWatches
E@@hbrk6:        test    WATCH_Flags[esi],1      ;in use?
        jz      E@@hbrk7
        ;
        ;Check if this watch changed.
        ;
        mov     edi,WATCH_Address[esi]
        mov     ecx,WATCH_Length[esi]
        xor     eax,eax
        xor     ebx,ebx
E@@hbrk8:        mov     bl,[edi]
        add     eax,ebx
        inc     edi
        dec     ecx
        jnz     E@@hbrk8
        cmp     eax,WATCH_Check[esi]
        jnz     E@@10            ;signal COND_WATCH
E@@hbrk7:        add     esi,size WATCH
        dec     ebp
        jnz     E@@hbrk6
;
;Check it wasn't a single step anyway.
;
        test    dword ExecuteFlags,1  ;single steping anyway?
        jnz     E@@8
        jmp     E@@7

;
;Set vars to trigger COND_WATCH
;
E@@10:   mov    byte ExceptionFlag,1 ;force trace flag setting.
        or      byte TraceFlag,-1
;
;Remove HBRK's
;
E@@8:

        mov     al,20h  ; MED 08/06/96, re-enable interrupts
        out     20h,al

        mov     esi,offset HBRKTable
        mov     ebp,4
E@@hbrk4:        test    HBRK_Flags[esi],2
        jz      E@@hbrk5
        and     HBRK_Flags[esi],not 2
        mov     bx,HBRK_Handle[esi]
        mov     ax,0b01h
        int     31h
E@@hbrk5:        add     esi,size HBRK
        dec     ebp
        jnz     E@@hbrk4
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
        cmp     byte TerminationFlag,0       ;program terminated?
        jz      E@@1
        or      eax,1 shl 10            ;COND_TERMINATE
        jmp     E@@9
E@@1:    cmp    byte  BreakKeyFlag,0
        jz      E@@20
        or      eax,1 shl 9             ;COND_USER
        jmp     E@@9
E@@20:   cmp    byte  BreakFlag,0             ;break point?
        jz      E@@2
        or      eax,1 shl 7             ;COND_BREAK
        jmp     E@@9
E@@2:    cmp    byte TraceFlag,0             ;trace point?
        jz      E@@3
        cmp     byte ExceptionFlag,1 ;hardware break point?
        jnz     E@@5
        or      eax,1 shl 8             ;COND_WATCH
        or      eax,1 shl 11            ;COND_EXCEPTION
        mov     dword ErrorNumber,12
        call    SetErrorText
        jmp     E@@9
E@@5:    or      eax,1 shl 6             ;COND_TRACE
        jmp     E@@9
E@@3:    cmp    byte  ExceptionFlag,-1        ;exception?
        jz      E@@4
        or      eax,1 shl 11            ;COND_EXCEPTION
        mov     dword ErrorNumber,7
        cmp     byte ExceptionFlag,0 ;div zero?
        jz      E@@12
        mov     dword ErrorNumber,8
        cmp     byte ExceptionFlag,12        ;stack?
        jz      E@@12
        mov     dword ErrorNumber,9
        cmp     byte ExceptionFlag,13        ;general?
        jz      E@@12
        mov     dword ErrorNumber,10
        cmp     byte ExceptionFlag,14        ;page?
        jz      E@@12
        mov     dword ErrorNumber,11
E@@12:   call    SetErrorText
        jmp     E@@9
E@@4:    or      eax,1 shl 8             ;COND_WATCH = dunno!
;
;Return to caller.
;
E@@9:
;    pops    ebx,ecx,edx,esi,edi,ebp
        pop     ebp
        pop     edi
        pop     esi
        pop     edx
        pop     ecx
        pop     ebx

        rets
Execute        endp


;*******************************************************************************
;
;Check if hardware break point executed.
;
;*******************************************************************************
IsHardBreak     proc    near
        pushad
        push    ds
        mov     ax,DGROUP
        mov     ds,ax
        mov     esi,offset HBRKTable
        mov     ecx,4
HB@@0:    cmp     HBRK_Flags[esi],1+2
        jnz     HB@@1
        mov     bx,HBRK_Handle[esi]
        mov     ax,0b02h
        int     31h
        jc      HB@@1
        test    ax,1
        jnz     HB@@8
HB@@1:    add     esi,size HBRK
        dec     ecx
        jnz     HB@@0
        or      eax,-1
        jmp     HB@@10
        ;
HB@@8:    xor     eax,eax
        ;
HB@@10:   pop     ds
        popad
        ret
IsHardBreak     endp


;*******************************************************************************
;Catch INT 0's.
;*******************************************************************************
Int00Handler    proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      I00@@Old
        mov     byte Executing,0
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
        ;Fetch origional Flags:CS:EIP,SS:ESP
        ;
        mov     eax,es:[ebp+(4+4+4+4)+(4+4)]
        mov     DebugEFL,eax
        mov     eax,es:[ebp+(4+4+4+4)+(4)]
        mov     DebugCS,ax
        mov     eax,es:[ebp+(4+4+4+4)+(0)]
        mov     DebugEIP,eax
        ;
        mov     byte ExceptionFlag,0
        ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset I00@@return
        mov     es:[ebp+(4+4+4+4)+(0)],eax
        mov     es:w[ebp+(4+4+4+4)+(4)],cs
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        iretd
        ;
I00@@return:       ;Now return control to exec caller.
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
I00@@Old:
;  pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldInt00]
        assume ds:DGROUP
OldInt00        df 0
Int00Handler    endp


;*******************************************************************************
;Catch single instruction trace and debug register traps.
;*******************************************************************************
Int01Handler    proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      I01@@Old
        mov     byte Executing,0
        call    IsHardBreak
        jnz     I01@@0
        mov     byte ExceptionFlag,1
I01@@0:    or      byte TraceFlag,-1
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
        mov     eax,offset I01@@return
        mov     es:[ebp+(4+4+4+4)+(0)],eax
        mov     es:w[ebp+(4+4+4+4)+(4)],cs
        and     es:w[ebp+(4+4+4+4)+(4+4)],65535-256
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax
        iretd
        ;
I01@@return:       ;Now return control to exec caller.
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
I01@@Old:
;  pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldInt01]
        assume ds:DGROUP
OldInt01        df 0
Int01Handler    endp


;*******************************************************************************
;Catch single step or debug register traps.
;*******************************************************************************
EInt01Handler   proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      E01@@Old
        mov     byte Executing,0
        call    IsHardBreak
        jnz     E01@@0
        mov     byte ExceptionFlag,1
E01@@0:    or      byte TraceFlag,-1
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
        mov     eax,offset E01@@return
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax
        retf
        ;
E01@@return:       ;Now return control to exec caller.
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
E01@@Old:
;  pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldEInt01]
        assume ds:DGROUP
OldEInt01       df 0
EInt01Handler   endp


;*******************************************************************************
;Catch INT 3's.
;*******************************************************************************
Int03Handler    proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      I03@@Old
        mov     byte Executing,0
        or      byte BreakFlag,-1
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
        cmp     byte ExceptionFlag,-1
        jnz     I03@@NoEIPDec
        dec     dword DebugEIP                ;account for int 3 instruction length.
        cmp     byte TerminationFlag,-1
        jnz     I03@@NoEIPDec
        dec     dword DebugEIP
I03@@NoEIPDec:
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset I03@@return
        mov     es:[ebp+(4+4+4+4)+(0)],eax
        mov     es:w[ebp+(4+4+4+4)+(4)],cs
        and     es:w[ebp+(4+4+4+4)+(4+4)],65535-256
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax
        iretd
        ;
I03@@return:       ;Now return control to exec caller.
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
I03@@Old:
;  pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldInt03]
        assume ds:DGROUP
OldInt03        df 0
Int03Handler    endp


;*******************************************************************************
;Catch break exceptions.
;*******************************************************************************
EInt03Handler   proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      E03@@Old
        mov     byte Executing,0
        or      byte BreakFlag,-1
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
        cmp     byte ExceptionFlag,-1
        jnz     E03@@NoEIPDec
        dec     dword DebugEIP                ;account for int 3 instruction length.
        cmp     byte TerminationFlag,-1
        jnz     E03@@NoEIPDec
        dec     dword DebugEIP
E03@@NoEIPDec:     ;
        ;Now modify origional CS:EIP,SS:ESP values and return control
        ;to this code via interupt structure to restore stacks.
        ;
        mov     eax,offset E03@@return
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax
        retf
        ;
E03@@return:       ;Now return control to exec caller.
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
E03@@Old:
;  pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldEInt03]
        assume ds:DGROUP
OldEInt03       df 0
EInt03Handler   endp


;*******************************************************************************
;Catch stack exceptions.
;*******************************************************************************
Exc12Handler    proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      E12@@Old
        mov     byte Executing,0
        mov     byte ExceptionFlag,12
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
        mov     eax,offset E12@@return
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        retf
        ;
E12@@return:       ;Now return control to exec caller.
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
E12@@Old:
;  pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldExc12]
        assume ds:DGROUP
OldExc12        df 0
Exc12Handler    endp


;*******************************************************************************
;Catch general protection faults.
;*******************************************************************************
Exc13Handler    proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      E13@@Old
        mov     byte Executing,0
        mov     byte ExceptionFlag,13
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
        mov     eax,offset E13@@return
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax
        retf
        ;
E13@@return:       ;Now return control to exec caller.
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
E13@@Old:
;  pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldExc13]
        assume ds:DGROUP
OldExc13        df 0
Exc13Handler    endp


;*******************************************************************************
;Catch page faults.
;*******************************************************************************
Exc14Handler    proc    near
;        pushs   eax,ebp,ds,es
        push    eax
        push    ebp
        push    ds
        push    es

        mov     ax,DGROUP               ;make our data addresable.
        mov     ds,ax
        cmp     byte Executing,0
        jz      E14@@Old
        mov     byte Executing,0
        mov     byte ExceptionFlag,14
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
        mov     eax,offset E14@@return
        mov     es:d[ebp+(4+4+4)+(4+4+4)+(0)+4],eax
        mov     es:w[ebp+(4+4+4)+(4+4+4)+(4)+4],cs
        and     es:w[ebp+(4+4+4)+(4+4+4)+(4+4)+4],65535-256
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax
        retf
        ;
E14@@return:       ;Now return control to exec caller.
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
E14@@Old:  cmp     byte LinearAddressCheck,0
        jz      E14@@shit
        mov     byte LinearAddressCheck,0
        xor     esi,esi
;        pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax
        retf
E14@@shit:
; pops    eax,ebp,ds,es
        pop     es
        pop     ds
        pop     ebp
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldExc14]
        assume ds:DGROUP
OldExc14        df 0
Exc14Handler    endp


;*******************************************************************************
;INT 21h handler to catch program termination.
;*******************************************************************************
Int21Handler    proc    far
        pushfd
        cmp     ah,4ch  ;terminate?
        jnz     I21@@Old
;
;        pushs   ax,bx,ds
        push    ax
        push    bx
        push    ds

        mov     ah,62h
        int     21h
        mov     ds,bx
        lds     bx,ds:[EPSP_ExecCount]
        cmp     b[bx],0
;        pops    ax,bx,ds
        pop     ds
        pop     bx
        pop     ax

        jnz     I21@@Old
;
        popfd
;        pushs   ds,eax
        push    ds
        push    eax

        mov     ax,DGROUP
        mov     ds,ax
        mov     byte TerminationFlag,-1
        mov     eax,esp
        mov     al,ss:[eax]
        mov     byte TerminateCode,al
;        pops    ds,eax
        pop     eax
        pop     ds

        jmp     Int03Handler
I21@@Old:  ;
        popfd
        assume ds:nothing
        jmp     cs:f[OldInt21h]
        assume ds:DGROUP
OldInt21h       df 0
Int21Handler    endp



;*******************************************************************************
;
;This should recieve ALL keyboard interrupts before anything else gets to see
;them.
;
;*******************************************************************************
BreakChecker    proc    near
;        pushs   eax,ebx,ebp,ds
        push    eax
        push    ebx
        push    ebp
        push    ds

        mov     ax,DGROUP
        mov     ds,ax
        inc     byte InInt09
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
        cmp     byte InInt09,2
        jnc     BC@@old
;
;Check if anything is running.
;
        cmp     byte Executing,0
        jz      BC@@old
;
;Check if our break combination is set.
;
        mov     ebx,offset BreakKeyList
        cmp     d[ebx],0                ;check if any keys in the list.
        jz      BC@@old
BC@@6:    cmp     d[ebx],0                ;End of the list?
        jz      BC@@7
        mov     eax,d[ebx]              ;Get scan code.
        cmp     b[KeyTable+eax],0
        jz      BC@@old
        add     ebx,4
        jmp     BC@@6
;
;Check if return CS:EIP & stack belong to the program we're running.
;
BC@@7:    pushad
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
        jc      BC@@nope
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
        mov     d[ebp],offset BC@@3
        popad
;        pops    eax,ebx,ebp,ds
        pop     ds
        pop     ebp
        pop     ebx
        pop     eax
        iretd
;
;We should be running on the applications stack with the applications registers
;now.
;
BC@@3:    pushfd
;        pushs   eax,ebp,ds
        push    eax
        push    ebp
        push    ds

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
        add     dword DebugESP,4+4+4+4
;
;Return to the debugger exec routine.
;
        mov     ax,DGROUP
        mov     es,ax
        mov     fs,ax
        mov     gs,ax
        or      byte BreakKeyFlag,-1
        mov     byte Executing,0
        dec     byte InInt09
;
;Clean up the key table.
;
        mov     ebx,offset BreakKeyList
BC@@60:   cmp     d[ebx],0                ;End of the list?
        jz      BC@@70
        mov     eax,d[ebx]              ;Get scan code.
        mov     b[KeyTable+eax],0
        add     ebx,4
        jmp     BC@@60
;
BC@@70:   lss     esp,f[DebuggerESP]
        ret
;
BC@@nope: popad
;
;Pass control to the origional handler.
;
BC@@old:  dec     byte InInt09
;        pops    eax,ebx,ebp,ds
        pop     ds
        pop     ebp
        pop     ebx
        pop     eax

        assume ds:nothing
        jmp     cs:f[OldInt09]
        assume ds:DGROUP
OldInt09        df 0
BreakChecker    endp


;*******************************************************************************
;
;This must be just before cwAPIpatch
;
;*******************************************************************************
cwIdentity      db "CAUSEWAY"
cwMajorVersion  db 0
cwMinorVersion  db 0
;*******************************************************************************
;
;This intercepts get/set int 9 vector stuff and makes sure BreakChecker stays
;first in the chain. Also monitor exec calls.
;
;*******************************************************************************
Int31Intercept  proc    near
        cmp     ax,0205h                ;Set vector?
        jnz     @@notset
        cmp     bl,9            ;INT 9?
        jnz     @@old
;        pushs   eax,edx,ds
        push    eax
        push    edx
        push    ds

        mov     ax,DGROUP
        mov     ds,ax
        assume ds:_TEXT
        mov     d[OldInt09],edx
        mov     w[OldInt09+4],cx
        assume ds:DGROUP
;        pops    eax,edx,ds
        pop     ds
        pop     edx
        pop     eax

        jmp     @@ret
        ;
@@notset:       cmp     ax,0204h                ;Get vector?
        jnz     @@old
        cmp     bl,9            ;INT 9?
        jnz     @@old
        assume ds:nothing
        mov     edx,cs:d[OldInt09]
        mov     cx,cs:w[OldInt09+4]
        assume ds:DGROUP
        ;
@@ret:
;  pushs   eax,ebp,ds
        push    eax
        push    ebp
        push    ds

        mov     ax,DGROUP
        mov     ds,ax
        mov     ebp,esp
        add     ebp,4+4+4+4+4
        and     ss:w[ebp],not 1 ;clear carry.

;        pops    eax,ebp,ds
        pop     ds
        pop     ebp
        pop     eax

        iretd
        ;
@@old:  assume ds:nothing
        jmp     cs:f[OldInt31]
        assume ds:DGROUP
OldInt31        df 0
Int31Intercept  endp


;*******************************************************************************
;
;Read config file if one exists.
;
;*******************************************************************************
ReadConfig      proc    near
;
;Try in the current directory.
;
;        calls   OpenFile,offset ConfigFile,0
        push    dword 0
        push    offset ConfigFile
        call    OpenFile
        lea     esp,[esp+8]

        or      eax,eax
        jz      RC@@0
;        calls   CloseFile,eax
        push    eax
        call    CloseFile
        lea     esp,[esp+4]

        jmp     RC@@3
;
;Get the execution path and use it to find the configuration file.
;
RC@@0:    mov     bx,__psp
        sys     GetSelDet32
        mov     bx,[edx+2ch]
        sys     GetSelDet32
        mov     edi,edx
        or      ecx,-1
        xor     al,al
RC@@4:    repne   scasb
        cmp     b[edi],0
        jnz     RC@@4
        add     edi,2
        mov     esi,edi
        mov     edi,offset ConfigName
;        callc   strcpy,edi,esi
        pushc
        push    esi
        push    edi
        call    strcpy
        lea     esp,[esp+8]
        popc

;        callc   strlen,edi
        pushc
        push    edi
        call    strlen
        lea     esp,[esp+4]
        popc

        mov     esi,edi
        add     edi,eax
RC@@1:    dec     edi
        cmp     esi,edi
        jnc     RC@@2
        cmp     b[edi],"\"
        jnz     RC@@1
RC@@2:    mov     b[edi+1],0
        mov     edi,esi
        mov     esi,offset ConfigFile
;        callc   strcat,edi,esi
        pushc
        push    esi
        push    edi
        call    strcat
        lea     esp,[esp+8]
        popc

        ;
;        calls   OpenFile,offset ConfigName,0
        push    dword 0
        push    offset ConfigName
        call    OpenFile
        lea     esp,[esp+8]

        or      eax,eax
        jz      RC@@9
;        calls   CloseFile,eax
        push    eax
        call    CloseFile
        lea     esp,[esp+4]
;
;config file exists so fetch our variables.
;
RC@@3:
;       calls   cfgGetInts,offset ConfigName,offset SetupName,offset BreakKeyVar, offset BreakKeyList, 4
;        calls  cfgGetOnOff,offset ConfigName,offset SetupName,offset ResetTimerVar, offset ResetTimer
;        calls   cfgGetOnOff,offset ConfigName,offset SetupName,offset DebugVar, offset DebugLevel

        push    dword 4
        push    offset BreakKeyList
        push    offset BreakKeyVar
        push    offset SetupName
        push    offset ConfigName
        call    cfgGetInts
        lea     esp,[esp+20]

        push    offset ResetTimer
        push    offset ResetTimerVar
        push    offset SetupName
        push    offset ConfigName
        call    cfgGetOnOff
        lea     esp,[esp+16]

        push    offset DebugLevel
        push    offset DebugVar
        push    offset SetupName
        push    ConfigName
        call    cfgGetOnOff
        lea     esp,[esp+16]
;
RC@@9:    ret
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
LoadTimer       proc    near
        cli
        jmp     LT@@8
LT@@8:    jmp     LT@@9
LT@@9:    push    ax
        mov     al,36h
        out     43h,al
        pop     ax
        jmp     LT@@0
LT@@0:    jmp     LT@@1
LT@@1:    out     40h,al
        mov     al,ah
        jmp     LT@@2
LT@@2:    jmp     LT@@3
LT@@3:    out     40h,al
        jmp     LT@@4
LT@@4:    jmp     LT@@5
LT@@5:    in      al,21h
        and     al,254
        jmp     LT@@6
LT@@6:    jmp     LT@@7
LT@@7:    out     21h,al
        sti
        ret
LoadTimer       endp


;*******************************************************************************
;Display contents of request buffer on screen for debugging.
;*******************************************************************************
DumpRequest     proc    near
        pushad

        mov     esi,offset ReqBuffer
        mov     ecx,ReqLength
DR@@0:
        mov     edi,offset DebugBuffer
        push    ecx
        mov     ecx,80
        mov     al," "
        rep     stosb
        pop     ecx

        mov     edi,offset DebugBuffer
        mov     ebp,esi
        xor     edx,edx

DR@@1:    or      ecx,ecx
        jz      DR@@2
        cmp     edx,16
        jz      DR@@2
        ;
        ;Do hex version of value.
        ;
        mov     al,[esi]
        inc     esi
        inc     edx
        dec     ecx
        push    ecx
        mov     ecx,2
;        calls   Bin2Hex,eax,ecx,edi
        push    edi
        push    ecx
        push    eax
        call    Bin2Hex
        lea     esp,[esp+12]

        mov     edi,eax
        pop     ecx
        mov     b[edi]," "
        inc     edi
        jmp     DR@@1
        ;
DR@@2:    xchg    esi,ebp
        mov     edi,offset DebugBuffer+(16*3)
DR@@3:    or      edx,edx
        jz      DR@@5
        ;
        ;Do ASCII version of value.
        ;
        mov     al,[esi]
        inc     esi
        mov     ah,"."
        cmp     al," "
        jc      DR@@4
        cmp     al,"z"+1
        jnc     DR@@4
        mov     ah,al
DR@@4:    mov     [edi],ah
        inc     edi
        dec     edx
        jmp     DR@@3

DR@@5:    mov     b[edi],13
        inc     edi
        mov     b[edi],10
        inc     edi
        mov     b[edi],"$"
        pushad
        mov     edx,offset DebugBuffer
        mov     ah,9
        int     21h
        popad

        xchg    ebp,esi
        or      ecx,ecx
        jnz     DR@@0

        mov     byte DebugBuffer,13
        mov     byte DebugBuffer+1,10
        mov     byte DebugBuffer+2,"$"
        mov     edx,offset DebugBuffer
        mov     ah,9
        int     21h

        popad
        ret
DumpRequest     endp


;*******************************************************************************
;Display contents of request buffer on screen for debugging.
;*******************************************************************************
DumpRequest2File proc near
        pushad

        mov     edx,offset LogFileName
        mov     ax,3d02h
        int     21h
        jnc     DF@@open
        mov     ah,3ch
        xor     cx,cx
        int     21h
        jc      DF@@balls
DF@@open: mov     LogFileHandle,eax
        mov     ebx,eax
        xor     cx,cx
        xor     dx,dx
        mov     ax,4202h
        int     21h

        mov     esi,offset ReqBuffer
        mov     ecx,ReqLength
DF@@0:
        mov     edi,offset DebugBuffer
        push    ecx
        mov     ecx,80
        mov     al," "
        rep     stosb
        pop     ecx

        mov     edi,offset DebugBuffer
        mov     ebp,esi
        xor     edx,edx

DF@@1:    or      ecx,ecx
        jz      DF@@2
        cmp     edx,16
        jz      DF@@2
        ;
        ;Do hex version of value.
        ;
        mov     al,[esi]
        inc     esi
        inc     edx
        dec     ecx
        push    ecx
        mov     ecx,2
;        calls   Bin2Hex,eax,ecx,edi
        push    edi
        push    ecx
        push    eax
        call    Bin2Hex
        lea     esp,[esp+12]

        mov     edi,eax
        pop     ecx
        mov     b[edi]," "
        inc     edi
        jmp     DF@@1
        ;
DF@@2:    xchg    esi,ebp
        mov     edi,offset DebugBuffer+(16*3)
DF@@3:    or      edx,edx
        jz      DF@@5
        ;
        ;Do ASCII version of value.
        ;
        mov     al,[esi]
        inc     esi
        mov     ah,"."
        cmp     al," "
        jc      DF@@4
        cmp     al,"z"+1
        jnc     DF@@4
        mov     ah,al
DF@@4:    mov     [edi],ah
        inc     edi
        dec     edx
        jmp     DF@@3

DF@@5:    mov     b[edi],13
        inc     edi
        mov     b[edi],10
        inc     edi
        pushad
        mov     edx,offset DebugBuffer
        sub     edi,edx
        mov     ecx,edi
        mov     ebx,LogFileHandle
        mov     ah,40h
        int     21h
        popad

        xchg    ebp,esi
        or      ecx,ecx
        jnz     DF@@0

        mov     byte DebugBuffer,13
        mov     byte DebugBuffer+1,10
        mov     edx,offset DebugBuffer
        mov     ebx,LogFileHandle
        mov     ecx,2
        mov     ah,40h
        int     21h

        mov     ah,68h
        mov     ebx,LogFileHandle
        int     21h

        mov     ebx,LogFileHandle
        mov     ah,3eh
        int     21h
        mov     dword LogFileHandle,0

DF@@balls:        popad
        ret
DumpRequest2File endp


;*******************************************************************************
;Display contents of request buffer on screen for debugging.
;*******************************************************************************
DumpReply2File proc near
        pushad


        mov     edx,offset LogFileName
        mov     ax,3d02h
        int     21h
        jnc     D2@@open
        mov     ah,3ch
        xor     cx,cx
        int     21h
        jc      D2@@balls
D2@@open: mov     LogFileHandle,eax
        mov     ebx,eax
        xor     cx,cx
        xor     dx,dx
        mov     ax,4202h
        int     21h

        mov     esi,ReqAddress
        mov     ecx,[esi+4]
        mov     esi,[esi]

D2@@0:
        mov     edi,offset DebugBuffer
        push    ecx
        mov     ecx,80
        mov     al," "
        rep     stosb
        pop     ecx

        mov     edi,offset DebugBuffer
        mov     ebp,esi
        xor     edx,edx

D2@@1:    or      ecx,ecx
        jz      D2@@2
        cmp     edx,16
        jz      D2@@2
        ;
        ;Do hex version of value.
        ;
        mov     al,[esi]
        inc     esi
        inc     edx
        dec     ecx
        push    ecx
        mov     ecx,2
;        calls   Bin2Hex,eax,ecx,edi
        push    edi
        push    ecx
        push    eax
        call    Bin2Hex
        lea     esp,[esp+12]

        mov     edi,eax
        pop     ecx
        mov     b[edi]," "
        inc     edi
        jmp     D2@@1
        ;
D2@@2:    xchg    esi,ebp
        mov     edi,offset DebugBuffer+(16*3)
D2@@3:    or      edx,edx
        jz      D2@@5
        ;
        ;Do ASCII version of value.
        ;
        mov     al,[esi]
        inc     esi
        mov     ah,"."
        cmp     al," "
        jc      D2@@4
        cmp     al,"z"+1
        jnc     D2@@4
        mov     ah,al
D2@@4:    mov     [edi],ah
        inc     edi
        dec     edx
        jmp     D2@@3

D2@@5:    mov     b[edi],13
        inc     edi
        mov     b[edi],10
        inc     edi
        pushad
        mov     edx,offset DebugBuffer
        sub     edi,edx
        mov     ecx,edi
        mov     ebx,LogFileHandle
        mov     ah,40h
        int     21h
        popad

        xchg    ebp,esi
        or      ecx,ecx
        jnz     D2@@0

        mov     byte DebugBuffer,13
        mov     byte DebugBuffer+1,10
        mov     byte DebugBuffer+2,13
        mov     byte DebugBuffer+3,10
        mov     edx,offset DebugBuffer
        mov     ebx,LogFileHandle
        mov     ecx,4
        mov     ah,40h
        int     21h

        mov     ah,68h
        mov     ebx,LogFileHandle
        int     21h

        mov     ebx,LogFileHandle
        mov     ah,3eh
        int     21h
        mov     dword LogFileHandle,0

D2@@balls:        popad
        ret
DumpReply2File endp


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
Bord    proc    near
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


        .data

dLockStart      label byte


ReqAddress      dd ?
ReqLength       dd ?
ReqBuffer       db 256 dup (0)
RealModeRegs    db size RealRegsStruc dup (0)
PSPSegment      dw ?

ReqTable        label dword
        dd 0    ;0
        dd 0    ;1
        dd 0    ;2
        dd 0    ;3
        dd 0    ;4
        dd 0    ;5
        dd REQ_GET_SYS_CONFIG   ;6
        dd REQ_MAP_ADDR         ;7
        dd REQ_ADDR_INFO                ;8
        dd REQ_CHECKSUM_MEM     ;9
        dd REQ_READ_MEM         ;10
        dd REQ_WRITE_MEM                ;11
        dd REQ_READ_IO          ;12
        dd REQ_WRITE_IO         ;13
        dd REQ_READ_CPU         ;14
        dd REQ_READ_FPU         ;15
        dd REQ_WRITE_CPU                ;16
        dd REQ_WRITE_FPU                ;17
        dd REQ_PROG_GO          ;18
        dd REQ_PROG_STEP                ;19
        dd REQ_PROG_LOAD                ;20
        dd REQ_PROG_KILL                ;21
        dd REQ_SET_WATCH                ;22
        dd REQ_CLEAR_WATCH              ;23
        dd REQ_SET_BREAK                ;24
        dd REQ_CLEAR_BREAK              ;25
        dd REQ_GET_NEXT_ALIAS   ;26
        dd 0    ;27
        dd 0    ;28
        dd 0    ;29
        dd REQ_GET_LIB_NAME     ;30
        dd REQ_GET_ERR_TEXT     ;31
        dd REQ_GET_MESSAGE_TEXT ;32
        dd REQ_REDIRECT_STDIN   ;33
        dd REQ_REDIRECT_STDOUT  ;34
        dd 0    ;35

; MED, 01/24/99
;       dd 0    ;36
;       dd 0    ;37
;       dd 0    ;38
        dd      REQ_READ_REGS   ;36
        dd      REQ_WRITE_REGS  ;37
        dd      REQ_MACHINE_DATA        ;38

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
DebugDS dw 0
DebugES dw 0
DebugSS dw 0
DebugCS dw 0
DebugFS dw 0
DebugGS dw 0
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

RealCall        dd 0

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


DebugBuffer     db 256 dup (0)
CarriageReturn  db 13,0
SpaceText       db " ",0
LogFileName     db "cwhelp.log",0
LogFileHandle   dd 0


dLockEnd        label byte

        autolock cLockStart,cLockEnd-cLockStart
        autolock dLockStart,dLockEnd-dLockStart


        end



