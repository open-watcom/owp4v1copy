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
* Description:  Memory management routines for linker.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __WATCOMC__
    #include <malloc.h>     /* for _expand() */
#endif
#ifdef TRMEM
    #include "trmem.h"
#endif
#include "linkstd.h"
#include "msg.h"
#include "wlnkmsg.h"
#include "library.h"
#include "virtmem.h"
#include "reloc.h"
#include "objcache.h"
#include "alloc.h"
#if defined( __QNX__ )
    #include <sys/seginfo.h>

    unsigned LastChanceSeg;
#endif
#if defined( __OS2__ )
#   define INCL_BASE
#   define INCL_ERRORS
#   define ERR _BSEXCP_ERR  //clash
#   include <os2.h>
#   undef ERR               //clash
#   include <stdint.h>
#   include <assert.h>

#if defined(__386__)

// 2014-05-21 SHL
// Define USE_OS2HMALLOC to enable _os2hmalloc logic
#define USE_OS2HMALLOC 1

#if defined( USE_OS2HMALLOC )
size_t os2_private_mem_left;
#endif

// Define OS2_MEM_HDR_MAGIC to enable inline OBJ_ANY logic
// 2014-05-21 SHL FIXME to be gone
/* The magic value for magic, magic1 and magic2. (Terry Pratchett) */
// #define OS2_MEM_HDR_MAGIC 0x19480428

#if defined(OS2_MEM_HDR_MAGIC)

    /* The OS/2 memory block header (16 bytes!) */
    typedef struct os2_mem_hdr
    {
        unsigned_32 magic;      // OS2_MEM_HDR_MAGIC
        unsigned_32 size;       // the real size of the block (w/ hdr).
        unsigned_32 magic1;     // OS2_MEM_HDR_MAGIC (for alignment really)
        unsigned_32 magic2;     //        ---  ""  ---
    } os2_mem_hdr;

    /* Checks if the specified pointer is pointing to a OS/2 memory block. */
#   define IS_OS2_MEM( p ) \
        (   ( (uintptr_t)(p) & 0xfff ) == sizeof( os2_mem_hdr ) \
         && ((os2_mem_hdr *)(p))[-1].magic  == OS2_MEM_HDR_MAGIC \
         && ((os2_mem_hdr *)(p))[-1].magic1 == OS2_MEM_HDR_MAGIC \
         && ((os2_mem_hdr *)(p))[-1].magic2 == OS2_MEM_HDR_MAGIC \
         && !( ((os2_mem_hdr *)(p))[-1].size & 0xfff ) \
         )

#endif // OS2_MEM_HDR_MAGIC

#endif // __386__
#endif // __OS2__

#ifdef _INT_DEBUG
    static  int     Chunks;
#endif

#ifdef TRMEM
#include "ideentry.h"

void    *TrHdl;

static void         PrintAllMem( void );
#endif

static bool         CacheRelease( void );

#ifdef TRMEM

void PrintLine( int *bogus, const char *buff, unsigned len )
{
    bogus = bogus;      /* to avoid a warning */
    len = len;
    WriteStdOut( (void *) buff );
}
#endif

void LnkMemInit( void )
/*********************/
{
#if defined( __QNX__ )
    /* allocate some memory we can give back to the system if it runs low */
    LastChanceSeg = qnx_segment_alloc( 65000 );
#endif
#ifdef _INT_DEBUG
    Chunks = 0;
#endif
#ifdef TRMEM
    TrHdl = _trmem_open( malloc, free, realloc, _expand,
            NULL, PrintLine,
            _TRMEM_ALLOC_SIZE_0 | _TRMEM_REALLOC_SIZE_0 | _TRMEM_REALLOC_NULL |
            _TRMEM_FREE_NULL | _TRMEM_OUT_OF_MEMORY | _TRMEM_CLOSE_CHECK_FREE );
#endif

// 2014-05-25 SHL
#if defined( USE_OS2HMALLOC )
    {
        APIRET apiret = DosQuerySysInfo( QSV_MAXPRMEM,
                                         QSV_MAXPRMEM,
                                         &os2_private_mem_left,
                                         sizeof( os2_private_mem_left ) );
        if (apiret)
            os2_private_mem_left = 0x20000000;   // 512MB
    }
#endif
}


void LnkMemFini( void )
/*********************/
{
#ifdef _INT_DEBUG
    if( Chunks != 0 ) {
        DEBUG( (DBG_ALWAYS, "%d Chunks unfreed", Chunks ) );
    }
#endif
#ifdef TRMEM
    PrintAllMem();
    _trmem_close( TrHdl );
#endif
}

#ifdef TRMEM
void *DoLAlloc( size_t size, void (*ra)( void ) )
/***********************************************/
#else
void *LAlloc( size_t size )
/*************************/
#endif
{
    void    *p;

    for( ;; ) {
#ifdef TRMEM
        p = _trmem_alloc( size, ra, TrHdl );

#elif defined( USE_OS2HMALLOC )

        /* Use all but 64MB of available low memory address space
           and then use available high memory.
           Os2_private_mem_left is a good enough estimate of
           address space remaining in the lower private arena.
           The goal is to leave a large chunk of address space available
           for the HLL_SECT_MISC section which is allocated at the end of pass1
           after most of the other memory allocations have been done
        */
        if( os2_private_mem_left > 64 * 1024 * 1024 &&
            size + 16 < os2_private_mem_left ) {
            p = malloc( size );
            if( p )
                os2_private_mem_left -= size + 16;
        }
        else {
            p = _os2hmalloc( size );
        }

#elif defined( OS2_MEM_HDR_MAGIC )
        /*
         * On OS/2 we will try use high memory when available. OS/2 has
         * two arenas for private memory, the Open Watcom heap is currently
         * only using the lower one. The lower arena is often smaller than
         * 128MB because of fragmentation caused by big applications (like
         * mozilla, openoffice, ..) in the shared arena above it. The high
         * arena on the other hand, is usually not subject to this kind of
         * fragmentation and will normally be able to provide more than
         * 400MB of contiguous virtual memory. Big links needs lots of
         * memory, so for medium/large allocations we try use the high memory.
         *
         * The allocations are identified by being at a fixed offset (16) into
         * a page and the header values (the 3 magics and the page aligned size).
         */
        static int fObjAny = -1;        /* -1 = check, 0 = no upper, 1 = have upper */
        static size_t minUpperAlloc = 0x2000;     /* 8 KBytes */
        if( fObjAny && size >= minUpperAlloc ) {
            APIRET rc;
            ULONG  ul;

            /*
             * Check if can allocate more than 512MB on the first call.
             * Will fail if upper arena does not exist.
             */
            if( fObjAny == -1) {
                rc = DosQuerySysInfo( QSV_VIRTUALADDRESSLIMIT,
                                      QSV_VIRTUALADDRESSLIMIT,
                                      &ul, sizeof( ul ) );
                fObjAny =   rc == NO_ERROR
                         && ul > 512 /*MB*/;
                if( !fObjAny )
                    continue; /* restart the loop */
            }

            /* Try allocate a block in the high arena. */
            ul = size + sizeof( os2_mem_hdr );  /* Make room for header */
            ul = ( ul + 0xfff ) & ~0xfffUL; /* Round up to 4K boundary */
            rc = DosAllocMem( &p, ul, OBJ_ANY | PAG_READ | PAG_WRITE | PAG_COMMIT );
            if(     rc == NO_ERROR
                &&  (uintptr_t)p >= 0x20000000UL /* 512MB */) {
               /* Mark allocation as coming from upper arena */
               os2_mem_hdr *hdr = (os2_mem_hdr *)p;
               hdr->magic = hdr->magic1 = hdr->magic2 = OS2_MEM_HDR_MAGIC;
               hdr->size = ul;
               p = hdr + 1;
               break;                   /* return pointer to caller */
            }
            if( rc == ERROR_INVALID_PARAMETER )
                fObjAny = FALSE;        /* Upper arena not supported */
            else if( rc == NO_ERROR )
                DosFreeMem( p );        /* Came for lower area - give back */
        }
        p = malloc( size );
        /* If lower arena allocation failed try upper arena unless time to die */
        if( p == NULL && fObjAny == TRUE && minUpperAlloc > 0x2000 ) {
            minUpperAlloc >>= 1;        /* Halve if more than 8192 */
            continue;                   /* Try again */
        }

#else /* ! OS2_MEM_HDR_MAGIC */
        p = malloc( size );
#endif
        if( p != NULL ) {
            memset( p, 0, size );
            break;
        }
        /* Try to free up some memory and try malloc */
        if( !FreeUpMemory() ) break;
    } /* for */
#ifdef _INT_DEBUG
    if( p != NULL ) ++Chunks;
#endif
    return( p );
}

#ifdef TRMEM
 void *LAlloc( size_t size )
/**************************/
{
    void        (*ra)( void );

    ra = _trmem_guess_who();

    return( DoLAlloc( size, ra ) );
}
#endif

void *ChkLAlloc( size_t size )
/****************************/
{
    void                *ptr;
#ifdef TRMEM
    void                (*ra)( void );

    ra = _trmem_guess_who();

    ptr = DoLAlloc( size, ra );
#else
    ptr = LAlloc( size );
#endif
    if( ptr == NULL ) {
        LnkMsg( FTL + MSG_NO_DYN_MEM, NULL );
    }
    return( ptr );
}


void LFree( void *p )
/**************************/
{
    if( p == NULL ) return;
#ifdef TRMEM
    _trmem_free( p, _trmem_guess_who(), TrHdl );

#elif defined( OS2_MEM_HDR_MAGIC )
    if( IS_OS2_MEM( p ) ) {
        APIRET rc = DosFreeMem( (os2_mem_hdr *)p - 1 );
        assert( rc == NO_ERROR ); (void)rc;
    } else {
        free( p );
    }

#else
    free( p );
#endif
#ifdef _INT_DEBUG
    --Chunks;
#endif
}

void *LnkExpand( void *src, size_t size )
/***************************************/
// try to expand a block of memory
{
#ifdef _ZDOS
     return ( NULL );

// 2014-05-21 SHL
#elif defined( USE_OS2HMALLOC )

        return( _expand( src, size ) );

#elif defined( OS2_MEM_HDR_MAGIC )

    if( IS_OS2_MEM( src ) ) {
        os2_mem_hdr *hdr = (os2_mem_hdr *)src - 1;

        if( hdr->size - sizeof( os2_mem_hdr ) < size )
            return( NULL );
        return( src );
    } else {
        return( _expand( src, size ) );
    }
#else
    #ifdef TRMEM
        return( _trmem_expand( src, size, _trmem_guess_who(), TrHdl ) );
    #else
        return( _expand( src, size ) );
    #endif
#endif
}

void *LnkReAlloc( void *src, size_t size )
/****************************************/
// reallocate a block of memory.
{
    void    *dest;
#ifdef TRMEM
    void        (*ra)( void );

    ra = _trmem_guess_who(); /* must be first thing */
#endif
    for( ;; ) {
#ifdef TRMEM
        dest = _trmem_realloc( src, size, ra, TrHdl );

// 2014-05-21 SHL
#elif defined( USE_OS2HMALLOC )

        dest = realloc( src, size );

#elif defined( OS2_MEM_HDR_MAGIC )
        if( IS_OS2_MEM( src ) ) {
            os2_mem_hdr *hdr = (os2_mem_hdr *)src - 1;

            /* don't bother to shrink */
            if( hdr->size - sizeof( os2_mem_hdr ) >= size ) {
                dest = src;
                break;
            }

            /* try expand the block. */
            dest = LAlloc( size );
            if( dest ) {
                memcpy( dest, src, hdr->size - sizeof( os2_mem_hdr ) );
                LFree( src );
            }
        } else {
            dest = realloc( src, size );
        }

#else
        dest = realloc( src, size );
#endif
        if( dest != NULL ) break;
        if( !CacheRelease() && !SwapOutVirt() && !SwapOutRelocs() ) {
            LnkMsg( FTL + MSG_NO_DYN_MEM, NULL );       // see note 1 below
        }
    }
    return( dest );
}
/* Notes for LnkReAlloc
 * NOTE 1: we don't want to call FreeUpMemory, since that does a permshrink
 * and this function is called from permshrink
*/

#ifdef TRMEM
int ValidateMem( void )
/*********************/
{
    return( _trmem_validate_all( TrHdl ) );
}

void PrintAllMem( void )
/**********************/
{
    if( _trmem_prt_list( TrHdl ) == 0 ) {
        _trmem_prt_usage( TrHdl );
    }
}
#endif

#ifndef NDEBUG
void DbgZapAlloc( void *tgt, size_t size )
/****************************************/
{
    memset( tgt, 0xA5, size );
}

void DbgZapFreed( void *tgt, size_t size )
/****************************************/
{
    memset( tgt, 0xBD, size );
}
#endif

static bool CacheRelease( void )
/******************************/
{
    bool   freed;

    freed = DumpObjCache();
    if( !freed ) {
        freed = DiscardDicts();      /* .. discard dictionarys */
    }
    return( freed );
}

bool FreeUpMemory( void )
/***********************/
// make sure LnkReAlloc is kept up to date with what is put in here.
{
#if defined( __QNX__ )
    if( LastChanceSeg != (unsigned)-1 ) {
        /*
            If we're low on memory, the system is low on memory. Give
            something back to the OS so it can do it's job, and don't
            ever ask it for anything more.
        */
        qnx_segment_free( LastChanceSeg );
        LastChanceSeg = -1;
        _heapenable( 0 );
    }
#endif
    return( PermShrink() || CacheRelease() || SwapOutVirt() || SwapOutRelocs() );
}

int __nmemneed( size_t amount )
/*****************************/
{
    amount = amount;
    return( FreeUpMemory() );
}

#ifdef _M_I86
int __fmemneed( size_t amount )
/*****************************/
{
    amount = amount;
    return( FreeUpMemory() );
}
#endif
