.func memccpy _fmemccpy _umemccpy
#include <string.h>
void *memccpy( void *dest, const void *src,
               int c, size_t cnt );
.ixfunc2 '&String' &func
.if &farfnc eq 1 .do begin
void __far *_fmemccpy( void __far *dest,
                       const void __far *src,
                       int c, size_t cnt );
.ixfunc2 '&String' &ffunc
.do end
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. void *_umemccpy( void *dest, const void *src,
:cmt.                  int c, size_t cnt );
:cmt. .ixfunc2 '&String' &ufunc
:cmt. .do end
.funcend
.desc begin
The &func function copies bytes from
.arg src
to
.arg dest
up to and including the first occurrence of the character
.arg c
or until
.arg cnt
bytes have been copied, whichever comes first.
.im farfunc
:cmt. .if &'length(&ufunc.) ne 0 .do begin
:cmt. .np
:cmt. The &ufunc Unicode function is identical to &func except that it
:cmt. operates on 16-bit Unicode characters.
:cmt. The argument
:cmt. .arg cnt
:cmt. is interpreted to mean the number of Unicode characters.
:cmt. .do end
.desc end
.return begin
The &func function returns a pointer to the byte in
.arg dest
following the character
.arg c
if one is found and copied, otherwise it returns NULL.
.return end
.see begin
.seelist memccpy memcpy memmove memset
.see end
.exmp begin
#include <stdio.h>
#include <string.h>

char *msg = "This is the string: not copied";
.exmp break
void main()
  {
    auto char buffer[80];
.exmp break
    memset( buffer, '\0', 80 );
    memccpy( buffer, msg, ':', 80 );
    printf( "%s\n", buffer );
  }
.exmp output
This is the string:
.exmp end
.class WATCOM
.system
