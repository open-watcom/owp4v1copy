#include "fail.h"

// preprocesor substitution and directive processing test for _asm

#define MOV_X_CONST(a) mov x,a

int main() {
#ifdef __X86__
    unsigned int    x;

    _asm    .8086
    _asm    .386
    _asm    .586
    _asm    .MMX
    _asm    .K3D
    _asm    .686
    _asm    .XMM
    _asm    .XMM2
    _asm    .XMM3
    _asm    .8086

    _asm    MOV_X_CONST( 0e1fh );
    if( x != 0x0e1f ) _fail;
#endif
    _PASS;
}
