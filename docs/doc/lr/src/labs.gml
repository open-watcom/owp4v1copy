.func labs
#include <stdlib.h>
long int labs( long int j );
.funcend
.desc begin
The &func function returns the absolute value
of its long-integer argument
.arg j.
.desc end
.return begin
The &func function returns the absolute value of its argument.
.return end
.see begin
.seelist labs abs fabs
.see end
.exmp begin
#include <stdio.h>
#include <stdlib.h>

void main()
  {
    long x, y;
.exmp break
    x = -50000L;
    y = labs( x );
    printf( "labs(%ld) = %ld\n", x, y );
  }
.exmp output
labs(-50000) = 50000
.exmp end
.class ANSI
.system
