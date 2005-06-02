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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "watcom.h"
#include "exepe.h"
#include "wresall.h"
#include "wrmergdi.h"
#include "rcmem.h"
#include "pass2.h"
#include "rcstr.h"
#include "exeutil.h"
#include "exeobj.h"
#include "global.h"
#include "errors.h"
#include "exereslx.h"
#include "iortns.h"

#if 0
/* structures and routines to manipulate a queue of PEResDirEntry * */
/* This uses a linked list representation despite the overhead of the pointer */
/* since the total number of entries in the queue should be small and this is */
/* easier to code while still being dynamic */

typedef struct QueueNode {
    struct QueueNode    *next;
    LXResEntry          *entry;
} QueueNode;

typedef struct DirEntryQueue {
    QueueNode           *front;
    QueueNode           *back;
} DirEntryQueue;

extern int RcPadFile( int, long );


static void QueueInit( DirEntryQueue *queue )
/*******************************************/
{
    queue->front = NULL;
    queue->back = NULL;
} /* QueueInit */


static void QueueEmpty( DirEntryQueue *queue )
/********************************************/
{
    QueueNode   *curr;
    QueueNode   *old;

    curr = queue->front;
    while( curr != NULL ) {
        old = curr;
        curr = curr->next;
        RcMemFree( old );
    }

    QueueInit( queue );
} /* QueueEmpty */


static int QueueIsEmpty( DirEntryQueue *queue )
/*********************************************/
{
    return( queue->front == NULL );
}


static void QueueAdd( DirEntryQueue *queue, LXResEntry *entry )
/*************************************************************/
{
    QueueNode   *new;

    new = RcMemMalloc( sizeof(QueueNode) );
    new->entry = entry;
    new->next = NULL;
    if( queue->front == NULL ) {
        queue->front = new;
        queue->back = new;
    } else {
        queue->back->next = new;
        queue->back = new;
    }
} /* QueueAdd */


static LXResEntry *QueueRemove( DirEntryQueue *queue )
/****************************************************/
{
    QueueNode       *old;
    LXResEntry      *entry;

    old = queue->front;
    if( old == NULL ) {
        return( NULL );
    }

    queue->front = old->next;
    if( queue->front == NULL ) {
        queue->back = NULL;
    }

    entry = old->entry;
    RcMemFree( old );
    return( entry );
} /* QueueRemove */


/*
 * traverseTree
 * NB when a visit function returns an error this function MUST return
 *    without altering errno
 */
static RcStatus traverseTree( PEResDir * dir, void * visit_data,
                    RcStatus (*visit)( PEResEntry *, void * visit_data ) )
/*******************************************************************/
/* Perfroms a level order traversal of a PEResDir tree calling visit at */
/* each entry */
{
    PEResEntry *    curr_entry;
    PEResEntry *    last_child;
    PEResDirEntry * curr_dir;
    DirEntryQueue   queue;
    RcStatus        ret;

    QueueInit( &queue );

    QueueAdd( &queue, &dir->Root );

    while( !QueueIsEmpty( &queue ) ) {
        curr_dir = QueueRemove( &queue );
        last_child = curr_dir->Children + curr_dir->Head.num_name_entries +
                        curr_dir->Head.num_id_entries;
        curr_entry = curr_dir->Children;
        while( curr_entry < last_child ) {
            ret = visit( curr_entry, visit_data );
            if( ret != RS_OK ) return( ret );
            if( curr_entry->IsDirEntry ) {
                QueueAdd( &queue, &curr_entry->u.Dir );
            }
            curr_entry++;
        }
    }

    QueueEmpty( &queue );

    return( RS_OK );
} /* traverseTree */

static RcStatus SetEntryOffset( PEResEntry * entry, void * _curr_offset )
/***********************************************************************/
{
    int     num_entries;
    uint_32 *curr_offset = _curr_offset;

    if( entry->IsDirEntry ) {
        entry->Entry.entry_rva = *curr_offset | PE_RESOURCE_MASK_ON;
        num_entries = entry->u.Dir.Head.num_name_entries +
                            entry->u.Dir.Head.num_id_entries;
        *curr_offset += sizeof(resource_dir_header) +
                            num_entries * sizeof(resource_dir_entry);
    } else {
        entry->Entry.entry_rva = *curr_offset;
        *curr_offset += sizeof(resource_entry);
    }
    return( RS_OK );
} /* SetEntryOffset */

static RcStatus AdjustNameEntry( PEResEntry * entry, void * _dir_size )
/*********************************************************************/
{
    uint_32     str_offset;
    uint_32    *dir_size = _dir_size;

    if( entry->Entry.id_name & PE_RESOURCE_MASK_ON ) {
        /* the id_name contains the offset into the string block */
        str_offset = entry->Entry.id_name & PE_RESOURCE_MASK;
        str_offset += *dir_size;
        entry->Entry.id_name = str_offset | PE_RESOURCE_MASK_ON;
    }

    return( RS_OK );
} /* AdjustNameEntry */

static RcStatus SortDirEntry( PEResEntry * entry, void * dummy )
/*********************************************************/
{
    int     num_entries;

    dummy = dummy;

    if( entry->IsDirEntry ) {
        num_entries = entry->u.Dir.Head.num_name_entries +
                    entry->u.Dir.Head.num_id_entries;
        qsort( entry->u.Dir.Children, num_entries, sizeof(PEResEntry),
                    ComparePEResIdName );
    }
    return( RS_OK );
} /* SortDirEntry */

static void CompleteTree( PEResDir * dir )
/****************************************/
{
    uint_32     curr_offset;
    int         num_entries;

    num_entries = dir->Root.Head.num_name_entries +
                                dir->Root.Head.num_id_entries;

    /* sort the entries at each level */
    qsort( dir->Root.Children, num_entries, sizeof(PEResEntry),
                ComparePEResIdName );
    traverseTree( dir, NULL, SortDirEntry );

    /* Set curr_offset to the size of the root entry */
    curr_offset = sizeof(resource_dir_header) +
                        num_entries * sizeof(resource_dir_entry);

    traverseTree( dir, &curr_offset, SetEntryOffset );
    dir->DirSize = curr_offset;


    traverseTree( dir, &curr_offset, AdjustNameEntry );
} /* CompleteTree */

typedef struct CopyResInfo {
    int                 to_handle;
    ExeFileInfo         *file;  // for setting debugging offset
    ResFileInfo         *curres;
    ResFileInfo         *errres;
} CopyResInfo;

/*
 * copyDataEntry
 * NB when an error occurs this function MUST return without altering errno
 */
static RcStatus copyDataEntry( PEResEntry *entry, void *_copy_info )
/******************************************************************/
{
    CopyResInfo         *copy_info = _copy_info;
    WResLangInfo        *res_info;
    long                seek_rc;
    uint_32             diff;
    RcStatus            status;
    ResFileInfo         *info;
    int                 closefile;

    closefile = FALSE;
    if( !entry->IsDirEntry ) {
        info = WResGetFileInfo( entry->u.Data.Wind );
        if( copy_info->curres == NULL || copy_info->curres == info ) {
            res_info = WResGetLangInfo( entry->u.Data.Wind );
            seek_rc = RcSeek( info->Handle, res_info->Offset, SEEK_SET );
            if( seek_rc == -1 ) return( RS_READ_ERROR );
            status = CopyExeData( info->Handle, copy_info->to_handle,
                                  res_info->Length );
            if( status != RS_OK ) {
                copy_info->errres = info;
                return( status );
            }
            diff = ALIGN_VALUE( res_info->Length, sizeof(uint_32) );
            if( diff != res_info->Length ) {
                /* add the padding */
//              seek_rc = RcSeek( copy_info->to_handle, diff - res_info->Length,
//                                  SEEK_CUR );
                RcPadFile( copy_info->to_handle, diff - res_info->Length );
                if( seek_rc == -1 ) return( RS_WRITE_ERROR );
            }
            CheckDebugOffset( copy_info->file );
        }
    }

    return( RS_OK );
} /* copyDataEntry */

/*
 * copyPEResources
 * NB when an error occurs this function MUST return without altering errno
 */
static RcStatus copyPEResources( ExeFileInfo * tmp, ResFileInfo *resfiles,
                                int to_handle, int writebyfile,
                                ResFileInfo **errres )
/****************************************************************/
{
    CopyResInfo     copy_info;
    PEExeInfo *     info;
    pe_va           start_rva;
    uint_32         start_off;
    long            seek_rc;
    RcStatus        ret;
    int             tmpopened;

    info = &tmp->u.PEInfo;

    start_rva = info->Res.ResRVA + info->Res.DirSize +
                    info->Res.String.StringBlockSize;
    start_off = info->Res.ResOffset + info->Res.DirSize +
                    info->Res.String.StringBlockSize;

    copy_info.to_handle = to_handle;
    copy_info.errres = NULL;
    copy_info.file = tmp;       /* for tracking debugging info offset */
    start_off = ALIGN_VALUE( start_off, sizeof(uint_32) );

    seek_rc = RcSeek( to_handle, start_off, SEEK_SET );
    if( seek_rc == -1 ) return( RS_WRITE_ERROR );
    if( !writebyfile ) {
        copy_info.curres = NULL;
        ret = traverseTree( &info->Res, &copy_info, copyDataEntry );
        *errres = copy_info.errres;
    } else {
        while( resfiles != NULL ) {
            copy_info.curres = resfiles;
            if( resfiles->IsOpen ) {
                tmpopened = FALSE;
            } else {
                resfiles->Handle = ResOpenFileRO( resfiles->name );
                if( resfiles->Handle == -1 ) {
                    ret = RS_OPEN_ERROR;
                    *errres = resfiles;
                    break;
                }
                resfiles->IsOpen = TRUE;
                tmpopened = TRUE;
            }
            ret = traverseTree( &info->Res, &copy_info, copyDataEntry );
            if( tmpopened ) {
                ResCloseFile( resfiles->Handle );
                resfiles->Handle = -1;
                resfiles->IsOpen = FALSE;
            }
            if( ret != RS_OK ) {
                *errres = resfiles;
                break;
            }
            resfiles = resfiles->next;
        }
    }
    return( ret );
} /* copyPEResources */

/*
 * writeDirEntry -
 * NB when an error occurs this function MUST return without altering errno
 */
static RcStatus writeDirEntry( PEResDirEntry *entry, int handle )
/***************************************************************/
{
    int     num_wrote;
    int     child_num;

    num_wrote = RcWrite( handle, &entry->Head, sizeof(resource_dir_header) );
    if( num_wrote != sizeof(resource_dir_header) ) return( RS_WRITE_ERROR );

    for( child_num = 0; child_num < entry->Head.num_name_entries +
                    entry->Head.num_id_entries; child_num++ ) {
        num_wrote = RcWrite( handle, entry->Children + child_num,
                        sizeof(resource_dir_entry) );
        if( num_wrote != sizeof(resource_dir_entry) ) return( RS_WRITE_ERROR );
    }

    return( RS_OK );
} /* writeDirEntry */

/*
 * writeDataEntry -
 * NB when an error occurs this function MUST return without altering errno
 */
static int writeDataEntry( PEResDataEntry * entry, int handle )
/*************************************************************/
{
    int     num_wrote;

    num_wrote = RcWrite( handle, &entry->Entry, sizeof(resource_entry) );
    if( num_wrote != sizeof(resource_entry) ) return( RS_WRITE_ERROR );
    return( RS_OK );
} /* writeDataEntry */

typedef struct {
    ResFileInfo         *curfile;
    unsigned_32         *rva;
} DataEntryCookie;

/*
 * setDataEntry
 */
static RcStatus setDataEntry( PEResEntry *entry, void *_info )
/************************************************************/
{
    WResLangInfo        *langinfo;
    ResFileInfo         *fileinfo;
    DataEntryCookie     *info = _info;

    if( !entry->IsDirEntry ) {
        fileinfo = WResGetFileInfo( entry->u.Data.Wind );
        if( info->curfile == NULL || info->curfile == fileinfo ) {
            langinfo = WResGetLangInfo( entry->u.Data.Wind );
            entry->u.Data.Entry.data_rva = *info->rva;
            entry->u.Data.Entry.size = langinfo->Length;
            entry->u.Data.Entry.code_page = 0;    /* should this be the UNICODE page*/
            entry->u.Data.Entry.rsvd = 0;
            *info->rva = *info->rva +
                        ALIGN_VALUE( langinfo->Length, sizeof(uint_32) );
        }
    }
    return( RS_OK );
}

/*
 * writeEntry-
 * NB when an error occurs this function MUST return without altering errno
 */
static RcStatus writeEntry( PEResEntry * entry, void * _handle )
/**************************************************************/
{
    int *handle = _handle;

    if( entry->IsDirEntry ) {
        return( writeDirEntry( &entry->u.Dir, *handle ) );
    } else {
        return( writeDataEntry( &entry->u.Data, *handle ) );
    }
} /* writeEntry */

/*
 * writeDirectory
 * NB when an error occurs this function MUST return without altering errno
 */
static RcStatus writeDirectory( PEResDir * dir, int handle )
/********************************************************************/
{
    long        seek_rc;
    int         num_wrote;
    RcStatus    ret;

    seek_rc = RcSeek( handle, dir->ResOffset, SEEK_SET );
    if( seek_rc == -1 ) return( RS_WRITE_ERROR );

    /* write the root entry header */
    ret = writeDirEntry( &dir->Root, handle );
    if( ret != RS_OK ) return( ret );

    ret = traverseTree( dir, &handle, writeEntry );
    if( ret != RS_OK ) return( ret );

    if( dir->String.StringBlock != 0 ) {
        num_wrote = RcWrite( handle, dir->String.StringBlock,
                                dir->String.StringBlockSize );
        if( num_wrote != dir->String.StringBlockSize ) {
            return( RS_WRITE_ERROR );
        }
    }

    return( RS_OK );
} /* writeDirectory */

static void FreeSubDir( PEResDirEntry * subdir )
/**********************************************/
{
    int             num_children;
    PEResEntry *    last_child;
    PEResEntry *    curr;

    num_children = subdir->Head.num_id_entries + subdir->Head.num_name_entries;
    last_child = subdir->Children + num_children;
    curr = subdir->Children;
    while( curr < last_child ) {
        if( curr->IsDirEntry ) {
            FreeSubDir( &curr->u.Dir );
        }
        curr++;
    }

    RcMemFree( subdir->Children );
}

static void FreePEResDir( PEResDir * dir )
/****************************************/
{
    FreeSubDir( &dir->Root );
    RcMemFree( dir->String.StringBlock );
    RcMemFree( dir->String.StringList );
}

/*
 * padObject
 * NB when an error occurs this function MUST return without altering errno
 */
static int padObject( PEResDir *dir, ExeFileInfo *tmp, long size )
{
    long        pos;
    long        pad;

    pos = RcTell( tmp->Handle );
    if( pos == -1 ) return( TRUE );
    pad = dir->ResOffset + size - pos;
    if( pad > 0 ) {
        RcPadFile( tmp->Handle, pad );
    }
    CheckDebugOffset( tmp );
    return( FALSE );
#if(0)
    long        seek_rc;
    char        zero=0;

    seek_rc = RcSeek( tmp->Handle, dir->ResOffset, SEEK_SET );
    if( seek_rc == -1 ) return( TRUE );
    seek_rc = RcSeek( tmp->Handle, size-1, SEEK_CUR );
    if( seek_rc == -1 ) return( TRUE );
    if( RcWrite( tmp->Handle, &zero, 1 ) != 1 ) return( TRUE );
    CheckDebugOffset( tmp );
    return( FALSE );
#endif
}

static void fillResourceObj( pe_object *res_obj, PEResDir *dir,
                             uint_32 alignment )
/****************************************************************/
{
#if 0
    unsigned_32     size;       /* object virtual size */
    unsigned_32     addr;       /* base virtual address */
    unsigned_32     flags;
    unsigned_32     mapidx;     /* page map index */
    unsigned_32     mapsize;    /* number of entries in page map */
    unsigned_32     reserved;
#endif

    res_obj->virtual_size = 0;
    res_obj->rva = dir->ResRVA;
    res_obj->physical_size = ALIGN_VALUE( dir->ResSize, alignment );
    res_obj->physical_offset = dir->ResOffset;
    res_obj->relocs_rva = 0;
    res_obj->linnum_rva = 0;
    res_obj->num_relocs = 0;
    res_obj->num_linnums = 0;
    res_obj->flags = PE_OBJ_INIT_DATA | PE_OBJ_READABLE;
}

static void setDataOffsets( PEResDir *dir, unsigned_32 *curr_rva,
                                ResFileInfo *resfiles, int writebyfile )
/****************************************************************/
{
    DataEntryCookie     cookie;

    cookie.rva = curr_rva;
    if( writebyfile ) {
        while( resfiles != NULL ) {
            cookie.curfile = resfiles;
            traverseTree( dir, &cookie, setDataEntry );
            resfiles = resfiles->next;
        }
    } else {
        cookie.curfile = NULL;
        traverseTree( dir, &cookie, setDataEntry );
    }
}
#endif

static int CompareLXResIdName( const void * _entry1, const void * _entry2 )
/*************************************************************************/
{
    const LXResEntry *entry1 = _entry1;
    const LXResEntry *entry2 = _entry2;

    if( entry1->resource.type_id > entry2->resource.type_id ) {
        return( 1 );
    } else if( entry1->resource.type_id < entry2->resource.type_id ) {
        return( -1 );
    } else {
        if( entry1->resource.name_id > entry2->resource.name_id ) {
            return( 1 );
        } else if( entry1->resource.name_id < entry2->resource.name_id ) {
            return( -1 );
        } else {
            return( 0 );
        }
    }
} /* CompareLXResIdName */


static int addRes( LXResTable *res, WResDirWindow wind )
/******************************************************/
{
    LXResEntry      *new_entry;
    WResResInfo     *resinfo;
    WResLangInfo    *res_lang;
    WResTypeInfo    *res_type;

    res_type = WResGetTypeInfo( wind );

    // RT_DEFAULTICON is not written into the executable, ignore
    if( res_type->TypeName.ID.Num == OS2_RT_DEFAULTICON )
        return( FALSE );

    /* realloc resource table if necessary (with 32 entries per increment
     * we won't need to realloc too often)
     */
    res->res_count++;
    if( sizeof( LXResEntry ) * res->res_count > res->table_size ) {
        LXResEntry      *curr_table;

        curr_table = res->resources;
        res->resources = RcMemRealloc( res->resources,
                            (res->res_count + 32) * sizeof( LXResEntry ) );
        if( res->resources == NULL ) {
            res->resources = curr_table;
            return( TRUE );
        }
    }

    resinfo  = WResGetResInfo( wind );
    res_lang = WResGetLangInfo( wind );

    /* add new resource entry into table */
    new_entry = &res->resources[res->res_count - 1];

    new_entry->wind = wind;
    new_entry->mem_flags = res_lang->MemoryFlags;
    new_entry->resource.res_size = res_lang->Length;
    if( resinfo->ResName.IsName ) {
        __asm int 3;
    }
    if( res_type->TypeName.IsName ) {
        __asm int 3;
    }

    new_entry->resource.type_id = res_type->TypeName.ID.Num;
    new_entry->resource.name_id = resinfo->ResName.ID.Num;
    new_entry->resource.object = 0;
    new_entry->resource.offset = res_lang->Offset;

    return( FALSE );
}


static int LXResTableBuild( LXResTable *res, WResDir dir )
/********************************************************/
{
    WResDirWindow   wind;

    res->num_objects = 0;
    res->num_pages   = 0;
    if( WResIsEmpty( dir ) ) {
        res->resources  = NULL;
        res->table_size = 0;
        res->res_count  = 0;
    } else {
        wind = WResFirstResource( dir );
        while( !WResIsEmptyWindow( wind ) ) {
            if( addRes( res, wind ) )
                return( TRUE );
            wind = WResNextResource( wind, dir );
        }
    }
    return( FALSE );
}


static void reportDuplicateResources( WResMergeError *errs )
/**********************************************************/
{
    WResMergeError  *curerr;
    ResFileInfo     *file1;
    ResFileInfo     *file2;
    WResResInfo     *resinfo;
    WResTypeInfo    *typeinfo;

    curerr = errs;
    while( curerr != NULL ) {
        resinfo = WResGetResInfo( curerr->dstres );
        typeinfo = WResGetTypeInfo( curerr->dstres );
        file1 = WResGetFileInfo( curerr->dstres );
        file2 = WResGetFileInfo( curerr->srcres );
        ReportDupResource( &resinfo->ResName, &typeinfo->TypeName,
                           file1->name, file2->name, FALSE );
        curerr = curerr->next;
    }
}


// merge the directories of all the res files into one large directory
// stored on the first resfileinfo node
static int mergeDirectory( ResFileInfo *resfiles, WResMergeError **errs )
/***********************************************************************/
{
    ResFileInfo         *cur;

    if( errs != NULL )
        *errs = NULL;
    if( resfiles == NULL )
        return( FALSE );
    cur = resfiles->next;
    while( cur != NULL ) {
        if( WResMergeDirs( resfiles->Dir, cur->Dir, errs ) )
            return( TRUE );
        cur = cur->next;
    }
    return( FALSE );
}


extern int BuildLXResourceObjects( ExeFileInfo *exeinfo, ResFileInfo *resinfo,
                                   object_record *res_obj, unsigned_32 rva,
                                   unsigned_32 offset, int writebyfile )
/**************************************************************************/
{
    LXResTable      *dir;
//    RcStatus        status;
    WResMergeError  *errs;
//    ResFileInfo     *errres;
    int             i;
    LXResEntry      *entry;
    unsigned_32     total = 0;
    unsigned_32     curr_total;

    dir = &exeinfo->u.LXInfo.Res;

    mergeDirectory( resinfo, &errs );
    if( errs != NULL ) {
        reportDuplicateResources( errs );
        WResFreeMergeErrors( errs );
        return( TRUE );
    }
    if( LXResTableBuild( dir, resinfo->Dir ) ) {
        RcError( ERR_INTERNAL, INTERR_ERR_BUILDING_RES_DIR );
        return( TRUE );
    }

    /* OS/2 2.x requires resources to be sorted */
    qsort( dir->resources, dir->res_count, sizeof( LXResEntry ),
            CompareLXResIdName );

    /* Assign resources to objects/pages and figure out exactly
     * how many we'll need, in order to determine the exact size
     * of executable's header section.
     */
    curr_total = 0;
    dir->num_objects++;
    exeinfo->u.LXInfo.OS2Head.num_rsrcs = dir->res_count;
    for( i = 0; i < dir->res_count; ++i ) {
        unsigned_32     res_size;

        entry = &dir->resources[i];

        // IBM's RC appears to align resources on 32-bit boundary
        res_size = (entry->resource.res_size + 3) & ~3;
        total += res_size;
        curr_total += res_size;
        while( curr_total > OSF_DEF_PAGE_SIZE ) {
            dir->num_pages++;
            curr_total -= OSF_DEF_PAGE_SIZE;
        }

        printf( "    %d.%d (%d bytes)\n", entry->resource.name_id,
            entry->resource.type_id, entry->resource.res_size );
    }
    if( curr_total )
        dir->num_pages++;

    printf( "total size: %d bytes in %d page(s)\n", total, dir->num_pages );

#if 0

    status = copyPEResources( exeinfo, resinfo, exeinfo->Handle,
                              writebyfile, &errres );
    // warning - the file names output in these messages could be
    //          incorrect if the -fr switch is in use
    if( status != RS_OK  ) {
        switch( status ) {
        case RS_WRITE_ERROR:
            RcError( ERR_WRITTING_FILE, exeinfo->name, strerror( errno ) );
            break;
        case RS_READ_ERROR:
            RcError( ERR_READING_RES, errres->name, strerror( errno )  );
            break;
        case RS_OPEN_ERROR:
            RcError( ERR_CANT_OPEN_FILE, errres->name, strerror( errno ) );
        case RS_READ_INCMPLT:
            RcError( ERR_UNEXPECTED_EOF, errres->name );
            break;
        default:
            RcError( ERR_INTERNAL, INTERR_UNKNOWN_RCSTATUS );
            break;
        }
        return( TRUE );
    }

    exeinfo->u.PEInfo.Res.ResSize = curr_rva - rva;
    fillResourceObj( res_obj, dir, exeinfo->u.PEInfo.WinHead->file_align );
    if( padObject( dir, exeinfo, res_obj->physical_size ) ) {
        RcError( ERR_WRITTING_FILE, exeinfo->name, strerror( errno ) );
        return( TRUE );
    }
#endif
    RcMemFree( dir->resources );

    return( FALSE );
} /* BuildLXResourceObjects */


#ifndef INSIDE_WLINK
int RcBuildLXResourceObjects( void )
/**********************************/
{
    object_record       *res_objects;
    RcStatus            error;
    ExeFileInfo         *exeinfo;

    exeinfo = &Pass2Info.TmpFile;
    if( CmdLineParms.NoResFile ) {
        exeinfo->u.LXInfo.OS2Head.num_rsrcs = 0;
    } else {
        res_objects = exeinfo->u.LXInfo.Objects;
        error = BuildLXResourceObjects( exeinfo, Pass2Info.ResFiles,
                                        res_objects, 0, 0, //rva, offset,
                                        !Pass2Info.AllResFilesOpen );
    }
    if( CmdLineParms.WritableRes ) {
        // Not sure if setting the resource objects writable would work
    }
    return( error );
}
#endif
