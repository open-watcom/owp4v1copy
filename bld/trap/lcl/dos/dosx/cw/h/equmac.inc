;*******************************************************************************
;Lazy variable access equates.
;*******************************************************************************
b	equ	byte ptr
w	equ	word ptr
d	equ	dword ptr
f	equ	fword ptr


;*******************************************************************************
;New macros
;*******************************************************************************
rets	macro
	ret
endm

pushc	macro
	push	ebx
	push	ecx
	push	edx
	push	esi
	push	edi
	push	ebp
	endm

popc	macro
	pop	ebp
	pop	edi
	pop	esi
	pop	edx
	pop	ecx
	pop	ebx
	endm

;*******************************************************************************
;Replacement for REP MOVSB. Does the operation with DWORD's where possible.
;*******************************************************************************
rep_movsb	macro
	local __0, __1, __2
	push	eax
;
;dword align source
;
	mov	eax,esi
	and	eax,3
	sub	eax,4
	neg	eax
	and	eax,3
	jz	__2
	cmp	ecx,eax
	jc	__2
	pushs	ecx
	mov	ecx,eax
	rep	movsb
	pops	ecx
	sub	ecx,eax
;
;do main copy.
;
__2:	push	ecx
	shr	ecx,2
	jz	__1
__0:	mov	eax,[esi]
	mov	[edi],eax
	add	esi,4
	add	edi,4
	dec	ecx
	jnz	__0
__1:	pop	ecx
	and	ecx,3
	rep	movsb
	pop	eax
	endm


;*******************************************************************************
;Replacement for REP MOVSW. Does the operation with DWORD's where possible.
;*******************************************************************************
rep_movsw	macro
	local __0, __1
	push	eax
	push	ecx
	shr	ecx,1
	jz	__1
__0:	mov	eax,[esi]
	mov	[edi],eax
	add	esi,4
	add	edi,4
	dec	ecx
	jnz	__0
__1:	pop	ecx
	and	ecx,1
	rep	movsw
	pop	eax
	endm


;*******************************************************************************
;Replacement for REP MOVSD. Allows us to change method easily.
;*******************************************************************************
rep_movsd	macro
	local __0, __1
	or	ecx,ecx
	jz	__1
__0:	mov	eax,[esi]
	mov	[edi],eax
	add	esi,4
	add	edi,4
	dec	ecx
	jnz	__0
__1:	;
	endm


;*******************************************************************************
;Replacement for REP STOSB. Does the operation with DWORD's where possible.
;*******************************************************************************
rep_stosb	macro
	local __0, __1
	push	eax
	mov	ah,al
	push	ax
	shl	eax,16
	pop	ax
	push	ecx
	shr	ecx,2
	jz	__1
__0:	mov	[edi],eax
	add	edi,4
	dec	ecx
	jnz	__0
__1:	pop	ecx
	and	ecx,3
	rep	stosb
	pop	eax
	endm


;*******************************************************************************
;Replacement for REP STOSW. Does the operation with DWORD's where possible.
;*******************************************************************************
rep_stosw	macro
	local __0, __1
	push	eax
	push	ax
	shl	eax,16
	pop	ax
	push	ecx
	shr	ecx,1
	jz	__1
__0:	mov	[edi],eax
	add	edi,4
	dec	ecx
	jnz	__0
__1:	pop	ecx
	and	ecx,1
	rep	stosw
	pop	eax
	endm


;*******************************************************************************
;Replacement for REP STOSD. Allows us to change method easily.
;*******************************************************************************
rep_stosd	macro
	local __0, __1
	or	ecx,ecx
	jz	__1
__0:	mov	[edi],eax
	add	edi,4
	dec	ecx
	jnz	__0
__1:	;
	endm


;*******************************************************************************
;Convert character in AL to upper case.
;*******************************************************************************
UpperChar	macro
	local __0
	cmp	al,61h		; 'a'
	jb	__0
	cmp	al,7Ah		; 'z'
	ja	__0
	and	al,5Fh		;convert to upper case.
__0:	;
	endm


;*******************************************************************************
;Add a memory region to the auto-lock list.
;*******************************************************************************
autolock	macro p1,p2
	extrn __autolock:near
	extrn __autounlock:near
_AUTOLOCKB	segment dword public 'DATA'
_AUTOLOCKB	ends
_AUTOLOCK	segment dword public 'DATA'
	dd p1,p2
_AUTOLOCK	ends
_AUTOLOCKE	segment dword public 'DATA'
_AUTOLOCKE	ends
	endm


