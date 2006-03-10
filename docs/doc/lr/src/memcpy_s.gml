.func memcpy_s wmemcpy_s
#define __STDC_WANT_LIB_EXT1__  1
#include <string.h>
errno_t memcpy_s( void * restrict s1,
                  rsize_t s1max,
                  const void * restrict s2,
                  rsize_t n );
.ixfunc2 '&Copy' &func
.if &'length(&ffunc.) ne 0 .do begin
errno_t _fmemcpy_s( void __far * restrict s1,
                    rsize_t s1max,
                    const void __far * restrict s2,
                    size_t n );
.ixfunc2 '&Copy' &ffunc
.do end
.if &'length(&wfunc.) ne 0 .do begin
#include <wchar.h>
errno_t wmemcpy_s( wchar_t * restrict s1,
                   rsize_t s1max,
                   const wchar_t * restrict s2,
                   size_t n );
.ixfunc2 '&Copy' &wfunc
.do end
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. void *_umemcpy_s( void * restrict s1,
:cmt.                   rsize_t s1max,
:cmt.                   const void * restrict s2,
:cmt.                   size_t n );
:cmt. .ixfunc2 '&Copy' &ufunc
:cmt. .do end
.funcend
.*
.rtconst begin
Neither
.arg s1
nor
.arg s2
shall be a null pointer. Neither
.arg s1max
nor
.arg n
shall be greater than
.kw RSIZE_MAX.
.arg n
shall not be greater than
.arg s1max.
. Copying shall not take place between
objects that overlap.
.np
If there is a runtime-constraint violation, the &func function stores zeros in the first
.arg s1max
characters of the object pointed to by
.arg s1
if
.arg s1
is not a null pointer and
.arg s1max
is
not greater than
.kw RSIZE_MAX.
.rtconst end
.*
.desc begin
The &func function copies
.arg n
characters from the buffer pointed to by
.arg s2
into the buffer pointed to by
.arg s1.
Copying between overlapping objects is not allowed.
See the
.kw memmove_s
function if you wish to copy objects that overlap.
.if &'length(&_func.) ne 0 .do begin
.im farfunc
.do end
.if &'length(&wfunc.) ne 0 .do begin
.np
The &wfunc wide-character function is identical to &func except that it
operates on characters of
.kw wchar_t
type.
The arguments
.arg s1max
and
.arg n
are interpreted to mean the number of wide characters.
.do end
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. .np
:cmt. The &ufunc Unicode function is identical to &func except that it
:cmt. operates on 16-bit Unicode character strings.
:cmt. The argument
:cmt. .arg n
:cmt. is interpreted to mean the number of Unicode characters.
:cmt. .do end
.desc end
.*
.return begin
.saferet
.return end
.*
.see begin
.seelist &function. memcpy memchr memcmp memcpy memicmp memmove memset memmove_s
.see end
.*
.exmp begin
#define __STDC_WANT_LIB_EXT1__  1
#include <stdio.h>
#include <string.h>

void main( void )
{
    char buffer[80];
.exmp break
    memcpy_s( buffer, sizeof( buffer ), "Hello", 5 );
    buffer[5] = '\0';
    printf( "%s\n", buffer );
}
.exmp end
.*
.class TR 24731
.system
