.func feclearexcept
#include <fenv.h>
void  feclearexcept( int __excepts );
.ixfunc2 'Floating Point Environment' &func
.funcend
.*
.desc begin
The &func function clears the supported floating-point exceptions
represented by its argument.
.desc end
.*
.return begin
No value is returned.
.return end
.*
.see begin
.seelist &function. fegetexceptflag feraiseexcept fesetexceptflag fetestexcept
.see end
.*
.exmp begin
#include <fenv.h>
.exmp break
void main( void )
{
	feclearexcept( FE_OVERFLOW|FE_UNDERFLOW );
}
.exmp end
.class C99
.system
