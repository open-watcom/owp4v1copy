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
* Description:  Processing of assembly directives.
*
****************************************************************************/


#include "asmglob.h"
#include <ctype.h>

#include "asmalloc.h"
#include "directiv.h"
#include "queues.h"
#include "asmexpnd.h"
#include "asmfixup.h"
#include "mangle.h"
#include "asmlabel.h"
#include "asminput.h"
#include "asmeval.h"

#include "myassert.h"

#define BIT16           0
#define BIT32           1

#define MAGIC_FLAT_GROUP        ModuleInfo.flat_grp

#define INIT_ALIGN      0x1
#define INIT_COMBINE    0x2
#define INIT_USE        0x4
#define INIT_CLASS      0x8
#define INIT_MEMORY     0x10
#define INIT_STACK      0x20

typedef struct {
      char      *string;        // the token string
      uint      value;          // value connected to this token
      uint      init;           // explained in direct.c ( look at SegDef() )
} typeinfo;

typedef struct {
    asm_sym             *symbol;        /* segment or group that is to
                                           be associated with the register */
    unsigned            error:1;        // the register is assumed to ERROR
    unsigned            flat:1;         // the register is assumed to FLAT
} assume_info;

typedef enum {
    SIM_CODE = 0,
    SIM_STACK,
    SIM_DATA,
    SIM_DATA_UN,            // .DATA?
    SIM_FARDATA,
    SIM_FARDATA_UN,         // .FARDATA?
    SIM_CONST,
    SIM_NONE,
    SIM_LAST = SIM_NONE
} sim_seg;

typedef struct {
    sim_seg     seg;                    // segment id
    char        close[MAX_LINE];        // closing line for this segment
    int_16      stack_size;             // size of stack segment
} last_seg_info;        // information about last opened simplified segment

enum {
#undef pick
#define pick( tok, str, val, init )     tok

#include "directd.h"
};


static typeinfo TypeInfo[] = {

#undef pick
#define pick( tok, string, value, init_val )    { string, value, init_val }

#include "directd.h"
};

#define LOCAL_DEFAULT_SIZE      2
#define DEFAULT_STACK_SIZE      1024

#define ARGUMENT_STRING         " [bp+ "
#define ARGUMENT_STRING_32      " [ebp+ "
#define LOCAL_STRING            " [bp- "
#define LOCAL_STRING_32         " [ebp- "

static char             *Check4Mangler( int *i );
static int              token_cmp( char **token, int start, int end );
static void             ModelAssumeInit( void );

extern  char            write_to_file;  // write if there is no error
extern  uint_32         BufSize;
extern  int_8           DefineProc;     // TRUE if the definition of procedure
                                        // has not ended
extern char             EndDirectiveFound;
extern struct asm_sym   *SegOverride;
extern struct asmfixup  *ModendFixup;   // start address fixup

seg_list                *CurrSeg;       // points to stack of opened segments

int                     in_prologue;

static assume_info      AssumeTable[ASSUME_LAST];

symbol_queue            Tables[TAB_LAST];// tables of definitions
module_info             ModuleInfo;

static seg_list         *ProcStack = NULL;

static char *StartupDosNear[] = {
        " mov     dx,DGROUP",
        " mov     ds,dx",
        " mov     bx,ss",
        " sub     bx,dx",
        " shl     bx,1",
        " shl     bx,1",
        " shl     bx,1",
        " shl     bx,1",
        " cli     ",
        " mov     ss,dx",
        " add     sp,bx",
        " sti     ",
        NULL
};
static char *StartupDosFar[] = {
        " mov     dx,DGROUP",
        " mov     ds,dx",
        NULL
};
static char *ExitOS2[] = { /* mov al, retval  followed by: */
        " xor ah, ah",
        " push 01h",
        " push ax",
        " call far DOSEXIT",
        NULL
};
static char *ExitDos[] = { /* mov al, retval  followed by: */
        " mov     ah,4ch",
        " int     21h",
        NULL
};
static char *RetVal = " mov    al, ";

static char *StartAddr = "`symbol_reserved_for_start_address`";
static char StartupDirectiveFound = FALSE;

#define DEFAULT_CODE_CLASS      "CODE"
#define DEFAULT_CODE_NAME       "_TEXT"
#define DEFAULT_DATA_NAME       "_DATA"
#define SIM_DATA_OFFSET         5        // strlen("_DATA")
#define SIM_FARDATA_OFFSET      8        // strlen("FAR_DATA")

/* code generated by @startup */

static char *SimCodeBegin[2][ SIM_LAST ] = {
    {
        "_TEXT SEGMENT WORD PUBLIC 'CODE' IGNORE",
        "STACK SEGMENT PARA STACK 'STACK' IGNORE",
        "_DATA SEGMENT WORD PUBLIC 'DATA' IGNORE",
        "_BSS  SEGMENT WORD PUBLIC 'BSS' IGNORE",
        "FAR_DATA SEGMENT PARA PRIVATE 'FAR_DATA' IGNORE",
        "FAR_BSS  SEGMENT PARA PRIVATE 'FAR_BSS' IGNORE",
        "CONST SEGMENT WORD PUBLIC 'CONST' READONLY IGNORE"
    },
    {
        "_TEXT SEGMENT DWORD USE32 PUBLIC 'CODE' IGNORE",
        "STACK SEGMENT DWORD USE32 STACK 'STACK' IGNORE",
        "_DATA SEGMENT DWORD USE32 PUBLIC 'DATA' IGNORE",
        "_BSS  SEGMENT DWORD USE32 PUBLIC 'BSS' IGNORE",
        "FAR_DATA SEGMENT DWORD USE32 PRIVATE 'FAR_DATA' IGNORE",
        "FAR_BSS  SEGMENT DWORD USE32 PRIVATE 'FAR_BSS' IGNORE",
        "CONST SEGMENT DWORD USE32 PUBLIC 'CONST' READONLY IGNORE"
    }
};

static char *SimCodeEnd[ SIM_LAST ] = {
        "_TEXT ENDS",
        "STACK ENDS",
        "_DATA ENDS",
        "_BSS  ENDS",
        "FAR_DATA ENDS",
        "FAR_BSS  ENDS",
        "CONST ENDS"
};

/* Code generated by simplified segment definitions */

static last_seg_info    lastseg;        // last opened simplified segment

static char *parm_reg[3][4]= {
    { " AL ", " DL ", " BL ", " CL " },
    { " AX ", " DX ", " BX ", " CX " },
    { " EAX ", " EDX ", " EBX ", " ECX " },
};

enum regsize {
    A_BYTE = 0,
    A_WORD,
    A_DWORD,
};

static asm_tok      const_CodeSize = { T_NUM, NULL, 0 };
static asm_tok      const_DataSize = { T_NUM, NULL, 0 };
static asm_tok      const_Model = { T_NUM, NULL, 0 };
static asm_tok      const_Interface = { T_NUM, NULL, 0 };

static const_info   info_CodeSize = { TRUE, 0, 0, TRUE, &const_CodeSize };
static const_info   info_DataSize = { TRUE, 0, 0, TRUE, &const_DataSize };
static const_info   info_Model = { TRUE, 0, 0, TRUE, &const_Model };
static const_info   info_Interface = { TRUE, 0, 0, TRUE, &const_Interface };
   

#define ROUND_UP( i, r ) (((i)+((r)-1)) & ~((r)-1))

int AddPredefinedConstant( char *name, const_info *info )
/*******************************************************/
{
    dir_node            *dir;

    dir = (dir_node *)AsmGetSymbol( name );
    if( dir == NULL ) {
        dir = dir_insert( name, TAB_CONST );
        if( dir == NULL ) {
            return( ERROR );
        }
    } else if( dir->sym.state == SYM_UNDEFINED ) {
        dir_change( dir, TAB_CONST );
    } else if( dir->sym.state != SYM_CONST ) {
        AsmError( LABEL_ALREADY_DEFINED );
        return( ERROR );
    }
    if( !dir->e.constinfo->predef ) {
        FreeInfo( dir );
        dir->e.constinfo = info;
    }
    return( NOT_ERROR );
}

static int get_watcom_argument_string( char *buffer, uint_8 size, uint_8 *parm_number )
/*************************************************************************************/
/* get the register for parms 0 to 3,
 * using the watcom register parm passing conventions ( A D B C ) */
{
    int parm = *parm_number;

    if( parm > 3 )
        return( FALSE );
    switch( size ) {
    case 1:
        sprintf( buffer, parm_reg[A_BYTE][parm] );
        break;
    case 2:
        sprintf( buffer, parm_reg[A_WORD][parm] );
        break;
    case 4:
        if( Use32 ) {
            sprintf( buffer, parm_reg[A_DWORD][parm] );
            break;
        } else {
            switch( parm ) {
            case 0:
                sprintf( buffer, " [DX AX]" );
                buffer[0] = 0;
                *parm_number = 1; // take up 0 and 1
                break;
            case 1:
            case 2:
                sprintf( buffer, " [CX BX]" );
                buffer[0] = 0;
                *parm_number = 3; // take up 2 and 3
                break;
            default:
                // passed on stack ... it's ok
                return( FALSE );
            }
            return( TRUE );
        }
    case 10:
        AsmErr( TBYTE_NOT_SUPPORTED );
        return( ERROR );
    case 6:
        if( Use32 ) {
            switch( parm ) {
            case 0:
                sprintf( buffer, " [DX EAX]" );
                buffer[0]=0;
                *parm_number = 1; // take up 0 and 1
                break;
            case 1:
            case 2:
                sprintf( buffer, " [CX EBX]" );
                buffer[0]=0;
                *parm_number = 3; // take up 2 and 3
                break;
            default:
                // passed on stack ... it's ok
                return( FALSE );
            }
            return( TRUE );
        }
        // fall through for 16 bit to default
    case 8:
        if( Use32 ) {
            switch( parm ) {
            case 0:
                sprintf( buffer, " [EDX EAX]" );
                buffer[0]=0;
                *parm_number = 1; // take up 0 and 1
                break;
            case 1:
            case 2:
                sprintf( buffer, " [ECX EBX]" );
                buffer[0]=0;
                *parm_number = 3; // take up 2 and 3
                break;
            default:
                // passed on stack ... it's ok
                return( FALSE );
            }
            return( TRUE );
        }
        // fall through for 16 bit to default
    default:
        // something wierd
        AsmError( STRANGE_PARM_TYPE );
        return( ERROR );
    }
    return( TRUE );
}

#ifdef DEBUG_OUT
void heap( char *func ) // for debugging only
/*********************/
{
    switch(_heapchk()) {
    case _HEAPBADNODE:
    case _HEAPBADBEGIN:
    DebugMsg(("Function : %s - ", func ));
        DebugMsg(("ERROR - heap is damaged\n"));
        exit(1);
        break;
    default:
        break;
    }
}
#endif

static int SetAssumeCSCurrSeg( void )
/*************************************/
{
    assume_info     *info;

    info = &(AssumeTable[ ASSUME_CS ]);
    if( CurrSeg == NULL ) {
        info->symbol = NULL;
        info->flat = FALSE;
        info->error = TRUE;
    } else {
        if( CurrSeg->seg->e.seginfo->group != NULL )
            info->symbol = GetGrp( &CurrSeg->seg->sym );
        else
            info->symbol = &CurrSeg->seg->sym;
        info->flat = FALSE;
        info->error = FALSE;
    }
    return( NOT_ERROR );
}

void push( void *stk, void *elt )
/*******************************/
{
    void        **stack = stk;
    stacknode   *node;

    node = AsmAlloc( sizeof( stacknode ));
    node->next = *stack;
    node->elt = elt;
    *stack = node;
}

void *pop( void *stk )
/********************/
{
    void        **stack = stk;
    stacknode   *node;
    void        *elt;

    node = (stacknode *)(*stack);
    *stack = node->next;
    elt = node->elt;
    AsmFree( node );
    return( elt );
}

static int push_seg( dir_node *seg )
/**********************************/
/* Push a segment into the current segment stack */
{
    seg_list    *curr;

    for( curr = CurrSeg; curr; curr = curr->next ) {
        if( curr->seg == seg ) {
            AsmError( BLOCK_NESTING_ERROR );
            return( ERROR );
        }
    }
    push( &CurrSeg, seg );
    SetAssumeCSCurrSeg();
    return( NOT_ERROR );
}

static dir_node *pop_seg( void )
/******************************/
/* Pop a segment out of the current segment stack */
{
    dir_node    *seg;

    /**/myassert( CurrSeg != NULL );
    seg = pop( &CurrSeg );
    SetAssumeCSCurrSeg();
    return( seg );
}

static void push_proc( dir_node *proc )
/*************************************/
{
    push( &ProcStack, proc );
    return;
}

static dir_node *pop_proc( void )
/*******************************/
{
    if( ProcStack == NULL )
        return( NULL );
    return( (dir_node *)pop( &ProcStack ) );
}

static void dir_add( dir_node *new, int tab )
/*******************************************/
{
    /* note: this is only for those above which do NOT return right away */
    /* put the new entry into the queue for its type of symbol */
    if( Tables[tab].head == NULL ) {
        Tables[tab].head = Tables[tab].tail = new;
        new->next = new->prev = NULL;
    } else {
        new->prev = Tables[tab].tail;
        Tables[tab].tail->next = new;
        Tables[tab].tail = new;
        new->next = NULL;
    }
}

static void dir_init( dir_node *dir, int tab )
/********************************************/
/* Change node and insert it into the table specified by tab */
{
    dir->line_num = LineNumber;
    dir->next = dir->prev = NULL;

    switch( tab ) {
    case TAB_SEG:
        dir->sym.state = SYM_SEG;
        dir->e.seginfo = AsmAlloc( sizeof( seg_info ) );
        dir->e.seginfo->idx = 0;
        dir->e.seginfo->group = NULL;
        break;
    case TAB_GRP:
        dir->sym.state = SYM_GRP;
        dir->e.grpinfo = AsmAlloc( sizeof( grp_info ) );
        dir->e.grpinfo->idx = 0;
        dir->e.grpinfo->seglist = NULL;
        dir->e.grpinfo->numseg = 0;
        break;
    case TAB_EXT:
        dir->sym.state = SYM_EXTERNAL;
        dir->e.extinfo = AsmAlloc( sizeof( ext_info ) );
        dir->e.extinfo->idx = 0;
        dir->e.extinfo->use32 = Use32;
        dir->e.extinfo->comm = 0;
        dir->e.extinfo->global = 0;
        break;
    case TAB_COMM:
        dir->sym.state = SYM_EXTERNAL;
        dir->sym.referenced = TRUE;
        dir->e.comminfo = AsmAlloc( sizeof( comm_info ) );
        dir->e.comminfo->idx = 0;
        dir->e.comminfo->use32 = Use32;
        dir->e.comminfo->comm = 1;
        tab = TAB_EXT;
        break;
    case TAB_CONST:
        dir->sym.state = SYM_CONST;
        dir->sym.segment = NULL;
        dir->sym.offset = 0;
        dir->e.constinfo = AsmAlloc( sizeof( const_info ) );
        dir->e.constinfo->data = NULL;
        dir->e.constinfo->count = 0;
        dir->e.constinfo->predef = FALSE;
        return;
    case TAB_PROC:
        dir->sym.state = SYM_PROC;
        dir->e.procinfo = AsmAlloc( sizeof( proc_info ) );
        dir->e.procinfo->regslist = NULL;
        dir->e.procinfo->paralist = NULL;
        dir->e.procinfo->locallist = NULL;
        break;
    case TAB_MACRO:
        dir->sym.state = SYM_MACRO;
        dir->e.macroinfo = AsmAlloc( sizeof( macro_info ) );
        dir->e.macroinfo->parmlist = NULL;
        dir->e.macroinfo->data = NULL;
        dir->e.macroinfo->srcfile = NULL;
        break;
    case TAB_CLASS_LNAME:
        dir->sym.state = SYM_CLASS_LNAME;
        dir->e.lnameinfo = AsmAlloc( sizeof( lname_info ) );
        dir->e.lnameinfo->idx = 0;
        break;
    case TAB_STRUCT:
        dir->sym.state = SYM_STRUCT;
        dir->e.structinfo = AsmAlloc( sizeof( struct_info ) );
        dir->e.structinfo->size = 0;
        dir->e.structinfo->alignment = 0;
        dir->e.structinfo->head = NULL;
        dir->e.structinfo->tail = NULL;
        return;
    case TAB_LIB:
        break;
    default:
        // unknown table
        /**/myassert( 0 );
        break;
    }
    dir_add( dir, tab );
    return;
}

static void dir_to_tail( dir_node *dir, int tab )
/***********************************************/
{
    if( dir->next == NULL )
        return;
    if( dir->prev == NULL ) {
        Tables[tab].head = dir->next;
        dir->next->prev = NULL;
    } else {
        dir->prev->next = dir->next;
        dir->next->prev = dir->prev;
    }
    dir->prev = Tables[tab].tail;
    dir->prev->next = dir;
    Tables[tab].tail = dir;
    dir->next = NULL;
}

static dir_node *RemoveFromTable( dir_node *dir )
/***********************************************/
{
    int tab;

    if( dir->prev != NULL && dir->next != NULL ) {
        dir->next->prev = dir->prev;
        dir->prev->next = dir->next;
    } else {
        switch( dir->sym.state ) {
        case SYM_EXTERNAL:
            tab = TAB_EXT;
            break;
        case SYM_SEG:
            tab = TAB_SEG;
            break;
        case SYM_GRP:
            tab = TAB_GRP;
            break;
        case SYM_PROC:
            tab = TAB_PROC;
            break;
        case SYM_MACRO:
            tab = TAB_MACRO;
            break;
        default:
            return( NULL );
        }
        if( dir->next != NULL ) {
            dir->next->prev = NULL;
            Tables[tab].head = dir->next;
        } else if( dir->prev != NULL ) {
            dir->prev->next = NULL;
            Tables[tab].tail = dir->prev;
        } else {
            Tables[tab].head = Tables[tab].tail = NULL;
        }
    }
    return( dir->next );
}

void dir_to_sym( dir_node *dir )
/******************************/
/* Remove node type/info to be symbol only again */
{
    FreeInfo( dir );
    RemoveFromTable( dir);
    dir->sym.state = SYM_UNDEFINED;
}

void dir_change( dir_node *dir, int tab )
/***************************************/
/* Change node type and insert it into the table specified by tab */
{
    FreeInfo( dir );
    RemoveFromTable( dir);
    dir_init( dir, tab );
}

dir_node *dir_insert( const char *name, int tab )
/***********************************************/
/* Insert a node into the table specified by tab */
{
    dir_node            *new;

    /**/myassert( name != NULL );

    /* don't put class lnames into the symbol table - separate name space */
    new = (dir_node *)AllocDSym( name, ( tab != TAB_CLASS_LNAME ) );
    if( new != NULL )
        dir_init( new, tab );
    return( new );
}

void FreeInfo( dir_node *dir )
/****************************/
{
    int i;

    switch( dir->sym.state ) {
    case SYM_GRP:
        {
            seg_list    *segcurr;
            seg_list    *segnext;

            segcurr = dir->e.grpinfo->seglist;
            if( segcurr != NULL ) {
                for( ;; ) {
                    segnext = segcurr->next;
                    AsmFree( segcurr );
                    if( segnext == NULL )
                        break;
                    segcurr = segnext;
                }
            }
            AsmFree( dir->e.grpinfo );
        }
        break;
    case SYM_SEG:
        AsmFree( dir->e.seginfo );
        break;
    case SYM_EXTERNAL:
        AsmFree( dir->e.extinfo );
        break;
    case SYM_CLASS_LNAME:
        AsmFree( dir->e.lnameinfo );
        break;
    case SYM_CONST:
        if( !dir->e.constinfo->predef ) {
#ifdef DEBUG_OUT
            if( ( dir->e.constinfo->count > 0 )
                && ( dir->e.constinfo->data[0].token != T_NUM ) ) {
                DebugMsg( ( "freeing const(String): %s = ", dir->sym.name ) );
            } else {
                DebugMsg( ( "freeing const(Number): %s = ", dir->sym.name ) );
            }
#endif
            for( i=0; i < dir->e.constinfo->count; i++ ) {
#ifdef DEBUG_OUT
                if( dir->e.constinfo->data[i].token == T_NUM ) {
                    DebugMsg(( "%d ", dir->e.constinfo->data[i].u.value ));
                } else {
                    DebugMsg(( "%s ", dir->e.constinfo->data[i].string_ptr ));
                }
#endif
                AsmFree( dir->e.constinfo->data[i].string_ptr );
            }
            DebugMsg(( "\n" ));
            AsmFree( dir->e.constinfo->data );
            AsmFree( dir->e.constinfo );
        }
        break;
    case SYM_PROC:
        {
            label_list  *labelcurr;
            label_list  *labelnext;
            regs_list   *regcurr;
            regs_list   *regnext;

            labelcurr = dir->e.procinfo->paralist;
            if( labelcurr != NULL ) {
                for( ;; ) {
                    labelnext = labelcurr->next;
                    AsmFree( labelcurr->label );
                    AsmFree( labelcurr->replace );
                    AsmFree( labelcurr );
                    if( labelnext == NULL )
                        break;
                    labelcurr = labelnext;
                }
            }

            labelcurr = dir->e.procinfo->locallist;
            if( labelcurr != NULL ) {
                for( ;; ) {
                    labelnext = labelcurr->next;
                    AsmFree( labelcurr->label );
                    AsmFree( labelcurr->replace );
                    AsmFree( labelcurr );
                    if( labelnext == NULL )
                        break;
                    labelcurr = labelnext;
                }
            }

            regcurr = dir->e.procinfo->regslist;
            if( regcurr != NULL ) {
                for( ;; ) {
                    regnext = regcurr->next;
                    AsmFree( regcurr->reg );
                    AsmFree( regcurr );
                    if( regnext == NULL )
                        break;
                    regcurr = regnext;
                }
            }
            AsmFree( dir->e.procinfo );
        }
        break;
    case SYM_MACRO:
        {
            parm_list       *labelcurr;
            parm_list       *labelnext;
            asmlines        *datacurr;
            asmlines        *datanext;

            /* free the parm list */
            labelcurr = dir->e.macroinfo->parmlist;
            if( labelcurr != NULL ) {
                for( ;; ) {
                    labelnext = labelcurr->next;
                    AsmFree( labelcurr->label );
                    if( labelcurr->replace != NULL ) {
                        AsmFree( labelcurr->replace );
                    }
                    if( labelcurr->def != NULL ) {
                        AsmFree( labelcurr->def );
                    }
                    AsmFree( labelcurr );
                    if( labelnext == NULL )
                        break;
                    labelcurr = labelnext;
                }
            }

            /* free the lines list */
            datacurr = dir->e.macroinfo->data;
            if( datacurr != NULL ) {
                for( ;; ) {
                    datanext = datacurr->next;
                    AsmFree( datacurr->line );
                    AsmFree( datacurr );
                    if( datanext == NULL )
                        break;
                    datacurr = datanext;
                }
            }
            AsmFree( dir->e.macroinfo );
        }
        break;
    case SYM_STRUCT:
        {
            field_list      *ptr;
            field_list      *next;

            for( ptr = dir->e.structinfo->head; ptr != NULL; ptr = next ) {
                AsmFree( ptr->initializer );
                AsmFree( ptr->value );
                next = ptr->next;
                AsmFree( ptr );
            }
            AsmFree( dir->e.structinfo );
        }
        break;
    default:
        break;
    }
}

static struct asm_sym *InsertClassLname( char *name )
/***************************************************/
{
    dir_node            *dir;

    if( strlen( name ) > MAX_LNAME ) {
        AsmError( LNAME_TOO_LONG );
        return( NULL );
    }
    for( dir = Tables[ TAB_CLASS_LNAME ].head; dir != NULL; dir = dir->next ) {
        if( stricmp( dir->sym.name, name ) == 0 ) {
            return( &dir->sym );
        }
    }
    dir = dir_insert( name, TAB_CLASS_LNAME );
    if( dir == NULL )
        return( NULL );
    AddLnameData( dir );
    return( &dir->sym );
}

void wipe_space( char *token )
/****************************/
/* wipe out the spaces at the beginning of a token */
{
    char        *start;

    if( token == NULL )
        return;
    if( strlen( token ) == 0 )
        return;

    for( start = token;; start++ ){
        if( *start != ' ' && *start != '\t' ) {
            break;
        }
    }
    if( start == token )
        return;

    memmove( token, start, strlen( start ) + 1 );
}

static uint checkword( char **token )
/***********************************/
/* wipes out prceding and tailing spaces, and make sure token contains only
   one word */
{
    char        *ptrhead;
    char        *ptrend;

    /* strip the space in the front */
    for( ptrhead = *token; ; ptrhead++ ) {
        if( ( *ptrhead != ' ' ) && ( *ptrhead != '\t' ) ) {
            break;
        }
    }

    /* Then search for the first ending space */
    ptrend = strchr( ptrhead, ' ' );
    if( ptrend == NULL ) {
        ptrend = strchr( ptrhead, '\t' );
    }
    if( ptrend == NULL ) {
        ptrend = strchr( ptrhead, '\n' );
    }

    /* Check if there is any letters following that ending space */
    if( ptrend != NULL ) {
        *ptrend = '\0';
        ptrend++;
        while( *ptrend != '\0' ) {
            if( ( *ptrend != ' ' ) && ( *ptrend != '\t' ) && ( *ptrend != '\n' ) ) {
                return( ERROR );
            }
            ptrend++;
        }
    }

    *token = ptrhead;
    return( NOT_ERROR );
}

uint_32 GetCurrAddr( void )
/*************************/
{
    if( CurrSeg == NULL )
        return( 0 );
    return( CurrSeg->seg->e.seginfo->current_loc );
}

dir_node *GetCurrSeg( void )
/**************************/
{
    if( CurrSeg == NULL )
        return( 0 );
    return( CurrSeg->seg );
}

uint_32 GetCurrSegAlign( void )
/*****************************/
{
    if( CurrSeg == NULL )
        return( 0 );
    switch( CurrSeg->seg->e.seginfo->align ) {
    case ALIGN_ABS: // same as byte aligned ?
    case ALIGN_BYTE:
        return( 1 );
    case ALIGN_WORD:
        return( 2 );
    case ALIGN_DWORD:
        return( 4 );
    case ALIGN_PARA:
        return( 16 );
    case ALIGN_PAGE:
        return( 256 );
    case ALIGN_4KPAGE:
        return( 4096 );
    default:
        return( 0 );
    }
}

uint_32 GetCurrSegStart( void )
/*****************************/
{
    if( CurrSeg == NULL )
        return( 0 );
    /**/myassert( !write_to_file
                || ( CurrSeg->seg->e.seginfo->current_loc - BufSize )
                     == CurrSeg->seg->e.seginfo->start_loc );
    return( CurrSeg->seg->e.seginfo->start_loc );
}

static int GetLangType( int *i )
/******************************/
{
    if( AsmBuffer[*i]->token == T_RES_ID) {
        switch( AsmBuffer[(*i)++]->u.value ) {
        case T_C:
            return( LANG_C );
        case T_BASIC:
            return( LANG_BASIC );
        case T_FORTRAN:
            return( LANG_FORTRAN );
        case T_PASCAL:
            return( LANG_PASCAL );
        case T_WATCOM_C:
            return( LANG_WATCOM_C );
        case T_STDCALL:
            return( LANG_STDCALL );
        case T_SYSCALL:
            return( LANG_SYSCALL );
        default:
            (*i)--;
            break;
        }
    }
    return( ModuleInfo.langtype );
}

int ExtDef( int i, bool glob_def )
/********************************/
{
    char                *token;
    char                *mangle_type = NULL;
    char                *typetoken;
    int                 type;
    memtype             mem_type;
    dir_node            *dir;
    int                 lang_type;

    mangle_type = Check4Mangler( &i );
    for( ; i < Token_Count; i++ ) {

        /* get the symbol language type if present */
        lang_type = GetLangType( &i );

        /* get the symbol name */
        token = AsmBuffer[i++]->string_ptr;

        /* go past the colon */
        if( AsmBuffer[i]->token != T_COLON ) {
            AsmError( COLON_EXPECTED );
            return( ERROR );
        }
        i++;

        typetoken = AsmBuffer[i]->string_ptr;
        type = token_cmp( &typetoken, TOK_EXT_NEAR, TOK_EXT_ABS );

        if( type == ERROR ) {
            if( glob_def || !IsLabelStruct( AsmBuffer[i]->string_ptr ) ) {
                AsmError( INVALID_QUALIFIED_TYPE );
                return( ERROR );
            }
            mem_type = MT_STRUCT;
        } else {
            mem_type = TypeInfo[type].value;
        }
        for( ; i< Token_Count && AsmBuffer[i]->token != T_COMMA; i++ );

        dir = (dir_node *)AsmGetSymbol( token );
        if( dir == NULL ) {
            dir = dir_insert( token, TAB_EXT );
            if( dir == NULL ) {
                return( ERROR );
            }
        } else if( dir->sym.state == SYM_UNDEFINED ) {
            dir_change( dir, TAB_EXT );
        } else if( dir->sym.mem_type != mem_type ) {
            AsmError( EXT_DEF_DIFF );
            return( ERROR );
        } else if( dir->sym.state != SYM_EXTERNAL ) {
            SetMangler( &dir->sym, mangle_type, lang_type );
            if( glob_def && !dir->sym.public ) {
                AddPublicData( dir );
            }
            return( NOT_ERROR );
        } else {
//            SetMangler( &dir->sym, mangle_type, lang_type );
            return( NOT_ERROR );
        }

        if( glob_def ) {
            dir->e.extinfo->global = TRUE;
        } else {
            dir->sym.referenced = TRUE;
        }
        GetSymInfo( &dir->sym );
        dir->sym.offset = 0;
        // FIXME !! symbol can have different type
        dir->sym.mem_type = mem_type;
        SetMangler( &dir->sym, mangle_type, lang_type );
    }
    return( NOT_ERROR );
}

static char *Check4Mangler( int *i )
/**********************************/
{
    char *mangle_type = NULL;

    if( AsmBuffer[*i]->token == T_STRING ) {
        mangle_type = AsmBuffer[*i]->string_ptr;
        (*i)++;
        if( AsmBuffer[*i]->token != T_COMMA ) {
            AsmWarn( 2, EXPECTING_COMMA );
        } else {
            (*i)++;
        }
    }
    return( mangle_type );
}

int PubDef( int i )
/*****************/
{
    char                *mangle_type = NULL;
    char                *token;
    dir_node            *dir;
    int                 lang_type;

    mangle_type = Check4Mangler( &i );
    for( ; i < Token_Count; i+=2 ) {

        /* get the symbol language type if present */
        lang_type = GetLangType( &i );

        /* get the symbol name */
        token = AsmBuffer[i]->string_ptr;

        /* Add the public name */
        if( checkword( &token ) == ERROR ) {
            AsmError( EXPECTING_COMMA );
            return( ERROR );
        }

        dir = (dir_node *)AsmGetSymbol( token );
        if( dir == NULL ) {
            dir = (dir_node *)AllocDSym( token, TRUE );
            AddPublicData( dir );
        } else if( dir->sym.state == SYM_CONST ) {
            /* check if the symbol expands to another symbol,
             * and if so, expand it */
            if( dir->e.constinfo->data[0].token == T_ID ) {
                ExpandTheWorld( i, FALSE, TRUE );
                return( PubDef( i ) );
            }
        }
        SetMangler( &dir->sym, mangle_type, lang_type );
        if( !dir->sym.public ) {
            /* put it into the pub table */
            AddPublicData( dir );
        }
    }
    return( NOT_ERROR );
}

static dir_node *CreateGroup( char *name )
/****************************************/
{
    dir_node    *grp;

    grp = (dir_node *)AsmGetSymbol( name );

    if( grp == NULL ) {
        grp = dir_insert( name, TAB_GRP );
        AddLnameData( grp );
    } else if( grp->sym.state == SYM_UNDEFINED ) {
        dir_change( grp, TAB_GRP );
        AddLnameData( grp );
    } else if( grp->sym.state != SYM_GRP ) {
        AsmErr( SYMBOL_PREVIOUSLY_DEFINED, name );
        grp = NULL;
    }
    return( grp );
}

int GrpDef( int i )
/*****************/
{
    char        *name;
    dir_node    *grp;
    seg_list    *new;
    seg_list    *curr;
    dir_node    *seg;

    if( i < 0 ) {
        AsmError( GRP_NAME_MISSING );
        return( ERROR );
    }

    name = AsmBuffer[i]->string_ptr;
    grp = CreateGroup( name );
    if( grp == NULL )
        return( ERROR );

    for( i+=2;              // skip over the GROUP directive
         i < Token_Count;   // stop at the end of the line
         i+=2 ) {           // skip over commas
        name = AsmBuffer[i]->string_ptr;
        /* Add the segment name */

        if( checkword( &name ) == ERROR ) {
            AsmError( EXPECTING_COMMA );
            return( ERROR );
        }

        seg = (dir_node *)AsmGetSymbol( name );
        if( seg == NULL ) {
            seg = dir_insert( name, TAB_SEG );
        } else if( seg->sym.state == SYM_UNDEFINED ) {
            dir_change( seg, TAB_SEG );
        } else if( seg->sym.state != SYM_SEG ) {
            AsmErr( SYMBOL_PREVIOUSLY_DEFINED, name );
            return( ERROR );
        }
        if( seg->e.seginfo->group == &grp->sym )    // segment is in group already
            continue;
        if( seg->e.seginfo->group != NULL ) {      // segment is in another group
            AsmError( SEGMENT_IN_GROUP );
            return( ERROR );
        }
        /* set the grp index of the segment */
        seg->e.seginfo->group = &grp->sym;

        new = AsmAlloc( sizeof(seg_list) );
        new->seg = seg;
        new->next = NULL;
        grp->e.grpinfo->numseg++;

        /* insert the segment at the end of linked list */
        if( grp->e.grpinfo->seglist == NULL ) {
            grp->e.grpinfo->seglist = new;
        } else {
            curr = grp->e.grpinfo->seglist;
            while( curr->next != NULL ) {
                curr = curr->next;
            }
            curr->next = new;
        }
    }
    return( NOT_ERROR );
}

static int SetUse32( void )
/*************************/
{
    if( CurrSeg == NULL ) {
        Use32 = ModuleInfo.defUse32;
    } else {
        Globals.code_seg = SEGISCODE( CurrSeg );
        Use32 = CurrSeg->seg->e.seginfo->use_32;
        if( Use32 && ( ( Code->info.cpu & P_CPU_MASK ) < P_386 ) ) {
            AsmError( WRONG_CPU_FOR_32BIT_SEGMENT );
            return( ERROR );
        }
    }
    return( NOT_ERROR );
}

int SetUse32Def( bool flag )
/**************************/
{
    if( ( CurrSeg == NULL )               // outside any segments
        && ( ModuleInfo.model == MOD_NONE             // model not defined
            || ModuleInfo.cmdline ) ) {   // model defined on cmdline by -m?
        ModuleInfo.defUse32 = flag;
    }
    return( SetUse32() );
}

int  SetCurrSeg( int i )
/**********************/
{
    char        *name;
    dir_node    *seg;

    name = AsmBuffer[i]->string_ptr;

    switch( AsmBuffer[i+1]->u.value ) {
    case T_SEGMENT:
        FlushCurrSeg();
        seg = (dir_node *)AsmGetSymbol( name );
        /**/ myassert( seg != NULL );
        push_seg( seg );
        break;
    case T_ENDS:
        FlushCurrSeg();
        pop_seg();
        break;
    default:
        break;
    }
    return( SetUse32() );
}

static int token_cmp( char **token, int start, int end )
/******************************************************/
/* compare token against those specified in TypeInfo[ start...end ] */
{
    int         i;
    char        *str;
    char        *tkn;

    str = *token;

    for( i = start; i <= end; i++ ) {
        tkn = TypeInfo[i].string;
        if( tkn == NULL )
            continue;
        if( stricmp( tkn, str ) == 0 ) {
            // type is found
            return( i );
        }
    }
    return( ERROR );        // No type is found
}

static seg_type ClassNameType( char *name )
/*****************************************/
{
    int     slen;
    char    uname[257];

    if( name == NULL )
        return( SEGTYPE_UNDEF );
    if( ModuleInfo.model != MOD_NONE ) {
        if( stricmp( name, Options.code_class ) == 0 ) {
            return( SEGTYPE_ISCODE );
        }
    }
    slen = strlen( name );
    strcpy( uname, name );
    strupr( uname );
    switch( slen ) {
    default:
    case 5:
        // 'CONST'
        if( memcmp( uname, "CONST", 6 ) == 0 )
            return( SEGTYPE_ISDATA );
        // 'STACK'
        if( memcmp( uname, "STACK", 6 ) == 0 )
            return( SEGTYPE_ISDATA );
        // 'DBTYP'
        if( memcmp( uname, "DBTYP", 6 ) == 0 )
            return( SEGTYPE_ISDATA );
        // 'DBSYM'
        if( memcmp( uname, "DBSYM", 6 ) == 0 )
            return( SEGTYPE_ISDATA );
    case 4:
        // 'CODE'
        if( memcmp( uname , "CODE", 5 ) == 0 )
            return( SEGTYPE_ISCODE );
        // '...DATA'
        if( memcmp( uname + slen - 4, "DATA", 4 ) == 0 )
            return( SEGTYPE_ISDATA );
    case 3:
        // '...BSS'
        if( memcmp( uname + slen - 3, "BSS", 3 ) == 0 )
            return( SEGTYPE_ISDATA );
    case 2:
    case 1:
    case 0:
        return( SEGTYPE_UNDEF );
    }
}

static seg_type SegmentNameType( char *name )
/*******************************************/
{
    int     slen;
    char    uname[257];

    slen = strlen( name );
    strcpy( uname, name );
    strupr( uname );
    switch( slen ) {
    default:
    case 5:
        // '..._TEXT'
        if( memcmp( uname + slen - 5, DEFAULT_CODE_NAME, 5 ) == 0 )
            return( SEGTYPE_ISCODE );
        // '..._DATA'
        if( memcmp( uname + slen - 5, DEFAULT_DATA_NAME, 5 ) == 0 )
            return( SEGTYPE_ISDATA );
        // 'CONST'
        if( memcmp( uname, "CONST", 5 ) == 0 )
            return( SEGTYPE_ISDATA );
    case 4:
        // '..._BSS'
        if( memcmp( uname + slen - 4, "_BSS", 4 ) == 0 )
            return( SEGTYPE_ISDATA );
    case 3:
    case 2:
    case 1:
    case 0:
        return( SEGTYPE_UNDEF );
    }
}


int SegDef( int i )
/*****************/
{
    char                *token;
    seg_info            *new;
    seg_info            *old;
    uint                type;       // type of option
    uint                initstate;  // to show if a field is initialized
    dir_node            *seg;
    char                *name;

    if( i < 0 ) {
        AsmError( SEG_NAME_MISSING );
        return( ERROR );
    }

    name = AsmBuffer[i]->string_ptr;

    switch( AsmBuffer[i+1]->u.value ) {
    case T_SEGMENT:
        /* Check to see if the segment is already defined */
        seg = (dir_node *)AsmGetSymbol( name );
        if( seg == NULL ) {
            // segment is not defined
            seg = dir_insert( name, TAB_SEG );
            old = new = seg->e.seginfo;
        } else if ( seg->sym.state == SYM_SEG ) {
            // segment already defined
            old = seg->e.seginfo;
            if( seg->sym.segment == NULL ) {
                // segment was mentioned in a group statement, but not really set up
                new = old;
            } else {
                new = AsmAlloc( sizeof( seg_info ) );
            }
        } else {
            // symbol is different kind, change to segment
            dir_change( seg, TAB_SEG );
            old = new = seg->e.seginfo;
        }

        /* Setting up default values */
        if( new == old ) {
            new->align = ALIGN_PARA;
            new->combine = COMB_INVALID;
            new->use_32 = ModuleInfo.defUse32;
            new->class_name = InsertClassLname( "" );
            new->readonly = FALSE;
            new->iscode = SEGTYPE_UNDEF;
        } else {
            new->readonly = old->readonly;
            new->iscode = old->iscode;
            new->align = old->align;
            new->combine = old->combine;
            if( !old->ignore ) {
                new->use_32 = old->use_32;
            } else {
                new->use_32 = ModuleInfo.defUse32;
            }
            new->class_name = old->class_name;
        }
        new->ignore = FALSE; // always false unless set explicitly
        new->length = 0;

        if( lastseg.stack_size > 0 ) {
            new->length = lastseg.stack_size;
            lastseg.stack_size = 0;
        }

        initstate = 0;
        i += 2;         /* go past segment name and "SEGMENT " */

        for( ; i < Token_Count; i ++ ) {
            if( AsmBuffer[i]->token == T_STRING ) {

                /* the class name - the only token which is of type STRING */
                token = AsmBuffer[i]->string_ptr;
                new->class_name = InsertClassLname( token );
                if( new->class_name == NULL ) {
                    goto error;
                }
                continue;
            }

            /* go through all tokens EXCEPT the class name */
            token = AsmBuffer[i]->string_ptr;

            // look up the type of token
            type = token_cmp( &token, TOK_READONLY, TOK_AT );
            if( type == ERROR ) {
                AsmError( UNDEFINED_SEGMENT_OPTION );
                goto error;
            }

            /* initstate is used to check if any field is already
            initialized */

            if( initstate & TypeInfo[type].init ) {
                AsmError( SEGMENT_PARA_DEFINED ); // initialized already
                goto error;
            } else {
                initstate |= TypeInfo[type].init; // mark it initialized
            }
            switch( type ) {
            case TOK_READONLY:
                new->readonly = TRUE;
                break;
            case TOK_BYTE:
            case TOK_WORD:
            case TOK_DWORD:
            case TOK_PARA:
            case TOK_PAGE:
                new->align = TypeInfo[type].value;
                break;
            case TOK_PRIVATE:
            case TOK_PUBLIC:
            case TOK_STACK:
            case TOK_COMMON:
            case TOK_MEMORY:
                new->combine = TypeInfo[type].value;
                break;
            case TOK_USE16:
            case TOK_USE32:
                new->use_32 = TypeInfo[type].value;
                break;
            case TOK_IGNORE:
                new->ignore = TRUE;
                break;
            case TOK_AT:
                new->align = SEGDEF_ALIGN_ABS;
                ExpandTheWorld( i + 1, FALSE, TRUE);
                if( AsmBuffer[i+1]->token == T_NUM ) {
                    i++;
                    new->abs_frame = AsmBuffer[i]->u.value;
                } else {
                    AsmError( UNDEFINED_SEGMENT_OPTION );
                    goto error;
                }
                break;
            default:
                AsmError( UNDEFINED_SEGMENT_OPTION );
                goto error;
            }
        }
        token = new->class_name->name;
        if( new->iscode != SEGTYPE_ISCODE ) {
            seg_type res;

            res = ClassNameType( token );
            if( res != SEGTYPE_UNDEF ) {
                new->iscode = res;
            } else {
                res = SegmentNameType( name );
                new->iscode = res;
            }
        }

        if( new == old ) {
            /* A new definition */
            new->start_loc = 0;
            new->current_loc = 0;
            seg->sym.segment = &seg->sym;
            seg->sym.offset = 0;
            dir_to_tail( seg, TAB_SEG );
            AddLnameData( seg );
        } else if( !old->ignore && !new->ignore ) {
            /* Check if new definition is different from previous one */
            if( ( old->readonly != new->readonly )
                || ( old->align != new->align )
                || ( old->combine != new->combine )
                || ( old->use_32 != new->use_32 )
                || ( old->class_name != new->class_name ) ) {
                AsmError( SEGDEF_CHANGED );
                goto error;
            } else {
                // definition is the same as before
            }
        } else if( old->ignore ) {
            /* reset to the new values */
            old->align = new->align;
            old->combine = new->combine;
            old->readonly = new->readonly;
            old->iscode = new->iscode;
            old->ignore = new->ignore;
            old->use_32 = new->use_32;
            old->class_name = new->class_name;
        } else {    // new->ignore
            /* keep the old values */
            new->align = old->align;
            new->combine = old->combine;
            new->readonly = old->readonly;
            new->iscode = old->iscode;
            new->ignore = old->ignore;
            new->use_32 = old->use_32;
            new->class_name = old->class_name;
        }
        if( !ModuleInfo.mseg && ( seg->e.seginfo->use_32 != ModuleInfo.use32 ) ) {
            ModuleInfo.mseg = TRUE;
        }
        if( seg->e.seginfo->use_32 ) {
            ModuleInfo.use32 = TRUE;
        }
        if( new != old )
            AsmFree( new );
        if( push_seg( seg ) == ERROR ) {
            return( ERROR );
        }
        break;
    case T_ENDS:
        if( CurrSeg == NULL ) {
            AsmError( SEGMENT_NOT_OPENED );
            return( ERROR );
        }

        seg = (dir_node *)AsmGetSymbol( name );
        if( seg == NULL ) {
            AsmErr( SEG_NOT_DEFINED, name );
            return( ERROR );
        } else if( seg != CurrSeg->seg ) {
            AsmError( BLOCK_NESTING_ERROR );
            return( ERROR );
        }
        pop_seg();
        break;
    default:
        return( ERROR );
    }
    return( SetUse32() );

error:
    if( new != old )
        AsmFree( new );
    return( ERROR );
}

int Include( int i )
/******************/
{
    switch( AsmBuffer[i]->token ) {
    case T_ID:
    case T_STRING:
    case T_PATH:
        return( InputQueueFile( AsmBuffer[i]->string_ptr ) );
    default:
        AsmError( EXPECTED_FILE_NAME );
        return( ERROR );
    }
}

int IncludeLib( int i )
/*********************/
{
    char *name;
    struct asm_sym *sym;

    name = AsmBuffer[i]->string_ptr;
    if( name == NULL ) {
        AsmError( LIBRARY_NAME_MISSING );
        return( ERROR );
    }

    sym = AsmGetSymbol( name );
    if( sym == NULL ) {
        // fixme
        if( dir_insert( name, TAB_LIB ) == NULL ) {
            return( ERROR );
        }
    }
    return( NOT_ERROR );
}

static void input_group( int type, char *name )
/*********************************************/
/* emit any GROUP instruction */
{
    char        buffer[MAX_LINE_LEN];

    if( ModuleInfo.model == MOD_FLAT )
        return;
    strcpy( buffer, "DGROUP GROUP " );
    if( name != NULL ) {
        strcat( buffer, name );
    } else {
        switch( type ) {
        case T_DOT_DATA:
            strcat( buffer, DEFAULT_DATA_NAME );
            break;
        case T_DOT_DATA_UN:
            strcat( buffer, "_BSS" );
            break;
        case T_DOT_CONST:
            strcat( buffer, "CONST" );
            break;
        case T_DOT_STACK:
            strcat( buffer, "STACK" );
            break;
        }
    }
    InputQueueLine( buffer );
}

static void close_lastseg( void )
/*******************************/
/* close the last opened simplified segment */
{
    if( lastseg.seg != SIM_NONE ) {
        lastseg.seg = SIM_NONE;
        if( ( CurrSeg == NULL ) && ( *lastseg.close != '\0' ) ) {
            AsmError( DONT_MIX_SIM_WITH_REAL_SEGDEFS );
            return;
        }
        InputQueueLine( lastseg.close );
    }
}

int Startup( int i )
/******************/
{
    /* handles .STARTUP directive */

    int         count;
    char        buffer[ MAX_LINE_LEN ];

    if( ModuleInfo.model == MOD_NONE ) {
        AsmError( MODEL_IS_NOT_DECLARED );
        return( ERROR );
    }
    ModuleInfo.cmdline = FALSE;

    switch( AsmBuffer[i]->u.value ) {
    case T_DOT_STARTUP:
        count = 0;
        strcpy( buffer, StartAddr );
        strcat( buffer, ":" );
        InputQueueLine( buffer );
        if( ModuleInfo.ostype == OPSYS_DOS ) {
            if( ModuleInfo.distance == STACK_NEAR ) {
                while( StartupDosNear[count] != NULL ) {
                    InputQueueLine( StartupDosNear[count++] );
                }
            } else {
                while( StartupDosFar[count] != NULL ) {
                    InputQueueLine( StartupDosFar[count++] );
                }
            }
        }
        StartupDirectiveFound = TRUE;
        break;
    case T_DOT_EXIT:
        i++;
        if( ( AsmBuffer[i]->string_ptr != NULL )
            && ( *(AsmBuffer[i]->string_ptr) != '\0' ) ) {
            strcpy( buffer, RetVal );
            strcat( buffer, AsmBuffer[i]->string_ptr );
            InputQueueLine( buffer );
        }
        count = 0;
        if( ModuleInfo.ostype == OPSYS_DOS ) {
            while( ExitDos[count] != NULL ) {
                InputQueueLine( ExitDos[count++] );
            }
        } else {
            while( ExitOS2[count] != NULL ) {
                InputQueueLine( ExitOS2[count++] );
            }
        }
        break;
    default:
        break;
    }
    return( NOT_ERROR );
}

static char *get_sim_code_beg( char *buffer, char *name, int bit )
/****************************************************************/
{
    strcpy( buffer, name );
    if( bit == BIT16 ) {
        strcat( buffer, " SEGMENT WORD PUBLIC '" );
    } else {
        strcat( buffer, " SEGMENT DWORD USE32 PUBLIC '" );
    }
    strcat( buffer, Options.code_class );
    strcat( buffer, "' IGNORE");
    return( buffer );
}

static char *get_sim_code_end( char *buffer, char *name )
/*******************************************************/
{
    strcpy( buffer, name );
    strcat( buffer, " ENDS");
    return( buffer );
}

int SimSeg( int i )
/*****************/
/* Handles simplified segment, based on optasm pg. 142-146 */
{
    char        buffer[ MAX_LINE_LEN ];
    int         bit;
    char        *string;
    int         type;
    int         seg;

    if( ModuleInfo.model == MOD_NONE ) {
        AsmError( MODEL_IS_NOT_DECLARED );
        return( ERROR );
    }
    ModuleInfo.cmdline = FALSE;

    if( AsmBuffer[i]->u.value != T_DOT_STACK ) {
        close_lastseg();
    }
    buffer[0] = '\0';
    bit = ( ModuleInfo.defUse32 ) ? BIT32 : BIT16;
    type = AsmBuffer[i]->u.value;
    i++; /* get past the directive token */
    if( i < Token_Count ) {
        string = AsmBuffer[i]->string_ptr;
    } else {
        string = NULL;
    }

    switch( type ) {
    case T_DOT_CODE:
        if( string == NULL )
            string = Options.text_seg;
        InputQueueLine( get_sim_code_beg( buffer, string, bit ) );
        get_sim_code_end( lastseg.close, string );
        lastseg.seg = SIM_CODE;

        if( ModuleInfo.model == MOD_TINY ) {
            InputQueueLine( "ASSUME CS:DGROUP" );
        } else {
            strcpy( buffer, "ASSUME CS:" );
            strcat( buffer, string );
            InputQueueLine( buffer );
        }
        break;
    case T_DOT_STACK:
        InputQueueLine( SimCodeBegin[bit][SIM_STACK] );
        input_group( type, NULL );
        InputQueueLine( SimCodeEnd[SIM_STACK] );
        if( i < Token_Count ) {
            if( AsmBuffer[i]->token != T_NUM ) {
                AsmError( CONSTANT_EXPECTED );
                return( ERROR );
            } else {
                lastseg.stack_size = AsmBuffer[i]->u.value;
            }
        } else {
            lastseg.stack_size = DEFAULT_STACK_SIZE;
        }
        break;
    case T_DOT_DATA:
    case T_DOT_DATA_UN:             // .data?
    case T_DOT_CONST:
        if( type == T_DOT_DATA ) {
            if( string == NULL )
                string = Options.data_seg;
            seg = SIM_DATA;
        } else if( type == T_DOT_DATA_UN ) {
            seg = SIM_DATA_UN;
            string = NULL;
        } else {
            seg = SIM_CONST;
            string = NULL;
        }

        if( string != NULL ) {
            strcpy( buffer, string );
            strcat( buffer, SimCodeBegin[bit][seg] + SIM_DATA_OFFSET );
        } else {
            strcpy( buffer, SimCodeBegin[bit][seg] );
        }
        InputQueueLine( buffer );
        InputQueueLine( "ASSUME CS:ERROR" );
        input_group( type, string );
        lastseg.seg = seg;
        if( string != NULL ) {
            strcpy( lastseg.close, string );
            strcat( lastseg.close, SimCodeEnd[seg] + SIM_DATA_OFFSET );
        } else {
            strcpy( lastseg.close, SimCodeEnd[seg] );
        }
        break;
    case T_DOT_FARDATA:
    case T_DOT_FARDATA_UN:  // .fardata?
        seg = ( type == T_DOT_FARDATA ) ? SIM_FARDATA : SIM_FARDATA_UN;
        if( string != NULL ) {
            strcpy( buffer, string );
            strcat( buffer, SimCodeBegin[bit][seg] + SIM_FARDATA_OFFSET );
        } else {
            strcpy( buffer, SimCodeBegin[bit][seg] );
        }
        InputQueueLine( buffer );
        InputQueueLine( "ASSUME CS:ERROR" );
        if( string != NULL ) {
            strcpy( lastseg.close, string );
            strcat( lastseg.close, SimCodeEnd[seg] + SIM_FARDATA_OFFSET );
        } else {
            strcpy( lastseg.close, SimCodeEnd[seg] );
        }
        lastseg.seg = seg;
        break;
    default:
        /**/myassert( 0 );
        break;
    }
    return( NOT_ERROR );
}

static void module_prologue( int type )
/*************************************/
/* Generates codes for .MODEL; based on optasm pg.142-146 */
{
    int         bit;
    char        buffer[ MAX_LINE_LEN ];

    bit = ( ModuleInfo.defUse32 ) ? BIT32 : BIT16;

    /* Generates codes for code segment */
    InputQueueLine( get_sim_code_beg( buffer, Options.text_seg, bit ) );
    InputQueueLine( get_sim_code_end( buffer, Options.text_seg ) );

    /* Generates codes for data segment */
    InputQueueLine( SimCodeBegin[bit][SIM_DATA] );
    InputQueueLine( SimCodeEnd[SIM_DATA] );

    if( type != MOD_FLAT ) {
        /* Generates codes for grouping */
        strcpy( buffer, "DGROUP GROUP " );
        if( type == MOD_TINY ) {
            strcat( buffer, Options.text_seg );
            strcat( buffer, "," );
        }
        strcat( buffer, Options.data_seg );
        InputQueueLine( buffer );
    }

    ModelAssumeInit();

// FIXME !! 
// it is temporary fix for PROC parameters
// but PTR operator should be resolved globaly
    /* Fix up PTR size */
    switch( type ) {
    case MOD_COMPACT:
    case MOD_LARGE:
    case MOD_HUGE:
    case MOD_FLAT:
        TypeInfo[TOK_EXT_PTR].value = MT_DWORD;
        break;
    }

    /* Set @CodeSize */
    switch( type ) {
    case MOD_MEDIUM:
    case MOD_LARGE:
    case MOD_HUGE:
        const_CodeSize.u.value = 1;
        break;
    default:
        const_CodeSize.u.value = 0;
        break;
    }
    AddPredefinedConstant( "@CodeSize", &info_CodeSize );

    /* Set @DataSize */
    switch( type ) {
    case MOD_COMPACT:
    case MOD_LARGE:
        const_DataSize.u.value = 1;
        break;
    case MOD_HUGE:
        const_DataSize.u.value = 2;
        break;
    default:
        const_DataSize.u.value = 0;
        break;
    }
    AddPredefinedConstant( "@DataSize", &info_DataSize );

    /* Set @Model */
    const_Model.u.value = ModuleInfo.model;
    AddPredefinedConstant( "@Model", &info_Model );

    /* Set @Interface */
    const_Interface.u.value = ModuleInfo.langtype;
    AddPredefinedConstant( "@Interface", &info_Interface );
}

void ModuleInit( void )
/*********************/
{
    ModuleInfo.model = MOD_NONE;
    ModuleInfo.distance = STACK_NONE;
    ModuleInfo.langtype = LANG_NONE;
    ModuleInfo.ostype = OPSYS_DOS;
    ModuleInfo.use32 = FALSE;
    ModuleInfo.defUse32 = FALSE;
    ModuleInfo.cmdline = FALSE;
    ModuleInfo.mseg = FALSE;
    ModuleInfo.flat_grp = NULL;
    *ModuleInfo.name = 0;
    // add source file to autodependency list
    ModuleInfo.srcfile = AddFlist( AsmFiles.fname[ASM] );
}

static void get_module_name( void )
/*********************************/
{
    char dummy[_MAX_EXT];
    char        *p;

    /**/myassert( AsmFiles.fname[ASM] != NULL );
    _splitpath( AsmFiles.fname[ASM], NULL, NULL, ModuleInfo.name, dummy );
    for( p = ModuleInfo.name; *p != '\0'; ++p ) {
        if( !( isalnum( *p ) || ( *p == '_' ) || ( *p == '$' )
            || ( *p == '@' ) || ( *p == '?') ) ) {
            /* it's not a legal character for a symbol name */
            *p = '_';
        }
    }
    /* first character can't be a number either */
    if( isdigit( ModuleInfo.name[0] ) ) {
        ModuleInfo.name[0] = '_';
    }
}

static void set_def_seg_name( void )
/***********************************/
{
    int len;

    /* set Options.code_class */
    if( Options.code_class == NULL ) {
        Options.code_class = AsmAlloc( sizeof( DEFAULT_CODE_CLASS ) + 1 );
        strcpy( Options.code_class, DEFAULT_CODE_CLASS );
    }
    /* set Options.text_seg based on module name */
    if( Options.text_seg == NULL ) {
        switch( ModuleInfo.model ) {
        case MOD_MEDIUM:
        case MOD_LARGE:
        case MOD_HUGE:
            len = strlen( ModuleInfo.name ) + sizeof( DEFAULT_CODE_NAME ) + 1;
            Options.text_seg = AsmAlloc( len );
            strcpy( Options.text_seg, ModuleInfo.name );
            strcat( Options.text_seg, DEFAULT_CODE_NAME );
            break;
        default:
            Options.text_seg = AsmAlloc( sizeof( DEFAULT_CODE_NAME ) + 1 );
            strcpy( Options.text_seg, DEFAULT_CODE_NAME );
            break;
        }
    }
    /* set Options.data_seg */
    if( Options.data_seg == NULL ) {
        Options.data_seg = AsmAlloc( sizeof( DEFAULT_DATA_NAME ) + 1 );
        strcpy( Options.data_seg, DEFAULT_DATA_NAME );
    }
    return;
}

void DefFlatGroup( void )
/***********************/
{
    if( MAGIC_FLAT_GROUP == NULL ) {
        MAGIC_FLAT_GROUP = &CreateGroup( "FLAT" )->sym;
    }
}

int Model( int i )
/****************/
{
    char        *token;
    int         initstate = 0;
    uint        type;           // type of option

    if( Parse_Pass != PASS_1 ) {
        ModelAssumeInit();
        return( NOT_ERROR );
    }

    if( ModuleInfo.model != MOD_NONE && !ModuleInfo.cmdline ) {
        AsmError( MODEL_DECLARED_ALREADY );
        return( ERROR );
    }
    ModuleInfo.cmdline = FALSE;

    get_module_name();

    for( i++; i < Token_Count; i++ ) {

        token = AsmBuffer[i]->string_ptr;
        wipe_space( token );
        /* Add the public name */

        // look up the type of token
        type = token_cmp( &token, TOK_TINY, TOK_FARSTACK );
        if( type == ERROR ) {
            type = token_cmp( &token, TOK_PROC_BASIC, TOK_PROC_SYSCALL );
            if( type == ERROR ) {
                type = token_cmp( &token, TOK_OS_OS2, TOK_OS_DOS );
                if( type == ERROR ) {
                    AsmError( UNDEFINED_MODEL_OPTION );
                    return( ERROR );
                }
            }
        }
        MakeConstantUnderscored( AsmBuffer[i]->u.value );

        if( initstate & TypeInfo[type].init ) {
            AsmError( MODEL_PARA_DEFINED ); // initialized already
            return( ERROR );
        } else {
            initstate |= TypeInfo[type].init; // mark it initialized
        }
        switch( type ) {
        case TOK_FLAT:
            DefFlatGroup();
            SetUse32Def( TRUE );
            // fall through
        case TOK_TINY:
        case TOK_SMALL:
        case TOK_COMPACT:
        case TOK_MEDIUM:
        case TOK_LARGE:
        case TOK_HUGE:
            ModuleInfo.model = TypeInfo[type].value;
            set_def_seg_name();
            break;
        case TOK_NEARSTACK:
        case TOK_FARSTACK:
            ModuleInfo.distance = TypeInfo[type].value;
            break;
        case TOK_PROC_BASIC:
        case TOK_PROC_FORTRAN:
        case TOK_PROC_PASCAL:
        case TOK_PROC_C:
        case TOK_PROC_WATCOM_C:
        case TOK_PROC_STDCALL:
        case TOK_PROC_SYSCALL:
            ModuleInfo.langtype = TypeInfo[type].value;
            break;
        case TOK_OS_DOS:
        case TOK_OS_OS2:
            ModuleInfo.ostype = TypeInfo[type].value;
            break;
        }
        i++;

        /* go past comma */
        if( ( i < Token_Count ) && ( AsmBuffer[i]->token != T_COMMA ) ) {
            AsmError( EXPECTING_COMMA );
            return( ERROR );
        }
    }

    if( ( initstate & INIT_MEMORY ) == 0 ) {
        AsmError( MEMORY_NOT_FOUND );
        return( ERROR );
    }

    module_prologue( ModuleInfo.model );
    lastseg.seg = SIM_NONE;
    lastseg.stack_size = 0;
    ModuleInfo.cmdline = (LineNumber == 0);
    return(NOT_ERROR);
}

void AssumeInit( void )
/*********************/
{
    enum assume_reg reg;

    for( reg = ASSUME_FIRST; reg < ASSUME_LAST; reg++ ) {
        AssumeTable[reg].symbol = NULL;
        AssumeTable[reg].error = FALSE;
        AssumeTable[reg].flat = FALSE;
    }
}

static void ModelAssumeInit( void )
/**********************************/
{
    char        buffer[ MAX_LINE_LEN ];

    /* Generates codes for assume */
    switch( ModuleInfo.model ) {
    case MOD_FLAT:
        InputQueueLine( "ASSUME CS:FLAT,DS:FLAT,SS:FLAT,ES:FLAT,FS:ERROR,GS:ERROR");
        break;
    case MOD_TINY:
        InputQueueLine( "ASSUME CS:DGROUP, DS:DGROUP, ES:DGROUP, SS:DGROUP" );
        break;
    case MOD_SMALL:
    case MOD_COMPACT:
    case MOD_MEDIUM:
    case MOD_LARGE:
    case MOD_HUGE:
        strcpy( buffer, "ASSUME CS:" );
        strcat( buffer, Options.text_seg );
        strcat( buffer, ", DS:DGROUP, SS:DGROUP" );
        InputQueueLine( buffer );
        break;
    }
}

int SetAssume( int i )
/********************/
/* Handles ASSUME statement */
{
    char            *token;
    char            *segloc;
    int             reg;
    assume_info     *info;
    struct asm_sym  *sym;


    for( i++; i < Token_Count; i++ ) {
        if( ( AsmBuffer[i]->token == T_RES_ID )
            && ( AsmBuffer[i]->u.value == T_NOTHING ) ) {
            AssumeInit();
            continue;
        }

        token = AsmBuffer[i]->string_ptr;
        wipe_space( token );

        i++;

        if( AsmBuffer[i]->token != T_COLON ) {
            AsmError( COLON_EXPECTED );
            return( ERROR );
        }
        i++;

        if( ( AsmBuffer[i]->token == T_UNARY_OPERATOR )
            && ( AsmBuffer[i]->u.value == T_SEG ) ) {
            i++;
        }

        segloc = AsmBuffer[i]->string_ptr;
        i++;
        if( *segloc == '\0' ) {
            AsmError( SEGLOCATION_EXPECTED );
            return( ERROR );
        }

        /*---- Now store the information ----*/

        reg = token_cmp( &token, TOK_DS, TOK_CS );
        if( reg == ERROR ) {
            AsmError( INVALID_REGISTER );
            return( ERROR );
        }
        if( ( ( Code->info.cpu & P_CPU_MASK ) < P_386 )
            && ( ( reg == TOK_FS ) || ( reg == TOK_GS ) ) ) {
            AsmError( INVALID_REGISTER );
            return( ERROR );
        }

        info = &(AssumeTable[TypeInfo[reg].value]);

        if( token_cmp( &segloc, TOK_ERROR, TOK_ERROR ) != ERROR ) {
            info->error = TRUE;
            info->flat = FALSE;
            info->symbol = NULL;
        } else if( token_cmp( &segloc, TOK_FLAT, TOK_FLAT ) != ERROR ) {
            DefFlatGroup();
            info->flat = TRUE;
            info->error = FALSE;
            info->symbol = NULL;
        } else if( token_cmp( &segloc, TOK_NOTHING, TOK_NOTHING ) != ERROR ) {
            info->flat = FALSE;
            info->error = FALSE;
            info->symbol = NULL;
        } else {
            sym = AsmLookup( segloc );
            if( sym == NULL )
                return( ERROR );
            if ( ( Parse_Pass != PASS_1 ) && ( sym->state == SYM_UNDEFINED ) ) {
                AsmErr( SYMBOL_NOT_DEFINED, segloc );
                return( ERROR );
            }
            info->symbol = sym;
            info->flat = FALSE;
            info->error = FALSE;
        }

        /* go past comma */
        if( ( i < Token_Count ) && ( AsmBuffer[i]->token != T_COMMA ) ) {
            AsmError( EXPECTING_COMMA );
            return( ERROR );
        }
    }
    return( NOT_ERROR );
}

extern struct asm_sym *GetGrp( struct asm_sym *sym )
/**************************************************/
/* get ptr to sym's group sym */
{
    dir_node            *curr;

    curr = GetSeg( sym );
    if( curr != NULL )
        return( curr->e.seginfo->group );
    return( NULL );
}

int SymIs32( struct asm_sym *sym )
/********************************/
/* get sym's segment size */
{
    dir_node            *curr;

    curr = GetSeg( sym );
    if( curr == NULL ) {
        if( sym->state == SYM_EXTERNAL ) {
            if( ModuleInfo.mseg ) {
                curr = (dir_node *)sym;
                return( curr->e.extinfo->use32);
            } else {
                return( ModuleInfo.use32 );
            }
        }
    } else {
        return( curr->e.seginfo->use_32 );
    }
    return( 0 );
}

int FixOverride( int index )
/**************************/
/* Fix segment or group override */
{
    struct asm_sym      *sym;

    sym = AsmLookup( AsmBuffer[index]->string_ptr );
    /**/myassert( sym != NULL );
    if( sym->state == SYM_GRP ) {
        SegOverride = sym;
        return( NOT_ERROR );
    } else if( sym->state == SYM_SEG ) {
        SegOverride = sym;
        return( NOT_ERROR );
    }
    AsmError( SYNTAX_ERROR );
    return( ERROR );
}

static enum assume_reg search_assume( struct asm_sym *sym,
                         enum assume_reg def, int override )
/**********************************************************/
{
    if( sym == NULL )
        return( ASSUME_NOTHING );
    if( def != ASSUME_NOTHING ) {
        if( AssumeTable[def].symbol != NULL ) {
            if( AssumeTable[def].symbol == sym )
                return( def );
            if( !override && ( AssumeTable[def].symbol == GetGrp( sym ) ) ) {
                return( def );
            }
        }
    }
    for( def = ASSUME_FIRST; def < ASSUME_LAST; def++ ) {
        if( AssumeTable[def].symbol == sym ) {
            return( def );
        }
    }
    if( override )
        return( ASSUME_NOTHING );

    for( def = ASSUME_FIRST; def < ASSUME_LAST; def++ ) {
        if( AssumeTable[def].symbol == NULL )
            continue;
        if( AssumeTable[def].symbol == GetGrp( sym ) ) {
            return( def );
        }
    }

    return( ASSUME_NOTHING );
}

int Use32Assume( enum assume_reg prefix )
/***************************************/
{
    dir_node        *dir;
    seg_list        *seg_l;
    struct asm_sym  *sym_assume;

    if( AssumeTable[prefix].flat )
        return( 1 );
    sym_assume = AssumeTable[prefix].symbol;
    if( sym_assume == NULL )
        return( EMPTY );
    if( sym_assume->state == SYM_SEG ) {
        dir = (dir_node *)sym_assume;
        return( dir->e.seginfo->use_32 );
    } else if( sym_assume->state == SYM_GRP ) {
        dir = (dir_node *)sym_assume;
        seg_l = dir->e.grpinfo->seglist;
        dir = seg_l->seg;
        return( dir->e.seginfo->use_32 );
    }
    return( EMPTY );
}

enum assume_reg GetPrefixAssume( struct asm_sym *sym, enum assume_reg prefix )
/****************************************************************************/
{
    struct asm_sym  *sym_assume;

    if( Parse_Pass == PASS_1 )
        return( prefix );

    if( AssumeTable[prefix].flat ) {
        Frame = MAGIC_FLAT_GROUP;
        return( prefix );
    }
    sym_assume = AssumeTable[prefix].symbol;
    if( sym_assume == NULL ) {
        if( sym->state == SYM_EXTERNAL ) {
#if 0 //NYI: Don't know what's going on here
            type = GetCurrGrp();
            if( type != 0 ) {
                Frame = FRAME_GRP;
            } else {
                type = GetSegIdx( GetCurrSeg() );
                /**/myassert( type != 0 );
                Frame = FRAME_SEG;
            }
            Frame_Datum = type;
#endif
            return( prefix );
        } else {
            return( ASSUME_NOTHING );
        }
    }

    if( sym_assume->state == SYM_SEG ) {
        Frame = sym_assume->segment;
    } else if( sym_assume->state == SYM_GRP ) {
        Frame = sym_assume;
    }
    if( ( sym->segment == sym_assume )
        || ( GetGrp( sym ) == sym_assume )
        || ( sym->state == SYM_EXTERNAL ) ) {
        return( prefix );
    } else {
        return( ASSUME_NOTHING );
    }
}

enum assume_reg GetAssume( struct asm_sym *sym, enum assume_reg def )
/*******************************************************************/
{
    enum assume_reg  reg;

    if( ( def != ASSUME_NOTHING ) && AssumeTable[def].flat ) {
        Frame = MAGIC_FLAT_GROUP;
        return( def );
    }
    if( SegOverride != NULL ) {
        reg = search_assume( SegOverride, def, 1 );
    } else {
        reg = search_assume( sym->segment, def, 0 );
    }
    if( reg == ASSUME_NOTHING ) {
        if( sym->state == SYM_EXTERNAL && sym->segment == NULL ) {
            reg = def;
        }
    }
    if( reg != ASSUME_NOTHING ) {
        if( AssumeTable[reg].symbol == NULL ) {
        } else if( AssumeTable[reg].symbol->state == SYM_SEG ) {
            Frame = AssumeTable[reg].symbol;
        } else {
            Frame = AssumeTable[reg].symbol;
        }
        return( reg );
    }
    return( ASSUME_NOTHING );
}

int ModuleEnd( int count )
/************************/
{
    if( lastseg.seg != SIM_NONE ) {
        close_lastseg();
        InputQueueLine( CurrString );
        return( NOT_ERROR );
    }

    if( Parse_Pass == PASS_1 ) {
        EndDirectiveFound = TRUE;
        return( NOT_ERROR );
    }

    if( StartupDirectiveFound ) {
        if( count == 2 ) {
            AsmError( SYNTAX_ERROR );
            return( ERROR );
        }
        strcat( CurrString, " " );
        strcat( CurrString, StartAddr );
        InputQueueLine( CurrString );
        StartupDirectiveFound = FALSE;
        return( NOT_ERROR );
    }

    EndDirectiveFound = TRUE;

    if( count == 1 ) {
        return( NOT_ERROR );
    }

    if( AsmBuffer[1]->token != T_ID ) {
        AsmError( INVALID_START_ADDRESS );
        return( ERROR );
    }
    ModendFixup = InsFixups[ OPND1 ];
    return( NOT_ERROR );
}

static int find_size( int type )
/******************************/
{
    int         ptr_size;

    switch( type ) {
    case TOK_EXT_BYTE:
    case TOK_EXT_SBYTE:
        return( 1 );
    case TOK_EXT_WORD:
    case TOK_EXT_SWORD:
        return( 2 );
    case TOK_EXT_DWORD:
    case TOK_EXT_SDWORD:
        return( 4 );
    case TOK_EXT_FWORD:
    case TOK_EXT_PWORD:
        return( 6 );
    case TOK_EXT_QWORD:
        return( 8 );
    case TOK_EXT_TBYTE:
        return( 10 );
    case TOK_EXT_OWORD:
        return( 16 );
    case TOK_EXT_PTR:
        /* first determine offset size */
        if( (Code->info.cpu&P_CPU_MASK) >= P_386 ) {
            ptr_size = 4;
        } else {
            ptr_size = 2;
        }
        if( (ModuleInfo.model == MOD_COMPACT)
         || (ModuleInfo.model == MOD_LARGE)
         || (ModuleInfo.model == MOD_HUGE) ) {
            ptr_size += 2;      /* add segment for far data pointers */
        }
        return( ptr_size );
    case TOK_PROC_VARARG:
        return( 0 );
    default:
        return( ERROR );
    }
}

static void size_override( char *buffer, int size )
/*************************************************/
{
    switch( size ) {
    default:
    case 0:
        buffer[0] = '\0';
        break;
    case 1:
        strcpy( buffer, " byte ptr " );
        break;
    case 2:
        strcpy( buffer, " word ptr " );
        break;
    case 4:
        strcpy( buffer, " dword ptr " );
        break;
    case 6:
        strcpy( buffer, " fword ptr " );
        break;
    case 8:
        strcpy( buffer, " qword ptr " );
        break;
    case 10:
        strcpy( buffer, " tbyte ptr " );
        break;
    case 16:
        strcpy( buffer, " oword ptr " );
        break;
    }
}

int LocalDef( int i )
/*******************/
{
    char        *string;
    int         type;
    label_list  *local;
    label_list  *curr;
    proc_info   *info;
    struct asm_sym      *sym;

/*

    LOCAL symbol[,symbol]...

    symbol:
          name [[count]] [:[type]]
    count:
          number of array elements, default is 1
    type:
          one of BYTE,SBYTE,WORD,SWORD,DWORD,SDWORD,PWORD,FWORD,TBYTE, default is WORD

 */

    if( DefineProc == FALSE ) {
        AsmError( LOCAL_VAR_MUST_FOLLOW_PROC );
        return( ERROR );
    }

    /**/myassert( CurrProc != NULL );

    info = CurrProc->e.procinfo;

    for( i++; i< Token_Count; i++ ) {
        string = AsmBuffer[i]->string_ptr;

        if( AsmBuffer[i]->token != T_ID ) {
            AsmError( LABEL_IS_EXPECTED );
            return( ERROR );
        }

        sym = AsmLookup( AsmBuffer[i]->string_ptr );
        if( sym == NULL )
            return( ERROR );

        if( sym->state != SYM_UNDEFINED ) {
            AsmErr( SYMBOL_PREVIOUSLY_DEFINED, sym->name );
            return( ERROR );
        } else {
            sym->state = SYM_INTERNAL;
            sym->mem_type = MT_WORD;
        }

        local = AsmAlloc( sizeof( label_list ) );
        local->label = AsmAlloc( strlen( AsmBuffer[i]->string_ptr ) + 1 );
        strcpy( local->label, AsmBuffer[i++]->string_ptr );
        local->size = LOCAL_DEFAULT_SIZE;
        local->replace = NULL;
        local->factor = 1;
        local->next = NULL;

        if( i < Token_Count ) {
            if( AsmBuffer[i]->token == T_OP_SQ_BRACKET ) {
                i++;
                if( ( AsmBuffer[i]->token != T_NUM ) || ( i >= Token_Count ) ) {
                    AsmError( SYNTAX_ERROR );
                    return( ERROR );
                }
                local->factor = AsmBuffer[i++]->u.value;
                if( ( AsmBuffer[i]->token != T_CL_SQ_BRACKET ) || ( i >= Token_Count ) ) {
                    AsmError( EXPECTED_CL_SQ_BRACKET );
                    return( ERROR );
                }
                i++;
            }
        }

        if( i < Token_Count ) {
            if( AsmBuffer[i]->token != T_COLON ) {
                AsmError( COLON_EXPECTED );
                return( ERROR );
            }
            i++;

            type = token_cmp( &(AsmBuffer[i]->string_ptr), TOK_EXT_BYTE,
                                TOK_EXT_TBYTE );
            if( type == ERROR ) {
                AsmError( INVALID_QUALIFIED_TYPE );
                return( ERROR );
            }
            sym->mem_type = TypeInfo[type].value;
            local->size = find_size( type );
        }

        info->localsize += ( local->size * local->factor );

        if( info->locallist == NULL ) {
            info->locallist = local;
        } else {
            for( curr = info->locallist;; curr = curr->next ) {
                if( curr->next == NULL ) {
                    break;
                }
            }
            curr->next = local;
        }

        /* go past comma */
        i++;
        if( ( i < Token_Count ) && ( AsmBuffer[i]->token != T_COMMA ) ) {
            AsmError( EXPECTING_COMMA );
            return( ERROR );
        }

    }
    return( NOT_ERROR );
}

static int proc_exam( dir_node *proc, int i )
/*******************************************/
{
    char            *token;
    char            *typetoken;
    int_8           minimum;        // Minimum value of the type of token to be read
    int_8           finish;
    proc_info       *info;
    regs_list       *regist;
    regs_list       *temp_regist;
    label_list      *paranode;
    label_list      *paracurr;
    int             type;
    struct asm_sym  *param;

    info = proc->e.procinfo;

    minimum = TOK_PROC_FAR;
    finish = FALSE;
    proc->sym.langtype = ModuleInfo.langtype;

    // fixme ... we need error checking here --- for nested procs

    /* Obtain all the default value */

    info->mem_type = IS_PROC_FAR() ? MT_FAR : MT_NEAR;
    info->parasize = 0;
    info->localsize = 0;
    info->export = FALSE;
    info->is_vararg = FALSE;
    info->pe_type = ( ( Code->info.cpu & P_CPU_MASK ) == P_286 ) || ( ( Code->info.cpu & P_CPU_MASK ) == P_386 );
    SetMangler( &proc->sym, NULL, LANG_NONE );

    /* Parse the definition line, except the parameters */
    for( i++; i < Token_Count && AsmBuffer[i]->token != T_COMMA; i++ ) {
        token = AsmBuffer[i]->string_ptr;
        if( AsmBuffer[i]->token == T_STRING ) {
            /* name mangling */
            SetMangler( &proc->sym, token, LANG_NONE );
            continue;
        }

        type = token_cmp( &token, TOK_PROC_FAR, TOK_PROC_USES );
        if( type == ERROR )
            break;
        if( type < minimum ) {
            AsmError( SYNTAX_ERROR );
            return( ERROR );
        }
        switch( type ) {
        case TOK_PROC_FAR:
        case TOK_PROC_NEAR:
            info->mem_type = TypeInfo[type].value;
            minimum = TOK_PROC_BASIC;
            break;
        case TOK_PROC_BASIC:
        case TOK_PROC_FORTRAN:
        case TOK_PROC_PASCAL:
        case TOK_PROC_C:
        case TOK_PROC_WATCOM_C:
        case TOK_PROC_STDCALL:
        case TOK_PROC_SYSCALL:
            proc->sym.langtype = TypeInfo[type].value;
            minimum = TOK_PROC_PRIVATE;
            break;
        case TOK_PROC_EXPORT:
            info->export = TRUE;
            // fall through
        case TOK_PROC_PUBLIC:
            if( !proc->sym.public ) {
                AddPublicData( proc );
            }
            // fall through
        case TOK_PROC_PRIVATE:
            minimum = TOK_PROC_USES;
            break;
        case TOK_PROC_USES:
            for( i++; ( i < Token_Count ) && ( AsmBuffer[i]->token != T_COMMA ); i++ ) {
                token = AsmBuffer[i]->string_ptr;
                regist = AsmAlloc( sizeof( regs_list ));
                regist->next = NULL;
                regist->reg = AsmAlloc( strlen(token) + 1 );
                strcpy( regist->reg, token );
                if( info->regslist == NULL ) {
                    info->regslist = regist;
                } else {
                    for( temp_regist = info->regslist;;
                            temp_regist = temp_regist->next ) {
                        if( temp_regist->next == NULL ) {
                            break;
                        }
                    }
                    temp_regist->next = regist;
                }
            }
            if( AsmBuffer[i]->token == T_COMMA )
                i--;
            break;
        default:
            goto parms;
        }
    }

parms:
    CurrProc = proc;
    DefineProc = TRUE;
    proc->sym.mem_type = info->mem_type;

    if( i >= Token_Count ) {
        return( NOT_ERROR );
    } else if( proc->sym.langtype == LANG_NONE ) {
        AsmError( LANG_MUST_BE_SPECIFIED );
        return( ERROR );
    } else if( AsmBuffer[i]->token == T_COMMA ) {
        i++;
    }

    /* now parse parms */

    for( ; i < Token_Count; i++ ) {
        /* read symbol */
        token = AsmBuffer[i++]->string_ptr;

        /* read colon */
        if( AsmBuffer[i]->token != T_COLON ) {
            AsmError( COLON_EXPECTED );
            return( ERROR );
        }
        i++;

        /* now read qualified type */
        typetoken = AsmBuffer[i]->string_ptr;

        type = token_cmp( &typetoken, TOK_EXT_BYTE, TOK_EXT_TBYTE );

        if( type == ERROR ) {
            type = token_cmp( &typetoken, TOK_PROC_VARARG, TOK_PROC_VARARG );
            if( type == ERROR ) {
                AsmError( INVALID_QUALIFIED_TYPE );
                return( ERROR );
            } else {
                switch( proc->sym.langtype ) {
                case LANG_NONE:
                case LANG_BASIC:
                case LANG_FORTRAN:
                case LANG_PASCAL:
                    AsmError( VARARG_REQUIRES_C_CALLING_CONVENTION );
                    return( ERROR );
                default:
                    break;
                }
            }
        }

        param = AsmLookup( token );
        if( param == NULL )
            return( ERROR );

        if( param->state != SYM_UNDEFINED ) {
            AsmErr( SYMBOL_PREVIOUSLY_DEFINED, param->name );
            return( ERROR );
        } else {
            param->state = SYM_INTERNAL;
            param->mem_type = TypeInfo[type].value;
        }

        paranode = AsmAlloc( sizeof( label_list ) );
        paranode->u.is_vararg = type == TOK_PROC_VARARG ? TRUE : FALSE;
        paranode->size = find_size( type );
        paranode->label = AsmAlloc( strlen( token ) + 1 );
        paranode->replace = NULL;
        strcpy( paranode->label, token );

        if( Use32 ) {
            info->parasize += ROUND_UP( paranode->size, 4 );
        } else {
            info->parasize += ROUND_UP( paranode->size, 2 );
        }
        info->is_vararg |= paranode->u.is_vararg;

        switch( proc->sym.langtype ) {
        case LANG_BASIC:
        case LANG_FORTRAN:
        case LANG_PASCAL:
            /* Parameters are stored in reverse order */
            paranode->next = info->paralist;
            info->paralist = paranode;
            break;
        default:
            paranode->next = NULL;
            if( info->paralist == NULL ) {
                info->paralist = paranode;
            } else {
                for( paracurr = info->paralist;; paracurr = paracurr->next ) {
                    if( paracurr->next == NULL ) {
                        break;
                    }
                }
                paracurr->next = paranode;
            }
            break;
        }
        /* go past comma */
        i++;
        if( ( i < Token_Count ) && ( AsmBuffer[i]->token != T_COMMA ) ) {
            AsmError( EXPECTING_COMMA );
            return( ERROR );
        }
    }
    return( NOT_ERROR );
}

int ProcDef( int i, bool proc_def )
/*********************************/
{
    dir_node            *dir;
    char                *name;

    if( CurrProc != NULL ) {
        /* nested procs ... push currproc on a stack */
        push_proc( CurrProc );
    }

    if( Parse_Pass == PASS_1 ) {
        if( i < 0 ) {
            AsmError( PROC_MUST_HAVE_A_NAME );
            return( ERROR );
        }
        name = AsmBuffer[i++]->string_ptr;
        dir = (dir_node *)AsmGetSymbol( name );
        if( dir == NULL ) {
            dir = dir_insert( name, TAB_PROC );
        } else if( dir->sym.state == SYM_UNDEFINED ) {
            /* alloc the procinfo struct */
            dir_change( dir, TAB_PROC );
        } else if( dir->sym.state == SYM_EXTERNAL ) {
            if( dir->e.extinfo->global ) {
                dir_change( dir, TAB_PROC );
                AddPublicData( dir );
            } else {
                AsmErr( SYMBOL_PREVIOUSLY_DEFINED, dir->sym.name );
                return( ERROR );
            }
        } else {
            AsmErr( SYMBOL_PREVIOUSLY_DEFINED, dir->sym.name );
            return( ERROR );
        }
        GetSymInfo( &dir->sym );
        if( proc_exam( dir, i ) == ERROR ) {
            return( ERROR );
        }
    } else {
        // fixme -- nested procs can be ok /**/myassert( CurrProc == NULL );
        /**/myassert( i >= 0 );
        dir = (dir_node *)AsmGetSymbol( AsmBuffer[i]->string_ptr );
        /**/myassert( dir != NULL );
        CurrProc = dir;
        GetSymInfo( &dir->sym );
        DefineProc = TRUE;
    }
    BackPatch( &dir->sym );
    return( NOT_ERROR );
}

static void ProcFini( void )
/**************************/
{
    proc_info   *info;
    label_list  *curr;

    info = CurrProc->e.procinfo;
    CurrProc->sym.total_size = GetCurrAddr() - CurrProc->sym.offset;

    if( Parse_Pass == PASS_1 ) {
        for( curr = info->paralist; curr; curr = curr->next ) {
            AsmTakeOut( curr->label );
        }
        for( curr = info->locallist; curr; curr = curr->next ) {
            AsmTakeOut( curr->label );
        }
    }
    CurrProc = pop_proc();
}

int ProcEnd( int i )
/******************/
{
    if( CurrProc == NULL ) {
        AsmError( BLOCK_NESTING_ERROR );
        return( ERROR );
    } else if( i < 0 ) {
        AsmError( PROC_MUST_HAVE_A_NAME );
        ProcFini();
        return( ERROR );
    } else if( (dir_node *)AsmGetSymbol( AsmBuffer[i]->string_ptr ) == CurrProc ) {
        ProcFini();
        return( NOT_ERROR );
    } else {
        AsmError( PROC_NAME_DOES_NOT_MATCH );
        ProcFini();
        return( NOT_ERROR );
    }
}

void CheckProcOpen( void )
/************************/
{
    while( CurrProc != NULL ) {
        if( Parse_Pass == PASS_1 )
            AsmErr( PROC_IS_NOT_CLOSED, CurrProc->sym.name );
        ProcFini();
    }
}

int WritePrologue( void )
/***********************/
{
    char                buffer[80];
    regs_list           *regist;
    int                 len;
    proc_info           *info;
    label_list          *curr;
    long                offset;
    unsigned long       size;
    int                 retcode;
    uint_8              parm_count = 0;
    int                 align = Use32 ? 4 : 2;

    /**/myassert( CurrProc != NULL );
    info = CurrProc->e.procinfo;

    /* save 1st instruction following a procedure definition */
    InputQueueLine( CurrString );

    if( Parse_Pass == PASS_1 ) {

        /* Figure out the replacing string for local variables */
        offset = 0;
        for( curr = info->locallist; curr != NULL; curr = curr->next ) {
            size = curr->size * curr->factor;
            offset += ROUND_UP( size, align );
            size_override( buffer, curr->size );
            sprintf( buffer + strlen(buffer), "%s%d] ",
                Use32 ? LOCAL_STRING_32 : LOCAL_STRING, offset );
            curr->replace = AsmAlloc( strlen( buffer ) + 1 );
            strcpy( curr->replace, buffer );
        }
        info->localsize = offset;

        /* Figure out the replacing string for the parameters */

        if( info->mem_type == MT_NEAR ) {
            offset = 4;         // offset from BP : return addr + old BP
        } else {
            offset = 6;
        }

        if( Use32 )
            offset *= 2;

        for( curr = info->paralist; curr; curr = curr->next, parm_count++ ) {
            if( CurrProc->sym.langtype == LANG_WATCOM_C ) {
                retcode = get_watcom_argument_string( buffer, curr->size, &parm_count );
                if( retcode == ERROR ) {
                    return( ERROR );
                }
            }
            if( ( CurrProc->sym.langtype != LANG_WATCOM_C ) || ( retcode == FALSE ) ) {
                size_override( buffer, curr->size );
                if( Use32 ) {
                    strcat( buffer, ARGUMENT_STRING_32 );
                } else {
                    strcat( buffer, ARGUMENT_STRING );
                }
                sprintf( buffer + strlen( buffer ), "%d", offset );
                strcat( buffer, "] " );

                offset += ROUND_UP( curr->size, align );
            }
            if( *buffer == '\0' ) {
                len = strlen( buffer+1 ) + 2;
                curr->replace = AsmAlloc( len );
                memcpy( curr->replace, buffer, len );
            } else {
                curr->replace = AsmAlloc( strlen( buffer ) + 1 );
                strcpy( curr->replace, buffer );
            }
        }
    }

    in_prologue = TRUE;
    PushLineQueue();
    if( ( info->localsize != 0 ) || ( info->parasize != 0 ) || info->is_vararg ) {
        // prolog code timmings
        //
        //                                                   best result
        //               size  86  286  386  486  P     86  286  386  486  P
        // push bp       2     11  3    2    1    1
        // mov bp,sp     2     2   2    2    1    1
        // sub sp,immed  4     4   3    2    1    1
        //              -----------------------------
        //               8     17  8    6    3    3     x   x    x    x    x
        //
        // push ebp      2     -   -    2    1    1
        // mov ebp,esp   2     -   -    2    1    1
        // sub esp,immed 6     -   -    2    1    1
        //              -----------------------------
        //               10    -   -    6    3    3              x    x    x
        //
        // enter imm,0   4     -   11   10   14   11
        //
        // write prolog code
        if( Use32 ) {
            // write 80386 prolog code
            // PUSH EBP
            // MOV  EBP, ESP
            // SUB  ESP, the number of localbytes
            strcpy( buffer, "push ebp" );
            InputQueueLine( buffer );
            strcpy( buffer, "mov ebp, esp" );
            if( info->localsize != 0 ) {
                InputQueueLine( buffer );
                strcpy( buffer, "sub esp, " );
                sprintf( buffer+strlen(buffer), "%d", info->localsize );
            }
        } else {
            // write 8086 prolog code
            // PUSH BP
            // MOV  BP, SP
            // SUB  SP, the number of localbytes
            strcpy( buffer, "push bp" );
            InputQueueLine( buffer );
            strcpy( buffer, "mov bp, sp" );
            if( info->localsize != 0 ) {
                InputQueueLine( buffer );
                strcpy( buffer, "sub sp, " );
                sprintf( buffer+strlen(buffer), "%d", info->localsize );
            }
        }
        InputQueueLine( buffer );
    }
    /* Push the registers */
    if( info->regslist ) {
        strcpy( buffer, "push " );
        len = strlen( buffer );
        for( regist = info->regslist; regist; regist = regist->next ) {
            strcpy( buffer + len, regist->reg );
            InputQueueLine( buffer );
        }
    }
    return( NOT_ERROR );
}

static void pop_register( regs_list *regist )
/*******************************************/
/* Pop the register when a procedure ends */
{
    char        buffer[20];

    if( regist == NULL )
        return;
    pop_register( regist->next );
    strcpy( buffer, "pop " );
    strcpy( buffer + strlen( buffer ), regist->reg );
    InputQueueLine( buffer );
}

static void write_epilogue( void )
/********************************/
{
    char        buffer[80];
    proc_info   *info;

    /**/myassert( CurrProc != NULL );
    info = CurrProc->e.procinfo;

    /* Pop the registers */
    pop_register( CurrProc->e.procinfo->regslist );

    if( ( info->localsize == 0 ) && ( info->parasize == 0 ) && !info->is_vararg )
        return;
    // epilog code timmings
    //
    //                                                  best result
    //              size  86  286  386  486  P      86  286  386  486  P
    // mov sp,bp    2     2   2    2    1    1
    // pop bp       2     8   5    4    4    1
    //             -----------------------------
    //              4     10  7    6    5    2      x             x    x
    //
    // mov esp,ebp  2     -   -    2    1    1
    // pop ebp      2     -   -    4    4    1
    //             -----------------------------
    //              4     -   -    6    5    2                    x    x
    //
    // leave        1     -   5    4    5    3          x    x    x
    //
    // !!!! DECISION !!!!
    //
    // leave will be used for .286 and .386
    // .286 code will be best working on 286,386 and 486 processors
    // .386 code will be best working on 386 and 486 processors
    // .486 code will be best working on 486 and above processors
    //
    //   without LEAVE
    //
    //         86  286  386  486  P
    //  .8086  0   -2   -2   0    +1
    //  .286   -   -2   -2   0    +1
    //  .386   -   -    -2   0    +1
    //  .486   -   -    -    0    +1
    //
    //   LEAVE 286 only
    //
    //         86  286  386  486  P
    //  .8086  0   -2   -2   0    +1
    //  .286   -   0    +2   0    -1
    //  .386   -   -    -2   0    +1
    //  .486   -   -    -    0    +1
    //
    //   LEAVE 286 and 386
    //
    //         86  286  386  486  P
    //  .8086  0   -2   -2   0    +1
    //  .286   -   0    +2   0    -1
    //  .386   -   -    0    0    -1
    //  .486   -   -    -    0    +1
    //
    //   LEAVE 286, 386 and 486
    //
    //         86  286  386  486  P
    //  .8086  0   -2   -2   0    +1
    //  .286   -   0    +2   0    -1
    //  .386   -   -    0    0    -1
    //  .486   -   -    -    0    -1
    //
    // write epilog code
    if( info->pe_type ) {
        // write 80286 and 80386 epilog code
        // LEAVE
        strcpy( buffer, "leave" );
    } else if( Use32 ) {
        // write 32-bit 80486 or P epilog code
        // Mov ESP, EBP
        // POP EBP
        if( info->localsize != 0 ) {
            strcpy( buffer, "mov esp, ebp" );
            InputQueueLine( buffer );
        }
        strcpy( buffer, "pop ebp" );
    } else {
        // write 16-bit 8086 or 80486 or P epilog code
        // Mov SP, BP
        // POP BP
        if( info->localsize != 0 ) {
            strcpy( buffer, "mov sp, bp" );
            InputQueueLine( buffer );
        }
        strcpy( buffer, "pop bp" );
    }
    InputQueueLine( buffer );
}

int Ret( int i, int count, int flag_iret )
/****************************************/
{
    char        buffer[20];
    proc_info   *info;
    expr_list   opndx;

    info = CurrProc->e.procinfo;

    if( flag_iret ) {
        if( AsmBuffer[i]->u.value == T_IRET ) {
            strcpy( buffer, "iretf" );
        } else {
            strcpy( buffer, "iretdf" );
        }
    } else {
        if( info->mem_type == MT_NEAR ) {
            strcpy( buffer, "retn " );
        } else {
            strcpy( buffer, "retf " );
        }
    }

    write_epilogue();

    if( !flag_iret ) {
        if( count == i + 1 ) {
            switch( CurrProc->sym.langtype ) {
            case LANG_BASIC:
            case LANG_FORTRAN:
            case LANG_PASCAL:
                if( info->parasize != 0 ) {
                    sprintf( buffer + strlen( buffer ), "%d", info->parasize );
                }
                break;
            case LANG_STDCALL:
                if( !info->is_vararg && info->parasize != 0 ) {
                    sprintf( buffer + strlen( buffer ), "%d", info->parasize );
                }
                break;
            default:
                break;
            }
        } else {
            ++i;
            if( (EvalOperand( &i, count, &opndx, TRUE ) == ERROR) || (opndx.type != EXPR_CONST) ) {
                AsmError( CONSTANT_EXPECTED );
                return( ERROR );
            }
            sprintf( buffer + strlen( buffer ), "%d", opndx.value );
        }
    }

    InputQueueLine( buffer );

    return( NOT_ERROR );
}

extern void GetSymInfo( struct asm_sym *sym )
/*******************************************/
{
    sym->segment = &GetCurrSeg()->sym;
    sym->offset = GetCurrAddr();
}

int Comment( int what_to_do, int i )
/***********************************/
{
    static int in_comment = FALSE;
    static char delim_char;

    switch( what_to_do ) {
    case QUERY_COMMENT:
        return( in_comment );
    case QUERY_COMMENT_DELIM:
        /* return delimiting character */
        return( (int)delim_char );
        break;
    case START_COMMENT:
        i++;
        if( AsmBuffer[i]->string_ptr == NULL ) {
            AsmError( COMMENT_DELIMITER_EXPECTED );
            return( ERROR );
        }
        delim_char = *(AsmBuffer[i]->string_ptr+strspn(AsmBuffer[i]->string_ptr," \t") );
        if( ( delim_char == '\0' )
            || ( strchr( AsmBuffer[i]->string_ptr, delim_char ) == NULL ) ) {
            AsmError( COMMENT_DELIMITER_EXPECTED );
            return( ERROR );
        }
        if( strchr( AsmBuffer[i]->string_ptr, delim_char )
            != strrchr( AsmBuffer[i]->string_ptr, delim_char ) ) {
            /* we have COMMENT delim. ..... delim. -- only 1 line */
        } else {
            in_comment = TRUE;
        }
        return( NOT_ERROR );
    case END_COMMENT:
        in_comment = FALSE;
        return( NOT_ERROR );
    }
    return( ERROR );
}

int AddAlias( int i )
/*******************/
{
    /* takes directive of form: <alias_name> alias <substitute_name> */
    char *tmp;

    if( i < 0 ) {
        AsmError( SYNTAX_ERROR );
        return( ERROR );
    }
    if( ( AsmBuffer[i]->token != T_ID )
        || ( AsmBuffer[i+2]->token != T_ID ) ) {
        AsmError( SYMBOL_EXPECTED );
        return( ERROR );
    }

    /* add this alias to the alias queue */

    /* aliases are stored as:  <len><alias_name><len><substitute_name> */

    tmp = AsmAlloc( strlen( AsmBuffer[i]->string_ptr ) +
                    strlen( AsmBuffer[i+2]->string_ptr ) + 2 );
    AddAliasData( tmp );

    strcpy( tmp, AsmBuffer[i]->string_ptr );
    tmp += strlen( tmp ) + 1 ;
    strcpy( tmp, AsmBuffer[i+2]->string_ptr );

    return( NOT_ERROR );
}

int NameDirective( int i )
/************************/
{
    if( Options.module_name != NULL )
        return( NOT_ERROR );
    Options.module_name = AsmAlloc( strlen( AsmBuffer[i+1]->string_ptr ) + 1 );
    strcpy( Options.module_name, AsmBuffer[i+1]->string_ptr );
    return( NOT_ERROR );
}

int CommDef( int i )
/******************/
{
    char            *token;
    char            *mangle_type = NULL;
    char            *typetoken;
    int             type;
    int             distance;
    int             count;
    dir_node        *dir;
    int             lang_type;
    memtype         mem_type;

    mangle_type = Check4Mangler( &i );
    for( ; i < Token_Count; i++ ) {
        count = 1;

        /* get the distance ( near or far ) */
        typetoken = AsmBuffer[i]->string_ptr;
        distance = token_cmp( &typetoken, TOK_EXT_NEAR, TOK_EXT_FAR );

        if( distance != ERROR ) {
            /* otherwise, there is no distance specified */
            i++;
        } else {
            distance = TOK_EXT_NEAR;
        }

        /* get the symbol language type if present */
        lang_type = GetLangType( &i );

        /* get the symbol name */
        token = AsmBuffer[i++]->string_ptr;

        /* go past the colon */
        if( AsmBuffer[i]->token != T_COLON ) {
            AsmError( COLON_EXPECTED );
            return( ERROR );
        }
        i++;
        typetoken = AsmBuffer[i]->string_ptr;
        type = token_cmp( &typetoken, TOK_EXT_BYTE, TOK_EXT_TBYTE );

        if( type == ERROR ) {
            AsmError( INVALID_QUALIFIED_TYPE );
            return( ERROR );
        }
        for( ; i< Token_Count && AsmBuffer[i]->token != T_COMMA; i++ ) {
            if( AsmBuffer[i]->token == T_COLON ) {
                i++;
                /* count */
                if( AsmBuffer[i]->token != T_NUM ) {
                    AsmError( EXPECTING_NUMBER );
                    return( ERROR );
                }
                count = AsmBuffer[i]->u.value;
            }
        }
        mem_type = TypeInfo[type].value;
        dir = (dir_node *)AsmGetSymbol( token );
        if( dir == NULL ) {
            dir = dir_insert( token, TAB_COMM );
            if( dir == NULL ) {
                return( ERROR );
            }
        } else if( dir->sym.state == SYM_UNDEFINED ) {
            dir_change( dir, TAB_COMM );
        } else if( dir->sym.mem_type != mem_type ) {
            AsmError( EXT_DEF_DIFF );
            return( ERROR );
        }
        GetSymInfo( &dir->sym );
        dir->sym.offset = 0;
        dir->sym.mem_type = mem_type;
        dir->e.comminfo->size = count;
        dir->e.comminfo->distance = TypeInfo[distance].value;
        SetMangler( &dir->sym, mangle_type, lang_type );
    }
    return( NOT_ERROR );
}
