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
* Description:  Librarian symbol table processing.
*
****************************************************************************/


#include "wlib.h"

static sym_table        FileTable;
sym_file                *CurrFile;
static sym_entry        **HashTable;
static sym_entry        **SortedSymbols;

#define HASH_SIZE       256

static int Hash( char *string, unsigned *plen );

void InitFileTab( void )
/**********************/
{
    FileTable.first = NULL;
    FileTable.add_to = &FileTable.first;
    SortedSymbols = NULL;
    HashTable = MemAllocGlobal( HASH_SIZE * sizeof( HashTable[ 0 ] ) );
    memset( HashTable, 0, HASH_SIZE * sizeof( HashTable[ 0 ] ) );
}

void FiniSymFile( sym_file *sfile )
/********************************/
{
    sym_entry           *sym, *next_sym;
    elf_import_sym      *temp;

    for( sym = sfile->first; sym != NULL; sym = next_sym ) {
        next_sym = sym->next;
        MemFreeGlobal( sym );
    }
    MemFreeGlobal( sfile->full_name );
    MemFreeGlobal( sfile->arch.name );
    MemFreeGlobal( sfile->arch.ffname );
    if( sfile->import != NULL ) {
        switch( sfile->import->type ) {
        case ELF:
        case ELFRENAMED:
            for( temp = sfile->import->symlist; temp != NULL;
                         temp = sfile->import->symlist ) {
                sfile->import->symlist = temp->next;
                MemFreeGlobal( temp->name );
                MemFreeGlobal( temp );
            }
            MemFreeGlobal( sfile->import->DLLName );
            break;
        default:
            MemFreeGlobal( sfile->import->DLLName );
            MemFreeGlobal( sfile->import->symName );
            MemFreeGlobal( sfile->import->exportedName );
            break;
        }
        MemFreeGlobal( sfile->import );
    }
    MemFreeGlobal( sfile );
}


void CleanFileTab( void )
/***********************/
{
    sym_file    *last = NULL;
    sym_file    *curr;
    sym_file    *next;

    for( curr = FileTable.first; curr; curr = next ) {
        next = curr->next;

        /*
         * If curr->first is NULL then either this file contains no
         * symbols or we are ignoring all of them.  Remove the file.
         */
        if( !curr->first ) {
            if( last ) {
                last->next = curr->next;
            } else {
                FileTable.first = curr->next;
            }

            if( &(curr->next) == FileTable.add_to ) {
                if( last ) {
                    FileTable.add_to = &(last->next);
                } else {
                    FileTable.add_to = &(FileTable.first);
                }
            }

            FiniSymFile( curr );
        } else {
            last = curr;
        }
    } /* for */
} /* CleanFileTab() */


void ResetFileTab( void )
/***********************/
{
    sym_file    *sfile;
    sym_file    *next_sfile;

    memset( HashTable, 0, HASH_SIZE * sizeof( HashTable[ 0 ] ) );
    for( sfile = FileTable.first; sfile != NULL; sfile = next_sfile ) {
        next_sfile = sfile->next;
        FiniSymFile( sfile );
    }
    FileTable.first = NULL;
    FileTable.add_to = &FileTable.first;
    MemFreeGlobal( SortedSymbols );
    SortedSymbols = NULL;
}

void FiniFileTab( void )
/***********************/
{
    MemFreeGlobal( HashTable );
}


void RemoveFromHashTable( sym_entry *sym )
/****************************************/
{
    sym_entry       *hash;
    sym_entry       *prev;
    int             hval;
    unsigned        len;

    hval = Hash( sym->name, &len );
    hash = HashTable[ hval ];

    if( hash == sym ) {
        HashTable[ hval ] = sym->hash;
    } else if( hash ) {
        prev = hash;

        for( hash = hash->hash; hash; hash = hash->hash ) {
            if( hash == sym ) {
                prev->hash = hash->hash;
                break;
            } else {
                prev = hash;
            }
        }
    }
} /* RemoveFromHashTable() */


static void NewSymFile( arch_header *arch )
/*****************************************/
{
    sym_file    *sfile;

    sfile = MemAllocGlobal( sizeof( sym_file ) );
    sfile->first = NULL;
    sfile->next = NULL;
    sfile->arch = *arch;
    sfile->import = NULL;
    sfile->inlib_offset = 0;
    sfile->full_name = DupStrGlobal( sfile->arch.name );
    sfile->arch.name = DupStrGlobal( TrimPath( sfile->arch.name ) ); // make own copy
    sfile->name_length = strlen( sfile->arch.name );
    if( sfile->arch.ffname ) {
        sfile->arch.ffname = DupStrGlobal( sfile->arch.ffname );
        sfile->ffname_length = strlen( sfile->arch.ffname );
    } else {
        sfile->ffname_length = 0;
    }
    *(FileTable.add_to) = sfile;
    FileTable.add_to = &sfile->next;
    CurrFile = sfile;
}



static int CompSyms( const void *ap, const void *bp )
/***************************************************/
{
    const sym_entry *a = *(const sym_entry **)ap;
    const sym_entry *b = *(const sym_entry **)bp;
    return( strcmp( a->name, b->name ) );
}

static void WriteFileHeader( arch_header *arch )
/**********************************************/
{
    ar_header   ar;

    CreateARHeader( &ar, arch );
    WriteNew( &ar, AR_HEADER_SIZE );
}

static void WritePad( file_offset size )
/**************************************/
{
    if( NeedsRounding( size ) ) {
        WriteNew( AR_FILE_PADDING_STRING, 1 );
    }
}

static long             NumFiles;
static long             NumSymbols;
static file_offset      TotalNameLength;
static file_offset      TotalFFNameLength;
static file_offset      TotalSymbolLength;

static void SortSymbols( void )
/*****************************/
{
    sym_file    *sfile;
    sym_entry   *sym;
    sym_entry   **sym_curr;
    int         i;
    int         name_length = 0;
    int         name_extra = 0;

    NumFiles = 0;
    NumSymbols = 0;
    TotalNameLength = 0;
    TotalFFNameLength = 0;
    TotalSymbolLength = 0;
    switch( Options.libtype ) {
    case WL_LTYPE_AR:
        name_extra = 1;
        break;
    case WL_LTYPE_MLIB:
        name_extra = 2;
        break;
    }
    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        ++NumFiles;
        sfile->name_offset = TotalNameLength;
        switch( Options.libtype ) {
        case WL_LTYPE_AR:
            // Always using "full" filename for AR
            if( sfile->arch.ffname ) {
                name_length = sfile->ffname_length;
            } else {
                sfile->ffname_length = 0;
                name_length = sfile->name_length;
            }
            break;
        case WL_LTYPE_MLIB:
            // If no full filename, assume name is full, and trim
            // it to get non-full filename.
            if( sfile->arch.ffname == NULL ) {
                sfile->arch.ffname = sfile->arch.name;
                sfile->ffname_length = strlen( sfile->arch.ffname );
                sfile->arch.name = MemAllocGlobal( _MAX_FNAME + _MAX_EXT + 1 );
                _splitpath( sfile->arch.ffname, NULL, NULL, sfile->arch.name, NULL );
                _splitpath( sfile->arch.ffname, NULL, NULL, NULL, sfile->arch.name + strlen( sfile->arch.name ) );
                sfile->name_length = strlen( sfile->arch.name );
            }
            name_length = sfile->name_length;
            break;
        }
        TotalNameLength += name_length + name_extra;
        TotalFFNameLength += sfile->ffname_length + 1;
        for( sym = sfile->first; sym != NULL; sym = sym->next ) {
            ++NumSymbols;
            TotalSymbolLength += sym->len + 1;
        }
    }

    if( NumSymbols == 0 ) {
        SortedSymbols = NULL;
        Warning( ERR_NO_SYMBOLS );
    } else {
        SortedSymbols = MemAllocGlobal( NumSymbols * sizeof( SortedSymbols[ 0 ] ) );
    }

    sym_curr = SortedSymbols;
    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        for( sym = sfile->first; sym != NULL; sym = sym->next ) {
            *sym_curr = sym;
            ++sym_curr;
        }
    }

    qsort( SortedSymbols, NumSymbols, sizeof(sym_entry *), CompSyms );

    // re-hook symbols onto files in sorted order

    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        sfile->first = NULL;
    }

    for( i = NumSymbols - 1; i >= 0; --i ) {
        sym = SortedSymbols[ i ];
        sym->next = sym->file->first;
        sym->file->first = sym;
    }
}

static void WriteOmfFileTable( void )
/***********************************/
{
    sym_file    *sfile;
    unsigned    num_blocks;
    unsigned    dict_offset;

    if( Options.page_size == 0 ) {
        Options.page_size = DEFAULT_PAGE_SIZE;
    }
    InitOmfUtil();
    PadOmf( TRUE );

    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        WriteOmfFile( sfile );
    }
    WriteOmfLibTrailer();
    dict_offset = LibTell( NewLibrary );
    num_blocks = WriteOmfDict( FileTable.first );
    WriteOmfLibHeader( dict_offset, num_blocks );
    FiniOmfUtil();
}

static void WriteArMlibFileTable( void )
/**************************************/
{
    arch_header     arch;
    sym_file        *sfile;
    sym_entry       *sym;
    file_offset     dict1_size = 0;
    file_offset     dict2_size = 0;
    file_offset     header_size = 0;
    int             i;
    time_t          currenttime = time( NULL );
    file_offset     obj_offset;
    int             index;
    libfile         io;
    char            buff[ 20 ];
    char            *stringpad = NULL;
    int             stringpadlen = 0;


    SortSymbols();

    // figure out this dictionary sizes

    switch( Options.libtype ) {
    case WL_LTYPE_AR:
        dict1_size = sizeof( unsigned_32 )
                   + NumSymbols * sizeof(unsigned_32)
                   + RoundWord( TotalSymbolLength );

        dict2_size = sizeof( unsigned_32 )
                   + NumFiles * sizeof( unsigned_32 )
                   + sizeof( unsigned_32 )
                   + NumSymbols * sizeof( unsigned_16 )
                   + RoundWord( TotalSymbolLength );

        header_size = AR_IDENT_LEN
                    + 3 * AR_HEADER_SIZE
                    + dict1_size
                    + dict2_size
                    + RoundWord( TotalNameLength );
        stringpad   = "\0";
        stringpadlen= 1;
        break;
    case WL_LTYPE_MLIB:
        dict2_size = sizeof( unsigned_32 )
                    + NumSymbols * (1 + sizeof( unsigned_32 ) )
                    + TotalSymbolLength;

        header_size = LIBMAG_LEN + LIB_CLASS_LEN + LIB_DATA_LEN
                    + 3 * LIB_HEADER_SIZE
                    + RoundWord( dict2_size )
                    + RoundWord( TotalNameLength )
                    + RoundWord( TotalFFNameLength );
        stringpad   = LIB_LONG_NAME_END_STRING;
        stringpadlen= LIB_LONG_NAME_END_STRING_LEN;
        break;
    }

    // calculate the object files offsets

    index = 0;
    obj_offset = 0;
    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        sfile->new_offset = obj_offset + header_size;
        sfile->index = ++index;
        obj_offset += RoundWord( sfile->arch.size ) + AR_HEADER_SIZE;
    }

    switch( Options.libtype ) {
    case WL_LTYPE_AR:
        WriteNew( AR_IDENT, AR_IDENT_LEN );
        break;
    case WL_LTYPE_MLIB:
        WriteNew( LIBMAG, LIBMAG_LEN );
        WriteNew( LIB_CLASS_DATA_SHOULDBE, LIB_CLASS_LEN + LIB_DATA_LEN );
        break;
    }

    // write the useless dictionary

    arch.date = currenttime;
    arch.uid = 0;
    arch.gid = 0;
    arch.mode = 0;
    if( Options.libtype == WL_LTYPE_AR ) {
        arch.size = dict1_size;
        arch.name = "/";
        WriteFileHeader( &arch );

        WriteBigEndian32( NumSymbols );
        for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
            for( sym = sfile->first; sym != NULL; sym = sym->next ) {
                WriteBigEndian32( sym->file->new_offset );
            }
        }
        for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
            for( sym = sfile->first; sym != NULL; sym = sym->next ) {
                WriteNew( sym->name, sym->len+1 );
            }
        }
        WritePad( TotalSymbolLength );
    }

    // write the useful dictionary

    arch.size = dict2_size;
    arch.name = "/";
    WriteFileHeader( &arch );

    if( Options.libtype == WL_LTYPE_AR ) {
        WriteLittleEndian32( NumFiles );
        for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
            WriteLittleEndian32( sfile->new_offset );
        }
    }

    WriteLittleEndian32( NumSymbols );
    switch( Options.libtype ) {
    case WL_LTYPE_AR:
        for( i = 0; i < NumSymbols; ++i ) {
            WriteLittleEndian16( SortedSymbols[ i ]->file->index );
        }
        break;
    case WL_LTYPE_MLIB:
        for( i = 0; i < NumSymbols; ++i ) {
            WriteLittleEndian32( SortedSymbols[ i ]->file->index );
        }
        for( i = 0; i < NumSymbols; ++i ) {
            WriteNew( &(SortedSymbols[ i ]->info), 1 );
        }
        break;
    }
    for( i = 0; i < NumSymbols; ++i ) {
        WriteNew( SortedSymbols[ i ]->name, SortedSymbols[ i ]->len + 1 );
    }
    switch( Options.libtype ) {
    case WL_LTYPE_AR:
        WritePad( TotalSymbolLength );
        break;
    case WL_LTYPE_MLIB:
        WritePad( dict2_size );
        break;
    }

    // write the string table

    arch.size = TotalNameLength;
    arch.name = "//";
    WriteFileHeader( &arch );
    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        if( sfile->name_offset == -1 )
            continue;
        // Always write the "full" filename for AR
        if( Options.libtype == WL_LTYPE_AR && sfile->arch.ffname ) {
            WriteNew( sfile->arch.ffname, sfile->ffname_length );
        } else {
            WriteNew( sfile->arch.name, sfile->name_length );
        }
        WriteNew( stringpad, stringpadlen );
    }
    WritePad( TotalNameLength );

    // write the full filename table

    if( Options.libtype == WL_LTYPE_MLIB ) {
        arch.size = TotalFFNameLength;
        arch.name = "///";
        WriteFileHeader( &arch );
        for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
            WriteNew( sfile->arch.ffname, sfile->ffname_length + 1 );
        }
        WritePad( TotalFFNameLength );
    }

    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        arch = sfile->arch;
        buff[ 0 ] = '/';
        itoa( sfile->name_offset, buff+1, 10 );
        arch.name = buff;
        WriteFileHeader( &arch );
        if( sfile->import == NULL ) {
            if( sfile->inlib_offset != 0 ) {
                LibSeek( InLibHandle( sfile->inlib ), sfile->inlib_offset, SEEK_SET );
                Copy( InLibHandle( sfile->inlib ), NewLibrary, arch.size );
            } else {
                io = LibOpen( sfile->full_name, LIBOPEN_BINARY_READ );
                Copy( io, NewLibrary, arch.size );
                LibClose( io );
            }
        } else {
            switch( sfile->import->type ) {
            case ELF:
            case ELFRENAMED:
                ElfWriteImport( NewLibrary, sfile );
                break;
            default:
                CoffWriteImport( NewLibrary, sfile );
                break;
            }
        }
        WritePad( sfile->arch.size );
    }
}

void WriteFileTable( void )
/*************************/
{
    if( Options.libtype == WL_LTYPE_NONE && Options.omf_found ) {
        if( Options.coff_found ) {
            Options.libtype = WL_LTYPE_AR;
        } else {
            Options.libtype = WL_LTYPE_OMF;
        }
    }
    if( Options.coff_found && (Options.libtype == WL_LTYPE_NONE || Options.libtype == WL_LTYPE_OMF) ) {
        Options.libtype = WL_LTYPE_AR;
    }
    if( Options.elf_found && (Options.libtype == WL_LTYPE_NONE || Options.libtype == WL_LTYPE_OMF) ) {
        Options.libtype = WL_LTYPE_AR;
    }
    if( Options.libtype == WL_LTYPE_AR || Options.libtype == WL_LTYPE_MLIB ) {
        WriteArMlibFileTable();
    } else {
        WriteOmfFileTable();
    }
}

static int Hash( char *string, unsigned *plen )
/*********************************************/
{
    unsigned long       g;
    unsigned long       h;

    h = 0;
    *plen = 0;
    while( *string != 0 ) {
        h = ( h << 4 ) + *string;
        if( (g = h & 0xf0000000) ) {
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
        ++string;
        ++*plen;
    }
    return( h % HASH_SIZE );
}

void AddSym( char *name, symbol_strength strength, unsigned char info )
/*********************************************************************/
{
    sym_entry   *sym,**owner;
    int         hash;
    unsigned    name_len;

    hash = Hash( name, &name_len );
    for( sym = HashTable[ hash ]; sym != NULL; sym = sym->hash ) {
        if( sym->len != name_len )
            continue;
        if( SymbolNameCmp( sym->name, name ) == 0 ) {
            if( strength > sym->strength ) {
                owner = &sym->file->first;
                while( *owner != sym ) {
                    owner = &(*owner)->next;
                }
                *owner = sym->next;
                owner = &HashTable[ hash ];
                while( *owner != sym ) {
                    owner = &(*owner)->hash;
                }
                *owner = sym->hash;
                MemFreeGlobal( sym );
                break; //db
            } else if( strength == sym->strength ) {
                if( strength == SYM_STRONG ) {
                    Warning( ERR_DUPLICATE_SYMBOL, FormSym( name ) );
                }
            }
            return;
        }
    }
    sym = MemAllocGlobal( sizeof( sym_entry ) + name_len );
    sym->len = name_len;
    sym->strength = strength;
    sym->info = info;
    memcpy( sym->name, name, name_len + 1 );
    sym->next = CurrFile->first;
    CurrFile->first = sym;
    sym->file = CurrFile;
    sym->hash = HashTable[ hash ];
    HashTable[ hash ] = sym;
}


#ifdef __DEBUG__
void DumpFileTable( void )
{
    sym_file    *sfile;
    sym_entry   *entry;
    sym_entry   *hash;
    unsigned    len;
    int         hval;
    long        files    = 0L;
    long        symbols  = 0L;

    printf("----------------------------------------------------------\n");
    printf("File Table Dump\n");
    printf("----------------------------------------------------------\n");

    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        ++files;
        printf( "File: \"%s\"\n", sfile->full_name );
        for( entry = sfile->first; entry; entry = entry->next ) {
            ++symbols;

            hval = Hash( entry->name, &len );
            printf( "\t\"%s\" (%d, %u, \"%s\")", entry->name, hval, len,
                    (HashTable[ hval ] ? HashTable[ hval ]->name : "(NULL)") );

            for( hash = entry->hash; hash; hash = hash->hash ) {
                printf( " -> \"%s\"", hash->name );
                fflush( stdout );
            }
            printf( "\n" );
        }
    }
    printf( "----------------------------------------------------------\n" );
    printf( "Files         : %ld\n", files );
    printf( "Symbols       : %ld\n", symbols );
    printf( "----------------------------------------------------------\n" );
} /* DumpFileTable() */


void DumpHashTable( void )
{
    sym_entry   *hash;
    int         i;
    int         length;

    printf( "----------------------------------------------------------\n" );
    printf( "Hash Table Dump\n" );
    printf( "----------------------------------------------------------\n" );

    for( i = 0; i < HASH_SIZE; ++i ) {
        length = 0;

        if( HashTable[ i ] ) {
            for( hash = HashTable[ i ]; hash; hash = hash->next ) {
                ++length;
            }
        }

        printf( "Offset %6d: %d\n", i, length );
    }
    printf( "----------------------------------------------------------\n" );
} /* DumpHashTable() */
#endif // __DEBUG__


bool RemoveObjectSymbols( char *name )
/************************************/
{
    sym_file    *sfile;
    sym_file    *prev_sfile;
    sym_entry   *sym;

    prev_sfile = NULL;
    for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
        if( SymbolNameCmp( sfile->full_name, name ) == 0 ) {
            if( prev_sfile != NULL ) {    /* Not deleting from head of list */
                prev_sfile->next = sfile->next;

                if( FileTable.add_to == &sfile->next ) { /* Last node in list */
                    FileTable.add_to = &prev_sfile->next;
                }
            } else {
                if( FileTable.add_to == &sfile->next ) { /* Only node in list */
                    FileTable.add_to = &FileTable.first;
                    FileTable.first = NULL;
                } else {  /* First node in the list */
                    FileTable.first = sfile->next;
                }
            }

            for( sym = sfile->first; sym != NULL; sym = sym->next ) {
                RemoveFromHashTable( sym );
            }

            FiniSymFile( sfile );
            Options.modified = TRUE;
            return( TRUE );
        }
        prev_sfile = sfile;
    }
    return( FALSE );
}

void AddObjectSymbols( arch_header *arch, libfile io, long offset )
/*****************************************************************/
{
    obj_file    *ofile;

    ofile = OpenLibFile( arch->name, io );
    if( ofile->orl ) {
        orl_file_handle orl;
        orl = ofile->orl;
        if( ORLFileGetFormat( ofile->orl ) == ORL_COFF ) {
            if( Options.libtype == WL_LTYPE_MLIB ) {
                FatalError( ERR_NOT_LIB, "COFF", LibFormat() );
            }
            Options.coff_found = 1;
        } else {
            Options.elf_found = 1;
            if( Options.omf_found == 1 ) {
                FatalError( ERR_MIXED_OBJ, "ELF", "OMF" );
            }
        }
    } else {
        if( Options.libtype == WL_LTYPE_MLIB ) {
            FatalError( ERR_NOT_LIB, "OMF", LibFormat() );
        }
        if( Options.elf_found == 1 ) {
            FatalError( ERR_MIXED_OBJ, "ELF", "OMF" );
        }
        Options.omf_found = 1;
    }
    NewSymFile( arch );
    CurrFile->inlib_offset = offset;
    CurrFile->inlib = FindInLib( io );
    ObjWalkSymList( ofile, CurrFile, AddSym );
    CloseLibFile( ofile );
}

void OmfMKImport( arch_header *arch, long ordinal, char *dll_name,
                  char *sym_name, char *exportedName, importType type )
{
    Options.omf_found = 1;
    if( Options.elf_found == 1 ) {
        FatalError( ERR_MIXED_OBJ, "ELF", "OMF" );
    }
    NewSymFile( arch );
    CurrFile->import = MemAllocGlobal( sizeof( import_sym ) );
    CurrFile->import->DLLName = DupStrGlobal( dll_name );
    CurrFile->import->ModName = DupStrGlobal( dll_name );
    _splitpath( dll_name, NULL, NULL, CurrFile->import->ModName, NULL );
    CurrFile->import->ordinal = ordinal;
    if( sym_name != NULL ) {
        CurrFile->import->symName = DupStrGlobal( sym_name );
    } else {
        CurrFile->import->symName = NULL;
    }
    if( exportedName != NULL ) {
        CurrFile->import->exportedName = DupStrGlobal( exportedName );
    } else {
        CurrFile->import->exportedName = NULL;
    }
    CurrFile->import->type = type;
    CurrFile->import->processor = WL_PROC_X86;  // set default JBS 99/12/21
}

void CoffMKImport( arch_header *arch, importType type,
                   long ordinal, char *DLLname, char *symName, 
                   char *exportedName, processor_type processor )
{

    NewSymFile( arch );
    Options.coff_found = TRUE;
    if( Options.elf_found == 1 ) {
        FatalError( ERR_MIXED_OBJ, "ELF", "COFF" );
    }

    CurrFile->import = MemAllocGlobal( sizeof( import_sym ) );
    CurrFile->import->type = type;
    CurrFile->import->ordinal = ordinal;
    CurrFile->import->DLLName = DupStrGlobal( DLLname );
    CurrFile->import->ModName = DupStrGlobal( DLLname );
    _splitpath( DLLname, NULL, NULL, CurrFile->import->ModName, NULL );
    if( symName != NULL ) {
        CurrFile->import->symName = DupStrGlobal( symName );
    } else {
        CurrFile->import->symName = NULL;
    }
    if( exportedName != NULL ) {
        CurrFile->import->exportedName = DupStrGlobal( exportedName );
    } else {
        CurrFile->import->exportedName = NULL;
    }
    CurrFile->import->processor = processor;
    CurrFile->arch.size = CoffImportSize( CurrFile->import );
}

void ElfMKImport( arch_header *arch, importType type, long export_size,
                  char *DLLname, char *strings, Elf32_Export *export_table,
                  Elf32_Sym *sym_table, processor_type processor )
{
    int                 i;
    elf_import_sym      **temp;

    NewSymFile( arch );
    Options.elf_found = 1;
    if( Options.coff_found == 1 ) {
        FatalError( ERR_MIXED_OBJ, "ELF", "COFF" );
    }
    if( Options.omf_found == 1 ) {
        FatalError( ERR_MIXED_OBJ, "ELF", "OMF" );
    }

    CurrFile->import = MemAllocGlobal( sizeof( import_sym ) );
    CurrFile->import->type = type;
    CurrFile->import->DLLName = DupStrGlobal( DLLname );
    CurrFile->import->ModName = DupStrGlobal( DLLname );
    _splitpath( DLLname, NULL, NULL, CurrFile->import->ModName, NULL );
    CurrFile->import->numsyms = 0;
    temp = &(CurrFile->import->symlist);

    for( i=0; i<export_size; i++ ) {
        if( export_table[ i ].exp_symbol ) {
            *temp = MemAllocGlobal( sizeof( elf_import_sym ) );
            (*temp)->name = DupStrGlobal(&(strings[ sym_table[ export_table[ i ].
                exp_symbol ].st_name ]));
            (*temp)->len = strlen( (*temp)->name );
            (*temp)->ordinal = export_table[ i ].exp_ordinal;
            if( type == ELF ) {
                AddSym( (*temp)->name, SYM_STRONG, ELF_IMPORT_SYM_INFO );
            }

            CurrFile->import->numsyms ++;

            temp = &((*temp)->next);
        }
    }
    *temp = NULL;
    CurrFile->import->processor = processor;
    CurrFile->arch.size = ElfImportSize( CurrFile->import );
}

#define MAX_MESSAGE_LEN 511
static char             listMsg[ MAX_MESSAGE_LEN + 1 ];
static unsigned         msgLength = 0;

static void listPrint( FILE *fp, char *str, ... )
{
    va_list             arglist;
    va_start( arglist, str );
    msgLength += _vbprintf( listMsg + msgLength, MAX_MESSAGE_LEN - msgLength, str, arglist );
    va_end( arglist );
}

static void listNewLine( FILE *fp )
{
    if( fp ) {
        fprintf( fp, listMsg );
        fputc( '\n', fp );
    } else {
        Message( listMsg );
    }
    msgLength = 0;
    listMsg[ 0 ] = ' ';
    listMsg[ 1 ] = '\0';
}

#define LINE_WIDTH 79
#define OFF_COLUMN 40

static void fpadch( FILE *fp, char ch, int len )
{
    if( len <= 0 ) {
        return;
    }
    len = min( MAX_MESSAGE_LEN - msgLength, len );
    memset( listMsg + msgLength, ch, len );
    msgLength += len;
    listMsg[ msgLength ] = '\0';
}

static void printVerboseTableEntry( arch_header *arch )
{
    char        member_mode[ 11 ];
    char        date[ 128 ];
    time_t      t;

    member_mode[ 10 ] = '\0';
    member_mode[ 9 ] = ' ';
    if( arch->mode & S_IRUSR ) {
        member_mode[ 0 ] = 'r';
    } else {
        member_mode[ 0 ] = '-';
    }
    if( arch->mode & S_IWUSR ) {
        member_mode[ 1 ] = 'w';
    } else {
        member_mode[ 1 ] = '-';
    }
    if( !( arch->mode & S_IXUSR ) && (arch->mode & S_ISUID ) ) {
        member_mode[ 2 ] = 'S';
    } else if( ( arch->mode & S_IXUSR ) && ( arch->mode & S_ISUID ) ) {
        member_mode[ 2 ] = 's';
    } else if( arch->mode & S_IXUSR ) {
        member_mode[ 2 ] = 'x';
    } else {
        member_mode[ 2 ] = '-';
    }
    if( arch->mode & S_IRGRP ) {
        member_mode[ 3 ] = 'r';
    } else {
        member_mode[ 3 ] = '-';
    }
    if( arch->mode & S_IWGRP ) {
        member_mode[ 4 ] = 'w';
    } else {
        member_mode[ 4 ] = '-';
    }
    if( !( arch->mode & S_IXGRP ) && (arch->mode & S_ISGID ) ) {
        member_mode[ 5 ] = 'S';
    } else if( ( arch->mode & S_IXGRP ) && ( arch->mode & S_ISGID ) ) {
        member_mode[ 5 ] = 's';
    } else if( arch->mode & S_IXGRP ) {
        member_mode[ 5 ] = 'x';
    } else {
        member_mode[ 5 ] = '-';
    }
    if( arch->mode & S_IROTH ) {
        member_mode[ 6 ] = 'r';
    } else {
        member_mode[ 6 ] = '-';
    }
    if( arch->mode & S_IWOTH ) {
        member_mode[ 7 ] = 'w';
    } else {
        member_mode[ 7 ] = '-';
    }
    if( arch->mode & S_IXOTH ) {
        member_mode[ 8 ] = 'x';
    } else {
        member_mode[ 8 ] = '-';
    }
    t = (time_t) arch->date;
    strftime( date, 127, "%b %d %H:%M %Y", localtime( &t ) );
    Message( "%s %u/%u %u %s %s", member_mode, arch->uid, arch->gid, arch->size,
        date, MakeFName( arch->name ) );
}


void ListContents( void )
/******************************/
{
    sym_file    *sfile;
    lib_cmd     *cmd;

    if( Options.ar ) {
        if( CmdList ) {
            for( cmd = CmdList; cmd != NULL; cmd = cmd->next ) {
                if( cmd->ops & OP_FOUND ) {
                    if( Options.verbose ) {
                        for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
                            if( SameName( sfile->arch.name, cmd->name ) ) {
                                if( Options.terse_listing ) {
                                    Message( sfile->arch.name );
                                } else {
                                    printVerboseTableEntry( &( sfile->arch ) );
                                }
                                break;
                            }
                        }
                    } else {
                        Message( MakeFName( cmd->name ) );
                    }
                }
            }
        } else {
            if( Options.verbose ) {
                for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
                    printVerboseTableEntry( & ( sfile->arch ) );
                }
            } else {
                for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
                    Message( "%s", MakeFName( sfile->arch.name ) );
                }
            }
        }
    } else {
        sym_entry       *sym;
        int             i;
        FILE            *fp;
        char            *name;
        unsigned        name_len;

        if( Options.terse_listing ) {
            SortSymbols();
            for( i = 0; i < NumSymbols; ++i ) {
                sym = SortedSymbols[ i ];
                name = FormSym( sym->name );
                name_len = strlen( name );
                Message(name);
            }
            return;
        }

        if( Options.list_file == NULL ) {
            Options.list_file = DupStr( MakeListName() );
        }
        if( Options.list_file[ 0 ] != 0 ) {
            fp = fopen( Options.list_file, "w" );
            if( fp == NULL ) {
                FatalError( ERR_CANT_OPEN, Options.list_file, strerror( errno ) );
            }
        } else {
            fp = NULL;
        }
        SortSymbols();

        for( i = 0; i < NumSymbols; ++i ) {
            sym = SortedSymbols[ i ];
            name = FormSym( sym->name );
            name_len = strlen( name );
            listPrint( fp, "%s..", name );
            fpadch( fp, '.', LINE_WIDTH - 2 - name_len - sym->file->name_length );
            listPrint( fp, "%s", sym->file->arch.name );
            listNewLine( fp );
        }

        listNewLine( fp );

        for( sfile = FileTable.first; sfile != NULL; sfile = sfile->next ) {
            if( sfile->arch.ffname ) {
                listPrint( fp, "%s ", sfile->arch.ffname );
            } else {
                listPrint( fp, "%s ", sfile->arch.name );
            }
            fpadch( fp, ' ', OFF_COLUMN - 1 - sfile->name_length - 16 );
            listPrint( fp, "Offset=%8.8xH", sfile->inlib_offset );
            listNewLine( fp );
            for( sym = sfile->first; sym != NULL; sym = sym->next ) {
                listPrint( fp, "    %s", FormSym( sym->name ) );
                listNewLine( fp );
            }
            listNewLine( fp );
        }

        if( fp ) {
            fclose( fp );
        }
    }
}
