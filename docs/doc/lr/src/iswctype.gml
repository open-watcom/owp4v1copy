.func iswctype
#include <wctype.h>
int iswctype( wint_t wc, wctype_t desc );
.ixfunc2 '&CharTest' &wfunc
.ixfunc2 '&Wide' &wfunc
.funcend
.*
.desc begin
The &func function determines whether the wide character
.arg wc
has the property described by
.arg desc.
Valid values of
.arg desc
are defined by the use of the
.kw wctype
function.
.np
The eleven expressions listed below have a truth-value equivalent to a
call to the wide character testing function shown.
.begterm 20
.termhd1 Expression
.termhd2 Equivalent
.*
.term iswctype(wc, wctype("alnum"))
iswalnum(wc)
.*
.term iswctype(wc, wctype("alpha"))
iswalpha(wc)
.*
.term iswctype(wc, wctype("cntrl"))
iswcntrl(wc)
.*
.term iswctype(wc, wctype("digit"))
iswdigit(wc)
.*
.term iswctype(wc, wctype("graph"))
iswgraph(wc)
.*
.term iswctype(wc, wctype("lower"))
iswlower(wc)
.*
.term iswctype(wc, wctype("print"))
iswprint(wc)
.*
.term iswctype(wc, wctype("punct"))
iswpunct(wc)
.*
.term iswctype(wc, wctype("space"))
iswspace(wc)
.*
.term iswctype(wc, wctype("upper"))
iswupper(wc)
.*
.term iswctype(wc, wctype("xdigit"))
iswxdigit(wc)
.endterm
.desc end
.*
.return begin
The &func function returns non-zero (true) if and only if the value of
the wide character
.arg wc
has the property described by
.arg desc.
.return end
.*
.see begin
.im seeis &function.
.see end
.*
.exmp begin
#include <stdio.h>
#include <wctype.h>

char *types[11] = {
    "alnum",
    "alpha",
    "cntrl",
    "digit",
    "graph",
    "lower",
    "print",
    "punct",
    "space",
    "upper",
    "xdigit"
};
.exmp break
void main( void )
{
    int     i;
    wint_t  wc = 'A';
.exmp break
    for( i = 0; i < 11; i++ )
        if( iswctype( wc, wctype( types[i] ) ) )
            printf( "%s\n", types[i] );
}
.exmp output
alnum
alpha
graph
print
upper
xdigit
.exmp end
.*
.class ANSI
.system
