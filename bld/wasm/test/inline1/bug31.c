void test (void)
{
    __asm {
        xor  ax, ax
l1:
        db 126 dup(90h)
        jnz  l1
a1:
        jnz  a1 - 2
        jnz  l2
        db 127 dup(90h)
l2:
        xor  ax, ax
    }
/*
    __asm {
        xor  ax, ax
l1:
        db 126 dup(90h)
a1:
        jnz  l1
a2:
        jnz  l3
l2:
        db 127 dup(90h)
l3:
        xor  ax, ax
    }
*/
}
