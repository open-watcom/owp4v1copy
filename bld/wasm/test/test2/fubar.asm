.386
.model small
.data
    foo dd 1234h

.code

ifdef errors
  jmp byte ptr [bx]
  jmp qword ptr [bx]
  jmp tbyte ptr [bx]
endif

jmp near ptr dword ptr [bx]
jmp near ptr dword ptr [bx]+2

ifdef errors
  jmp near ptr dword ptr foo
  jmp near ptr dword ptr foo+2
endif

jmpf word ptr [bx]
jmpf word ptr [bx]+2

jmp dword ptr [bx]
jmp dword ptr [bx]+2

jmp far ptr dword ptr [bx]
jmp far ptr dword ptr [bx]+2

jmpf dword ptr [bx]
jmpf dword ptr [bx]+2

jmp dword ptr foo
jmp dword ptr foo+2

jmp far ptr dword ptr foo
jmp far ptr dword ptr foo+2

jmpf dword ptr foo
jmpf dword ptr foo+2

end
