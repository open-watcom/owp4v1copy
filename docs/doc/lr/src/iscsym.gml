.func __iscsym
#include <ctype.h>
int __iscsym( int c );
.ixfunc2 '&CharTest' &func
.funcend
.desc begin
The &func function tests for a letter, underscore or digit.
.desc end
.return begin
A non-zero value is returned when the character is a letter,
underscore or digit; otherwise, zero is returned.
.return end
.see begin
.im seeis &function.
.see end
.exmp begin
#include <stdio.h>
#include <ctype.h>

char chars[] = {
    'A',
    0x80,
    '_',
    '9',
    '+'
};
.exmp break
#define SIZE sizeof( chars ) / sizeof( char )
.exmp break
void main()
{
    int   i;
.exmp break
    for( i = 0; i < SIZE; i++ ) {
        printf( "Char %c is %sa C symbol character\n",
                chars[i],
                ( __iscsym( chars[i] ) ) ? "" : "not " );
    }
}
.exmp output
Char A is a C symbol character
Char   is not a C symbol character
Char _ is a C symbol character
Char 9 is a C symbol character
Char + is not a C symbol character
.exmp end
.class WATCOM
.system
