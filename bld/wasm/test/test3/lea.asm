; Test different type of the LEA instruction

.386	; default use to use32

extern a:fword
extern b:qword
extern cc:tbyte
extern d:dword
extern e:byte

.model small
.data
a1	df	?
b1	dq	?
cc1	dt	?
d1	dd	?
e1	db	?
.code
    lea bx, a
    lea bx, b
    lea bx, cc
    lea cx, d
    lea cx, e
    lea ebx, a
    lea ebx, b
    lea ebx, cc
    lea ecx, d
    lea ecx, e
    lea bx, a1
    lea bx, b1
    lea bx, cc1
    lea cx, d1
    lea cx, e1
    lea ebx, a1
    lea ebx, b1
    lea ebx, cc1
    lea ecx, d1
    lea ecx, e1
    lea dx,word ptr[bx + 4]
    lea dx,word ptr[ebx + 4]
    lea edx,word ptr[bx + 4]
    lea edx,word ptr[ebx + 4]
    ret
end
