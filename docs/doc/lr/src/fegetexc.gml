.func fegetexceptflag
#include <fenv.h>
void fegetexceptflag( fexcept_t *__flagp, int __excepts );
.ixfunc2 'Floating Point Environment' &func
.funcend
.*
.desc begin
The &func function stores a representation of the states of the
floating-point status flags indicated by the argument excepts in the
object pointed to by the argument flagp.
.np
Valid exceptions are
.kw FE_INVALID
.kw FE_DENORMAL
.kw FE_DIVBYZERO
.kw FE_OVERFLOW
.kw FE_UNDERFLOW
.kw FE_INEXACT
.np
The value
.kw FE_ALL_EXCEPT
is the logical or of these values.
.desc end
.*
.return begin
No value is returned.
.return end
.*
.see begin
.seelist &function. __fdisableexcept
.see end
.*
.exmp begin
#include <fenv.h>
.exmp break
void main( void )
{
    fexcept_t flags;
    fegetexceptflag( &flags, FE_DIVBYZERO );
}
.exmp end
.class C99
.system
