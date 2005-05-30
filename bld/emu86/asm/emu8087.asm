.286

ifdef __SMALL__
_SMALL_CODE = 1
else ifdef __COMPACT__
_SMALL_CODE = 1
else
_SMALL_CODE = 0
endif

_TEXT   segment word public 'CODE'

modstart        macro   modname
                endm

xdefp           macro   xsym
ifdef _DEBUG
                public  xsym
endif
                endm

defp            macro   dsym
         dsym   proc    near
                endm

xref            macro   dsym
                endm

endproc         macro   dsym
         dsym   endp
                endm

endmod          macro
_TEXT           ends
                endm

lcall           macro   dest
if _SMALL_CODE
                call    dest
else
                push    cs
                call    near ptr dest
endif
                endm

include struct.inc
include xception.inc
include fstatus.inc
include cmpconst.inc
include fpe86.inc
include flda086.asm
include fldc086.asm
include fldd086.asm
include fldm086.asm
include ldi4086.asm
include e86ldi8.inc
include i4ld086.asm
include fdld086.asm
include ldfs086.asm
include ldfd086.asm
include fsld086.asm
include e86i8ld.inc
include e86round.inc
include e86atan.inc
include e86fprem.inc
include e86fxam.inc
include e86log.inc
include sindata.inc
include e86sin.inc
include e86f2xm1.inc
include sqrt086.asm
include e86poly.inc

        endmod
        end
