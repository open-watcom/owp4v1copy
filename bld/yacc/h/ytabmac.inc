
ljmp    macro   label
        db      0e9h
        dw      label-$-2
        endm

_jna    macro   label
        ja      $+5
        ljmp    label
        endm

_jnae   macro   label
        jae     $+5
        ljmp    label
        endm

_jnb    macro   label
        jb      $+5
        ljmp    label
        endm

_jnbe   macro   label
        jbe     $+5
        ljmp    label
        endm

_jnc    macro   label
        jc      $+5
        ljmp    label
        endm

_jne    macro   label
        je      $+5
        ljmp    label
        endm

_jng    macro   label
        jg      $+5
        ljmp    label
        endm

_jnge   macro   label
        jge     $+5
        ljmp    label
        endm

_jnl    macro   label
        jl      $+5
        ljmp    label
        endm

_jnle   macro   label
        jle     $+5
        ljmp    label
        endm

_jno    macro   label
        jo      $+5
        ljmp    label
        endm

_jnp    macro   label
        jp      $+5
        ljmp    label
        endm

_jns    macro   label
        js      $+5
        ljmp    label
        endm

_jnz    macro   label
        jz      $+5
        ljmp    label
        endm

_ja     macro   label
        jna     $+5
        ljmp    label
        endm

_jae    macro   label
        jnae    $+5
        ljmp    label
        endm

_jb     macro   label
        jnb     $+5
        ljmp    label
        endm

_jbe    macro   label
        jnbe    $+5
        ljmp    label
        endm

_jc     macro   label
        jnc     $+5
        ljmp    label
        endm

_je     macro   label
        jne     $+5
        ljmp    label
        endm

_jg     macro   label
        jng     $+5
        ljmp    label
        endm

_jge    macro   label
        jnge    $+5
        ljmp    label
        endm

_jl     macro   label
        jnl     $+5
        ljmp    label
        endm

_jle    macro   label
        jnle    $+5
        ljmp    label
        endm

_jo     macro   label
        jno     $+5
        ljmp    label
        endm

_jp     macro   label
        jnp     $+5
        ljmp    label
        endm

_js     macro   label
        jns     $+5
        ljmp    label
        endm

_jz     macro   label
        jnz     $+5
        ljmp    label
        endm

_jpe    macro   label
        jpo     $+5
        ljmp    label
        endm

_jpo    macro   label
        jpe     $+5
        ljmp    label
        endm

_lbl    macro   label
        endm

_action macro   len,pnum
        push    ax
        mov     ax,len
        push    ax
        mov     ax,pnum
        push    ax
        call    _yyaction
        pop     ax
        pop     ax
        pop     ax
        endm

_reduce macro   len,lhs
        mov     ah,lhs
        add     sp,2*len
        ret     -2
        endm

_scan   macro
        call    _yylex
        endm

_call   macro   label
        call    label
        endm

_tcmp   macro   value
        cmp     al,value
        endm

_vcmp   macro   value
        cmp     ah,value
        endm

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
_TEXT   ENDS

_DATA   SEGMENT WORD PUBLIC 'DATA'
_DATA   ENDS

DGROUP  GROUP   _DATA
        ASSUME  CS: _TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP

        EXTRN   _yylex:NEAR
        EXTRN   _yyaction:NEAR
        EXTRN   _yyerror:NEAR

_TEXT   SEGMENT

L0:     mov     ax,OFFSET DGROUP:msg
        push    ax
        call    _yyerror                        ; and return
        pop     ax

L1:     add     sp,4
        pop     di
        pop     si
        pop     bp
        ret

;       void yyparse()

        PUBLIC  _yyparse
_yyparse:
        push    bp
        push    si
        push    di
_TEXT   ENDS

_DATA   SEGMENT
msg     db      "syntax error",0
_DATA   ENDS
