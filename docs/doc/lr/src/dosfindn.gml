.func _dos_findnext
#include <&doshdr>
unsigned _dos_findnext( struct find_t *buffer );

struct find_t {
    char reserved[21];      /* reserved for use by DOS   */
    char attrib;            /* attribute byte for file   */
    unsigned short wr_time; /* time of last write to file*/
    unsigned short wr_date; /* date of last write to file*/
    unsigned long  size;    /* length of file in bytes   */
    char name[13];          /* null-terminated filename  */
};
.ixfunc2 '&DosFunc' &func
.funcend
.desc begin
The &func function uses system call 0x4F to return information on the
next file whose name and attributes match the pattern supplied to the
.kw _dos_findfirst
function.
The function
.kw _dos_findfirst
must be called before &func
to get information on the first file that matches.
The information is returned in a
.kw find_t
structure pointed to by
.arg buffer.
.desc end
.return begin
The &func function returns zero if successful.
Otherwise, it returns an OS error code and sets
.kw errno
accordingly.
.return end
.see begin
.seelist _dos_findnext _dos_findfirst opendir readdir closedir
.see end
.exmp begin
#include <stdio.h>
#include <&doshdr>
.exmp break
void main()
{
    struct find_t   fileinfo;
    unsigned        rc;         /* return code */
.exmp break
    /* Display name and size of "*.c" files */
    rc = _dos_findfirst( "*.c", _A_NORMAL, &fileinfo );
    while( rc == 0 ) {
        printf( "%14s %10ld\n", fileinfo.name,
                                fileinfo.size );
        rc = _dos_findnext( &fileinfo );
    }
}
.exmp end
.class DOS
.system
