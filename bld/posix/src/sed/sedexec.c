/*
sedexec.c -- execute compiled form of stream editor commands

   The single entry point of this module is the function execute(). It
may take a string argument (the name of a file to be used as text)  or
the argument NULL which tells it to filter standard input. It executes
the compiled commands in cmds[] on each line in turn.
   The function command() does most of the work. Match() and advance()
are used for matching text against precompiled regular expressions and
dosub() does right-hand-side substitution.  Getline() does text input;
readout() and memeql() are output and string-comparison utilities.

==== Written for the GNU operating system by Eric S. Raymond ====

18NOV86 Fixed bug in 'selected()' that prevented address ranges from
    working.                - Billy G. Allie.
21FEB88 Refixed bug in 'selected()'     - Charles Marslett
*/

#include <assert.h>
#include <stdio.h>                      /* {f}puts, {f}printf, etc. */
#include <ctype.h>                      /* isprint(), isdigit(), toascii() */
#include <stdlib.h>                     /* for exit() */
#include "sed.h"                        /* command structures & constants */

#define MAXHOLD         MAXBUF          /* size of the hold space */
#define GENSIZ          MAXBUF          /* maximum genbuf size */

#define TRUE            1
#define FALSE           0

static char const       LTLMSG[] = "sed: line too long \"%.*s\"\n";
static char const       NOROOM[] = "sed: can only fit %d bytes at line %ld\n";
static char const       INTERR[] = "sed: internal error: %s\n";

#define ABORTEX(msg) fprintf( stderr, msg, sizeof genbuf, genbuf ), exit( 2 )

static char     *spend;                 /* current end-of-line-buffer pointer */
static long     lnum = 0L;              /* current source line number */

                                        /* append buffer maintenance */
static sedcmd   *appends[MAXAPPENDS];   /* array of ptrs to a,i,c commands */
static sedcmd   **aptr = appends;       /* ptr to current append */

                                        /* genbuf and its pointers */
static char     genbuf[GENSIZ];
static char     *loc1;
static char     *loc2;
static char     *locs;

                                        /* command-logic flags */
static int      lastline;               /* do-line flag */
static int      jump;                   /* jump to cmd's link address if set */
static int      delete;                 /* delete command flag */

                                        /* tagged-pattern tracking */
static char     *bracend[MAXTAGS];      /* tagged pattern start pointers */
static char     *brastart[MAXTAGS];     /* tagged pattern end pointers */

static int      selected( sedcmd *ipc );
static int      match( char *expbuf, int gf );
static int      advance( register char *lp, register char *ep );
static int      substitute( sedcmd const *ipc );
static void     dosub( char const *rhsbuf );
static char     *place( register char *asp, register char const *al1,
    register char const *al2 );
static void     listto( register char const *p1, FILE *fp );
static void     command( sedcmd *ipc );
static char     *getline( register char *buf );
static int      memeql( register char const *a, register char const *b, int count );
static void     readout( void );

/* execute the compiled commands in cmds[] on a file */
void execute( const char *file )        /* name of text source file to filter */
{
    register char const *p1;            /* dummy copy ptrs */
    register sedcmd     *ipc;           /* ptr to current command */
    char                *execp;         /* ptr to source */

    if( file != NULL )                  /* filter text from a named file */
        if( freopen( file, "r", stdin ) == NULL )
            fprintf( stderr, "sed: can't open %s\n", file );

    if( pending ) {                     /* there's a command waiting */
        ipc = pending;                  /* it will be first executed */
        pending = FALSE;                /*   turn off the waiting flag */
        goto doit;                      /*   go to execute it immediately */
    }
                                        /* the main command-execution loop */
    for( ;; ) {
                                        /* get next line to filter */
                                        /* jump is set but not cleared by D */
        if( ( execp = getline( jump ? spend : linebuf ) ) == BAD ) {
            if( jump ) {
                for( p1 = linebuf; p1 < spend; p1++ )
                    putc( *p1, stdout );
                putc( '\n', stdout );
            }
            return;
        }
        jump = FALSE;
        spend = execp;
                                        /* compiled commands execute loop */
        for( ipc = cmds; ipc->command; ) {
            if( !selected( ipc ) ) {
                ipc++;
                continue;
            }
        doit:
            command( ipc );             /* execute the command pointed at */

            if( delete )                /* if delete flag is set */
                break;                  /* don't exec rest of compiled cmds */

            if( jump ) {                /* if jump set, follow cmd's link */
                jump = FALSE;
                if( ( ipc = ipc->u.link ) == 0 ) {
                    ipc = cmds;
                    break;
                }
            }
            else                        /* normal goto next command */
                ipc++;
        }
                                        /* all commands now done on the line */
                                        /* output the transformed line is */
        if( !nflag && !delete ) {
            for( p1 = linebuf; p1 < spend; p1++ )
                putc( *p1, stdout );
            putc( '\n', stdout );
        }
                                        /* if appendn set, emit that text */
        if( aptr > appends )
            readout();

        delete = FALSE;                 /* clear delete flag; get next cmd */
    }
}

/* is current command selected */
static int selected( sedcmd *ipc )
{
    register char               *p1 = ipc->addr1;       /* first address */
    register char * const       p2 = ipc->addr2;        /*   and second */
    char                        c;
    int const                   allbut = ipc->flags.allbut;

    if( !p1 )
        return( !allbut );

    if( ipc->flags.inrange ) {
        if( *p2 == CEND )
            p1 = NULL;
        else if( *p2 == CLNUM ) {
            c = p2[1];
            if( lnum > linenum[c] ) {
                ipc->flags.inrange = FALSE;
                if( ipc->flags.allbut )
                    return( TRUE );
                return( FALSE );
            }
            if( lnum == linenum[c] )
                ipc->flags.inrange = FALSE;
        } else if( match( p2, 0 ) )
            ipc->flags.inrange = FALSE;
    } else if( *p1 == CEND ) {
        if( !lastline ) {
            if( ipc->flags.allbut )
                return( TRUE );
            return( FALSE );
        }
    }
    else if( *p1 == CLNUM ) {
        c = p1[1];
        if( lnum != linenum[c] ) {
            if( ipc->flags.allbut )
                return( TRUE );
            return( FALSE );
        }
        if( p2 )
            ipc->flags.inrange = TRUE;
    } else if( match( p1, 0 ) ) {
        if( p2 )
            ipc->flags.inrange = TRUE;
    } else {
        if( ipc->flags.allbut )
            return( TRUE );
        return( FALSE );
    }
    if( ipc->flags.allbut )             /* If we reach this point, the */
        return( FALSE );                /* range test is satisfied, so */
    return( TRUE );                     /* return TRUE unless "!"ed    */
}

/* match RE at expbuf against linebuf; if gf set, copy linebuf from genbuf */
static int match( char *expbuf, int gf )
{
    register char       *p1;
    register char       *p2;
    register char       c;

    if( gf ) {
        if( *expbuf )
            return( FALSE );
        p1 = linebuf; 
        p2 = genbuf;
        while( ( *p1++ = *p2++ ) != 0 ) ;
        locs = p1 = loc2;
    } else {
        p1 = linebuf;
        locs = FALSE;
    }

    p2 = expbuf;
    if( *p2++ ) {
        loc1 = p1;
        if( *p2 == CCHR && p2[1] != *p1 ) /* 1st char is wrong */
            return( FALSE );            /*   so fail */
        return( advance( p1, p2 ) );    /* else try to match rest */
    }
                                        /* literal 1st character quick check */
    if( *p2 == CCHR ) {
        c = p2[1];                      /* pull out character to search for */
        do {
            if( *p1 == c )              /* scan the source string */
                if( advance( p1, p2 ) ) /* found it, match the rest */
                    return( loc1 = p1, 1 );
        } while( *p1++ );
        return( FALSE );                /* didn't find that first char */
    }
                                        /* else try unanchored pattern match */
    do {
        if( advance( p1, p2 ) )
            return( loc1 = p1, 1 );
    } while( *p1++ );
                                        /* didn't match either way */
    return( FALSE );
}

/* attempt to advance match pointer by one pattern element */
static int advance(
    register char       *lp,            /* source (linebuf) ptr */
    register char       *ep )           /* regular expression element ptr */
{
    register char const *curlp;         /* save ptr for closures */
    char                c;              /* scratch character holder */
    char const          *bbeg;
    int                 ct;

    for( ;; )
        switch( *ep++ ) {
        case CCHR:                      /* literal character */
            if( *ep++ != *lp++ )        /* if chars unequal */
                return( FALSE );        /* return false */
            break;                      /* matched */

        case CDOT:                      /* anything but newline */
            if( *lp == 0                /* first NUL is at EOL */
            ||  *lp++ == '\n' )
                return( FALSE );        /* return false */
            break;                      /* matched */

        case CNL:                       /* start-of-line */
        case CDOL:                      /* end-of-line */
            if( *lp != 0 )              /* found that first NUL? */
                return( FALSE );        /* return false */
            break;                      /* matched */

        case CEOF:                      /* end-of-address mark */
            loc2 = lp;                  /* set second loc */
            return( TRUE );             /* return true */

        case CCL:                       /* a closure */
            c = *lp++ &0177;
            if( !( ep[c >> 3] & bits[c & 07] ) ) /* is char in set? */
                return( FALSE );        /* return false */
            ep += 16;                   /* skip rest of bitmask */
            break;                      /*   and keep going */

        case CBRA:                      /* start of tagged pattern */
            brastart[*ep++] = lp;       /* mark it */
            break;                      /* and go */

        case CKET:                      /* end of tagged pattern */
            bracend[*ep++] = lp;        /* mark it */
            break;                      /* and go */

        case CBACK:
            bbeg = brastart[*ep];
            ct = bracend[*ep++] - bbeg;

            if( !memeql( bbeg, lp, ct ) )
                return( FALSE );        /* return false */
            lp += ct;
            break;                      /* matched */

        case CBACK | STAR:
            bbeg = brastart[*ep];
            ct = bracend[*ep++] - bbeg;
            curlp = lp;
            while( memeql( bbeg, lp, ct ) )
                lp += ct;

            while( lp >= curlp ) {
                if( advance( lp, ep ) )
                    return( TRUE );
                lp -= ct;
            }
            return( FALSE );

        case CDOT | STAR:               /* match .* */
            curlp = lp;                 /* save closure start loc */
            while( ( *lp++ ) != 0 ) ;   /* match anything */
            goto star;                  /* now look for followers */

        case CCHR | STAR:               /* match <literal char>* */
            curlp = lp;                 /* save closure start loc */
            while( (int)( *lp++ == *ep ) != 0 ) ; /* match many of that char */
            ep++;                       /* to start of next element */
            goto star;                  /* match it and followers */

        case CCL | STAR:                /* match [...]* */
            curlp = lp;                 /* save closure start loc */
            do {
                c = *lp++ & 0x7F;       /* match any in set */
            } while( ep[c >> 3] & bits[c & 07] );
            ep += 16;                   /* skip past the set */
            goto star;                  /* match followers */

        star:                           /* the repeat part of a * or + match */
            if( --lp == curlp )         /* 0 matches */
                break;

            if( *ep == CCHR ) {
                c = ep[1];
                do {
                    if( *lp == c )
                        if( advance( lp, ep ) )
                            return( TRUE );
                } while( lp-- > curlp );
                return( FALSE );
            }

            if( *ep == CBACK ) {
                c = *( brastart[ep[1]] );
                do {
                    if( *lp == c )
                        if( advance( lp, ep ) )
                            return( TRUE );
                } while( lp-- > curlp );
                return( FALSE );
            }

            do {
                if( lp == locs )
                    break;
                if( advance( lp, ep ) )
                    return( TRUE );
            } while( lp-- > curlp );
            return( FALSE );

        default:
            fprintf( stderr, "sed: RE error, %o\n", *--ep );
        }
}

/* perform s command */
static int substitute( sedcmd const *ipc ) /* ptr to s command struct */
{
    if( !match( ipc->u.lhs, 0 ) )       /* if no match */
        return( FALSE );                /* command fails */
    dosub( ipc->rhs );                  /* perform it once */

    if( ipc->flags.global )             /* if global flag enabled */
        while( *loc2 && match( ipc->u.lhs, 1 ) ) /* cycle through possibles */
            dosub( ipc->rhs );          /* so substitute */
    return( TRUE );                     /* we succeeded */
}

/* generate substituted right-hand side (of s command) */
static void dosub( char const *rhsbuf ) /* where to put the result */
                                        /* uses linebuf, genbuf, spend */
{
    register char       *lp;
    register char       *sp;
    register char const *rp;
    int                 c;
                                        /* linebuf upto location 1 -> genbuf */
    lp = linebuf; 
    sp = genbuf;
    while( lp < loc1 ) {
        if( sp >= genbuf + sizeof genbuf )
            ABORTEX( LTLMSG );         /* Not exercised by sedtest.mak */
        *sp++ = *lp++;
    }

    for( rp = rhsbuf; ( c = *rp++ ) != 0; ) {
        if( c == '&' ) {
            sp = place( sp, loc1, loc2 );
        } else if( c & 0200 && ( c &= 0177 ) >= '1' && c < MAXTAGS + '1' ) {
            sp = place( sp, brastart[c - '1'], bracend[c - '1'] );
        } else {
            if( sp >= genbuf + sizeof genbuf )
                ABORTEX( LTLMSG );      /* Not exercised by sedtest.mak */
            *sp++ = c & 0177;
        }
    }
    lp = loc2;
    loc2 = sp - ( genbuf - linebuf );
    do{
        if( sp >= genbuf + sizeof genbuf )
            ABORTEX( LTLMSG );          /* Not exercised by sedtest.mak */
    } while( ( *sp++ = *lp++ ) != 0 );
    lp = linebuf; 
    sp = genbuf;
    while( ( *lp++ = *sp++ ) != 0 ) ;
    spend = lp - 1;
}

/* place chars at *al1...*(al1 - 1) at asp... in genbuf[] */
static char *place(
    register char       *asp,
    register char const *al1,
    register char const *al2 )
{
    while( al1 < al2 ) {
        if( asp >= genbuf + sizeof genbuf )
            ABORTEX( LTLMSG );          /* Not exercised by sedtest.mak */
        *asp++ = *al1++;
    }
    return( asp );
}

/* write a hex dump expansion of *p1... to fp */
static void listto(
    register char const *p1,            /* the source */
    FILE                *fp )           /* output stream to write to */
{
    p1--;
    while( *++p1 )
        if( isprint( *p1 ) )
            putc( *p1, fp );            /* pass it through */
        else {
            putc( '\134', fp );         /* emit a backslash */
            switch( *p1 ) {
            case '\10':     
                putc( 'b', fp ); 
                break;                  /* BS */
            case '\11':     
                putc( 't', fp ); 
                break;                  /* TAB */
                                        /* \11 was \9 --MRY */
            case '\12':     
                putc( 'n', fp ); 
                break;                  /* NL */
            case '\15':     
                putc( 'r', fp ); 
                break;                  /* CR */
            case '\33':     
                putc( 'e', fp ); 
                break;                  /* ESC */
            default:        
                fprintf( fp, "%02x", *p1 & 0xFF );
            }
        }
    putc( '\n', fp );
}

/* execute compiled command pointed at by ipc */
static void command( sedcmd *ipc )
{
    static int      didsub;             /* true if last s succeeded */
    static char     holdsp[MAXHOLD];    /* the hold space */
    static char     *hspend = holdsp;   /* hold space end pointer */
    register char   *p1;
    register char   *p2;
    char            *execp;

    switch( ipc->command ) {
    case ACMD:                          /* append */
        *aptr++ = ipc;
        if( aptr >= appends + MAXAPPENDS ) /* Not exercised by sedtest.mak */
            fprintf( stderr, "sed: too many appends after line %ld\n", lnum );
        *aptr = 0;
        break;

    case CCMD:                          /* change pattern space */
        delete = TRUE;
        if( !ipc->flags.inrange || lastline )
            printf( "%s\n", ipc->u.lhs );
        break;

    case DCMD:                          /* delete pattern space */
        delete++;
        break;

    case CDCMD:                         /* delete a line in pattern space */
        p1 = p2 = linebuf;
        while( *p1 && *p1 != '\n' ) p1++;
        if( !*p1++ )
            return;
        while( ( *p2++ = *p1++ ) != 0 ) ;
        spend = p2 - 1;
        delete = jump = TRUE;
        break;

    case EQCMD:                         /* show current line number */
        fprintf( stdout, "%ld\n", lnum );
        break;

    case GCMD:                          /* copy hold space to pattern space */
        p1 = linebuf;   
        p2 = holdsp;    
        while( ( *p1++ = *p2++ ) != 0 ) ;
        spend = p1 - 1;
        break;

    case CGCMD:                         /* append hold space to pattern space */
        *spend++ = '\n';
        p1 = spend;     
        p2 = holdsp;
        while( ( *p1++ = *p2++ ) != 0 )
            if( p1 >= linebuf + MAXBUF )
                break;
        spend = p1 - 1;
        break;

    case HCMD:                          /* copy pattern space to hold space */
        p1 = holdsp;    
        p2 = linebuf;   
        while( ( *p1++ = *p2++ ) != 0 ) ;
        hspend = p1 - 1;
        break;

    case CHCMD:                         /* append pattern space to hold space */
        *hspend++ = '\n';
        p1 = hspend;    
        p2 = linebuf;
        while( ( *p1++ = *p2++ ) != 0 )
            if( p1 >= holdsp + MAXBUF )
                break;
        hspend = p1 - 1;
        break;

    case ICMD:                          /* insert text */
        printf( "%s\n", ipc->u.lhs );
        break;

    case BCMD:                          /* branch to label */
        jump = TRUE;
        break;

    case LCMD:                          /* list text */
        listto( linebuf, ( ipc->fout != NULL ) ? ipc->fout : stdout ); 
        break;

    case NCMD:                          /* read next line into pattern space */
        if( !nflag )
            puts( linebuf );            /* flush out the current line */
        if( aptr > appends )
            readout();                  /* do pending a, r commands */
        if( ( execp = getline( linebuf ) ) == BAD ) {
            pending = ipc;
            delete = TRUE;
            break;
        }
        spend = execp;
        break;

    case CNCMD:                         /* append next line to pattern space */
        if( aptr > appends )
            readout();
        *spend++ = '\n';
        if( ( execp = getline( spend ) ) == BAD ) {
            *--spend = '\0';            /* Remove '\n' added for new line */
            pending = ipc;
            delete = TRUE;
            break;
        }
        spend = execp;
        break;

    case PCMD:                          /* print pattern space */
        puts( linebuf );
        break;

    case CPCMD:                         /* print one line from pattern space */
        for( p1 = linebuf; *p1 != '\n' && *p1 != '\0'; p1++ )
            putc( *p1, stdout );
        putc( '\n', stdout );
        break;

    case QCMD:                          /* quit the stream editor */
        if( !nflag )
            puts( linebuf );            /* flush out the current line */
        if( aptr > appends )
            readout();                  /* do any pending a and r commands */
        exit( 0 );

    case RCMD:                          /* read a file into the stream */
        *aptr++ = ipc;
        if( aptr >= appends + MAXAPPENDS ) /* Not exercised by sedtest.mak */
            fprintf( stderr, "sed: too many reads after line %ld\n", lnum );
        *aptr = 0;
        break;

    case SCMD:                          /* substitute RE */
        if( ( didsub = substitute( ipc ) ) != 0 ) {
            switch( ipc->flags.print ) {
            case 1:
                puts( linebuf );
                break;
            case 2:
                for( p1 = linebuf; *p1 != '\n' && *p1 != '\0'; p1++ )
                    putc( *p1, stdout );
                putc( '\n', stdout );
                break;
            }
            if( ipc->fout )
                fprintf( ipc->fout, "%s\n", linebuf );
        }
        break;

    case TCMD:                          /* branch on last s successful */
    case CTCMD:                         /* branch on last s failed */
        if( didsub == (int)( ipc->command == CTCMD ) )
            break;                      /* no branch if last s failed, else */
        didsub = FALSE;
        jump = TRUE;                    /*  set up to jump to assoc'd label */
        break;

    case CWCMD:                         /* write one line from pattern space */
        for( p1 = linebuf; *p1 != '\n' && *p1 != '\0'; )
            putc( *p1, ipc->fout ), p1++;
        putc( '\n', ipc->fout );
        break;

    case WCMD:                          /* write pattern space to file */
        fprintf( ipc->fout, "%s\n", linebuf );
        break;

    case XCMD:                          /* exchange pattern and hold spaces */
        p1 = linebuf;   
        p2 = genbuf;    
        while( ( *p2++ = *p1++ ) != 0 ) ;
        p1 = holdsp;    
        p2 = linebuf;   
        while( ( *p2++ = *p1++ ) != 0 ) ;
        spend = p2 - 1;
        p1 = genbuf;    
        p2 = holdsp;    
        while( ( *p2++ = *p1++ ) != 0 ) ;
        hspend = p2 - 1;
        break;

    case YCMD:
        p1 = linebuf;   
        p2 = ipc->u.lhs;
        while( ( *p1 = p2[*p1] ) != 0 )
            p1++;
        break;

    default: /* Can never happen */
        fprintf( stderr, INTERR, "unrecognised command" );
    }
}

/* get next line of text to be filtered */
static char *getline( register char *buf )  /* where to send the input */
{
    static char const * const   linebufend = linebuf + MAXBUF + 2;
    int const                   room = linebufend - buf;
    int                         temp;

    assert( buf >= linebuf && buf < linebufend );

    if (fgets(buf, room, stdin) != NULL) { /* gets() can smash program - WFB */
        lnum++;                         /* note that we got another line */
        while( ( *buf++ ) != 0 ) ;      /* find the end of the input */
        if( --buf, *--buf != '\n' )
            fprintf( stderr, NOROOM, room, lnum ), buf++;
        else
            *buf = 0;
        if( eargc == 0 ) {              /* if no more args */
            lastline = ( ( temp = getc( stdin ) ) == EOF );
            (void)ungetc( temp, stdin );
        }
        return( buf );                  /* return ptr to terminating null */
    } else {
        if( eargc == 0 )                /* if no more args */
            lastline = TRUE;            /*    set a flag */
        return( BAD );
    }
}

static int memeql( register char const *a, register char const *b, int count )
/* return TRUE if *a... == *b... for count chars, FALSE otherwise */
{
    while( count-- )                    /* look at count characters */
        if( *a++ != *b++ )              /* if any are nonequal   */
            return( FALSE );            /*    return FALSE for false */
    return( TRUE );                     /* compare succeeded */
}

/* write file indicated by r command to output */
static void readout( void )
{
    register int        t;              /* hold input char or EOF */
    FILE                *fi;            /* ptr to file to be read */
    sedcmd * const      *ap;            /* Loops through appends */

    for( ap = appends; *ap; ap ++ )
        if( ( *ap )->command == ACMD ) /* process "a" cmd */
            printf( "%s\n", ( *ap )->u.lhs );
        else {                          /* process "r" cmd */
            if( ( fi = fopen( ( *ap )->u.lhs, "r" ) ) != NULL ) {
                while( ( t = getc( fi ) ) != EOF )
                    putc( ( char ) t, stdout );
                fclose( fi );
            }
        }

    aptr = appends;                     /* reset the append ptr */
    *aptr = 0;
}

/* sedexec.c ends here */
