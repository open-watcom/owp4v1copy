.func memcmp _fmemcmp _umemcmp
#include <string.h>
int memcmp( const void *s1,
            const void *s2,
            size_t length );
.ixfunc2 '&Compare' &func
.if &farfnc eq 1 .do begin
int _fmemcmp( const void __far *s1,
              const void __far *s2,
              size_t length );
.ixfunc2 '&Compare' &ffunc
.do end
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. int _umemcmp( const void *s1,
:cmt.               const void *s2,
:cmt.               size_t length );
:cmt. .ixfunc2 '&Compare' &ufunc
:cmt. .do end
.funcend
.desc begin
The &func function compares the first
.arg length
characters of the object pointed to by
.arg s1
to the object pointed to by
.arg s2.
.im farparm
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. .np
:cmt. The &ufunc Unicode function is identical to &func except that it
:cmt. operates on 16-bit Unicode character strings.
:cmt. The argument
:cmt. .arg length
:cmt. is interpreted to mean the number of Unicode characters.
:cmt. .do end
.desc end
.return begin
The &func function returns an integer less than, equal to, or greater
than zero, indicating that the object pointed to by
.arg s1
is less than, equal to, or greater than the object pointed to by
.arg s2.
.return end
.see begin
.seelist &function. memchr memcmp memcpy memicmp memset
.see end
.exmp begin
#include <stdio.h>
#include <string.h>

void main()
  {
    auto char buffer[80];
.exmp break
    strcpy( buffer, "world" );
    if( memcmp( buffer, "Hello ", 6 ) < 0 ) {
      printf( "Less than\n" );
    }
  }
.exmp end
.class ANSI
.system
