.func feraiseexcept
#include <fenv.h>
void feraiseexcept( int __excepts );
.ixfunc2 'Floating Point Environment' &func
.funcend
.*
.desc begin
The &func function raises the supported floating-point exceptions
represented by its argument.
.desc end
.*
.return begin
No value is returned.
.return end
.*
.see begin
.seelist &function. feclearexcept fegetexceptflag fetestexcept
.see end
.*
.exmp begin
#include <fenv.h>
.exmp break
void main( void )
{
	feraiseexcept( FE_DIVBYZERO );
}
.exmp end
.class C99
.system
