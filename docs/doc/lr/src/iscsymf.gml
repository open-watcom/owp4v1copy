.func __iscsymf
#include <ctype.h>
int __iscsymf( int c );
.ixfunc2 '&CharTest' &func
.funcend
.desc begin
The &func function tests for a letter or underscore.
.desc end
.return begin
A non-zero value is returned when the character is a letter or
underscore; otherwise, zero is returned.
.return end
.see begin
.seelist &function. isalpha isalnum iscntrl isdigit isgraph islower isprint
.seelist &function. ispunct isspace isupper isxdigit tolower toupper
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
        printf( "Char %c is %sa csymf character\n",
                chars[i],
                ( __iscsymf( chars[i] ) ) ? "" : "not " );
    }
}
.exmp output
Char A is a csymf character
Char   is not a csymf character
Char _ is a csymf character
Char 9 is not a csymf character
Char + is not a csymf character
.exmp end
.class WATCOM
.system
