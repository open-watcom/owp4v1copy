/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  diff.c - public domain context diff program
*
****************************************************************************/

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>

#ifdef __LINUX__
    #include <sys/stat.h> /* For stat, etc. */
#endif

#include "diff.h"

typedef unsigned long ULONG;
typedef signed long SLONG;
typedef unsigned short USHORT;
typedef int INT;

#define  EOS    0
#define  TEMPFILE  "diff.tmp"
#define  TRUE   1
#define  FALSE      0

typedef struct candidate {
    SLONG       b;          /* Line in fileB     */
    SLONG       a;          /* Line in fileA     */
    SLONG       link;       /* Previous candidate    */
}               CANDIDATE;

typedef struct line {
    USHORT      hash;       /* Hash value etc.       */
    short       serial;     /* Line number        */
}               LINE;

LINE            *file[2];       /* Hash/line for total file  */
LINE            *sfile[2];      /* Hash/line after prefix  */
SLONG           len[2];         /* Actual lines in each file  */
SLONG           slen[2];        /* Squished lengths      */
SLONG           prefix;         /* Identical lines at start  */
SLONG           suffix;         /* Identical lenes at end  */

FILE            *infd[2] = { NULL, NULL}; /* Input file identifiers  */
FILE            *tempfd;        /* Temp for input redirection  */

/*
 * The following vectors overlay the area defined by file[0]
 */

short           *class;         /* Unsorted line numbers  */
SLONG           *klist;         /* Index of element in clist  */
CANDIDATE       *clist;         /* Storage pool for candidates  */
ULONG           clength = 0;    /* Number of active candidates  */
#define CSIZE_INC 50            /* How many to allocate each time we have to */
ULONG           csize = CSIZE_INC;      /* Current size of storage pool */

SLONG           *match;         /* Longest subsequence       */
long            *oldseek;       /* Seek position in file A  */

/*
 * The following vectors overlay the area defined by file[1]
 */

short           *member;        /* Concatenated equiv. classes  */
long            *newseek;       /* Seek position in file B  */

/*
 * Global variables
 */

INT             Hflag = FALSE;  /* half hearted algorithm */
INT             nflag = FALSE;  /* Edit script requested  */
INT             eflag = FALSE;  /* Edit script requested  */
INT             bflag = FALSE;  /* Blank supress requested  */
INT             cflag = FALSE;  /* Context printout      */
INT             iflag = FALSE;  /* Ignore case requested  */
INT             tflag = FALSE;  /* Test for enough memory flag */
INT             xflag = 0;      /* Test for enough memory flag */
INT             havediffs = FALSE;
char            text[1025];      /* Input line from file1  */
char            textb[1025];     /* Input from file2 for check  */

char const      *cmdusage =
"usage:\n"
"        diff [options] file1 file2\n"
"\n"
"options:\n"
"        -b           ignore blanks\n"
"        -c[n]        print n context line (defaults to 3)\n"
"        -e           generate an edit script\n"
"        -H           Use half hearted algorithm if memory runs out\n"
"        -i           ignore case\n"
"        -n           generate an edit script for RCS\n"
"        -t           quiet mode. return 3 if not enough memory\n"
"        -x           shift return codes by 100\n"
;

void    input( SLONG which );
void    squish( void );
void    sort( LINE *vector, SLONG vecsize );
void    equiv( void );
void    unsort( void );
ULONG   subseq( void );
SLONG   newcand( SLONG a, SLONG b, SLONG pred );
ULONG   search( ULONG low, ULONG high, SLONG b );
void    unravel( SLONG k );
INT     check( const char *fileAname, const char *fileBname );
void    output( const char *fileAname, const char *fileBname );
void    change( SLONG astart, SLONG aend, SLONG bstart, SLONG bend );
void    range( SLONG from, SLONG to, SLONG w );
void    fetch( const long *seekvec, SLONG start, SLONG end, SLONG trueend, FILE *fd, const char *pfx );
INT     getline( FILE *fd, char *buffer );
USHORT  hash( const char *buffer );
void    *myalloc( ULONG amount, const char *why );
void    myfree( void *what );
void    *compact( void *pointer, ULONG new_amount, const  char *why );
void    noroom( const char *why );
#ifdef DEBUG
#if 0
void    rdump( SLONG *pointer, char *why );
void    dump( LINE *d_linep, SLONG d_len, SLONG d_which );
#endif
void    dumpklist( ULONG kmax, const char *why );
#endif
INT     streq( const char *s1, const char *s2 );
void    cant( const char *filename, const char *what, SLONG fatalflag );
void    fatal( const char *format, ... );
void    error( const char *format, ... );
void    fputss( const char *s, FILE *iop );
char    *fgetss( char *s, SLONG n, FILE *iop );

/*
 * Diff main program
 */

// #include "diff.def"

INT main( INT argc, char * const *argv )
{
    SLONG       i;
    char const  *ap;
    struct stat st;
    char        path[_MAX_PATH];
    char        fname[_MAX_FNAME];
    char        ext[_MAX_EXT];
    ULONG       sub;

    while( argc > 1 && *( ap = argv[1] ) == '-' && *++ap != EOS ) {
        while( *ap != EOS ) {
            switch( ( *ap++ ) ) {
            case 'b':
                bflag++;
                break;

            case 'c':
                if( *ap > '0' && *ap <= '9' )
                    cflag = *ap++ -'0';
                else
                    cflag = 3;
                break;

            case 'e':
                eflag++;
                break;

            case 'H':
                Hflag++;
                break;

            case 'n':
                nflag++;
                break;

            case 'i':
                iflag++;
                break;

            case 't':
                tflag++;
                break;

            case 'x':
                xflag = DIFF_RETURN_ADD;
                break;

            default:
                error( "bad option '-%c'\n", ap[ -1] );
                exit( xflag + DIFF_NOT_COMPARED );
            }
        }
        argc--;
        argv++;
    }

    if( argc != 3 ) {
        error( cmdusage );
        exit( xflag + DIFF_NOT_COMPARED );
    }
    if( nflag + (INT)( cflag != 0 ) + eflag > 1 ) {
        error( " -c, -n and -e are incompatible.\n" );
        exit( xflag + DIFF_NOT_COMPARED );
    }
    argv++;
    for( i = 0; i <= 1; i++ ) {
        if( argv[i][0] == '-' && argv[i][1] == EOS ) {
            infd[i] = stdin;
            if( ( tempfd = fopen( TEMPFILE, "w" ) ) == NULL ) {
                cant( TEMPFILE, "work", 1 );
            }
        } else {
            strcpy( path, argv[i] );
            if( i == 1 && stat( argv[i], &st ) == 0 && S_ISDIR( st.st_mode ) ) {
                _splitpath( argv[i - 1], NULL, NULL, fname, ext );
                _makepath( path, NULL, argv[i], fname, ext );
            }
            infd[i] = fopen( path, "r" );
            if( !infd[i] ) {
                cant( path, "input", 2 );      /* Fatal error */
            }
        }
    }

    if( infd[0] == stdin && infd[1] == stdin ) {
        error( "Can't diff two things both on standard input." );
        exit( xflag + DIFF_NOT_COMPARED );
    }
    if( infd[0] == NULL && infd[1] == NULL ) {
        cant( argv[0], "input", 0 );
        cant( argv[1], "input", 1 );
    }

    /*
     * Read input, building hash tables.
     */
    input( 0 );
    input( 1 );
    squish();
#ifdef DEBUG
    printf( "before sort\n" );
    for( i = 1; i <= slen[0]; i++ ) {
        printf( "sfile[0][%d] = %6d %06o\n",
                i, sfile[0][i].serial, sfile[0][i].hash );
    }
    for( i = 1; i <= slen[1]; i++ ) {
        printf( "sfile[1][%d] = %6d %06o\n",
                i, sfile[1][i].serial, sfile[1][i].hash );
    }
#endif
    sort( sfile[0], slen[0] );
    sort( sfile[1], slen[1] );
#ifdef DEBUG
    printf( "after sort\n" );
    for( i = 1; i <= slen[0]; i++ ) {
        printf( "sfile[0][%d] = %6d %06o\n",
                i, sfile[0][i].serial, sfile[1][i].hash );
    }
    for( i = 1; i <= slen[1]; i++ ) {
        printf( "sfile[1][%d] = %6d %06o\n",
                i, sfile[1][i].serial, sfile[1][i].hash );
    }
#endif

    /*
     * Build equivalence classes.
     */
    member = (void*)file[1];
    equiv();
    /* Contrary to appearances, *member is a short */
    member = compact( member, ( (ULONG)slen[1] + 2 ) * sizeof( SLONG ),
                                 "squeezing member vector" );
    file[1] = (void*)member;

    /*
     * Reorder equivalence classes into array class[]
     */
    class = (void*)file[0];
    unsort();
    class = compact( class, ( (ULONG)slen[0] + 2 ) * sizeof( SLONG ),
                                "compacting class vector" );
    file[0] = (void*)class;
    /*
     * Find longest subsequences
     */
    klist = myalloc( ( (ULONG)slen[0] + 2 ) * sizeof( SLONG ), "klist" );
    clist = myalloc( csize * sizeof( CANDIDATE ), "clist" );
    sub = subseq();
    myfree( &member );
    file[1] = NULL;
    myfree( &class );
    file[0] = NULL;
    match = myalloc( ( (ULONG)len[0] + 2 ) * sizeof( SLONG ), "match" );
    unravel( klist[sub] );
    myfree( &clist );
    myfree( &klist );

    /*
     * Check for fortuitous matches and output differences
     */
    oldseek = myalloc( ( (ULONG)len[0] + 2 ) * sizeof *oldseek, "oldseek" );
    newseek = myalloc( ( (ULONG)len[1] + 2 ) * sizeof *newseek, "newseek" );
    if( check( argv[0], argv[1] ) ) {
#ifdef DEBUG
        fprintf( stderr, "Spurious match, output is not optimal\n" );
#else
        ;
#endif
    }
    output( argv[0], argv[1] );
    if( tempfd != NULL ) {
        fclose( tempfd );
        unlink( TEMPFILE );
    }
    myfree( &oldseek );
    myfree( &newseek );
    myfree( file + 0 );
    myfree( file + 1 );

    return( xflag + ( havediffs ? DIFF_HAVE_DIFFS : DIFF_NO_DIFFS ) );
}

/*
 * Read the file, building hash table
 */

void input( SLONG which )      /* 0 or 1 to select from infd[]  */
{
    LINE       *lentry;
    SLONG      linect = 0;
    FILE                *fd;
#define LSIZE_INC 200           /* # of line entries to alloc at once */
    SLONG               lsize = LSIZE_INC;

    lentry = myalloc( ( (ULONG)lsize + 3 ) * sizeof *lentry, "line" );
    fd = infd[which];
    while( !getline( fd, text ) ) {
        if( ++linect >= lsize ) {
            lsize += 200;
            lentry = compact( lentry, ( (ULONG)lsize + 3 ) * sizeof *lentry,
                                         "extending line vector" );
        }
        lentry[linect].hash = hash( text );
    }

    /*
     * If input was from stdin ("-" command), finish off the temp file.
     */
    if( fd == stdin ) {
        fclose( tempfd );
        tempfd = infd[which] = fopen( TEMPFILE, "r" );
    }

    /*
     * If we wanted to be stingy with memory, we could realloc lentry down to
     * its exact size (+3 for some odd reason) here.  No need?
     */
    len[which] = linect;
    file[which] = lentry;
}

/*
 * Look for initial and trailing sequences that have identical hash values.
 * Don't bother building them into the candidate vector.
 */

void squish( void )
{
    SLONG       i;
    LINE        *ap;
    LINE const  *bp;
    SLONG       j;
    SLONG       k;

    /*
     * prefix -> first line (from start) that doesn't hash identically
     */
    i = 0;
    ap = file[0] + 1;
    bp = file[1] + 1;
    while( i < len[0] && i < len[1] && ap->hash == bp->hash ) {
        i++;
        ap++;
        bp++;
    }
    prefix = i;

    /*
     * suffix -> first line (from end) that doesn't hash identically
     */
    j = len[0] - i;
    k = len[1] - i;
    ap = file[0] + len[0];
    bp = file[1] + len[1];
    i = 0;
    while( i < j && i < k && ap->hash == bp->hash ) {
        i++;
        ap--;
        bp--;
    }
    suffix = i;

    /*
     * Tuck the counts away
     */
    for( k = 0; k <= 1; k++ ) {
        sfile[k] = file[k] + prefix;
        j = slen[k] = len[k] - ( prefix + suffix );

        for( i = 0, ap = sfile[k]; i <= slen[k]; i++, ap++ ) {
            ap->serial = (short)i;
        }
    }
}

/*
 * Sort hash entries
 */

void sort(
     LINE       *vector,     /* What to sort       */
     SLONG      vecsize )    /* How many to sort      */
{
    SLONG       j;
    LINE        *aim;
    LINE        *ai;
    SLONG       mid;
    SLONG       k;
    LINE        work;

    for( j = 1; j <= vecsize; j *= 2 )
        ;
    mid = ( j - 1 );
    while( ( mid /= 2 ) != 0 ) {
        k = vecsize - mid;
        for( j = 1; j <= k; j++ ) {
            for( ai = &vector[j]; ai > vector; ai -= mid ) {
                aim = &ai[mid];
                if( aim < ai )
		    break;      /* ?? Why ??     */
                if( aim->hash > ai->hash ||
                    ( aim->hash == ai->hash &&
                     aim->serial > ai->serial ) ) break;
                work.hash = ai->hash;
                ai->hash = aim->hash;
                aim->hash = work.hash;
                work.serial = ai->serial;
                ai->serial = aim->serial;
                aim->serial = work.serial;
            }
        }
    }
}

/*
 * Build equivalence class vector
 */

void equiv( void )
{
    LINE        *ap;
    union {
        LINE    *bp;
        short   *mp;
    }           r;
    SLONG       j;
    LINE const  *atop;

#ifdef DEBUG
    printf( "equiv entry\n" );
    for( j = 1; j <= slen[0]; j++ ) {
        printf( "sfile[0][%d] = %6d %06o\n",
                j, sfile[0][j].serial, sfile[0][j].hash );
    }
    for( j = 1; j <= slen[1]; j++ ) {
        printf( "sfile[1][%d] = %6d %06o\n",
                j, sfile[1][j].serial, sfile[1][j].hash );
    }
#endif
    j = 1;
    ap = sfile[0] + 1;
    r.bp = sfile[1] + 1;
    atop = sfile[0] + slen[0];
    while( ap <= atop && j <= slen[1] ) {
        if( ap->hash < r.bp->hash ) {
            ap->hash = 0;
            ap++;
        } else if( ap->hash == r.bp->hash ) {
            ap->hash = (USHORT)j;
            ap++;
        } else {
            r.bp++;
            j++;
        }
    }
    while( ap <= atop ) {
        ap->hash = 0;
        ap++;
    }
    sfile[1][slen[1] + 1].hash = 0;
#ifdef DEBUG
    printf( "equiv exit\n" );
    for( j = 1; j <= slen[0]; j++ ) {
        printf( "sfile[0][%d] = %6d %06o\n",
                j, sfile[0][j].serial, sfile[0][j].hash );
    }
    for( j = 1; j <= slen[1]; j++ ) {
        printf( "sfile[1][%d] = %6d %06o\n",
                j, sfile[1][j].serial, sfile[1][j].hash );
    }
#endif
    ap = sfile[1] + 0;
    atop = sfile[1] + slen[1];
    r.mp = &member[0];
    while( ++ap <= atop ) {
        r.mp++;
        *r.mp = -( ap->serial );
        while( ap[1].hash == ap->hash ) {
            ap++;
            r.mp++;
            *r.mp = ap->serial;
        }
    }
    r.mp[1] = -1;
#ifdef DEBUG
    for( j = 0; j <= slen[1]; j++ ) {
        printf( "member[%d] = %d\n", j, member[j] );
    }
#endif
}

/*
 * Build class vector
 */

void unsort( void )
{
    SLONG       *temp;
    SLONG const *tp;
    union {
        LINE    *ap;
        short   *cp;
    }           u;
    LINE const  *evec;
    short const *eclass;
#ifdef DEBUG
    SLONG       i;
#endif

    temp = myalloc( ( (ULONG)slen[0] + 1 ) * sizeof *temp, "unsort scratch" );
    u.ap = sfile[0] + 1;
    evec = sfile[0] + slen[0];
    while( u.ap <= evec ) {
#ifdef DEBUG
        printf( "temp[%2d] := %06o\n", u.ap->serial, u.ap->hash );
#endif
        temp[u.ap->serial] = u.ap->hash;
        u.ap++;
    }

    /*
     * Copy into class vector and free work space
     */
    u.cp = class + 1;
    eclass = class + slen[0];
    tp = temp + 1;
    while( u.cp <= eclass ) {
        *u.cp++ = (short)*tp++;
    }
    myfree( &temp );
#ifdef DEBUG
    printf( "unsort exit\n" );
    for( i = 1; i <= slen[0]; i++ ) {
        printf( "class[%d] = %d %06o\n", i, class[i], class[i] );
    }
#endif
}

/*
 * Generate maximum common subsequence chain in clist[]
 */

ULONG subseq( void )
{
    SLONG       a;
    ULONG       ktop;
    SLONG       b;
    ULONG       s;
    ULONG       r;
    SLONG       i;
    SLONG       cand;

    klist[0] = newcand( 0, 0, -1 );
    klist[1] = newcand( slen[0] + 1, slen[1] + 1, -1 );
    ktop = 1;                   /* -> guard entry  */
    for( a = 1; a <= slen[0]; a++ ) {
        /*
         * For each non-zero element in file[0] ...
         */
        if( ( i = class[a] ) == 0 )
	    continue;
        cand = klist[0];        /* No candidate now  */
        r = 0;                  /* Current r-candidate  */
        do {
#ifdef DEBUG
            printf( "a = %d, i = %d, b = %d\n", a, i, member[i] );
#endif

            /*
             * Perform the merge algorithm
             */
            if( ( b = member[i] ) < 0 ) {
                b = -b;
            }
            s = search( r, ktop, b );
#ifdef DEBUG
            printf( "search(%d, %d, %d) -> %lu\n", r, ktop, b, s );
#endif
            if( s != 0 ) {
                if( clist[klist[s]].b > b ) {
                    klist[r] = cand;
                    r = s;
                    cand = newcand( a, b, klist[s - 1] );
#ifdef DEBUG
                    dumpklist( ktop, "klist[s-1]->b > b" );
#endif
                }
                if( s >= ktop ) {
                    klist[ktop + 1] = klist[ktop];
                    ktop++;
#ifdef DEBUG
                    klist[r] = cand;
                    dumpklist( ktop, "extend" );
#endif
                    break;
                }
            }
        } while( member[++i] > 0 );
        klist[r] = cand;
    }
#ifdef DEBUG
    printf( "last entry = %d\n", ktop - 1 );
#endif
    return( ktop - 1 );          /* Last entry found  */
}

SLONG newcand(
    SLONG       a,          /* Line in file[0]      */
    SLONG       b,          /* Line in file[1]      */
    SLONG       pred )      /* Link to predecessor, index in cand[]  */
{
    CANDIDATE   *new;

    clength++;
    if( ++clength >= csize ) {
        csize += CSIZE_INC;
        clist = compact( clist, csize * sizeof *clist, "extending clist" );
    }
    new = &clist[clength - 1];
    new->a = a;
    new->b = b;
    new->link = pred;
    return( (SLONG)clength - 1 );
}

/*
 * Search klist[low..top] (inclusive) for b.  If klist[low]->b >= b,
 * return zero.  Else return s such that klist[s-1]->b < b and
 * klist[s]->b >= b.  Note that the algorithm presupposes the two
 * preset "fence" elements, (0, 0) and (slen[0], slen[1]).
 */

ULONG search(
     ULONG     low,
     ULONG     high,
     SLONG     b )
{
    SLONG      temp;
    ULONG      mid;

    if( clist[klist[low]].b >= b )
        return( 0 );
    while( ( mid = ( low + high ) / 2 ) > low ) {
        if( ( temp = clist[klist[mid]].b ) > b )
            high = mid;
        else if( temp < b )
            low = mid;
        else {
            return( mid );
        }
    }
    return( mid + 1 );
}

void unravel( SLONG k )
{
    SLONG               i;
    CANDIDATE const     *cp;
    SLONG               first_trailer;
    SLONG               difference;

    first_trailer = len[0] - suffix;
    difference = len[1] - len[0];
#ifdef DEBUG
    printf( "first trailer = %d, difference = %d\n",
            first_trailer, difference );
#endif
    for( i = 0; i <= len[0]; i++ ) {
        match[i] = ( i <= prefix ) ? i
            : ( i > first_trailer ) ? i + difference
            : 0;
    }
#ifdef DEBUG
    printf( "unravel\n" );
#endif
    while( k != -1 ) {
        cp = &clist[k];
#ifdef DEBUG
        if( k < 0 || (ULONG)k >= clength )
            error( "Illegal link -> %d", k );
        printf( "match[%d] := %d\n", cp->a + prefix, cp->b + prefix );
#endif
        match[cp->a + prefix] = cp->b + prefix;
        k = cp->link;
    }
}

/*
 * Check for hash matches (jackpots) and collect random access indices to
 * the two files.
 *
 * It should be possible to avoid doing most of the ftell's by noticing
 * that we are not doing a context diff and noticing that if a line
 * compares equal to the other file, we will not ever need to know its
 * file position.  FIXME.
 */

INT check( const char *fileAname, const char *fileBname )
{
    SLONG       a;          /* Current line in file A  */
    SLONG       b;          /* Current line in file B  */
    SLONG       jackpot;

    /*
     * The VAX C ftell() returns the address of the CURRENT record, not the
     * next one (as in DECUS C or, effectively, other C's).  Hence, the values
     * are "off by one" in the array.  OFFSET compensates for this.
     */

#define OFFSET 0

    b = 1;
    rewind( infd[0] );
    rewind( infd[1] );
    /*
     * See above; these would be over-written on VMS anyway.
     */

    oldseek[0] = ftell( infd[0] );
    newseek[0] = ftell( infd[1] );

    jackpot = 0;
#ifdef DEBUG
    printf( "match vector\n" );
    for( a = 0; a <= len[0]; a++ )
        printf( "match[%d] = %d\n", a, match[a] );
#endif
    for( a = 1; a <= len[0]; a++ ) {
        if( match[a] == 0 ) {
            /* Unique line in A */
            oldseek[a + OFFSET] = ftell( infd[0] );
            getline( infd[0], text );
            continue;
        }
        while( b < match[a] ) {
            /* Skip over unique lines in B */
            newseek[b + OFFSET] = ftell( infd[1] );
            getline( infd[1], textb );
            b++;
        }

        /*
         * Compare the two, supposedly matching, lines. Unless we are going
         * to print these lines, don't bother to remember where they are.  We
         * only print matching lines if a context diff is happening, or if a
         * jackpot occurs.
         */
        //      if (cflag) {
        oldseek[a + OFFSET] = ftell( infd[0] );
        newseek[b + OFFSET] = ftell( infd[1] );
        //      }
        getline( infd[0], text );
        getline( infd[1], textb );
        if( !streq( text, textb ) ) {
#ifdef DEBUG
            fprintf( stderr, "Spurious match:\n" );
            fprintf( stderr, "line %d in %s, \"%s\"\n",
                     a, fileAname, text );
            fprintf( stderr, "line %d in %s, \"%s\"\n",
                     b, fileBname, textb );
#else
            (void)fileAname, (void)fileBname;
#endif
            match[a] = 0;
            jackpot++;
        }
        b++;
    }
    for( ; b <= len[1]; b++ ) {
        newseek[b + OFFSET] = ftell( infd[1] );
        getline( infd[1], textb );
    }
    /*
     * The logical converse to the code up above, for NON-VMS systems, to
     * store away an fseek() pointer at the beginning of the file.  For VMS,
     * we need one at EOF...
     */

    return( jackpot );
}

void output( const char *fileAname, const char *fileBname )
{
    SLONG       astart;
    SLONG       aend = 0;
    SLONG       bstart;
    SLONG       bend;

    rewind( infd[0] );
    rewind( infd[1] );
    match[0] = 0;
    match[len[0] + 1] = len[1] + 1;
    if( !eflag ) {
        if( cflag ) {

            /*
             * Should include ctime style dates after the file names, but
             * this would be non-trivial on OSK.  Perhaps there should be a
             * special case for stdin.
             */
            printf( "*** %s\n--- %s\n", fileAname, fileBname );
        }

        /*
         * Normal printout
         */
        for( astart = 1; astart <= len[0]; astart = aend + 1 ) {

            /*
             * New subsequence, skip over matching stuff
             */
            while( astart <= len[0] && match[astart] == ( match[astart - 1] + 1 ) ) {
                astart++;
            }

            /*
             * Found a difference, setup range and print it
             */
            bstart = match[astart - 1] + 1;
            aend = astart - 1;
            while( aend < len[0] && match[aend + 1] == 0 ) {
                aend++;
            }
            bend = match[aend + 1] - 1;
            match[aend] = bend;
            change( astart, aend, bstart, bend );
        }
    } else {

        /*
         * Edit script output -- differences are output "backwards" for the
         * benefit of a line-oriented editor.
         */
        for( aend = len[0]; aend >= 1; aend = astart - 1 ) {
            while( aend >= 1 && match[aend] == ( match[aend + 1] - 1 )
                   && match[aend] != 0 ) {
                aend--;
            }
            bend = match[aend + 1] - 1;
            astart = aend + 1;
            while( astart > 1 && match[astart - 1] == 0 ) {
                astart--;
            }
            bstart = match[astart - 1] + 1;
            match[astart] = bstart;
            change( astart, aend, bstart, bend );
        }
    }
    if( len[0] == 0 ) {
        change( 1, 0, 1, len[1] );
    }
}

/*
 * Output a change entry: file[0][astart..aend] changed to file[1][bstart..bend]
 */

void change( SLONG astart, SLONG aend, SLONG bstart, SLONG bend )
{
    char        c;

    /*
     * This catches a "dummy" last entry
     */
    if( astart > aend && bstart > bend )
        return;
    havediffs = TRUE;
    c = ( astart > aend ) ? 'a' : ( bstart > bend ) ? 'd' : 'c';
    if( nflag ) {
        if( c == 'a' ) {
            printf( "a%ld %ld\n", astart - 1, bend + 1 - bstart );
            fetch( newseek, bstart, bend, len[1], infd[1], "" );
        } else if( c == 'd' ) {
            printf( "d%ld %ld\n", astart, aend + 1 - astart );
        } else {
            printf( "d%ld %ld\n", astart, aend + 1 - astart );
            printf( "a%ld %ld\n", aend, bend + 1 - bstart );
            fetch( newseek, bstart, bend, len[1], infd[1], "" );
        }
        return;
    }
    if( cflag ) {
        fputs( "**************\n*** ", stdout );
    }

    if( c == 'a' && !cflag ) {
        range( astart - 1, astart - 1, 0 );       /* Addition: just print one
                                                   * odd # */
    } else {
        range( astart, aend, 0 ); /* Print both, if different */
    }
    if( !cflag ) {
        putchar( c );
        if( !eflag ) {
            if( c == 'd' ) {
                range( bstart - 1, bstart - 1, 1 );       /* Deletion: just print
                                                           * one odd # */
            } else {
                range( bstart, bend, 1 ); /* Print both, if different */
            }
        }
    }
    putchar( '\n' );
    if( ( !eflag && c != 'a' ) || cflag ) {
        fetch( oldseek, astart, aend, len[0], infd[0],
               cflag ? ( c == 'd' ? "- " : "! " ) : "< " );
        if( cflag ) {
            fputs( "--- ", stdout );
            range( bstart, bend, 1 );
            fputs( " -----\n", stdout );
        } else if( astart <= aend && bstart <= bend ) {
            printf( "---\n" );
        }
    }
    if( bstart <= bend ) {
        fetch( newseek, bstart, bend, len[1], infd[1],
               cflag ? ( c == 'a' ? "+ " : "! " ) : ( eflag ? "" : "> " ) );
        if( eflag )
	    printf( ".\n" );
    }
}

/*
 * Print a range
 */

void range( SLONG from, SLONG to, SLONG w )
{
    if( cflag ) {
        if( ( from -= cflag ) <= 0 ) {
            from = 1;
        }
        if( ( to += cflag ) > len[w] ) {
            to = len[w];
        }
    }
    if( to > from ) {
        printf( "%ld,%ld", from, to );
    } else if( to < from ) {
        printf( "%ld,%ld", to, from );
    } else {
        printf( "%ld", from );
    }
}

/*
 * Print the appropriate text
 */

void fetch( const long *seekvec, SLONG start, SLONG end, SLONG trueend, FILE *fd, const char *pfx )
{
    SLONG       i;
    SLONG       first;
    SLONG       last;

    if( cflag ) {
        if( ( first = start - cflag ) <= 0 ) {
            first = 1;
        }
        if( ( last = end + cflag ) > trueend ) {
            last = trueend;
        }
    } else {
        first = start;
        last = end;
    }
    if( fseek( fd, seekvec[first], 0 ) != 0 ) {
        fatal( "?Can't read line %d at %08lx (hex) in file%c\n", start,
            seekvec[first], ( fd == infd[0] ) ? 'A' : 'B' );
    } else {
        for( i = first; i <= last; i++ ) {
            if( fgetss( text, sizeof text, fd ) == NULL ) {
                fatal( "** Unexpected end of file\n" );
                break;
            }
#ifdef DEBUG
            printf( "%5d: %s%s\n", i, pfx, text );
#else
            fputs( ( cflag && ( i < start || i > end ) ) ? "  " : pfx, stdout );
            fputs( text, stdout );
            putchar( '\n' );
#endif
        }
    }
}

/*
 * Input routine, read one line to buffer[], return TRUE on eof, else FALSE.
 * The terminating newline is always removed.  If "-b" was given, trailing
 * whitespace (blanks and tabs) are removed and strings of blanks and
 * tabs are replaced by a single blank.  Getline() does all hacking for
 * redirected input files.
 */

INT getline( FILE *fd, char *buffer )
{
    char        *top;
    char const  *fromp;
    char        c;

    if( fgetss( buffer, sizeof text, fd ) == NULL ) {
        *buffer = EOS;
        return( TRUE );
    }
    if( fd == stdin ) {
        fputss( buffer, tempfd );
    }
    if( bflag || iflag ) {
        top = buffer;
        fromp = buffer;
        while( ( c = *fromp++ ) != EOS ) {
            if( bflag && ( c == ' ' || c == '\t' ) ) {
                c = ' ';
                while( *fromp == ' ' || *fromp == '\t' )
                    fromp++;
            }
            if( iflag ) {
                c = (char)tolower( c );
            }
            *top++ = c;
        }
        if( bflag && top[ -1] == ' ' ) {
            top--;
        }
        *top = EOS;
    }
    return( FALSE );
}

static USHORT    crc16a[] = {
    0000000, 0140301, 0140601, 0000500,
    0141401, 0001700, 0001200, 0141101,
    0143001, 0003300, 0003600, 0143501,
    0002400, 0142701, 0142201, 0002100,
};

static USHORT    crc16b[] = {
    0000000, 0146001, 0154001, 0012000,
    0170001, 0036000, 0024000, 0162001,
    0120001, 0066000, 0074000, 0132001,
    0050000, 0116001, 0104001, 0043000,
};

/*
 * Return the CRC16 hash code for the buffer
 * Algorithm from Stu Wecker (Digital memo 130-959-002-00).
 */

USHORT hash( const char *buffer )
{
    USHORT      crc;
    char const  *tp;
    short       temp;

    crc = 0;
    for( tp = buffer; *tp != EOS; ) {
        temp = (short)( *tp++ ^ crc );     /* XOR crc with new char  */
        crc = ( crc >> 8 ) ^ crc16a[ ( temp & 0017 )]
            ^ crc16b[ ( temp & 0360 ) >> 4];
    }
    return( ( crc == 0 ) ? ( USHORT ) 1 : crc );
}

/*
 * Allocate or crash.
 */

void *myalloc( ULONG amount, const char *why )
{
    void        *pointer;

#if !defined( __386__ )
    if( amount > UINT_MAX )
        noroom( why );
#endif
    if( ( pointer = malloc( amount ) ) == NULL )
        noroom( why );
    return( pointer );
}

void myfree( void *what )
{
    free( *( char **) what );
    *( char **) what = NULL;
}

void *compact( void *pointer, ULONG new_amount, const char *why )
{
    void                *new_pointer;
#ifdef DEBUG
    void const * const  old_pointer = pointer;
#endif

#if !defined( __386__ )
    if( new_amount > UINT_MAX )
        noroom( why );
#endif
    //    if (new_pointer = (char *)realloc((void *)pointer, (size_t)new_amount) == NULL)
    if( new_pointer = realloc( pointer, ( size_t ) new_amount ),
        new_pointer == NULL )
        noroom( why );

#ifdef DEBUG
    if( new_pointer != old_pointer ) {
        fprintf( stderr, "moved from %p to %p\n", old_pointer, new_pointer );
    }
#endif
    return( new_pointer );
}

void noroom( const char *why )
{
    if( tflag ) {
        exit( xflag + DIFF_NO_MEMORY );
    } else if( Hflag ) {
        #define freeup( x ) if( x ) myfree( &x );
        freeup( klist );
        freeup( clist );
        freeup( match );
        freeup( oldseek );
        freeup( newseek );
        freeup( file[0] );
        freeup( file[1] );
        printf( "d1 %ld\n", len[0] );
        printf( "a1 %ld\n", len[1] );
        fseek( infd[1], 0, 0 );
        while( fgetss( text, sizeof text, infd[1] ) != NULL )
            printf( "%s\n", text );
        exit( xflag + DIFF_HAVE_DIFFS );
    } else {
        error( "Out of memory when %s\n", why );
        exit( xflag + DIFF_NOT_COMPARED );
    }
}

#ifdef DEBUG
#if 0
/*
 * Dump memory block
 */

void rdump( SLONG *pointer, char *why )
{
    SLONG       *last;
    SLONG       count;

    last = ( ( SLONG **) (void*)pointer )[ -1];
    fprintf( stderr, "dump %s of %p -> %p, %d words",
             why, pointer, last, last - pointer );
    last = ( SLONG *) ( ( ( SLONG ) last ) & ~1 );
    for( count = 0; pointer < last; ++count ) {
        if( ( count & 07 ) == 0 ) {
            fprintf( stderr, "\n%p", pointer );
        }
        fprintf( stderr, "\t%ld", *pointer );
        pointer++;
    }
    fprintf( stderr, "\n" );
}
#endif
#endif

#ifdef DEBUG
#if 0
void dump( LINE *d_linep, SLONG d_len, SLONG d_which )
{
    SLONG       i;

    printf( "Dump of file%c, %d elements\n", "AB"[d_which], d_len );
    printf( "linep @ %p\n", d_linep );
    for( i = 0; i <= d_len; i++ ) {
        printf( "%3d  %6d  %06o\n", i,
                d_linep[i].serial, d_linep[i].hash );
    }
}
#endif

/*
 * Dump klist
 */

void dumpklist( ULONG kmax, const char *why )
{
    ULONG       i;
    CANDIDATE   *cp;
    SLONG       count;

    printf( "\nklist[0..%d] %s, clength = %lu\n", kmax, why, clength );
    for( i = 0; i <= kmax; i++ ) {
        cp = &clist[klist[i]];
        printf( "%2d %2d", i, klist[i] );
        if( cp >= &clist[0] && cp < &clist[clength] )
            printf( " (%2d %2d -> %2d)\n", cp->a, cp->b, cp->link );
        else if( klist[i] == -1 )
            printf( " End of chain\n" );
        else
            printf( " illegal klist element\n" );
    }
    for( i = 0; i <= kmax; i++ ) {
        count = -1;
        for( cp = (void *)klist[i]; cp > &clist[0]; cp = (void *)&cp->link ) {
            if( ++count >= 6 ) {
                printf( "\n    " );
                count = 0;
            }
            printf( " (%2d: %2d,%2d -> %d)",
                    cp - clist, cp->a, cp->b, cp->link );
        }
        printf( "\n" );
    }
    printf( "*\n" );
}
#endif

/*
 * TRUE if strings are identical
 */

INT streq( const char *s1, const char *s2 )
{
    while( *s1++ == *s2 ) {
        if( *s2++ == EOS )
	    return( TRUE );
    }
    return( FALSE );
}

/*
 * Can't open file message
 */

void cant( const char *filename, const char *what, SLONG fatalflag )
{
    fprintf( stderr, "Can't open %s file \"%s\": ", what, filename );
    perror( ( char *) NULL );
    if( fatalflag ) {
        exit( xflag + DIFF_NOT_COMPARED );
    }
}

void fatal( const char *format, ... )
{
    va_list     args;

    va_start( args, format );
    fprintf( stderr, "Internal error: " );
    vfprintf( stderr, format, args );
    va_end( args );
    putc( '\n', stderr );
    exit( xflag + DIFF_NOT_COMPARED );
}
/*
 * Error message before retiring.
 */

void error( const char *format, ... )
{
    va_list args;

    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    putc( '\n', stderr );
    fflush( stderr );
}

/*
 * Like fput() except that it puts a newline at the end of the line.
 */

void fputss( const char *s, FILE *iop )
{
    fputs( s, iop );
    putc( '\n', iop );
}

/*
 * Fgetss() is like fgets() except that the terminating newline
 * is removed.
 */

char *fgetss( char *s, SLONG n, FILE *iop )
{
    char        *cs;
    size_t      len_s;

    if( fgets( s, n, iop ) == NULL )
        return( ( char *) NULL );
    len_s = strlen( s );
    cs = s + len_s - 1;
    if( *cs == '\n' ) {
        *cs = '\0';
    }
    --cs;
    if( len_s > 1 && *cs == '\r' ) {
        *cs = '\0';
    }
    return( s );
}
