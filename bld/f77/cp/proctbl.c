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
// PROCTBL      : jump table of statement processor routines
//

#include "ftnstd.h"

// Note: If you wish to add a new statement processor, there are four files
//       that need changing: PRDEFN.H, KWLIST.C, PROCTBL.C, CFTABLE.C

extern  void            BadStmt();
extern  void            CpBackSp();
extern  void            CpClose();
extern  void            CpEndfile();
extern  void            CpInquire();
extern  void            CpOpen();
extern  void            CpPrint();
extern  void            CpRead();
extern  void            CpRewind();
extern  void            CpWrite();
extern  void            CpAdmit();
extern  void            CpAtEnd();
extern  void            CpCase();
extern  void            CpSelect();
extern  void            CpElse();
extern  void            CpElseIf();
extern  void            CpEndAtEnd();
extern  void            CpEndBlock();
extern  void            CpEndSelect();
extern  void            CpDo();
extern  void            CpEnd();
extern  void            CpEndDo();
extern  void            CpEndGuess();
extern  void            CpEndif();
extern  void            CpEndLoop();
extern  void            CpEndWhile();
extern  void            CpGuess();
extern  void            CpLogIf();
extern  void            CpOtherwise();
extern  void            CpLoop();
extern  void            CpQuit();
extern  void            CpRemBlock();
extern  void            CpUntil();
extern  void            CpWhile();
extern  void            CpAsgnmt();
extern  void            CpStmtFunc();
extern  void            CpCall();
extern  void            CpContinue();
extern  void            CpGoto();
extern  void            CpArithIf();
extern  void            CpAssign();
extern  void            CpPause();
extern  void            CpStop();
extern  void            CpFormat();
extern  void            CpSubroutine();
extern  void            CpFunction();
extern  void            CpProgram();
extern  void            CpReturn();
extern  void            CpEntry();
extern  void            CpIntVar();
extern  void            CpRealVar();
extern  void            CpCmplxVar();
extern  void            CpDCmplxVar();
extern  void            CpXCmplxVar();
extern  void            CpDbleVar();
extern  void            CpXDbleVar();
extern  void            CpLogVar();
extern  void            CpCharVar();
extern  void            CpDimension();
extern  void            CpImplicit();
extern  void            CpParameter();
extern  void            CpCommon();
extern  void            CpEquivalence();
extern  void            CpExecute();
extern  void            CpData();
extern  void            CpSave();
extern  void            CpBlockData();
extern  void            CpExternal();
extern  void            CpIntrinsic();
extern  void            CpInclude();
extern  void            CpDoWhile();
extern  void            CpExit();
extern  void            CpCycle();
extern  void            CpStructure();
extern  void            CpEndStructure();
extern  void            CpUnion();
extern  void            CpEndUnion();
extern  void            CpMap();
extern  void            CpEndMap();
extern  void            CpRecord();
extern  void            CpNameList();
extern  void            CpAllocate();
extern  void            CpDeAllocate();
extern  void            CpVolatile();

void    (* const __FAR ProcTable[])() = {
    &BadStmt,                   // Unknown Statement
    &CpAdmit,                   // ADMIT
    &CpAllocate,                // ALLOCATE
    &CpAssign,                  // ASSIGN
    &CpAtEnd,                   // ATEND
    &CpBackSp,                  // BACKSPACE
    &CpBlockData,               // BLOCKDATA
    &CpCall,                    // CALL
    &CpCase,                    // CASE
    &CpCharVar,                 // CHARACTER
    &CpClose,                   // CLOSE
    &CpCommon,                  // COMMON
    &CpCmplxVar,                // COMPLEX
    &CpContinue,                // CONTINUE
    &CpCycle,                   // CYCLE
    &CpData,                    // DATA
    &CpDeAllocate,              // DEALLOCATE
    &CpDimension,               // DIMENSION
    &CpDoWhile,                 // DO WHILE
    &CpDCmplxVar,               // DOUBLECOMPLEX
    &CpDbleVar,                 // DOUBLEPRECISION
    &CpElse,                    // ELSE
    &CpElseIf,                  // ELSEIF
    &CpEnd,                     // END
    &CpEndAtEnd,                // ENDATEND
    &CpEndBlock,                // ENDBLOCK
    &CpEndDo,                   // ENDDO
    &CpEndfile,                 // ENDFILE
    &CpEndGuess,                // ENDGUESS
    &CpEndif,                   // ENDIF
    &CpEndLoop,                 // ENDLOOP
    &CpEndMap,                  // ENDMAP
    &CpEndSelect,               // ENDSELECT
    &CpEndStructure,            // ENDSTRUCTURE
    &CpEndUnion,                // ENDUNION
    &CpEndWhile,                // ENDWHILE
    &CpEntry,                   // ENTRY
    &CpEquivalence,             // EQUIVALENCE
    &CpExecute,                 // EXECUTE
    &CpExit,                    // EXIT
    &CpXCmplxVar,               // EXTENDEDCOMPLEX
    &CpXDbleVar,                // EXTENDEDPRECISION
    &CpExternal,                // EXTERNAL
    &CpFormat,                  // FORMAT
    &CpFunction,                // FUNCTION
    &CpGoto,                    // GOTO
    &CpGuess,                   // GUESS
    &CpLogIf,                   // IF
    &CpImplicit,                // IMPLICIT
    &CpInclude,                 // INCLUDE
    &CpInquire,                 // INQUIRE
    &CpIntVar,                  // INTEGER
    &CpIntrinsic,               // INTRINSIC
    &CpLogVar,                  // LOGICAL
    &CpLoop,                    // LOOP
    &CpMap,                     // MAP
    &CpNameList,                // NAMELIST
    &CpOpen,                    // OPEN
    &CpOtherwise,               // OTHERWISE
    &CpParameter,               // PARAMETER
    &CpPause,                   // PAUSE
    &CpPrint,                   // PRINT
    &CpProgram,                 // PROGRAM
    &CpQuit,                    // QUIT
    &CpRead,                    // READ
    &CpRealVar,                 // REAL
    &CpRecord,                  // RECORD
    &CpRemBlock,                // REMOTEBLOCK
    &CpReturn,                  // RETURN
    &CpRewind,                  // REWIND
    &CpSave,                    // SAVE
    &CpSelect,                  // SELECT
    &CpStop,                    // STOP
    &CpStructure,               // STRUCTURE
    &CpSubroutine,              // SUBROUTINE
    &CpUnion,                   // UNION
    &CpUntil,                   // UNTIL
    &CpVolatile,                // VOLATILE
    &CpWhile,                   // WHILE
    &CpWrite,                   // WRITE
    &CpDo,                      // DO
    &CpAsgnmt,                  // = (assignment statement)
    &CpArithIf,                 // IF( expr ) 10, 20, 30
    &CpStmtFunc                 // statement function
};
