.386

_c16 segment use16 'CODE'
	push 7fh
	push 0ffh
	push 07fffh
	push 0ffffh
	push 07fffffffh
	push 0ffffffffh
	pushw 7fh
	pushw 0ffh
	pushw 07fffh
	pushw 0ffffh
	pushw 07fffffffh
	pushw 0ffffffffh
	pushd 7fh
	pushd 0ffh
	pushd 07fffh
	pushd 0ffffh
	pushd 07fffffffh
	pushd 0ffffffffh
_c16 ends

_c32 segment use32 'CODE'
	push 7fh
	push 0ffh
	push 07fffh
	push 0ffffh
	push 07fffffffh
	push 0ffffffffh
	pushw 7fh
	pushw 0ffh
	pushw 07fffh
	pushw 0ffffh
	pushw 07fffffffh
	pushw 0ffffffffh
	pushd 7fh
	pushd 0ffh
	pushd 07fffh
	pushd 0ffffh
	pushd 07fffffffh
	pushd 0ffffffffh
_c32 ends

end
