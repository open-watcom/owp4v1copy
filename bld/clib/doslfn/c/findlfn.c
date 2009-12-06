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
* Description:  LFN-enabled findfirst/next/close functions for DOS.
*
****************************************************************************/


#include "variety.h"
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include <dos.h>
#include "seterrno.h"


/* The find block for the LFN findfirst */
struct lfnfind_t {
    long    attributes;
    long    creattime;
    long    creatdate;
    long    accesstime;
    long    accessdate;
    long    wrtime;
    long    wrdate;
    long    hfilesize;
    long    lfilesize;
    char    reserved[8];
    char    lfn[260];
    char    sfn[14];
};

/* The normal findfirst/next functions written in assembler */
_WCRTLINK extern unsigned _old_dos_findfirst( const char *path,
                            unsigned attr, struct find_t *buf );
_WCRTLINK extern unsigned _old_dos_findnext( struct find_t *buf );


static void convert_to_find_t( struct find_t *dosblock,
                               struct lfnfind_t *lfnblock )
{
    dosblock->attrib  = lfnblock->attributes;
    dosblock->cr_time = lfnblock->creattime;
    dosblock->cr_date = lfnblock->creatdate;
    dosblock->ac_time = lfnblock->accesstime;
    dosblock->ac_date = lfnblock->accessdate;
    dosblock->wr_time = lfnblock->wrtime;
    dosblock->wr_date = lfnblock->wrdate;
    dosblock->size    = lfnblock->lfilesize;
    if( lfnblock->lfn ) {
        memcpy( dosblock->name, lfnblock->lfn, sizeof( lfnblock->lfn ) );
    } else {
        memcpy( dosblock->name, lfnblock->sfn, sizeof( lfnblock->sfn ) );
    }
}

_WCRTLINK unsigned _dos_findfirst( const char *path, unsigned attr,
    struct find_t *buf )
{
    union  REGS         r;
    struct SREGS        s;
    struct lfnfind_t    lfnblock;

    buf->lfnax = buf->lfnsup = 0; /* Zero find handle and LFN-supported flag */
    r.w.ax = 0x714E;              /* LFN Findfirst */
    r.w.cx = attr;
    r.w.si = 1;                   /* Use DOS date/time format */
    s.ds   = FP_SEG( path );      /* path goes in DS:DX */
    r.w.dx = FP_OFF( path );
    s.es   = FP_SEG( &lfnblock ); /* LFN find block goes in ES:DI */
    r.w.di = FP_OFF( &lfnblock );

    intdosx( &r, &r, &s );        /* Let's call the LFN version */

    /*
     * If ax = 7100, there is probably an LFN TSR but no LFN support for
     * whatever drive or directory is being searched. In that case, fall back on
     * the old findfirst.  Also if the function fails, it could be because of
     * no LFN TSR so fall back to the old findfirst.
     */
    if( r.w.ax == 0x7100 || r.w.cflag )
        return( _old_dos_findfirst( path, attr, buf ) );

    /*
     * If there was no failure, the next step is to move the values from the
     * LFN block into the non-lfn block
     */
    buf->lfnax = r.w.ax;              /* The find handle for findnext */
    buf->lfnsup = _LFN_SIGN;
    convert_to_find_t( buf, &lfnblock );
    /*
     * 0 is always returned because we've already checked cflag; the
     * old_dos_findfirst will handle any errors
     */
    return( 0 );
}

_WCRTLINK unsigned _dos_findnext( struct find_t *buf )
{
    union  REGS         r;
    struct SREGS        s;
    struct lfnfind_t    lfnblock;

    /*
     * Before going through the possibly unnecessary steps of calling the LFN
     * function first, buf->lfnsup will tell us if LFN was supported with the
     * previous call to findfirst.
     */
    if( buf->lfnsup != _LFN_SIGN ) {
        return( _old_dos_findnext( buf ) );
    }

    r.w.ax = 0x714F;
    r.w.bx = buf->lfnax;                /* The lfn handle set by findfirst */
    r.w.si = 1;                         /* Use DOS times */
    s.es   = FP_SEG( &lfnblock );
    r.w.di = FP_OFF( &lfnblock );       /* The LFN find block */

    intdosx( &r, &r, &s );              /* Call the function */

    /* Check for errors */
    if( r.w.cflag ) {
        __set_errno_dos( r.w.ax );
        return( r.w.ax );
    }
    convert_to_find_t( buf, &lfnblock );
    return( 0 );
}

_WCRTLINK unsigned _dos_findclose( struct find_t *buf )
{
    union REGS  r;

    /* Let's check if LFN was used; if not, there is no need for findclose */
    if( buf->lfnsup != _LFN_SIGN )
        return( 0 );

    r.w.ax = 0x71A1;            /* LFN findclose */
    r.w.bx = buf->lfnax;        /* Findfirst handle */
    intdos( &r, &r );

    /* Check for errors (which really shouldn't be a problem anyways
     * except for bad code)
     */
    if( r.w.cflag ) {
        __set_errno_dos( r.w.ax );
        return( r.w.ax );
    }
    return( 0 );
}
