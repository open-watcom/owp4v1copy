.func memset _fmemset wmemset _umemset
#include <string.h>
void *memset( void *dst, int c, size_t length );
.ixfunc2 '&String' &func
.if &farfnc eq 1 .do begin
void __far *_fmemset( void __far *dst, int c,
                      size_t length );
.ixfunc2 '&String' &ffunc
.do end
.if &'length(&wfunc.) ne 0 .do begin
wchar_t *wmemset( wchar_t *dst,
                  wchar_t c,
                  size_t length );
.ixfunc2 '&String' &wfunc
.do end
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. void *_umemset( void *dst,
:cmt.                 int c,
:cmt.                 size_t length );
:cmt. .ixfunc2 '&String' &ufunc
:cmt. .do end
.funcend
.desc begin
The &func function fills the first
.arg length
characters of the object pointed to by
.arg dst
with the value
.arg c.
.im farfunc
.if &'length(&wfunc.) ne 0 .do begin
.np
The &wfunc wide-character function is identical to &func except that it
operates on characters of
.kw wchar_t
type.
The argument
.arg length
is interpreted to mean the number of wide characters.
.do end
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. .np
:cmt. The &ufunc Unicode function is identical to &func except that it
:cmt. operates with 16-bit Unicode characters.
:cmt. The argument
:cmt. .arg length
:cmt. is interpreted to mean the number of Unicode characters.
:cmt. .do end
.desc end
.return begin
The &func function returns the pointer
.arg dst.
.return end
.see begin
.seelist &function. memchr memcmp memcpy memicmp memmove memset
.see end
.exmp begin
#include <string.h>

void main( void )
{
    char buffer[80];
.exmp break
    memset( buffer, '=', 80 );
}
.exmp end
.class ANSI
.system
