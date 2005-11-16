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


// Note : If you wish to add a new statement processor there are five files
//        that need changing: PRDEFN.H, KWLIST.C, PROCTBL.C, CFTABLE.C

typedef enum {
    PR_NULL,
    PR_ADMIT,           // ADMIT
    PR_ALLOCATE,        // ALLOCATE
    PR_ASSIGN,          // ASSIGN
    PR_ATEND,           // ATEND
    PR_BKSP,            // BACKSPACE
    PR_BLKDAT,          // BLOCKDATA
    PR_CALL,            // CALL
    PR_CASE,            // CASE
    PR_CHAR,            // CHARACTER
    PR_CLOSE,           // CLOSE
    PR_COMN,            // COMMON
    PR_COMP,            // COMPLEX
    PR_CONT,            // CONTINUE
    PR_CYCLE,           // CYCLE
    PR_DATA,            // DATA
    PR_DEALLOCATE,      // DEALLOCATE
    PR_DIM,             // DIMENSION
    PR_DO_WHILE,        // DOWHILE
    PR_DBLE_COMPLEX,    // DOUBLECOMPLEX
    PR_DP,              // DOUBLEPRECISION
    PR_ELSE,            // ELSE
    PR_ELSEIF,          // ELSEIF
    PR_END,             // END
    PR_EATEND,          // ENDATEND
    PR_EBLOCK,          // ENDBLOCK
    PR_ENDDO,           // ENDDO
    PR_EFILE,           // ENDFILE
    PR_EGUESS,          // ENDGUESS
    PR_ENDIF,           // ENDIF
    PR_ELOOP,           // ENDLOOP
    PR_ENDMAP,          // ENDMAP
    PR_ENDSTRUCTURE,    // ENDSTRUCTURE
    PR_ENDUNION,        // ENDUNION
    PR_ESELECT,         // ENDSELECT
    PR_EWHILE,          // ENDWHILE
    PR_ENTRY,           // ENTRY
    PR_EQUIV,           // EQUIVALENCE
    PR_EXEC,            // EXECUTE
    PR_EXIT,            // EXIT
    PR_EXT_COMPLEX,     // EXTENDEDCOMPLEX
    PR_EXTENDED,        // EXTENDEDPRECISION
    PR_EXT,             // EXTERNAL
    PR_FMT,             // FORMAT
    PR_FUNC,            // FUNCTION
    PR_GOTO,            // GOTO
    PR_GUESS,           // GUESS
    PR_IF,              // IF
    PR_IMP,             // IMPLICIT
    PR_INCLUDE,         // INCLUDE
    PR_INQ,             // INQUIRE
    PR_INT,             // INTEGER
    PR_INTRNS,          // INTRINSIC
    PR_LOG,             // LOGICAL
    PR_LOOP,            // LOOP
    PR_MAP,             // MAP
    PR_NAMELIST,        // NAMELIST
    PR_OPEN,            // OPEN
    PR_OTHERWISE,       // OTHERWISE
    PR_PARM,            // PARAMETER
    PR_PAUSE,           // PAUSE
    PR_PRINT,           // PRINT
    PR_PROG,            // PROGRAM
    PR_QUIT,            // QUIT
    PR_READ,            // READ
    PR_REAL,            // REAL
    PR_RECORD,          // RECORD
    PR_REMBLK,          // REMOTEBLOCK
    PR_RET,             // RETURN
    PR_REW,             // REWIND
    PR_SAVE,            // SAVE
    PR_SELECT,          // SELECT
    PR_STOP,            // STOP
    PR_STRUCTURE,       // STRUCTURE
    PR_SUB,             // SUBROUTINE
    PR_UNION,           // UNION
    PR_UNTIL,           // UNTIL
    PR_VOLATILE,        // VOLATILE
    PR_WHILE,           // WHILE
    PR_WRITE,           // WRITE

    // end of keywords that can be looked up in table

    PR_DO,              // DO
    PR_ASNMNT,          // assignment (=)
    PR_ARIF,            // arithmetic if
    PR_STMTFUNC         // statement function
} stmtproc;

#define PR_KW_MAX       (PR_WRITE - 1)
