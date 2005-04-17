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


//
// CFTABLE      : control flags table - attributes of each statement processor
//

// Note: If you wish to add a new statement processor, there are four files
//       that need changing: PRDEFN.H, KWLIST.C, PROCTBL.C, CFTABLE.C

#include "ftnstd.h"
#include "ctrlflgs.h"
#include "global.h"

#define BD      CF_BAD_DO_ENDING
#define NS      CF_NOT_SIMPLE_STMT
#define NE      CF_NOT_EXECUTABLE
#define BB      CF_BAD_BRANCH_OBJECT
#define SL      CF_NEED_SET_LINE
#define SS      CF_SPECIFICATION
#define SP      CF_SUBPROGRAM
#define BL      CF_BLOCK_DATA
#define DB      CF_DBUG_BEFORE_LINE
#define IL      CF_IMPLICIT_LABEL
#define ST      CF_NOT_IN_STRUCTURE
#define UN      CF_NOT_IN_UNION
#define MA      CF_NOT_IN_MAP

// These control flags are used for various purposes.

// BAD_DO_ENDING     - these are statements that cannot be allowed on the
//                     terminal statement of a regular FORTRAN DO loop
// NOT_SIMPLE_STMT   - these are statements that cannot be allowed as the
//                     object of a logical IF or ATEND statement
// NOT_EXECUTABLE    - these are not executable statements and have
//                     certain restrictions outlined in the FORTRAN
//                     standard
// BAD_BRANCH_OBJECT - these are statements that would be meaningless if
//                     they were branched to
// NEED_SET_LINE     - before most statements are compiled, there needs to
//                     be a run time call emitted to set the line number
//                     currently executed
//                     WHILE and ELSEIF are special cases because they
//                     generate the run time call themselves so that the
//                     SetLine procedure will be called at the right time
// SPECIFICATION     - these statements are specification statements
//                     and must be compiled before a function/subroutine
//                     prologue is emitted.
// SUBPROGRAM        - SUBROUTINE, (typ)FUNCTION, BLKDATA, PROGRAM stmts
//                     also all type declaration statements have this bit
//                     on since typ*lenFUNCTION starts a subprogram
// BLOCK_DATA        - these statements are the only ones allowed in
//                     a BLOCK DATA subprogram
// DBUG_BEFORE_LINE  - need to call debugger ISN routine before statement
//                     processed
// IMPLICIT_LABEL    - used to flag those statements which have an
//                     implicit label (ENDIF, ENDGUESS, ENDATEND)
// NOT_IN_STRUCTURE  - used to flag those specification statements which cannot
//                     appear in a structure definition
// NOT_IN_UNION      - used to flag those specification statements which cannot
//                     appear in a union definition
// NOT_IN_MAP        - used to flag those specification statements which cannot
//                     appear in a map definition

const unsigned_16 __FAR CFTable[] = {
    0,                                          // null - no statement
    BD | NS | BB,                               // ADMIT
    SL | DB,                                    // ALLOCATE
    SL | DB,                                    // ASSIGN
    NS | BB | SL | DB,                          // ATEND
    SL | DB,                                    // BACKSPACE
    BD | NS | NE | BB | SP,                     // BLOCKDATA
    SL | DB,                                    // CALL
    BD | NS | BB,                               // CASE
    BD | NS | NE | BB | SS | SP | BL | UN,      // CHARACTER
    SL | DB,                                    // CLOSE
    BD | NS | NE | BB | SS | BL | ST | UN | MA, // COMMON
    BD | NS | NE | BB | SS | SP | BL | UN,      // COMPLEX
    DB,                                         // CONTINUE
    SL | DB,                                    // CYCLE
    BD | NS | NE | BB | BL | SS,                // DATA
    SL | DB,                                    // DEALLOCATE
    BD | NS | NE | BB | SS | BL | ST | UN | MA, // DIMENSION
    BD | NS,                                    // DO WHILE
    BD | NS | NE | BB | SS | SP | BL | UN,      // DOUBLECOMPLEX
    BD | NS | NE | BB | SS | SP | BL | UN,      // DOUBLEPRECISION
    BD | NS | BB,                               // ELSE
    BD | NS | BB,                               // ELSEIF
    BD | NS | BL | DB | SL,                     // END
    BD | NS | BB | IL,                          // ENDATEND
    BD | NS | BB | DB,                          // ENDBLOCK
    NS | BB | DB,                               // ENDDO
    SL | DB,                                    // ENDFILE
    BD | NS | IL,                               // ENDGUESS
    BD | NS | IL,                               // ENDIF
    BD | NS | BB | SL | DB,                     // ENDLOOP
    BD | NS | NE | BB | SS | BL,                // ENDMAP
    BD | NS | BB | IL,                          // ENDSELECT
    BD | NS | NE | BB | SS | BL | UN | MA,      // ENDSTRUCTURE
    BD | NS | NE | BB | SS | BL | MA,           // ENDUNION
    BD | NS | BB | DB,                          // ENDWHILE
    BD | NS | BB | SS | ST | UN | MA,           // ENTRY
    BD | NS | NE | BB | SS | BL | ST | UN | MA, // EQUIVALENCE
    SL | DB,                                    // EXECUTE
    SL | DB,                                    // EXIT
    BD | NS | NE | BB | SS | SP | BL | UN,      // EXTENDEDCOMPLEX
    BD | NS | NE | BB | SS | SP | BL | UN,      // EXTENDEDPRECISION
    BD | NS | NE | BB | SS | ST | UN | MA,      // EXTERNAL
    BD | NS | NE | BB,                          // FORMAT
    BD | NS | NE | BB | SP,                     // FUNCTION
    BD | SL | DB,                               // GOTO
    BD | NS | DB,                               // GUESS
    NS | SL | DB,                               // IF
    BD | NS | NE | BB | SS | BL | ST | UN | MA, // IMPLICIT
    BD | NS | NE | BB | BL,                     // INCLUDE
    SL | DB,                                    // INQUIRE
    BD | NS | NE | BB | SS | SP | BL | UN,      // INTEGER
    BD | NS | NE | BB | SS | ST | UN | MA,      // INTRINSIC
    BD | NS | NE | BB | SS | SP | BL | UN,      // LOGICAL
    BD | NS,                                    // LOOP
    BD | NS | NE | BB | SS | BL | ST | MA,      // MAP
    NS | NE | BB | SS | BL | ST | UN | MA,      // NAMELIST
    SL | DB,                                    // OPEN
    BD | NS | BB,                               // OTHERWISE
    BD | NS | NE | BB | SS | BL | ST | UN | MA, // PARAMETER
    SL | DB,                                    // PAUSE
    SL | DB,                                    // PRINT
    BD | NS | NE | BB | SP,                     // PROGRAM
    SL | DB,                                    // QUIT
    SL | DB,                                    // READ
    BD | NS | NE | BB | SS | SP | BL | UN,      // REAL
    BD | NS | NE | BB | SS | SP | BL | UN,      // RECORD
    BD | NS | NE | BB,                          // REMOTEBLOCK
    BD | DB | SL,                               // RETURN
    SL | DB,                                    // REWIND
    BD | NS | NE | BB | SS | BL | ST | UN | MA, // SAVE
    BD | NS | SL | DB,                          // SELECT
    BD | SL | DB,                               // STOP
    BD | NS | NE | BB | SS | BL | ST,           // STRUCTURE
    BD | NS | NE | BB | SP,                     // SUBROUTINE
    BD | NS | NE | BB | SS | BL | MA,           // UNION
    BD | NS | BB | SL | DB,                     // UNTIL
    BD | NS | NE | BB | SS | BL | ST | UN | MA, // VOLATILE
    BD | NS,                                    // WHILE
    SL | DB,                                    // WRITE
    BD | NS | SL | DB,                          // DO
    SL | DB,                                    // assignment
    BD | SL | DB,                               // arithmetic if
    NS | BB | NE | BD };                        // statement function


//
// CtrlFlgOn -- checks if a bit is on in statement control flags
//

#ifndef CtrlFlgOn

// for speed this is macro'd in ctrlflgs.h

bool    CtrlFlgOn( unsigned_16 bits ) {
//=====================================

    return( ( CtrlFlgs & bits ) != 0 );
}

#endif
