.func strftime wcsftime _wstrftime_ms _ustrftime _ustrftime_ms
.funcw wcsftime
#include <time.h>
size_t strftime( char *s,
                 size_t maxsize,
                 const char *format,
                 const struct tm *timeptr );
.ixfunc2 '&TimeFunc' &func
.if &'length(&wfunc.) ne 0 .do begin
#include <wchar.h>
size_t wcsftime( wchar_t *s,
                 size_t maxsize,
                 const wchar_t *format,
                 const struct tm *timeptr );
.ixfunc2 '&TimeFunc' &wfunc
.ixfunc2 '&Wide' &wfunc
#include <time.h>
size_t _wstrftime_ms( wchar_t *s,
                      size_t maxsize,
                      const char *format,
                      const struct tm *timeptr );
.ixfunc2 '&TimeFunc' _wstrftime_ms
.ixfunc2 '&Wide' _wstrftime_ms
.do end
.if &'length(&ufunc.) ne 0 .do begin
size_t _ustrftime( wchar_t *s,
                   size_t maxsize,
                   const wchar_t *format,
                   const struct tm *timeptr );
.ixfunc2 '&TimeFunc' &ufunc
size_t _ustrftime_ms( wchar_t *s,
                      size_t maxsize,
                      const char *format,
                      const struct tm *timeptr );
.ixfunc2 '&TimeFunc' &ufunc._ms
.do end
.im structtm
.funcend
.desc begin
The &func function formats the time in the argument
.arg timeptr
into the array pointed to by the argument
.arg s
according to the
.arg format
argument.
.im widefunc
.if &'length(&wfunc.) ne 0 .do begin
.np
The
.kw _wstrftime_ms
function is identical to &wfunc except that the
.arg format
is not a wide-character string.
.do end
.im unifunc
.if &'length(&ufunc.) ne 0 .do begin
.np
The
.kw _ustrftime_ms
Unicode function is identical to &ufunc except that the
.arg format
is not a Unicode string.
.do end
.np
The
.arg format
string consists of zero or more directives and ordinary characters.
A directive consists of a '%' character followed by a character
that determines the substitution that is to take place.
All ordinary characters are copied unchanged into the array.
No more than
.arg maxsize
characters are placed in the array.
The format directives %D, %h, %n, %r, %t, and %T are from POSIX.
.begnote
.termhd1 Directive
.termhd2 Meaning
.note %a
locale's abbreviated weekday name
.note %A
locale's full weekday name
.note %b
locale's abbreviated month name
.note %B
locale's full month name
.note %c
locale's appropriate date and time representation
.note %d
day of the month as a decimal number (01-31)
.note %D
date in the format mm/dd/yy (POSIX)
.note %h
locale's abbreviated month name (POSIX)
.note %H
hour (24-hour clock) as a decimal number (00-23)
.note %I
hour (12-hour clock) as a decimal number (01-12)
.note %j
day of the year as a decimal number (001-366)
.note %m
month as a decimal number (01-12)
.note %M
minute as a decimal number (00-59)
.note %n
newline character (POSIX)
.note %p
locale's equivalent of either AM or PM
.note %r
12-hour clock time (01-12) using the AM/PM notation in the format
HH:MM:SS (AM|PM) (POSIX)
.note %S
second as a decimal number (00-59)
.note %t
tab character (POSIX)
.note %T
24-hour clock time in the format HH:MM:SS (POSIX)
.note %U
week number of the year as a decimal number (00-52) where Sunday
is the first day of the week
.note %w
weekday as a decimal number (0-6) where 0 is Sunday
.note %W
week number of the year as a decimal number (00-52) where Monday
is the first day of the week
.note %x
locale's appropriate date representation
.note %X
locale's appropriate time representation
.note %y
year without century as a decimal number (00-99)
.note %Y
year with century as a decimal number
.note %Z, %z
timezone name, or by no characters if no timezone exists
(%z is an extension to ANSI/POSIX)
.note %%
character %
.endnote
.np
When the
.mono %Z
or
.mono %z
directive is specified, the
.kw tzset
function is called.
.desc end
.return begin
If the number of characters to be placed into the array is less than
.arg maxsize,
the &func function returns the number of characters placed into the
array pointed to by
.arg s
not including the terminating null character.
Otherwise, zero is returned.
.im errnoref
.return end
.see begin
.seelist strftime setlocale
.im seetime strftime
.see end
.exmp begin
#include <stdio.h>
#include <time.h>

void main()
  {
    time_t time_of_day;
    char buffer[ 80 ];
.exmp break
    time_of_day = time( NULL );
    strftime( buffer, 80, "Today is %A %B %d, %Y",
               localtime( &time_of_day ) );
    printf( "%s\n", buffer );
  }
.exmp output
Today is Friday December 25, 1987
.exmp end
.* The following line forces an entry out for the function
.sr mfunc='x _wstrftime_ms'
.class ANSI, POSIX
.system
