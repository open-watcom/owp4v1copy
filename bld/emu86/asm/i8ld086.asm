
ifdef _BUILDING_MATHLIB

include mdef.inc
include struct.inc
include xception.inc

        modstart    i8ld086, word


endif

;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
;<>
;<> __I8LD - convert 64-bit integer to long double
;<>
;<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

        xdefp   __I8LD
        xdefp   __U8LD

;       __I8LD - convert long into long double
;       __U8LD - convert unsigned long into long double
; input:
;       DS:SI - pointer to long long
;       DS:DI - pointer to long double
;
ifdef _BUILDING_MATHLIB
__I8LD  proc
else
__I8LD  proc    near
endif
        push    ax
        mov     ax,0
        mov     [di],ax
        mov     2[di],ax
        mov     4[di],ax
        mov     6[di],ax
        mov     8[di],ax
        pop     ax
        ret
__I8LD  endp

; input:
;       DS:SI - pointer to unsigned long long
;       DS:DI - pointer to long double
;
ifdef _BUILDING_MATHLIB
__U8LD  proc
else
__U8LD  proc    near
endif
        push    ax
        mov     ax,0
        mov     [di],ax
        mov     2[di],ax
        mov     4[di],ax
        mov     6[di],ax
        mov     8[di],ax
        pop     ax
        ret
__U8LD  endp

ifdef _BUILDING_MATHLIB

        endmod

        endf    equ end
else
        endf    equ <>

endif

endf
