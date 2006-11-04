.func fegetenv
#include <fenv.h>
void fegetenv( fenv_t *__envp );
.ixfunc2 'Floating Point Environment' &func
.funcend
.*
.desc begin
The &func function stores the current floating-point environment in the object
pointed to by envp.
.desc end
.*
.return begin
No value is returned.
.return end
.*
.see begin
.seelist &function. feholdexcept fesetenv feupdateenv
.see end
.*
.exmp begin
#include <stdio.h>
#include <fenv.h>
.exmp break
void main( void )
{
    fenv_t env;
    fegetenv( &env );
}
.exmp end
.class C99
.system
