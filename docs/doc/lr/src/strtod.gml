.func strtod wcstod _ustrtod
#include <stdlib.h>
double strtod( const char *ptr, char **endptr );
.ixfunc2 '&Conversion' &func
.if &'length(&wfunc.) ne 0 .do begin
#include <wchar.h>
double wcstod( const wchar_t *ptr, wchar_t **endptr );
.ixfunc2 '&Conversion' &wfunc
.ixfunc2 '&Wide' &wfunc
.do end
.if &'length(&ufunc.) ne 0 .do begin
double _ustrtod( const wchar_t *ptr, wchar_t **endptr );
.ixfunc2 '&Conversion' &ufunc
.do end
.funcend
.desc begin
The &func function converts the string pointed to by
.arg ptr
to
.id double
representation.
The function recognizes a string containing:
.begbull $compact
.bull
optional white space,
.bull
an optional plus or minus sign,
.bull
a sequence of digits containing an optional decimal point,
.bull
an optional 'e' or 'E' followed by an optionally signed sequence of
digits.
.endbull
.np
The conversion ends at the first unrecognized character.
A pointer to that character will be stored in the object to which
.arg endptr
points if
.arg endptr
is not
.mono NULL.
By comparing the "end" pointer with
.arg ptr,
it can be determined how much of the string, if any, was scanned by
the &func function.
.im widefunc
.im unifunc
.desc end
.return begin
The &func function returns the converted value.
If the correct value would cause overflow, plus or minus
.kw HUGE_VAL
is returned according to the sign, and
.kw errno
is set to
.kw ERANGE.
If the correct value would cause underflow, then zero is returned, and
.kw errno
is set to
.kw ERANGE.
.im errnocnv
.return end
.see begin
.seelist strtod atof
.see end
.exmp begin
#include <stdio.h>
#include <stdlib.h>

void main()
  {
    double pi;
.exmp break
    pi = strtod( "3.141592653589793", NULL );
    printf( "pi=%17.15f\n",pi );
  }
.exmp end
.class ANSI
.system
